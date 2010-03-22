#include <stdio.h>
#include "utarray.h"

int main() {
  UT_array *strs;
  utarray_new(strs, &ut_str_icd);
  utarray_free(strs);
  return 0;
}
