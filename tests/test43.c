#include <stdio.h>
#include "utarray.h"

typedef struct {
    int a;
    int b;
} intpair_t;

UT_icd pairicd = { sizeof(intpair_t),NULL,NULL,NULL};

int main() {
  UT_array *pairs, *pairs_cpy;
  intpair_t it, *ip;
  utarray_new(pairs, &pairicd);
  printf("length is %d\n", utarray_len(pairs));
  it.a = 1; it.b=2; utarray_push_back(pairs, &it); printf("push\n");
  printf("length is %d\n", utarray_len(pairs));
  ip = utarray_back(pairs);
  printf("back is %d %d\n", ip->a, ip->b);
  utarray_pop_back(pairs); printf("pop\n");
  printf("length is %d\n", utarray_len(pairs));
  it.a = 1; it.b=2; utarray_push_back(pairs, &it); printf("push\n");
  it.a = 3; it.b=4; utarray_push_back(pairs, &it); printf("push\n");
  printf("length is %d\n", utarray_len(pairs));
  ip=NULL;
  while( (ip=utarray_next(pairs,ip))) printf("%d %d\n", ip->a, ip->b);
  utarray_erase(pairs,0,1); printf("erase [0]\n");
  printf("length is %d\n", utarray_len(pairs));
  while( (ip=utarray_next(pairs,ip))) printf("%d %d\n", ip->a, ip->b);
  it.a = 1; it.b=2; utarray_push_back(pairs, &it); printf("push\n");
  while( (ip=utarray_next(pairs,ip))) printf("%d %d\n", ip->a, ip->b);
  utarray_clear(pairs); printf("clear\n");
  printf("length is %d\n", utarray_len(pairs));
  utarray_extend_back(pairs,ip); printf("extend\n");
  printf("length is %d\n", utarray_len(pairs));
  printf("ip points to [0] ? %s\n", (ip==utarray_front(pairs)) ? "yes" : "no");
  it.a = 1; it.b=2; utarray_push_back(pairs, &it); printf("push\n");
  ip=NULL;
  while( (ip=utarray_next(pairs,ip))) printf("%d %d\n", ip->a, ip->b);
  utarray_erase(pairs,1,1); printf("erase [1]\n");
  printf("length is %d\n", utarray_len(pairs));
  while( (ip=utarray_next(pairs,ip))) printf("%d %d\n", ip->a, ip->b);
  it.a = 3; it.b=4; utarray_push_back(pairs, &it); printf("push\n");
  for(ip=utarray_front(pairs);ip;ip=utarray_next(pairs,ip)) {
    printf("%d %d\n", ip->a,ip->b);
  }
  ip = utarray_back(pairs);
  printf("back is %d %d\n", ip->a, ip->b);
  utarray_new(pairs_cpy, &pairicd);
  utarray_cpy(pairs_cpy, pairs);  printf("copy\n");
  printf("cpy length is %d\n", utarray_len(pairs_cpy));
  ip=NULL;
  while( (ip=utarray_next(pairs_cpy,ip))) printf("cpy %d %d\n", ip->a, ip->b);
  it.a=5; it.b=6; utarray_insert(pairs_cpy, &it, 0); printf("insert cpy[0]\n");
  printf("cpy length is %d\n", utarray_len(pairs_cpy));
  while( (ip=utarray_next(pairs_cpy,ip))) printf("cpy %d %d\n", ip->a, ip->b);
  utarray_erase(pairs_cpy,0,2); printf("erase cpy [0] [1]\n");
  printf("cpy length is %d\n", utarray_len(pairs_cpy));
  while( (ip=utarray_next(pairs_cpy,ip))) printf("cpy %d %d\n", ip->a, ip->b);
  utarray_inserta(pairs_cpy, pairs, 1); printf("inserta at cpy[1]\n");
  printf("cpy length is %d\n", utarray_len(pairs_cpy));
  while( (ip=utarray_next(pairs_cpy,ip))) printf("cpy %d %d\n", ip->a, ip->b);
  utarray_free(pairs_cpy); printf("free cpy\n");
  printf("length is %d\n", utarray_len(pairs));
  utarray_free(pairs); printf("free\n");
  return 0;
}
