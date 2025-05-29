#include "./mesh.h"
#include "array.h"
#include <stdio.h>
#include <string.h>

mesh_t mesh = {.vertices = NULL, .faces = NULL, .rotation = {0, 0, 0}, .scale = {1, 1, 1}, .translation = {0, 0, 0}};

void load_obj_data(char *file_src) {
  FILE *file = fopen(file_src, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file %s! (check the path?)\n", file_src);
    return;
  }

  char buffer[128];
  while (fgets(buffer, sizeof(buffer), file)) {

    if (strncmp(buffer, "v ", 2) == 0) {
      vec3_t vertex;
      sscanf(buffer, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      array_push(mesh.vertices, vertex);

    } else if (strncmp(buffer, "f ", 2) == 0) {
      int32_t vertex_indices[3];
      int32_t texture_indices[3];
      int32_t normal_indices[3];

      sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertex_indices[0],
             &texture_indices[0], &normal_indices[0], &vertex_indices[1],
             &texture_indices[1], &normal_indices[1], &vertex_indices[2],
             &texture_indices[2], &normal_indices[2]);

      face_t face = {vertex_indices[0], vertex_indices[1], vertex_indices[2]};
      array_push(mesh.faces, face);
    }
  }
  fclose(file);
}
