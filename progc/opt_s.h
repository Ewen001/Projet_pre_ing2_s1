#ifndef OPT_S_H
#define OPT_S_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//definition des structures
typedef struct tp1 {
    int id;
    int etp;
    float s;
    float max;
    float min;
    float moy;
}info;

typedef struct tp2 {
    float num;
    struct tp1 trj;
    int hauteur;
    struct tp2 *fg;
    struct tp2 *fd;
}n_avl;






#endif