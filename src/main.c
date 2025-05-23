#include "./array.h"
#include "./display.h"
#include "./mesh.h"
#include "./vector.h"

bool is_running = false;
bool enable_backface_culling = true;
float fov_factor = 900; // our scalar
triangle_t *triangles_to_render = NULL;

uint32_t previous_frame_time = 0;
uint32_t time_to_wait = 0;

vec3_t camera_pos = {0, 0, 0};

void setup(void) {
  color_buffer = malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);
  load_obj_data("./assets/cube.obj");
}

vec2_t project(vec3_t point) {
  vec2_t projected_point = {(fov_factor * point.x) / point.z,
                            (fov_factor * point.y) / point.z};
  return projected_point;
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    is_running = false;
    break;

  // TODO:
  // Pressing “F1” displays the wireframe and a small red dot for each triangle vertex
  // Pressing “F2” displays only the wireframe lines
  // Pressing “F3” displays filled triangles with a solid color
  // Pressing “F4” displays both filled triangles and wireframe lines
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      is_running = false;
      break;
    }

    if (event.key.keysym.sym == SDLK_F1) {
        // TODO: Do something
        break;
    }

    if (event.key.keysym.sym == SDLK_F2) {
        // TODO: Do something
        break;
    }

    if (event.key.keysym.sym == SDLK_F3) {
        // TODO: Do something
        break;
    }

    if (event.key.keysym.sym == SDLK_F4) {
        // TODO: Do something
        break;
    }

    if (event.key.keysym.sym == SDLK_F5) {
        (enable_backface_culling) ? (enable_backface_culling = false) :
        (enable_backface_culling = true);
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

  mesh.rotation.x += 0.01;
  mesh.rotation.y += 0.01;
  mesh.rotation.z += 0.01;

  uint32_t num_faces = array_length(mesh.faces);

  for (size_t i = 0; i < num_faces; i++) {
    face_t current_mesh_face = mesh.faces[i];
    vec3_t face_vertices[3];

    face_vertices[0] = mesh.vertices[current_mesh_face.a - 1];
    face_vertices[1] = mesh.vertices[current_mesh_face.b - 1];
    face_vertices[2] = mesh.vertices[current_mesh_face.c - 1];

    vec3_t transformed_vertices[3];

    // loop all three vertices of the current face and transform them
    for (size_t j = 0; j < 3; j++) {
      vec3_t transformed_vertex = face_vertices[j];
      transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

      // translate vertex away from cam
      transformed_vertex.z += 5;
      transformed_vertices[j] = transformed_vertex;
    }

    if (enable_backface_culling) {
      // check backface culling
      vec3_t vector_a = transformed_vertices[0]; /*    A    */
      vec3_t vector_b = transformed_vertices[1]; /*   / \   */
      vec3_t vector_c = transformed_vertices[2]; /*  C  B   */

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

    triangle_t projected_triangle;
    // loop all 3 vertices to perform projection
    for (size_t j = 0; j < 3; j++) {
      // project current vertex
      vec2_t projected_point = project(transformed_vertices[j]);

      // scale and translate projected points to middle of screen
      projected_point.x += (uint16_t)(WINDOW_WIDTH / 2);
      projected_point.y += (uint16_t)(WINDOW_HEIGHT / 2);

      projected_triangle.points[j] = projected_point;
    }

    // save the projected triangle in the array of triangles to render
    array_push(triangles_to_render, projected_triangle);
  }
}

void render(void) {
  draw_grid();

  // loop all projected triangles and render them
  uint32_t num_of_triangles = array_length(triangles_to_render);

  for (size_t i = 0; i < num_of_triangles; i++) {
    triangle_t current_triangle = triangles_to_render[i];

    // TODO: Switch statements here? think about how to sep each rendering mode?
    draw_filled_triangle(
        current_triangle.points[0].x, current_triangle.points[0].y,
        current_triangle.points[1].x, current_triangle.points[1].y,
        current_triangle.points[2].x, current_triangle.points[2].y, 0xFFFFFFFF);

    draw_triangle(current_triangle.points[0].x, current_triangle.points[0].y,
                  current_triangle.points[1].x, current_triangle.points[1].y,
                  current_triangle.points[2].x, current_triangle.points[2].y,
                  0xFF000000);
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
