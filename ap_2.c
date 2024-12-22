/*
 * Seems to work.
 *
 */

#include "mathl.h"

// CAMERA DATA
float CAMPositon[3] = {5.0f, 5.0f, 5.0f}; // Rando position.
// Angles
float a = PI5 / 4;
float b = PI5 / 4;
float g = PI5 / 8;

// GLOBALS
matrix transform;     // the magic sauce
vec3 translate, n, t; // camera position, direction of the camera, camera tilt.
float fov = 5.0f;     // distance between the camera and the viewer.
float **vertices;     // the vertex buffer.

int getProjected(vec3 *coordinates, vec3 *ret);
float **generateCube(float side);

int main() {
  // INITIALIZATIONS

  create_vectori(&translate, CAMPositon);

  scale_vector(&translate, -1.0,
               &translate); // to be removed from the coordinates to shift.

  // I can see my future self scratching my head at these 2 initializations.
  // I shall hopefully write a detailed explaination of my method.
  eulerVector(a, b, &n);
  normalize_vector(&n, &n);

  eulerVector(-a, b + g, &t);
  normalize_vector(&t, &t);

  // CREATE THE TRANSFORM MATRIX
  // uses the resluting basis vector approach.
  vec3 z;
  create_vector(&z, 0.0f, 0.0f, 0.0f);
  scale_vector(&n, 1.0f, &z); // maybe I should implement an equals function.

  vec3 y;
  create_vector(&y, 0.0f, 0.0f, 0.0f);
  crossprod_vector(&n, &t, &y);
  normalize_vector(&y, &y);

  vec3 x;
  create_vector(&x, 0.0f, 0.0f, 0.0f);
  crossprod_vector(&z, &y, &x);
  normalize_vector(&x, &x);

  constructTransform_matrix(&x, &y, &z, &transform);

  // PROJECT
  // As you will see projecting the origin helps.
  vec3 origin;
  create_vector(&origin, 0.0f, 0.0f, 0.0f);
  getProjected(&origin, &origin);
  puts("Origin:");
  print_vector(&origin);

  puts("_______________________________________");

  vertices = generateCube(1.0f);
  vec3 vertex;

  vec3 z_removal;
  create_vector(&z_removal, 0.0f, 0.0f, -origin.coord.z);

  puts("\nVertices:");
  for (int i = 0; i != 8; i++) {
    create_vectori(&vertex, vertices[i]);
    getProjected(&vertex, &vertex);
    sum_vector(
        &vertex, &z_removal,
        &vertex); // we do not care about the complete z coordinate. Or maybe we
                  // do, not sure yet. The z coordinate is visibly nonsensical.
    putchar('A' + i);
    print_vector(&vertex);
  }

  puts("_______________________________________");

  puts("\nAxes (shifted):");

  // the new basis vectors, keep in mind they have been shifted.
  vec3 axis;
  vec3 out;
  // x-axis
  create_vector(&axis, 1.0f, 0.0f, 0.0f);
  getProjected(&axis, &out);
  print_vector(&out);
  // y-axis
  create_vector(&axis, 0.0f, 1.0f, 0.0f);
  getProjected(&axis, &out);
  print_vector(&out);
  // z-axis
  create_vector(&axis, 0.0f, 0.0f, 1.0f);
  getProjected(&axis, &out);
  print_vector(&out);

  return OK;
}

int getProjected(vec3 *coordinates, vec3 *ret) {

  // Transform
  vec3 v_tf;
  transform_vector(&transform, coordinates, &v_tf);

  // Translate
  sum_vector(&v_tf, &translate, ret);
  float zcs = fov / ret->coord.z;

  // Scale
  scale_vector(ret, 1.0f, ret);

  return OK;
}

// splendid solution that I found on the internet.
// https://catonif.github.io/cube/
// Cube position can be easily implemented.
// The vertices of a cube centered at the origin with side 1 is odly reminicent
// of the binary numbers 0-7.
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
