#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opt_s.h"


//declaration de cpt, utile pour le parcours final
int cpt;

//fonction de base
n_avl *creer_avl(info *distance) {
    n_avl *nv = malloc(sizeof(n_avl));
    if(nv != NULL) {
        nv->trj = *distance;
        nv->hauteur =  1;
        nv->fg = NULL;
        nv->fd = NULL;
        return nv;
    }
    return NULL;
}

info *creer_info(int n, float distance) {
    info *nv = malloc(sizeof(info));
    if(nv != NULL) {
        nv->id = n;
        nv->etp = 1;
        nv->moy = distance;
        nv->s = 0;
        nv->min = distance;
        nv->max = distance;
        return nv;
    }
    return NULL;
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
    n_avl *tp_2 = x->fd;

    y->fd = y;
    y->fg = tp_2;

    y->hauteur = 1 + max(getH(y->fg), getH(y->fd));
    x->hauteur = 1 + max(getH(x->fg), getH(x->fd));

    return x;
}

n_avl *rota_g(n_avl *x) {
    n_avl *y = x->fd;
    n_avl *tp_2 = y->fg;

    y->fg = x;
    x->fd = tp_2;

    x->hauteur = 1 + max(getH(x->fg), getH(x->fd));
    y->hauteur = 1 + max(getH(y->fg), getH(y->fd));

    return y;
}

n_avl *insert2(n_avl *avl, info *nv_info) {
    if(avl == NULL){
        return creer_avl(nv_info);
    }
    int num = nv_info->id;

    if(num > avl->trj.id) {
        avl->fd = insert2(avl->fd, nv_info);
    }
    else if(num < avl->trj.id) {
        avl->fg = insert2(avl->fg, nv_info);
    }
    else if(num == avl->trj.id){ 
        if(nv_info->max > avl->trj.max) {avl->trj.max = nv_info->max;}
        if(nv_info->min < avl->trj.min) {avl->trj.min = nv_info->min;}
        avl->trj.moy += nv_info->min;
        avl->trj.etp++;
    }

    int eq = getB(avl);
    //on équilibre l'avl
    if (eq > 1 && num < avl->fg->trj.id) {
        return rota_d(avl);
    }

    if (eq < -1 && num > avl->fd->trj.id){
        return rota_g(avl);
    }

    if (eq > 1 && num > avl->fg->trj.id) {
        avl->fg = rota_g(avl->fg);
        return rota_d(avl);
    }
    if (eq < -1 && num < avl->fd->trj.id) {
        avl->fd = rota_d(avl->fd);
        return rota_g(avl);
    }
    if (eq > 1 && num > avl->fg->trj.id) {
        avl->fg = rota_g(avl->fg);
    return rota_d(avl);
    }

    if (eq < -1 && num < avl->fd->trj.id) {
        avl->fd = rota_d(avl->fd);
        return rota_g(avl);
    }

    return avl;

}

//fonction principale
n_avl * traitement_s(n_avl *avl, char *csv, int row) {
    FILE *fichier = fopen(csv, "r");
    if( fichier == NULL) {printf("Erreur d'ouverture du fichier\n"); exit(1);}
    //variables permettant le parcours
    char ligne[256];
    int col;
    int ct = 0 ; // variable qui permet d'éviter les répétitions de villes
    int trajet;
    int fligne = 1;

    while(fgets(ligne, sizeof(ligne), fichier) != NULL && ct < row) {
        char *tp_ligne = strtok(ligne, ";");  //récupération de la ligne
        col = 1;
        if(fligne) {
            //cas non utile à considérer
            fligne = 0;
            ct++;
            continue;
        }

        while(tp_ligne != NULL && col <= 6) {       //extraction des infos de la ligne puis insertion dans l'avl
            if(col == 1) {
                trajet = atoi(tp_ligne);
            }
            else if(col == 5) {
                info *nv_info = creer_info(trajet, atof(tp_ligne));
                avl = insert2(avl, nv_info);
            }
            tp_ligne = strtok(NULL, ";");
            col++; 
        }
        ct++;
    }
    printf("fclose fichier\n");
    fclose(fichier);
    return avl;
}

n_avl *insert(n_avl *avl, info *distance) {
    if(avl == NULL){
        return creer_avl(distance);
    }
    float num = distance->s;
    if(num > avl->trj.s) {
        avl->fd = insert(avl->fd, distance);
    }
    else if(num < avl->trj.s) {
        avl->fg = insert(avl->fg, distance);
    }
    else if(num == avl->trj.s){ //somme identique mais trajet nécéserrement différent, les doublons ont été rassemblé
    }

    int eq = getB(avl);

    //equilibrage
    if (eq > 1 && num < avl->fg->trj.s) {
        return rota_d(avl);
    }

    if (eq < -1 && num > avl->fd->trj.s){
        return rota_g(avl);
    }

    if (eq > 1 && num > avl->fg->trj.s) {
        avl->fg = rota_g(avl->fg);
    }

    if (eq < -1 && num < avl->fd->trj.s) {
        avl->fd = rota_d(avl->fd);
        return rota_g(avl);
    }
    if (eq > 1 && num > avl->fg->trj.s) {
        avl->fg = rota_g(avl->fg);
        return rota_d(avl);
    }

    if (eq < -1 && num < avl->fd->trj.s) {
        avl->fd = rota_d(avl->fd);
        return rota_g(avl);
    }

    return avl;
}

n_avl *tri_final(n_avl **avl, n_avl *avl2) {    //derniere insertion dans l'avl final
    if(avl2 == NULL) {
        return NULL;
    }
    tri_final(avl, avl2->fg);
    info *nv = &avl2->trj;
    *avl = insert(*avl, nv);
    tri_final(avl, avl2->fd);
    return *avl;
}

n_avl *calcul_champ(n_avl *avl) {    //calcul des champs dans l'avl
    if(avl == NULL) {
        return NULL;
    }
    calcul_champ(avl->fg);

    avl->trj.moy = (avl->trj.moy)/avl->trj.etp;
    avl->trj.s = avl->trj.max - avl->trj.min;
    calcul_champ(avl->fd);

    return avl;
}

void parcours(n_avl *avl) { // inverser l'ordre (affichage du terminal trop petit donc + grand en dernier)
    if(avl == NULL) {
        return;
    }
    parcours(avl->fd);

    if(cpt < 50) {
        printf("%d |ID|%d|TOTAL|%f|MOYENNE|%f|ETAPES|%d|MAX|%f|MIN|%f\n", cpt, avl->trj.id, avl->trj.s, avl->trj.moy, avl->trj.etp, avl->trj.max, avl->trj.min);
        cpt++;
    }

    parcours(avl->fg);
}

void free_avl(n_avl *avl){
    if(avl != NULL){
        free_avl(avl->fg);
        free_avl(avl->fd);
        free(avl);
    }
}

int main(int argc, char *argv[]) {
    int row = 0;
    char csv[50];

    n_avl *avl = NULL;
    n_avl *avl2 = NULL;

    if(argc < 2) {
        printf("Utilisation : ./fichier <nombre colonne> <fichier csv>\n");
        exit(1);
    }
    else {
        row = atoi(argv[1]);
        strcpy(csv, argv[2]);
    }
    avl2 = traitement_s(avl2, csv, row);
    avl2 = calcul_champ(avl2); // calcule les champs utile 
    avl = tri_final(&avl, avl2);
    cpt = 0;
    parcours(avl);      //affiche les 50 premiers résultats
    free_avl(avl);      //libération de la mémoire
    free_avl(avl2);
}