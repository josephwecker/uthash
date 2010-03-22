#include <stdio.h>
#include "utarray.h"

int reverse(const void *a,const void*b) {
    int _a = *(int*)a;
    int _b = *(int*)b;
    return _b - _a;
}

int main() {
  UT_array *a;
  int i, *p;
  utarray_new(a, &ut_int_icd);
  for(i=0;i<10;i++) utarray_push_back(a,&i);
  for(p=utarray_front(a); p; p=utarray_next(a,p)) printf("%d ",*p); 
  printf("\n");
  utarray_sort(a,reverse);
  while ( (p=utarray_next(a,p))) printf("%d ", *p);
  printf("\n");
  utarray_erase(a,3,3);
  while ( (p=utarray_next(a,p))) printf("%d ", *p);
  printf("\n");
  utarray_erase(a,1,2);
  while ( (p=utarray_next(a,p))) printf("%d ", *p);
  printf("\n");
  utarray_erase(a,0,1);
  while ( (p=utarray_next(a,p))) printf("%d ", *p);
  printf("\n");
  utarray_erase(a,3,1);
  while ( (p=utarray_next(a,p))) printf("%d ", *p);
  printf("\n");
  utarray_erase(a,0,3);
  while ( (p=utarray_next(a,p))) printf("%d ", *p);
  printf("\n");
  utarray_free(a);
  return 0;
}

