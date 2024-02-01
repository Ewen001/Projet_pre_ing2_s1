#!/bin/bash

# on accorde les droits d'éxécutions à l'ensemble des fichiers
chmod +x shell/opt_d1.sh
chmod +x shell/opt_d2.sh
chmod +x shell/opt_l.sh
chmod +x shell/opt_t.sh
chmod +x shell/opt_s.sh

# Vérifier le nombre d'arguments
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <chemin_du_fichier_CSV> <choix_traitement>"
    exit 1
fi

#
if [ ! -d "temp" ]; then
        echo "Création du dossier temp"
        mkdir -p temp
    else
        echo "Le dossier temp existe déjà, on le nettoie."
        rm -f temp/*
    fi

if [ ! -d "image" ]; then
        echo "Création du dossier image"
        mkdir -p image
    else
        echo "Le dossier image existe déjà, on le nettoie."
        rm -f image/*
    fi

# Récupérer les paramètres
fichier_ch=$1
opt=$2

# Vérifier l'existence du fichier CSV
if [ ! -f "$fichier_ch" ]; then
    echo "Le fichier CSV '$fichier_ch' n'existe pas."
    exit 1
fi

if [ "$opt" = "h" ] || [ -z "$opt" ]; then
    h="help.txt"
    if [ -f "$h" ]; then
        cat "$h"
    else
        echo "erreur help"
        exit 2
    fi
fi

for opt in "${@:2}"; do
  case $opt in
    d1) # Option -d1 : Conducteurs avec le plus de trajets
      echo "Conducteurs avec le plus de trajets"
      ./shell/opt_d1.sh "$fichier_ch"
      ;;
    d2) # Option -d2 : Conducteurs et la plus grande distance
      echo "Conducteurs avec la plus grande distance parcourue"
      ./shell/opt_d2.sh "$fichier_ch"
      ;;
    l) # Option -l : Les 10 trajets les plus longs
      echo "Les 10 trajets les plus longs"
      ./shell/opt_l.sh "$fichier_ch"
      ;;
    t) # Option -t : Les 10 villes les plus traversées
      echo "Les 10 villes les plus traversées"
      ./shell/opt_t.sh "progc/opt_t.c" "$fichier_ch"
      ;;
    s) # Option -s : Statistiques sur les étapes
      echo "Statistiques sur les étapes"
     ./shell/opt_s.sh "progc/opt_s.c" "$fichier_ch"
      ;;
    *)
      echo "Option invalide: -$OPTARG. Utilisez l'option -h pour afficher l'aide."
      ;;
  esac
  echo
done

