// TODO:
// implement smooth shading with Gouraud shading algorithm
// implement smooth shading with Phong shading algorithm

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"

triangle_t *triangles_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = 0};

mat4_t projection_matrix;
light_t directional_light;

bool is_running = false;
int previous_frame_time = 0;

void setup(void)
{
    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height);

    //  initialize the perspective projection matrix
    float fov = M_PI / 3; // 180 / 3, 60deg
    float aspect = (float)window_height / (float)window_width;
    projection_matrix = mat4_make_perspective(fov, aspect, 0.1, 100.0);

    vec3_t direction = {0, 0, 1};
    directional_light.direction = direction;

    // Manually load the hardcoded texture data from the static array
    mesh_texture = (uint32_t *)REDBRICK_TEXTURE;

    // load_obj_file_data("./assets/f22.obj");
    load_cube_mesh_data();
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        if (event.key.keysym.sym == SDLK_1)
        {
            render_method = RENDER_WIRE_VERTEX;
        }
        if (event.key.keysym.sym == SDLK_2)
        {
            render_method = RENDER_WIRE;
        }
        if (event.key.keysym.sym == SDLK_3)
        {
            render_method = RENDER_FILL_TRIANGLE;
        }
        if (event.key.keysym.sym == SDLK_4)
        {
            render_method = RENDER_FILL_TRIANGLE_WIRE;
        }
        if (event.key.keysym.sym == SDLK_5)
        {
            render_method = RENDER_TEXTURED;
        }
        if (event.key.keysym.sym == SDLK_6)
        {
            render_method = RENDER_TEXTURED_WIRE;
        }
        if (event.key.keysym.sym == SDLK_c)
        {
            cull_method = CULL_BACKFACE;
        }
        if (event.key.keysym.sym == SDLK_d)
        {
            cull_method = CULL_NONE;
        }
        break;
    }
}

void update(void)
{
    triangles_to_render = NULL;

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    mesh.rotation.x += 0.05;
    // mesh.rotation.y += 0.05;
    // mesh.rotation.z += 0.05;

    // mesh.scale.x += 0.002;
    // mesh.scale.y += 0.001;

    // mesh.translation.x += 0.01;
    mesh.translation.z = 5;

    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

    int num_faces = array_length(mesh.faces);

    for (int i = 0; i < num_faces; i++)
    {
        face_t face_mesh = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[face_mesh.a - 1];
        face_vertices[1] = mesh.vertices[face_mesh.b - 1];
        face_vertices[2] = mesh.vertices[face_mesh.c - 1];

        vec4_t transformed_vertices[3];

        // 3 for all the vertices of the current face
        for (int j = 0; j < 3; j++)
        {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
            mat4_t world_matrix = mat4_identity();

            // Order matters, always have to scale, rotate and then translate: [T]*[R]*[S]*v
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            transformed_vertices[j] = transformed_vertex;
        }

        /*
         * algo is as follow:
         * - find B-A and C-A
         * - find the normal N by getting the cross product
         * - find the camera ray (vector of the camera position and A)
         * - find the dot product between N and the camera ray
         * - return true if greater than 0 (render face)
         */
        // back face culling (display the face or not)
        // get ABC in clockwise order
        vec3_t vertex_a = vec3_from_vec4(transformed_vertices[0]);
        vec3_t vertex_b = vec3_from_vec4(transformed_vertices[1]);
        vec3_t vertex_c = vec3_from_vec4(transformed_vertices[2]);

        vec3_t vector_ba = vec3_sub(vertex_b, vertex_a);
        vec3_t vector_ca = vec3_sub(vertex_c, vertex_a);

        vec3_normalize(&vector_ba);
        vec3_normalize(&vector_ca);

        vec3_t normal = vec3_cross(vector_ba, vector_ca);
        vec3_normalize(&normal);

        vec3_t camera_ray = vec3_sub(camera_position, vertex_a);

        float dot_product = vec3_dot(camera_ray, normal);

        if (cull_method == CULL_BACKFACE && dot_product <= 0)
        {
            continue;
        }

        vec4_t projected_points[3];
        for (int j = 0; j < 3; j++)
        {
            projected_points[j] = mat4_mul_vec4_project(projection_matrix, transformed_vertices[j]);
            // scale into the view
            projected_points[j].x *= (window_width / 2.0);
            projected_points[j].y *= (window_height / 2.0);

            projected_points[j].y *= -1;

            // translate the projected points to the middle of the screen
            projected_points[j].x += (window_width / 2.0);
            projected_points[j].y += (window_height / 2.0);
        }

        float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

        float percentage_exposition = -vec3_dot(normal, directional_light.direction);
        uint32_t new_color = light_apply_intensity(face_mesh.color, percentage_exposition);

        triangle_t projected_triangle = {
            .points = {
                {projected_points[0].x, projected_points[0].y},
                {projected_points[1].x, projected_points[1].y},
                {projected_points[2].x, projected_points[2].y},
            },
            .texcoords = {
                {face_mesh.a_uv.u, face_mesh.a_uv.v},
                {face_mesh.b_uv.u, face_mesh.b_uv.v},
                {face_mesh.c_uv.u, face_mesh.c_uv.v},
            },
            .color = new_color,
            .avg_depth = avg_depth};

        array_push(triangles_to_render, projected_triangle);
    }

    bubblesort(triangles_to_render);
}

void render(void)
{
    draw_grid();

    int number_triangles = array_length(triangles_to_render);

    for (int i = 0; i < number_triangles; i++)
    {
        triangle_t triangle = triangles_to_render[i];

        if (render_method == RENDER_WIRE_VERTEX)
        {
            draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, 0xFF00FF00);
            draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, 0xFF00FF00);
            draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, 0xFF00FF00);
        }

        if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
        {
            draw_filled_triangle(triangle.points[0].x,
                                 triangle.points[0].y,
                                 triangle.points[1].x,
                                 triangle.points[1].y,
                                 triangle.points[2].x,
                                 triangle.points[2].y,
                                 triangle.color);
        }

        // Draw textured triangle
        if (render_method == RENDER_TEXTURED || render_method == RENDER_TEXTURED_WIRE)
        {
            draw_textured_triangle(triangle.points[0].x, triangle.points[0].y, triangle.texcoords[0].u, triangle.texcoords[0].v,
                                   triangle.points[1].x, triangle.points[1].y, triangle.texcoords[1].u, triangle.texcoords[1].v,
                                   triangle.points[2].x, triangle.points[2].y, triangle.texcoords[2].u, triangle.texcoords[2].v,
                                   mesh_texture);
        }

        if (render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_TEXTURED_WIRE)
        {
            draw_triangle(triangle.points[0].x,
                          triangle.points[0].y,
                          triangle.points[1].x,
                          triangle.points[1].y,
                          triangle.points[2].x,
                          triangle.points[2].y,
                          0xFFCCCCCC);
        }
    }

    array_free(triangles_to_render);

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void free_resources(void)
{
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
}

int main()
{
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();
    free_resources();

    return 0;
}
