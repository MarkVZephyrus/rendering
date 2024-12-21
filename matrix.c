// Taken help from https://github.com/ibrohimsub/s21_matrix. For create_matrix
// and print.
#include "matrix.h"

// Quake Fast Inverse Square, because I love stolen code.
float quake_FISR(float number) {
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y = number;
  i = *(long *)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float *)&i;
  y = y * (threehalfs - (x2 * y * y));
  y = y * (threehalfs - (x2 * y * y));

  return y;
}

int create_matrix(int r, int c, matrix *res) {
  res->r = r;
  res->c = c;
  res->mat = malloc(r * sizeof(float *));
  for (int i = 0; i != res->r; i++) {
    res->mat[i] = malloc(c * sizeof(float));
  }
  return OK;
}

int print_matrix(const matrix *a) {
  for (int i = 0; i != a->r; i++) {
    for (int j = 0; j != a->c; j++) {
      printf("%.2f\t", a->mat[i][j]);
    }
    puts("\n");
  }
  return OK;
}

int assign_matrix(matrix *out, float inp[out->r][out->c]) {
  // Make sure out is initailaized using create_matrix.
  for (int i = 0; i != out->r; i++) {
    for (int j = 0; j != out->c; j++) {
      out->mat[i][j] = inp[i][j];
    }
  }
  return OK;
}

int transpose_matrix(const matrix *in, matrix *out) {
  create_matrix(in->c, in->r, out);
  for (int i = 0; i != in->c; i++) {
    for (int j = 0; j != in->r; j++) {
      out->mat[i][j] = in->mat[j][i];
    }
  }
  return OK;
}

int multiply_matrix(const matrix *a, const matrix *b, matrix *out) {
  if (a->c != b->r)
    return ERR;
  create_matrix(a->r, b->c, out);
  float temp_sum = 0;
  for (int i = 0; i != a->r; i++) {
    for (int j = 0; j != b->c; j++) {
      for (int k = 0; k != b->r; k++) {
        temp_sum += a->mat[i][k] * b->mat[k][j];
      }
      out->mat[i][j] = temp_sum;
      temp_sum = 0;
    }
  }
  return OK;
}

int identity_matrix(matrix *out) {
  for (int i = 0; i != out->r; i++) {
    for (int j = 0; j != out->c; j++) {
      out->mat[i][j] = (i == j) ? 1 : 0;
    }
  }
  return OK;
}

int sum_matrix(const matrix *a, const matrix *b, matrix *out) {
  if (a->r != b->r && a->c != b->c)
    return ERR;
  for (int i = 0; i != a->r; i++) {
    for (int j = 0; j != b->c; j++) {
      out->mat[i][j] = a->mat[i][j] + b->mat[i][j];
    }
  }
  return OK;
}

int scale_matrix(const matrix *a, float scalar, matrix *out) {
  for (int i = 0; i != a->c; i++) {
    for (int j = 0; j != a->r; j++) {
      out->mat[i][j] = scalar * a->mat[i][j];
    }
  }
  return OK;
}

//__________________________________________________________________

int create_vector(vec3 *a, float x, float y, float z) {
  a->coord.x = x;
  a->coord.y = y;
  a->coord.z = z;
  return OK;
}

int create_vectori(vec3 *a, float *Vec) {
  for (int i = 0; i != 3; i++)
    a->vec3_i[i] = Vec[i];
  return OK;
}

float dotprod_vector(const vec3 *a, const vec3 *b) {
  float dot = 0;
  for (int i = 0; i != 3; i++) {
    dot += a->vec3_i[i] * b->vec3_i[i];
  }
  return dot;
}

int crossprod_vector(const vec3 *a, const vec3 *b, vec3 *out) {
  out->coord.x = a->coord.y * b->coord.z - a->coord.z * b->coord.y;
  out->coord.y = a->coord.z * b->coord.x - a->coord.x * b->coord.z;
  out->coord.z = a->coord.x * b->coord.y - a->coord.y * b->coord.x;
  return OK;
}

int transform_vector(const matrix *T, const vec3 *V, vec3 *out) {
  if (T->r != 3 && T->c != 3)
    return ERR;
  float Vec_M[3][1] = {{V->coord.x}, {V->coord.y}, {V->coord.z}};
  matrix vec_m;
  create_matrix(3, 1, &vec_m);
  assign_matrix(&vec_m, Vec_M);
  matrix result;
  multiply_matrix(T, &vec_m, &result);
  for (int i = 0; i != 3; i++) {
    out->vec3_i[i] = result.mat[i][0];
  }
  return OK;
}

int normalize_vector(const vec3 *raw, vec3 *out) {
  float sqsm = 0;
  for (int i = 0; i != 3; i++) {
    sqsm += raw->vec3_i[i] * raw->vec3_i[i];
  }
  float mag_inv = quake_FISR(sqsm);

  for (int i = 0; i != 3; i++)
    out->vec3_i[i] = raw->vec3_i[i] * mag_inv;
  return OK;
}

int print_vector(const vec3 *a) {
  printf("[x: %.2f\t y: %.2f\t z: %.2f]\n", a->coord.x, a->coord.y, a->coord.z);
  return OK;
}

int sum_vector(const vec3 *a, const vec3 *b, vec3 *out) {
  for (int i = 0; i != 3; i++)
    out->vec3_i[i] = a->vec3_i[i] + b->vec3_i[i];
  return OK;
}

int scale_vector(const vec3 *a, float scalar, vec3 *out) {
  for (int i = 0; i != 3; i++)
    out->vec3_i[i] = a->vec3_i[i] * scalar;
  return OK;
}

int constructTransform_matrix(const vec3 *x, const vec3 *y, const vec3 *z,
                              float scale, matrix *transform) {
  create_matrix(3, 3, transform);
  float buffer[3][3] = {{x->coord.x, y->coord.x, z->coord.x},
                        {x->coord.y, y->coord.y, z->coord.y},
                        {x->coord.z, y->coord.z, z->coord.z}};
  assign_matrix(transform, buffer);
  scale_matrix(transform, scale, transform);
  return OK;
}
