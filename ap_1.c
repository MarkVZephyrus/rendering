/*  shitcode do not use.
 *  I will keep this around for sentimental value.
 *  The idea was to reverse rotate the coordinates using rotation matrices.
 *  The output is bullshit.
 */

#include "matrix.h"

#define DIMENSIONS 3 // No more than 3 fucking dimensions.

// CAMERA
float CAMNormal_Raw[DIMENSIONS] = {-1.0f, -1.0f, -1.0f};
float CAMPosition[DIMENSIONS] = {5.0f, 5.0f, 5.0f};
float persp = 5.0f;

// COORDINATE
float **vertices;

// GLOBALS
matrix Ry_Rz;
vec3 n;
vec3 translate;
float fov;

int getProjected(const vec3 *v, vec3 *v_P);
float **generateCube(float side);

int main() {
  create_vectori(&n, CAMNormal_Raw);
  normalize_vector(&n, &n);

  // TRIGGO
  float sqs_ac = n.coord.x * n.coord.x + n.coord.z + n.coord.z;
  float sqs_abc =
      n.coord.x * n.coord.x + n.coord.y * n.coord.y + n.coord.z + n.coord.z;
  float SINA = n.coord.x * quake_FISR(sqs_ac);
  float SINB = n.coord.y * quake_FISR(sqs_abc);
  float COSA = n.coord.z * quake_FISR(sqs_ac);
  float COSB = quake_FISR(sqs_abc) /
               quake_FISR(sqs_ac); // Seems good enough, I just do not want to
                                   // import the math library.

  // ROTATION TRANSFORM
  float Ry_Rz_a[3][3] = {
      // maybe this can be declared a better way.
      {COSA * COSB, COSA * SINB, SINA},          // fuck
      {-SINB, COSB, 0.0F},                       // my
      {-1 * SINA * COSB, -1 * SINA * COSB, COSA} // formatter.
  };
  create_matrix(3, 3, &Ry_Rz);
  assign_matrix(&Ry_Rz, Ry_Rz_a);

  // PROJECTION SCALE
  fov = persp / n.coord.z;

  // TRANSLATE VECTOR
  create_vectori(&translate, CAMPosition); // The fuck LOL!?
  scale_vector(&translate, -1.0f, &translate);

  // VERTICES
  vertices = generateCube(1.0f);

  // PROJECT
  vec3 origin;
  create_vector(&origin, 0.0f, 0.0f, 0.0f);
  vec3 origin_P; // Should remain zero
  create_vector(&origin_P, 1.0f, 2.0f,
                3.0f); // to make sure something happened.
  getProjected(&origin, &origin_P);
  print_vector(&origin_P);
  puts("___________________________________________________________");
  vec3 vertex;
  vec3 vertex_p;
  for (int i = 0; i != 8; i++) {
    create_vectori(&vertex, vertices[i]);
    getProjected(&vertex, &vertex_p);
    print_vector(&vertex_p);
  }

  return 0;
}

int getProjected(const vec3 *v, vec3 *v_P) {
  // Translate
  vec3 v_t;
  create_vector(&v_t, 0.0f, 0.0f, 0.0f);
  sum_vector(v, &translate, &v_t);

  // Create Transform
  matrix transform;
  create_matrix(3, 3, &transform);
  scale_matrix(&Ry_Rz, fov, &transform);

  // Final Transformation
  create_vector(v_P, 0.0f, 0.0f, 0.0f);
  transform_vector(&transform, &v_t, v_P);

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
