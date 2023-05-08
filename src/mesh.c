#include "mesh.h"

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1},  // 2
    {.x = 1, .y = 1, .z = -1},   // 3
    {.x = 1, .y = -1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = 1},   // 7
    {.x = -1, .y = -1, .z = 1},  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3, .color = 0xFFFF0000},
    {.a = 1, .b = 3, .c = 4, .color = 0xFFFF0000},
    // right
    {.a = 4, .b = 3, .c = 5, .color = 0xFF00FF00},
    {.a = 4, .b = 5, .c = 6, .color = 0xFF00FF00},
    // back
    {.a = 6, .b = 5, .c = 7, .color = 0xFF0000FF},
    {.a = 6, .b = 7, .c = 8, .color = 0xFF0000FF},
    // left
    {.a = 8, .b = 7, .c = 2, .color = 0xFFFFFF00},
    {.a = 8, .b = 2, .c = 1, .color = 0xFFFFFF00},
    // top
    {.a = 2, .b = 7, .c = 5, .color = 0xFFFF00FF},
    {.a = 2, .b = 5, .c = 3, .color = 0xFFFF00FF},
    // bottom
    {.a = 6, .b = 8, .c = 1, .color = 0xFF00FFFF},
    {.a = 6, .b = 1, .c = 4, .color = 0xFF00FFFF},
};

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {.x = 0, .y = 0, .z = 0}};

void load_cube_mesh_data(void)
{
    for (int i = 0; i < N_CUBE_VERTICES; i++)
    {
        array_push(mesh.vertices, cube_vertices[i]);
    }

    for (int i = 0; i < N_CUBE_FACES; i++)
    {
        array_push(mesh.faces, cube_faces[i]);
    }
}

void load_obj_file_data(char *filename)
{
    FILE *file = fopen(filename, "r");
    char current_line[1024];

    if (file != NULL)
    {
        while (fgets(current_line, 1024, file))
        {
            if (strncmp(current_line, "v ", 2) == 0)
            {
                vec3_t vertex;
                sscanf(current_line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                array_push(mesh.vertices, vertex);
            }

            if (strncmp(current_line, "f ", 2) == 0)
            {
                face_t face;
                int trash;

                sscanf(current_line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                       &face.a, &trash, &trash,
                       &face.b, &trash, &trash,
                       &face.c, &trash, &trash);

                array_push(mesh.faces, face);
            }
        }
    }
    else
    {
        printf("file doesn't exist");
    }

    fclose(file);
}