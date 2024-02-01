#ifndef OPT_T_H
#define OPT_T_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 350000 

//déclaration structure
typedef struct tp1 {
    char *nm;
    int occu;
    int occu_d;
}info;

typedef struct tp2 {
    struct tp1 ville;
    int hauteur;
    struct tp2 *fd;
    struct tp2 *fg;
    int TD[2000];
    int idxTID;
    char tab_donne[MAXSIZE][60];
    int tab_occu_d[MAXSIZE];
    int idx;
}n_avl;



#endif
