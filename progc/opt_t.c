#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opt_t.h"


#define IGN1 1

//déclaration cpt pour parcours final
int cpt;

//fonction de base
info *creer_info(char *nm, int occu_d) {
    info *nv = malloc(sizeof(info));
    if(nv != NULL) {
        nv->nm = malloc(60*sizeof(char));
        strcpy(nv->nm, nm);
        nv->occu_d = occu_d;
        nv->occu = 0;
        return nv;
    }
    else {
        printf("creer_info a echoue pour %s\n", nm);
        return NULL;
    }
}
n_avl *creer_avl(info *ville) {
    n_avl *nv = malloc(sizeof(n_avl));
    if(nv != NULL) {

    nv->ville = *ville;
        nv->hauteur = -1;
        nv->fd = NULL;
        nv->fg = NULL;
        nv->idx = 0;
        return nv;
    }
        else {
        printf("creer_avl a echoue pour %s\n", ville->nm);
        return NULL;
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    else {
        return b;
    } 
}

int getH(n_avl *avl) {
    if(avl == NULL) {
        return 0;
    }
    return avl->hauteur;
}

int getB(n_avl *avl) {
    if(avl == NULL) {
        return 0;
    }
    return getH(avl->fg) - getH(avl->fd);
}

n_avl *rota_d(n_avl *y) {
    n_avl *x = y->fg;
    n_avl *tp2 = x->fd;

    x->fd = y;
    y->fg = tp2;

    y->hauteur = 1 + max(getH(y->fg), getH(y->fd));
    x->hauteur = 1 + max(getH(x->fg), getH(x->fd));

    return x;
}

n_avl *rota_g(n_avl *x) {
    n_avl *y = x->fd;
    n_avl *tp2 = y->fg;

    y->fg = x;
    x->fd = tp2;

    x->hauteur = 1 + max(getH(x->fg), getH(x->fd));
    y->hauteur = 1 + max(getH(y->fg), getH(y->fd));

    return y;
}
int compID(int tab[2000], int TD, int size) {
    for(int i = 0; i < size; i++) {
        if(tab[i] == TD) {
            return 0;
        }
    }
    return TD;
}

n_avl *insert(n_avl *avl, info *ville, int TD) {
    if(avl == NULL) {
        return creer_avl(ville);
    }
    
    int key = strcmp(ville->nm, avl->ville.nm);
    

    if(key > 0) {
        avl->fd = insert(avl->fd, ville, TD);
    }
    else if(key < 0) {
        avl->fg = insert(avl->fg, ville, TD);
    }
    else if(key == 0) {
        if(compID(avl->TD, TD, avl->idxTID) == 0) { 
        free(ville->nm);
        free(ville);
        return avl;           
        }
        else {
        avl->TD[avl->idxTID] = TD;
            avl->idxTID++;

        avl->ville.occu++;
        avl->ville.occu_d += ville->occu_d;
        free(ville->nm);
        free(ville);
        }
    }
    
    int eq = getB(avl);
    if(avl->fg || avl->fd == NULL) {
        return avl;
    }

    if(avl->fg != NULL) {
    //ici rotations
        if (eq > 1 && key < avl->fg->ville.occu) {
            return rota_d(avl);
        }
    }

    if(avl->fd != NULL) {
        if (eq < -1 && key > avl->fd->ville.occu){
            return rota_g(avl);
        }
    }
    if(avl->fg != NULL) {
        if (eq > 1 && key > avl->fg->ville.occu) {
            avl->fg = rota_g(avl->fg);
            return rota_d(avl);
        }
    }

    if(avl->fd !=  NULL) {
        if (eq < -1 && key < avl->fd->ville.occu) {
            avl->fd = rota_d(avl->fd);
            return rota_g(avl);
        }
    }
    if(avl->fg != NULL) {
        if (eq > 1 && key > avl->fg->ville.occu) {
            avl->fg = rota_g(avl->fg);
            return rota_d(avl);
        }
    }

    if(avl->fd != NULL) {
        if (eq < -1 && key < avl->fd->ville.occu) {
            avl->fd = rota_d(avl->fd);
            return rota_g(avl);
        }
    }

    return avl;
}

n_avl * traitement_t(char *csv, int row, n_avl *avl) {
    FILE *file = fopen(csv, "r");
    if( file == NULL) {printf("file NULL\n"); exit(1);}
    char buff[256];
    int col;
    int ct = 0 ; // 1 pour eviter town a town b
    int step;
    int tid;
    int fline = 1;

    while(fgets(buff, sizeof(buff), file) != NULL && ct < row) {
        char *token = strtok(buff, ";");
        col = 1;
        if(fline) {
            fline = 0;
            ct++;
            continue;
        }

        while(token != NULL && col <= 6) {
            if(col == 1) {
                tid = atoi(token);
            }
            if(col == 2) {
                step = atoi(token);
            }
            else if(col == 3 || col == 4) {
                if(step == 1 && col == 3) {//logique d'ajt en cas de ville1 == épart
                    info *ville = creer_info(token, 1);
                    avl = insert(avl, ville, tid);
                }
                else {
                    info *ville = creer_info(token, 0);
                    avl = insert(avl, ville, tid);
                }
            }
            token = strtok(NULL, ";");
            col++; 
        }
    ct++;
    }
fclose(file);
return avl;
}


n_avl *insert2(n_avl *avl, info *ville) {
    if(avl == NULL) { 
        return creer_avl(ville);
    }

    int key = ville->occu;

    if(key > avl->ville.occu) {
        avl->fd = insert2(avl->fd, ville);
    }
    else if(key < avl->ville.occu) {
        avl->fg = insert2(avl->fg, ville);

    }
    else if(key == avl->ville.occu) {        
        strcpy(avl->tab_donne[avl->idx], ville->nm);
        avl->tab_occu_d[avl->idx] = ville->occu_d;
        avl->idx++;
    }
    
    int eq = getB(avl);
//ici rotations
    if (eq > 1 && key < avl->fg->ville.occu) {
        return rota_d(avl);
    }

    if (eq < -1 && key > avl->fd->ville.occu){
        return rota_g(avl);
    }

    if (eq > 1 && key > avl->fg->ville.occu) {
        avl->fg = rota_g(avl->fg);
        return rota_d(avl);
    }

    if (eq < -1 && key < avl->fd->ville.occu) {
        avl->fd = rota_d(avl->fd);
        return rota_g(avl);
    }
    if (eq > 1 && key > avl->fg->ville.occu) {
        avl->fg = rota_g(avl->fg);
        return rota_d(avl);
    }

    if (eq < -1 && key < avl->fd->ville.occu) {
        avl->fd = rota_d(avl->fd);
        return rota_g(avl);
    }

    return avl;
}

n_avl *parseAvl(n_avl *avl1, n_avl **avl2) {
    if(avl1 == NULL) {
        return NULL;
    }
    parseAvl(avl1->fg, avl2);
    if((avl1->ville.occu > 1 &&  IGN1) || IGN1 == 0) {
        *avl2 = insert2(*avl2, &avl1->ville);
    }
    parseAvl(avl1->fd, avl2);
    return *avl2;
}

void parcours(n_avl *avl, int a1[], int a2[], char a3[][60]){
    if(avl != NULL) {
    parcours(avl->fd, a1, a2, a3);    
    if(cpt < 10) {
        strcpy(a3[cpt], avl->ville.nm);
        a1[cpt] = avl->ville.occu;
        a2[cpt] = avl->ville.occu_d;
        cpt++; }    
        for(int i = 0; i < avl->idx; i++) {
            if(cpt < 10) {
            strcpy(a3[cpt], avl->tab_donne[i]);
            a1[cpt] = avl->ville.occu;
            a2[cpt] = avl->tab_occu_d[i];
            cpt++;
            }
        }    
    parcours(avl->fg, a1 , a2, a3);
    }
}
void inv_tab(char a3[][60], int a1[], int a2[], int j, int j2) {

    char temp[60];
    strcpy(temp, a3[j]);
    strcpy(a3[j], a3[j2]);
    strcpy(a3[j2], temp);

    int temp1 = a1[j];
    a1[j] = a1[j2];
    a1[j2] = temp1;

    int temp2 = a2[j];
    a2[j] = a2[j2];
    a2[j2] = temp2;

}

void tri(int n, int a1[], int a2[], char a3[][60]) {
    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(a3[j], a3[j + 1]) > 0) {
                inv_tab(a3, a1, a2, j, j + 1);
            }
        }
    }
}

void print(int n, int a1[], int a2[], char a3[][60]){

    for(int i = 0; i < n; i++) {
        printf("%d |%s;|OCC|%d;|OCCD| %d\n", i, a3[i], a1[i], a2[i]);
    } 
}

void free_avl(n_avl *avl){
    if(avl != NULL){
        free_avl(avl->fg);
        free_avl(avl->fd);
        free(avl);
    }
}

int main(int argc, char *argv[]){   
    int occdTAB[10];
    int occTAB[10];
    char villeTAB[10][60];
    int row = 0;
    char csv[50];
    if(argc < 2) {
        printf("usage : ./file <rows> <csv file>\n");
    }
    else {
        row = atoi(argv[1]);
        strcpy(csv, argv[2]);
    }
    n_avl *avl1 = NULL;
    n_avl *avl2 = NULL;
    avl1 = traitement_t(csv, row, avl1);    
    avl2 = parseAvl(avl1, &avl2);
    free_avl(avl1);    
    if(avl2 == NULL){printf("avl2 = NULL\n");exit(1);}
    cpt = 0;
    parcours(avl2, occTAB, occdTAB, villeTAB);
    tri(10, occTAB, occdTAB, villeTAB);
    print(10, occTAB, occdTAB, villeTAB);
    free_avl(avl2);
}