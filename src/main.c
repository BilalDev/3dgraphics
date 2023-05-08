#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

triangle_t *triangles_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = 0};

float fov_factor = 640;

bool is_running = false;
int previous_frame_time;

void setup(void)
{
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             window_width,
                                             window_height);

    // load_cube_mesh_data();
    load_obj_file_data("./cube.obj");
    mesh.rotation.x = 10;
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
        break;
    }
}

// Projection that receives a 3D vector and returns a projected 2D point
vec2_t project(vec3_t point)
{
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z};

    return projected_point;
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

    mesh.rotation.y += 0.01;

    int num_faces = array_length(mesh.faces);

    for (int i = 0; i < num_faces; i++)
    {
        face_t face_mesh = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[face_mesh.a - 1];
        face_vertices[1] = mesh.vertices[face_mesh.b - 1];
        face_vertices[2] = mesh.vertices[face_mesh.c - 1];

        triangle_t projected_triangle;

        vec3_t transformed_vertices[3];

        // 3 for all the vertices of the current face
        for (int j = 0; j < 3; j++)
        {
            vec3_t transformed_vertex = vec3_rotate_x(face_vertices[j], mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            transformed_vertex.z += 5;

            transformed_vertices[j] = transformed_vertex;
        }

        // back face culling (display the face or not)
        if (!should_render_face(transformed_vertices, camera_position))
        {
            continue;
        }

        for (int j = 0; j < 3; j++)
        {
            vec2_t projected_point = project(transformed_vertices[j]);
            projected_point.x += (window_width / 2);
            projected_point.y += (window_height / 2);

            projected_triangle.points[j] = projected_point;
        }

        array_push(triangles_to_render, projected_triangle);
    }
}

void render(void)
{
    draw_grid();

    int number_triangles = array_length(triangles_to_render);

    for (int i = 0; i < number_triangles; i++)
    {
        triangle_t triangle = triangles_to_render[i];

        // draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, 0xFF00FF00);
        // draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, 0xFF00FF00);
        // draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, 0xFF00FF00);
        // draw_filled_triangle(triangle.points[0].x,
        //               triangle.points[0].y,
        //               triangle.points[1].x,
        //               triangle.points[1].y,
        //               triangle.points[2].x,
        //               triangle.points[2].y,
        //               0xFFB01E00);

        draw_triangle(triangle.points[0].x,
                      triangle.points[0].y,
                      triangle.points[1].x,
                      triangle.points[1].y,
                      triangle.points[2].x,
                      triangle.points[2].y,
                      0xFF2DFCAD);
    }

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);

    array_free(triangles_to_render);
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

    free_resources();

    destroy_window();

    return 0;
}
