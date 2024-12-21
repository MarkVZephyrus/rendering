#ifndef MATRIX_H
#define MATRIX_H
#include <stdio.h>
#include <stdlib.h>
#define OK 0
#define ERR -1

float quake_FISR(float number);

typedef struct matrix {
  float **mat;
  int r;
  int c;
} matrix;
int create_matrix(int r, int c, matrix *res);
int print_matrix(const matrix *res);
int assign_matrix(matrix *out, float inp[out->r][out->c]);
int transpose_matrix(const matrix *in, matrix *out);
int multiply_matrix(const matrix *a, const matrix *b, matrix *out);
int identity_matrix(matrix *out);
int sum_matrix(const matrix *a, const matrix *b, matrix *out);
int scale_matrix(const matrix *a, float scalar, matrix *out);

// Saw this "hack" to make "struct"s "iterable". A lot of quotes because I am
//  unsure about this approach;
typedef union vec3_u {
  struct vec3_c {
    float x, y, z;
  } coord;
  float vec3_i[3];
} vec3;
// I am not sure if it would be better to return a vec3 datatype. Would
// definitely make my life easier.
int create_vector(vec3 *a, float x, float y, float z);
int create_vectori(vec3 *a, float *Vec);
float dotprod_vector(const vec3 *a, const vec3 *b);
int crossprod_vector(const vec3 *a, const vec3 *b, vec3 *out);
int transform_vector(const matrix *T, const vec3 *V, vec3 *out);
int normalize_vector(const vec3 *raw, vec3 *out);
int print_vector(const vec3 *a);
int sum_vector(const vec3 *a, const vec3 *b, vec3 *out);
int scale_vector(const vec3 *a, float scalar, vec3 *out);

int constructTransform_matrix(const vec3 *x, const vec3 *y, const vec3 *z,
                              float scale, matrix *transform);

#endif
