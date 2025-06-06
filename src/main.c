#include "../math/matrix.h"
#include "../math/vector.h"
#include "./array.h"
#include "./display.h"
#include "./mesh.h"

// FIXME: Currently takes 4 seconds to run the program?

#define MAX_FILE_LENGTH 150

bool is_running = false;
bool enable_backface_culling = true;
uint8_t render_modes = 1;
mat4_t proj_matrix;
triangle_t *triangles_to_render = NULL;
uint32_t previous_frame_time = 0;
uint32_t time_to_wait = 0;
vec3_t camera_pos = {0, 0, 0};

void setup(void) {
  color_buffer = malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);

  // init perspective projection matrix
  float fov = 3.14159 / 3.0; // 180 / 3 (60 deg)
  float aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
  float z_near = 0.1;
  float z_far = 100.0;
  proj_matrix = mat4_perspective(fov, aspect, z_near, z_far);

  FILE *f = fopen("./assets/obj_data.txt", "r");
  if (!f) {
    fprintf(stderr, "Failed opening obj_data file!\n");
    exit(1);
  }

  char buffer[MAX_FILE_LENGTH];
  if (!fgets(buffer, sizeof(buffer), f)) {
    fprintf(stderr, "Failed to read the obj_data file!\n");
    fclose(f);
    exit(1);
  };

  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] != '\n') {
    fprintf(stderr, "Filename too long for buffer!\n");
    exit(1);
  }

  fclose(f);
  f = NULL;

  // very nice function. replaces the first '\n'
  // with a '\0'
  buffer[strcspn(buffer, "\n")] = 0;
  load_obj_data(buffer);
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    is_running = false;
    break;

  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      is_running = false;
      break;
    }

    if (event.key.keysym.sym == SDLK_F1) {
      render_modes = 1;
      break;
    }

    if (event.key.keysym.sym == SDLK_F2) {
      render_modes = 2;
      break;
    }

    if (event.key.keysym.sym == SDLK_F3) {
      render_modes = 3;
      break;
    }

    if (event.key.keysym.sym == SDLK_F4) {
      render_modes = 4;
      break;
    }

    if (event.key.keysym.sym == SDLK_F5) {
      (enable_backface_culling) ? (enable_backface_culling = false)
                                : (enable_backface_culling = true);
      break;
    }
  }
}

void update(void) {
  time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    SDL_Delay(time_to_wait);

  previous_frame_time = SDL_GetTicks();
  triangles_to_render = NULL;

  mesh.rotation.x += 0.03;
  mesh.rotation.y += 0.01;
  // mesh.rotation.z += 0.01;

  // mesh.scale.x += 0.002;
  // mesh.scale.y += 0.002;

  mesh.translation.x = 0.01;
  mesh.translation.y = 0.5;
  mesh.translation.z = 5;

  // scale, rotation, and translation matrix for multiplying mesh vertices
  mat4_t scale_m = mat4_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  mat4_t rot_m_x = mat4_rotation_x(mesh.rotation.x);
  mat4_t rot_m_y = mat4_rotation_y(mesh.rotation.y);
  mat4_t rot_m_z = mat4_rotation_z(mesh.rotation.z);
  mat4_t transl_m = mat4_translation(mesh.translation.x, mesh.translation.y,
                                     mesh.translation.z);

  uint32_t num_faces = array_length(mesh.faces);

  for (size_t i = 0; i < num_faces; i++) {
    face_t current_mesh_face = mesh.faces[i];
    vec3_t face_vertices[3];

    face_vertices[0] = mesh.vertices[current_mesh_face.a - 1];
    face_vertices[1] = mesh.vertices[current_mesh_face.b - 1];
    face_vertices[2] = mesh.vertices[current_mesh_face.c - 1];

    vec4_t transformed_vertices[3];

    // loop all three vertices of the current face and transform them
    for (size_t j = 0; j < 3; j++) {
      vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

      // Create world matrix, combines scale, rotation, and
      // translation matrices
      mat4_t world_matrix = mat4_identity();
      world_matrix = mat4_mul_mat4(scale_m, world_matrix);
      world_matrix = mat4_mul_mat4(rot_m_x, world_matrix);
      world_matrix = mat4_mul_mat4(rot_m_y, world_matrix);
      world_matrix = mat4_mul_mat4(rot_m_z, world_matrix);
      world_matrix = mat4_mul_mat4(transl_m, world_matrix);

      // Multiply world matrix by OG vector
      transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

      transformed_vertices[j] = transformed_vertex;
    }

    if (enable_backface_culling) {
      // check backface culling
      vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /*    A    */
      vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*   / \   */
      vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /*  C  B   */

      vec3_t vector_ab = vec3_sub(vector_b, vector_a);
      vec3_t vector_ac = vec3_sub(vector_c, vector_a);
      vec3_normalize(&vector_ab);
      vec3_normalize(&vector_ac);

      vec3_t normal = vec3_cross(vector_ab, vector_ac);
      // NOTE: it's normal to normalize the normal vector. perfectly normal.
      vec3_normalize(&normal);

      vec3_t camera_ray = vec3_sub(camera_pos, vector_a);

      // check if dot product is negative (out of view)
      float cam_norm_dot = vec3_dot(normal, camera_ray);
      if (cam_norm_dot < 0)
        continue;
    }

    vec4_t projected_point[3];

    // FIXME: Scaling issues

    // loop all 3 vertices to perform projection
    for (size_t j = 0; j < 3; j++) {
      // project current vertex
      projected_point[j] =
          mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

      // scale
      projected_point[j].x *= (uint16_t)(WINDOW_WIDTH / 2);
      projected_point[j].y *= (uint16_t)(WINDOW_HEIGHT / 2);

      // translate projected points to middle of screen
      projected_point[j].x += (uint16_t)(WINDOW_WIDTH / 2);
      projected_point[j].y += (uint16_t)(WINDOW_HEIGHT / 2);
    }

    float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z +
                       transformed_vertices[2].z / 3.0);

    triangle_t projected_triangle = {
        .points =
            {
                {projected_point[0].x, projected_point[0].y},
                {projected_point[1].x, projected_point[1].y},
                {projected_point[2].x, projected_point[2].y},
            },
        .avg_depth = avg_depth};
    // save the projected triangle in the array of triangles to render
    array_push(triangles_to_render, projected_triangle);
  }

  uint32_t arr_len = array_length(triangles_to_render);
  size_t i = 0;
  bool swapped = false;

  // bubble sort for triangles based on
  // avg_depth. sketchy temporary workaround
  // for having no z_index yet
  do {
    swapped = false;
    for (size_t j = 0; j < arr_len - 1 - i; j++) {
      if (triangles_to_render[j].avg_depth >
          triangles_to_render[j + 1].avg_depth) {
        float temp = triangles_to_render[j].avg_depth;
        triangles_to_render[j].avg_depth = triangles_to_render[j + 1].avg_depth;
        triangles_to_render[j + 1].avg_depth = temp;
        swapped = true;
      }
    }
    i++;
  } while (swapped);
}

void render(void) {
  draw_grid();

  // loop all projected triangles and render them
  uint32_t num_of_triangles = array_length(triangles_to_render);

  // NOTE: Rendering modes will fail to render in the desired z_order
  // as of right now, unless manually placed in this arrangement
  for (size_t i = 0; i < num_of_triangles; i++) {
    triangle_t current_triangle = triangles_to_render[i];

    // filled object
    if (render_modes == 3 || render_modes == 4) {
      draw_filled_triangle(
          current_triangle.points[0].x, current_triangle.points[0].y,
          current_triangle.points[1].x, current_triangle.points[1].y,
          current_triangle.points[2].x, current_triangle.points[2].y,
          0xFFFFFFFF);
    }
    // wireframe
    if (render_modes == 1 || render_modes == 2 || render_modes == 4) {
      draw_triangle(current_triangle.points[0].x, current_triangle.points[0].y,
                    current_triangle.points[1].x, current_triangle.points[1].y,
                    current_triangle.points[2].x, current_triangle.points[2].y,
                    0xFF00FF00);
    }
    // red vertex dots
    if (render_modes == 2) {
      draw_rectangle(current_triangle.points[0].x, current_triangle.points[0].y,
                     4, 4, 0xFFFF0000);
      draw_rectangle(current_triangle.points[1].x, current_triangle.points[1].y,
                     4, 4, 0xFFFF0000);
      draw_rectangle(current_triangle.points[2].x, current_triangle.points[2].y,
                     4, 4, 0xFFFF0000);
    }
  }

  // clear array since its redone every frame
  // NOTE: change soon 2025-05-10
  array_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  // double buffer
  SDL_RenderPresent(renderer);
}

void free_resources(void) {
  array_free(mesh.faces);
  array_free(mesh.vertices);
  free(color_buffer);
}

int main(void) {
  is_running = init_window();

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
  free_resources();
}
