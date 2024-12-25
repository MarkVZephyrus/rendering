/*
 * Seems to work.
 *
 */

#include "mathl.h"

// CAMERA DATA
float CAMPositon[3] = {5.0f, 5.0f, 5.0f}; // Rando position.
// Angles
float a = 0.6154;
float b = 0.7853;
float g = 0;

// GLOBALS
matrix transform;     // the magic sauce
vec3 translate, n, t; // camera position, direction of the camera, camera tilt.
float d = 10.0f;      // distance between the camera and the viewer.
float **vertices;     // the vertex buffer.

int getProjected(vec3 *coordinates, vec3 *ret);
float **generateRect(float length, float breadth, float height, // Dimensions
                     float x, float y, float z);                // Position

int main() {
  // INITIALIZATIONS

  create_vectori(&translate, CAMPositon);

  scale_vector(&translate, -1.0,
               &translate); // to be removed from the coordinates to shift.

  // I can see my future self scratching my head at these 2 initializations.
  // I shall hopefully write a detailed explaination of my method.
  eulerVector(a, b, &n);
  // create_vector(&n, 1.0f, 1.0f, 1.0f);
  normalize_vector(&n, &n);
  print_vector(&n);

  eulerVector(-a, b + g, &t);
  // create_vector(&t, 1.0f, 0.0f, 0.0f);
  normalize_vector(&t, &t);
  print_vector(&t);

  // CREATE THE TRANSFORM MATRIX
  // uses the resluting basis vector approach.
  vec3 z;
  create_vector(&z, 0.0f, 0.0f, 0.0f);
  scale_vector(&n, 1.0f, &z); // maybe I should implement an equals function.

  vec3 y;
  create_vector(&y, 0.0f, 0.0f, 0.0f);
  crossprod_vector(&n, &t, &y);
  scale_vector(&y, 1.0f, &y); // maybe I should implement an equals function.
  normalize_vector(&y, &y);

  vec3 x;
  create_vector(&x, 0.0f, 0.0f, 0.0f);
  crossprod_vector(&z, &y, &x);
  normalize_vector(&x, &x);

  constructTransform_matrix(&z, &x, &y, &transform);

  // PROJECT
  // As you will see projecting the origin helps.
  vec3 origin;
  create_vector(&origin, 0.0f, 0.0f, 0.0f);
  getProjected(&origin, &origin);
  puts("Origin:");
  print_vector(&origin);

  puts("_______________________________________");

  float vertices_A[5][3] = {
      {0, 1, 0}, {1, 1, -1}, {1, -1, -1}, {-1, -1, -1}, {-1, 1, -1}};
  // vertices = generateRect(1, 1, 1, 0, 0, 0);
  vec3 vertex;

  vec3 z_removal;
  create_vector(&z_removal, 0.0f, 0.0f, -origin.coord.z);

  puts("\nVertices:");
  for (int i = 0; i != 5; i++) {
    create_vectori(&vertex, vertices_A[i]);
    // print_vector(&vertex);
    getProjected(&vertex, &vertex);
    sum_vector(&vertex, &z_removal,
               &vertex); // we do not care about the complete z coordinate. Or
                         // maybe we do, not sure yet.
    printf("%c=(%.2f,%.2f)\n", 'A' + i, vertex.coord.x, vertex.coord.y);
  }

  puts("_______________________________________");

  /* puts("\nAxes (shifted):"); */
  /**/
  /* // the new basis vectors, keep in mind they have been shifted. */
  /* vec3 axis; */
  /* vec3 out; */
  /* // x-axis */
  /* create_vector(&axis, 1.0f, 0.0f, 0.0f); */
  /* getProjected(&axis, &out); */
  /* print_vector(&out); */
  /* // y-axis */
  /* create_vector(&axis, 0.0f, 1.0f, 0.0f); */
  /* getProjected(&axis, &out); */
  /* print_vector(&out); */
  /* // z-axis */
  /* create_vector(&axis, 0.0f, 0.0f, 1.0f); */
  /* getProjected(&axis, &out); */
  /* print_vector(&out); */

  return OK;
}

float __sq(float number) { return (number * number); }
float __invmag(vec3 *n) {
  return quake_FISR(__sq(n->coord.x) + __sq(n->coord.y) + __sq(n->coord.z));
}

int scale(vec3 *v, vec3 *ret) {
  float _vz = v->coord.z;
  float _cz = -n.coord.z * (1 / __invmag(&translate));
  float _vi, _ci, mag_c, _nc, det_vc, ns_cv, _pi, _pz;
  create_vector(ret, 0.00f, 0.00f, 0.00f);
  for (int i = 0; i != 2; i++) {
    _vi = v->vec3_i[i];
    _ci = n.vec3_i[i] * (1 / __invmag(&translate));

    mag_c = quake_FISR(_ci * _ci + _cz * _cz);
    det_vc = _vi * _cz + _vz * _ci;
    _nc = (1 / mag_c) - (mag_c * (_vz * _vz + _vi * _vi));
    ns_cv = (_cz - _vz) / (_ci - _vi);

    _pi = (det_vc / (_cz + (_ci * ns_cv))) * ((d / _nc) + (_ci / (_ci - _vi)));

    // printf("%f, %f, %f, %f, %f, %f, %f, %f, %f\n", _vz, _cz, _vi, _ci, mag_c,
    // _nc, det_vc,ns_cv,_pi);
    if (_pi != 0) {
      _pz = (_pi * (_cz - _vz) - det_vc) / (_ci * _vi);
      ret->coord.z = -_pz;
      // printf("%f ", _pz);
    }
    ret->vec3_i[i] = _pi;
  }
  // putchar('\n');
  // printf("(%.2f,%.2f,%.2f)\n", ret->coord.x, ret->coord.y, ret->coord.z);
  // printf("(%.2f,%.2f,%.2f)\n", v->coord.x, v->coord.y, v->coord.z);
  return OK; // kill me
}

int getProjected(vec3 *coordinates, vec3 *ret) {
  // Scale
  vec3 v_tl;
  /* float zcs = */
  /*     fov / (translate.coord.z - coordinates->coord.z); // = 1 for
   * orthogonal. */
  /* float x = coordinates->coord.x; */
  /* float y = coordinates->coord.y; */
  /* float z = coordinates->coord.z; */
  /* create_vector(&v_s, x * zcs, y * zcs, z); */
  // scale(coordinates, &v_s);
  // Transform
  // vec3 v_tf;
  // transform_vector(&transform, &v_s, &v_tf);

  // Translate
  sum_vector(coordinates, &translate, &v_tl);
  vec3 v_tf;
  transform_vector(&transform, &v_tl, &v_tf);
  float zcs = 1; //-d / v_tf.coord.z;
  create_vector(ret, v_tf.coord.x / zcs, v_tf.coord.y / zcs, v_tf.coord.z);
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
