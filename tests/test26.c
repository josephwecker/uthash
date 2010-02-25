#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utlist.h"

#define BUFLEN 20

typedef struct el {
    char bname[BUFLEN];
    struct el *next, *prev;
} el;

int namecmp(el *a, el *b) {
    return strcmp(a->bname,b->bname);
}

el *head = NULL; /* important- initialize to NULL! */

int main(int argc, char *argv[]) {
    el *name, *elt, *tmp;

    char linebuf[BUFLEN];
    FILE *file;

    if ( (file = fopen( "test11.dat", "r" )) == NULL ) {
        perror("can't open: "); 
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        if ( (name = (el*)malloc(sizeof(el))) == NULL) exit(-1);
        strncpy(name->bname,linebuf,BUFLEN);
        DL_APPEND(head, name);
    }
    DL_SORT(head, namecmp);
    DL_FOREACH(head,elt) printf("%s", elt->bname);

    /* now delete each element, use the safe iterator */
    DL_FOREACH_SAFE(head,elt,tmp) {
      DL_DELETE(head,elt);
    }

    fclose(file);

    return 0;
}
