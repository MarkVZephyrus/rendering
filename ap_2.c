/*
 * Seems to work.
 *
 */

#include "mathl.h"

// CAMERA DATA
float CAMPositon[3] = {5.0f, 2.0f, 2.0f}; // Rando position.
// Angles
float a = PI5 / 4;
float b = 0.0;
float g = 0.0;

// GLOBALS
matrix transform;     // the magic sauce
vec3 translate, n, t; // camera position, direction of the camera, camera tilt.
float fov = 5.0f;     // distance between the camera and the viewer.
float **vertices;     // the vertex buffer.

int getProjected(vec3 *coordinates, vec3 *ret, int debug);
float **generateRect(float length, float breadth, float height, float x,
                     float y, float z);

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
  getProjected(&origin, &origin, 0);
  puts("Origin:");
  print_vector(&origin);

  puts("_______________________________________");

  vertices = generateRect(5, 4, 2, 2.5, 2, 1);
  vec3 vertex;

  vec3 z_removal;
  create_vector(&z_removal, 0.0f, 0.0f, -origin.coord.z);

  puts("\nVertices:");
  for (int i = 0; i != 8; i++) {
    create_vectori(&vertex, vertices[i]);
    getProjected(&vertex, &vertex, 0);
    sum_vector(&vertex, &z_removal,
               &vertex); // we do not care about the complete z coordinate. Or
                         // maybe we do, not sure yet.
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
  getProjected(&axis, &out, 0);
  print_vector(&out);
  // y-axis
  create_vector(&axis, 0.0f, 1.0f, 0.0f);
  getProjected(&axis, &out, 0);
  print_vector(&out);
  // z-axis
  create_vector(&axis, 0.0f, 0.0f, 1.0f);
  getProjected(&axis, &out, 0);
  print_vector(&out);

  return OK;
}

int getProjected(vec3 *coordinates, vec3 *ret, int debug) {
  if (debug)
    print_vector(coordinates);
  // Scale
  vec3 v_s;
  float zcs = fov / (coordinates->coord.z - fov); // = 1 for orthogonal.
  create_vector(&v_s, zcs * coordinates->coord.x, zcs * coordinates->coord.y,
                coordinates->coord.z);
  if (debug)
    print_vector(&v_s);
  // Transform
  vec3 v_tf;
  transform_vector(&transform, &v_s, &v_tf);
  if (debug)
    print_vector(&v_tf);

  // Translate
  sum_vector(&v_tf, &translate, ret);
  if (debug)
    print_vector(ret);
  return OK;
}

// splendid solution that I found on the internet.
// https://catonif.github.io/cube/
// Cube position can be easily implemented.
// The vertices of a cube centered at the origin with side 1 is odly reminicent
// of the binary numbers 0-7.
float **generateRect(float length, float breadth, float height, float x,
                     float y, float z) {
  float **vertices;
  length *= 0.5;
  breadth *= 0.5;
  height *= 0.5;
  vertices = malloc(8 * sizeof(float *));
  for (int i = 0; i != 8; i++) {
    vertices[i] = malloc(3 * sizeof(float));
    vertices[i][0] = ((i & 4) != 0 ? -length : length) + x;
    vertices[i][1] = ((i & 2) != 0 ? -breadth : breadth) + y;
    vertices[i][2] = ((i & 1) != 0 ? -height : height) + z;
  }
  return vertices;
}
