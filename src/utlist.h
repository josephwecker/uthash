/*
Copyright (c) 2007-2010, Troy D. Hanson
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef UTLIST_H
#define UTLIST_H

#define UTLIST_VERSION 1.9

/* 
 * This file contains macros to manipulate singly and doubly-linked lists.
 *
 * 1. LL_ macros:  singly-linked lists.
 * 2. DL_ macros:  doubly-linked lists.
 * 3. CDL_ macros: circular doubly-linked lists.
 *
 * To use singly-linked lists, your structure must have a "next" pointer.
 * To use doubly-linked lists, your structure must "prev" and "next" pointers.
 * Either way, the pointer to the head of the list must be initialized to NULL.
 * 
 * ----------------.EXAMPLE -------------------------
 * struct item {
 *      int id;
 *      struct item *prev, *next;
 * }
 *
 * struct item *list = NULL:
 *
 * int main() {
 *      struct item *item;
 *      ... allocate and populate item ...
 *      DL_APPEND(list, item);
 * }
 * --------------------------------------------------
 *
 * For doubly-linked lists, the append and delete macros are O(1)
 * For singly-linked lists, append and delete are O(n) but prepend is O(1)
 * The sort macro is O(n log(n)) for all types of single/double/circular lists.
 */

/* These macros use decltype or the earlier __typeof GNU extension.
   But decltype is only available in newer compilers (VS2010 or gcc 4.3+).
   For VS2008 where neither is available the code uses casting workarounds. */
#ifdef _MSC_VER         /* MS compiler */
#if _MSC_VER <= 1500    /* VS2008 or older */
#define NO_DECLTYPE
#define DECLTYPE(x) char*
#else                   /* VS2010+ */
#define DECLTYPE(x) decltype(x)
#endif
#else                   /* GNU, Sun and other compilers */
#define DECLTYPE(x) __typeof(x)
#endif

/******************************************************************************
 * The sort macro is an adaptation of Simon Tatham's O(n log(n)) mergesort    *
 * Unwieldy variable names used here to avoid shadowing passed-in variables.  *
 *****************************************************************************/
#ifdef NO_DECLTYPE
#define _SV(elt) _tmp = (char*)(list); (char*)(list)=(elt)
#define _NEXT(elt) ((char*)((list)->next))
#define _RS (char*)(list)=_tmp
#else 
#define _SV(elt)
#define _NEXT(elt) ((elt)->next)
#define _RS
#endif

#define LL_SORT(list, cmp)                             \
do {                                                                             \
  DECLTYPE(list) _ls_p, _ls_q, _ls_e, _ls_tail, _ls_oldhead, _tmp;              \
  int _ls_insize, _ls_nmerges, _ls_psize, _ls_qsize, _ls_i, _ls_looping;         \
  if (list) {                                                                    \
    _ls_insize = 1;                                                              \
    _ls_looping = 1;                                                             \
    while (_ls_looping) {                                                        \
      _ls_p = list;                                                              \
      _ls_oldhead = list;                                                        \
      list = NULL;                                                               \
      _ls_tail = NULL;                                                           \
      _ls_nmerges = 0;                                                           \
      while (_ls_p) {                                                            \
        _ls_nmerges++;                                                           \
        _ls_q = _ls_p;                                                           \
        _ls_psize = 0;                                                           \
        for (_ls_i = 0; _ls_i < _ls_insize; _ls_i++) {                           \
          _ls_psize++;                                                           \
          _SV(_ls_q); _ls_q = _NEXT(_ls_q); _RS; \
          if (!_ls_q) break;                                                     \
        }                                                                        \
        _ls_qsize = _ls_insize;                                                  \
        while (_ls_psize > 0 || (_ls_qsize > 0 && _ls_q)) {                      \
          if (_ls_psize == 0) {                                                  \
            _ls_e = _ls_q; _SV(_ls_q); _ls_q = _NEXT(_ls_q); _RS; _ls_qsize--;                 \
          } else if (_ls_qsize == 0 || !_ls_q) {                                 \
            _ls_e = _ls_p; _SV(_ls_p); _ls_p = _NEXT(_ls_p); _RS; _ls_psize--;                 \
          } else if (cmp(_ls_p,_ls_q) <= 0) {          \
            _ls_e = _ls_p; _SV(_ls_p); _ls_p = _NEXT(_ls_p); _RS; _ls_psize--;                 \
          } else {                                                               \
            _ls_e = _ls_q; _SV(_ls_q); _ls_q = _NEXT(_ls_q); _RS; _ls_qsize--;                 \
          }                                                                      \
          if (_ls_tail) {                                                        \
            _SV(_ls_tail); _NEXT(_ls_tail) = _ls_e; _RS;                              \
          } else {                                                               \
            list = _ls_e;                                           \
          }                                                                      \
          _ls_tail = _ls_e;                                                      \
        }                                                                        \
        _ls_p = _ls_q;                                                           \
      }                                                                          \
      _SV(_ls_tail); _NEXT(_ls_tail) = NULL; _RS;                                          \
      if (_ls_nmerges <= 1) {                                                    \
        _ls_looping=0;                                                           \
      }                                                                          \
      _ls_insize *= 2;                                                           \
    }                                                                            \
  }                                                                              \
} while (0)

#define DL_SORT(list, cmp)                             \
do {                                                                             \
  DECLTYPE(list) _ls_p, _ls_q, _ls_e, _ls_tail, _ls_oldhead;              \
  int _ls_insize, _ls_nmerges, _ls_psize, _ls_qsize, _ls_i, _ls_looping;         \
  if (list) {                                                                    \
    _ls_insize = 1;                                                              \
    _ls_looping = 1;                                                             \
    while (_ls_looping) {                                                        \
      _ls_p = list;                                                              \
      _ls_oldhead = list;                                                        \
      list = NULL;                                                               \
      _ls_tail = NULL;                                                           \
      _ls_nmerges = 0;                                                           \
      while (_ls_p) {                                                            \
        _ls_nmerges++;                                                           \
        _ls_q = _ls_p;                                                           \
        _ls_psize = 0;                                                           \
        for (_ls_i = 0; _ls_i < _ls_insize; _ls_i++) {                           \
          _ls_psize++;                                                           \
          _ls_q = _ls_q->next;                                             \
          if (!_ls_q) break;                                                     \
        }                                                                        \
        _ls_qsize = _ls_insize;                                                  \
        while (_ls_psize > 0 || (_ls_qsize > 0 && _ls_q)) {                      \
          if (_ls_psize == 0) {                                                  \
            _ls_e = _ls_q; _ls_q = _ls_q->next; _ls_qsize--;                 \
          } else if (_ls_qsize == 0 || !_ls_q) {                                 \
            _ls_e = _ls_p; _ls_p = _ls_p->next; _ls_psize--;                 \
          } else if (cmp(_ls_p,_ls_q) <= 0) {          \
            _ls_e = _ls_p; _ls_p = _ls_p->next; _ls_psize--;                 \
          } else {                                                               \
            _ls_e = _ls_q; _ls_q = _ls_q->next; _ls_qsize--;                 \
          }                                                                      \
          if (_ls_tail) {                                                        \
            _ls_tail->next = _ls_e;                                   \
          } else {                                                               \
            list = _ls_e;                                           \
          }                                                                      \
          _ls_e->prev = _ls_tail;                                   \
          _ls_tail = _ls_e;                                                      \
        }                                                                        \
        _ls_p = _ls_q;                                                           \
      }                                                                          \
      list->prev = _ls_tail;                                      \
      _ls_tail->next = NULL;                                               \
      if (_ls_nmerges <= 1) {                                                    \
        _ls_looping=0;                                                           \
      }                                                                          \
      _ls_insize *= 2;                                                           \
    }                                                                            \
  }                                                                              \
} while (0)

#define CDL_SORT(list, cmp)                             \
do {                                                                             \
  DECLTYPE(list) _ls_p, _ls_q, _ls_e, _ls_tail, _ls_oldhead;              \
  int _ls_insize, _ls_nmerges, _ls_psize, _ls_qsize, _ls_i, _ls_looping;         \
  if (list) {                                                                    \
    _ls_insize = 1;                                                              \
    _ls_looping = 1;                                                             \
    while (_ls_looping) {                                                        \
      _ls_p = list;                                                              \
      _ls_oldhead = list;                                                        \
      list = NULL;                                                               \
      _ls_tail = NULL;                                                           \
      _ls_nmerges = 0;                                                           \
      while (_ls_p) {                                                            \
        _ls_nmerges++;                                                           \
        _ls_q = _ls_p;                                                           \
        _ls_psize = 0;                                                           \
        for (_ls_i = 0; _ls_i < _ls_insize; _ls_i++) {                           \
          _ls_psize++;                                                           \
          _ls_q = ((_ls_q->next == _ls_oldhead) ? NULL : _ls_q->next); \
          if (!_ls_q) break;                                                     \
        }                                                                        \
        _ls_qsize = _ls_insize;                                                  \
        while (_ls_psize > 0 || (_ls_qsize > 0 && _ls_q)) {                      \
          if (_ls_psize == 0) {                                                  \
            _ls_e = _ls_q; _ls_q = _ls_q->next; _ls_qsize--;                 \
            if (_ls_q == _ls_oldhead) { _ls_q = NULL; }           \
          } else if (_ls_qsize == 0 || !_ls_q) {                                 \
            _ls_e = _ls_p; _ls_p = _ls_p->next; _ls_psize--;                 \
            if (_ls_p == _ls_oldhead) { _ls_p = NULL; }         \
          } else if (cmp(_ls_p,_ls_q) <= 0) {          \
            _ls_e = _ls_p; _ls_p = _ls_p->next; _ls_psize--;                 \
            if (_ls_p == _ls_oldhead) { _ls_p = NULL; }         \
          } else {                                                               \
            _ls_e = _ls_q; _ls_q = _ls_q->next; _ls_qsize--;                 \
            if (_ls_q == _ls_oldhead) { _ls_q = NULL; }         \
          }                                                                      \
          if (_ls_tail) {                                                        \
            _ls_tail->next = _ls_e;                                   \
          } else {                                                               \
            list = _ls_e;                                           \
          }                                                                      \
          _ls_e->prev = _ls_tail;                                   \
          _ls_tail = _ls_e;                                                      \
        }                                                                        \
        _ls_p = _ls_q;                                                           \
      }                                                                          \
      list->prev = _ls_tail;                                      \
      _ls_tail->next = list;                                        \
      if (_ls_nmerges <= 1) {                                                    \
        _ls_looping=0;                                                           \
      }                                                                          \
      _ls_insize *= 2;                                                           \
    }                                                                            \
  }                                                                              \
} while (0)

/******************************************************************************
 * singly linked list macros (non-circular)                                   *
 *****************************************************************************/
#define LL_PREPEND(head,add)                                                     \
do {                                                                             \
  (add)->next = head;                                                            \
  head = add;                                                                    \
} while (0)

#define LL_APPEND(head,add)                                                      \
do {                                                                             \
  DECLTYPE(head) _tmp;                                                         \
  (add)->next=NULL;                                                              \
  if (head) {                                                                    \
    _tmp = head;                                                                 \
    while (_tmp->next) { _tmp = _tmp->next; }         \
    _tmp->next=(add);                                         \
  } else {                                                                       \
    (head)=(add);                                                                \
  }                                                                              \
} while (0)

#define LL_DELETE(head,del)                                                      \
do {                                                                             \
  DECLTYPE(head) _tmp;                                                         \
  if ((head) == (del)) {                                                         \
    (head)=(head)->next;                                                         \
  } else {                                                                       \
    _tmp = head;                                                                 \
    while (_tmp->next && (_tmp->next != (del))) { \
      _tmp = _tmp->next;                                         \
    }                                                                            \
    if (_tmp->next) {                                                \
      _tmp->next = ((del)->next);                             \
    }                                                                            \
  }                                                                              \
} while (0)

#define LL_FOREACH(head,el)                                                      \
    for(el=head;el;el=el->next)

#define LL_FOREACH_SAFE(head,el,tmp)                                             \
  for((el)=(head);(el) && (tmp = (el)->next, 1); (el) = tmp)

#define LL_SEARCH_SCALAR(head,out,field,val) \
do { \
    LL_FOREACH(head,out) { \
      if ((out)->field == (val)) break; \
    } \
} while(0) 

#define LL_SEARCH(head,out,elt,cmp) \
do { \
    LL_FOREACH(head,out) { \
      if ((cmp(out,elt))==0) break; \
    } \
} while(0) 

/******************************************************************************
 * doubly linked list macros (non-circular)                                   *
 *****************************************************************************/
#define DL_PREPEND(head,add)                                                     \
do {                                                                             \
 (add)->next = head;                                                             \
 if (head) {                                                                     \
   (add)->prev = (head)->prev;                                                   \
   (head)->prev = (add);                                                         \
 } else {                                                                        \
   (add)->prev = (add);                                                          \
 }                                                                               \
 (head) = (add);                                                                 \
} while (0)

#define DL_APPEND(head,add)                                                      \
do {                                                                             \
  if (head) {                                                                    \
      (add)->prev = (head)->prev;                                                \
      (head)->prev->next = (add);                                                \
      (head)->prev = (add);                                                      \
      (add)->next = NULL;                                                        \
  } else {                                                                       \
      (head)=(add);                                                              \
      (head)->prev = (head);                                                     \
      (head)->next = NULL;                                                       \
  }                                                                              \
} while (0);

#define DL_DELETE(head,del)                                                      \
do {                                                                             \
  if ((del)->prev == (del)) {                                                    \
      (head)=NULL;                                                               \
  } else if ((del)==(head)) {                                                    \
      (del)->next->prev = (del)->prev;                                           \
      (head) = (del)->next;                                                      \
  } else {                                                                       \
      (del)->prev->next = (del)->next;                                           \
      if ((del)->next) {                                                         \
          (del)->next->prev = (del)->prev;                                       \
      } else {                                                                   \
          (head)->prev = (del)->prev;                                            \
      }                                                                          \
  }                                                                              \
} while (0);


#define DL_FOREACH(head,el)                                                      \
    for(el=head;el;el=el->next)

/* this version is safe for deleting the elements during iteration */
#define DL_FOREACH_SAFE(head,el,tmp)                                             \
  for((el)=(head);(el) && (tmp = (el)->next, 1); (el) = tmp)

/* these are identical to their singly-linked list counterparts */
#define DL_SEARCH_SCALAR LL_SEARCH_SCALAR
#define DL_SEARCH LL_SEARCH

/******************************************************************************
 * circular doubly linked list macros                                         *
 *****************************************************************************/
#define CDL_PREPEND(head,add)                                                    \
do {                                                                             \
 if (head) {                                                                     \
   (add)->prev = (head)->prev;                                                   \
   (add)->next = (head);                                                         \
   (head)->prev = (add);                                                         \
   (add)->prev->next = (add);                                                    \
 } else {                                                                        \
   (add)->prev = (add);                                                          \
   (add)->next = (add);                                                          \
 }                                                                               \
(head)=(add);                                                                    \
} while (0)

#define CDL_DELETE(head,del)                                                     \
do {                                                                             \
  if ( ((head)==(del)) && ((head)->next == (head))) {                            \
      (head) = 0L;                                                               \
  } else {                                                                       \
     (del)->next->prev = (del)->prev;                                            \
     (del)->prev->next = (del)->next;                                            \
     if ((del) == (head)) (head)=(del)->next;                                    \
  }                                                                              \
} while (0);

#define CDL_FOREACH(head,el)                                                     \
    for(el=head;el;el=(el->next==head ? 0L : el->next)) 

#define CDL_FOREACH_SAFE(head,el,tmp1,tmp2)                                       \
  for((el)=(head), ((tmp1)=(head)?((head)->prev):NULL); \
      (el) && ((tmp2)=(el)->next, 1); \
      ((el) = (((el)==(tmp1)) ? 0L : (tmp2))))

#define CDL_SEARCH_SCALAR(head,out,field,val) \
do { \
    CDL_FOREACH(head,out) { \
      if ((out)->field == (val)) break; \
    } \
} while(0) 

#define CDL_SEARCH(head,out,elt,cmp) \
do { \
    CDL_FOREACH(head,out) { \
      if ((cmp(out,elt))==0) break; \
    } \
} while(0) 

#endif /* UTLIST_H */

