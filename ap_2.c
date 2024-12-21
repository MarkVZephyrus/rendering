#include "matrix.h"

// CAMERA DATA
float CAMPositon[3] = {5.0f, 5.0f, 5.0f};    // Initialize Bitch
float CAMNormal_Raw[3] = {1.0f, 1.0f, 1.0f}; // Initialize Bitch
float CAMTilt_Raw[3] = {1.0f, 0.0f, 0.0f};   // Initialize Bitch

// GLOBALS
matrix transform;
vec3 translate, n, t;
float project;
float **vertices;

int getTransform(matrix *ret);
int getProjected(vec3 *coordinates, vec3 *ret);
float **generateCube(float side);

int main() {
  create_vectori(&translate, CAMPositon);
  scale_vector(&translate, -1.0, &translate);
  create_vectori(&n, CAMNormal_Raw);
  normalize_vector(&n, &n);
  create_vectori(&t, CAMTilt_Raw);
  normalize_vector(&t, &t);
  project = 10;

  vec3 z;
  create_vector(&z, 0.0f, 0.0f, 0.0f);
  scale_vector(&n, -1.0f, &z);

  vec3 y;
  create_vector(&y, 0.0f, 0.0f, 0.0f);
  crossprod_vector(&n, &t, &y);
  normalize_vector(&y, &y);

  vec3 x;
  create_vector(&x, 0.0f, 0.0f, 0.0f);
  crossprod_vector(&z, &y, &x);
  normalize_vector(&x, &x);

  constructTransform_matrix(&x, &y, &z, project, &transform);
  getTransform(&transform);

  vec3 origin;
  create_vector(&origin, 0.0f, 0.0f, 0.0f);
  vec3 projected;
  create_vector(&projected, 1.0f, 2.0f, 3.0f);
  getProjected(&origin, &projected);
  print_vector(&projected);
  puts("_______________________________________");

  vertices = generateCube(1.0f);
  vec3 vertex;
  vec3 z_removal;
  create_vector(&z_removal, 0.0f, 0.0f, -projected.coord.z);
  for (int i = 0; i != 8; i++) {
    create_vectori(&vertex, vertices[i]);
    getProjected(&vertex, &vertex);
    sum_vector(&vertex, &z_removal, &vertex);
    print_vector(&vertex);
  }

  puts("_______________________________________");
  vec3 axis;
  create_vector(&axis, 1.0f, 0.0f, 0.0f);
  vec3 out;
  getProjected(&axis, &out);
  print_vector(&out);
  create_vector(&axis, 0.0f, 1.0f, 0.0f);
  getProjected(&axis, &out);
  print_vector(&out);
  create_vector(&axis, 0.0f, 0.0f, 1.0f);
  getProjected(&axis, &out);
  print_vector(&out);

  return OK;
}

int getTransform(matrix *ret) {
  vec3 z;
  create_vector(&z, 0.0f, 0.0f, 0.0f);
  scale_vector(&n, -1.0f, &z);

  vec3 y;
  create_vector(&y, 0.0f, 0.0f, 0.0f);
  crossprod_vector(&n, &t, &y);
  normalize_vector(&y, &y);

  vec3 x;
  create_vector(&x, 0.0f, 0.0f, 0.0f);
  crossprod_vector(&z, &y, &x);
  normalize_vector(&x, &x);

  constructTransform_matrix(&x, &y, &z, project, ret);

  return OK;
}

int getProjected(vec3 *coordinates, vec3 *ret) {

  // Translate
  vec3 v_t;
  create_vector(&v_t, 0.0f, 0.0f, 0.0f);
  sum_vector(coordinates, &translate, &v_t);

  // Depth Scaling
  float zcs = 1 / v_t.coord.z;

  // Transform
  create_vector(ret, 0.0f, 0.0f, 0.0f);
  transform_vector(&transform, &v_t, ret);
  scale_vector(ret, zcs, ret);

  return OK;
}

// splendid solution that I found on the internet.
// https://catonif.github.io/cube/
// Cube position can be easily implemented.
float **generateCube(float side) {
  float **vertices;
  float c = side * 0.5;
  vertices = malloc(8 * sizeof(float *));
  for (int i = 0; i != 8; i++) {
    vertices[i] = malloc(3 * sizeof(float));
    vertices[i][0] = (i & 4) != 0 ? -c : c;
    vertices[i][1] = (i & 2) != 0 ? -c : c;
    vertices[i][2] = (i & 1) != 0 ? -c : c;
  }
  return vertices;
}
