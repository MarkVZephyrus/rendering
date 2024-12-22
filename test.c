#include "mathl.h"

int main() {
  float a = 62;
  printf("%f, cos = %f, sin = %f\n\n", a, cos_apx(a), sin_apx(a));
  return OK;
}
