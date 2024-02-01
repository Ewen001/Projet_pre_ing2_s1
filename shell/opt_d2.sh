#!/bin/bash

# Répertoire du fichier .csv
home="/home/e_679/Projet"

# Vérification de l'argument 
if [ $# -eq 0 ]; then
    echo "Usage: $0 <csv_filename>"
    exit 1
fi

# Chemin complet du fichier CSV
csv_file="${home}/${1}"

# Vérification si le fichier CSV existe
if [ ! -f "$csv_file" ]; then
    echo "Le fichier CSV '$1' n'existe pas dans le répertoire '$home'."
    exit 1
fi

# Chemin complet du fichier de sortie
output_file="${home}/temp/conducteur_distance.dat"

start_time=$(date +%s.%N)

awk -b -F';' '
    NR > 1 {
        key = $6;
        distance = $5;  # La distance se trouve dans le cinquième champ

        # Vérifier si la distance est un nombre
        if (distance ~ /^[0-9]+(\.[0-9]+)?$/) {
            total_distance[key] += distance;
        }
    }
    END {
        # Trier les conducteurs par distance parcourue
        PROCINFO["sorted_in"] = "@val_num_desc";
        for (driver in total_distance) {
            print  total_distance[driver],";", driver;
        }
    }
' "$csv_file" | head -n 10 > "$output_file"
end_time=$(date +%s.%N)
elapsed_time=$(echo "$end_time - $start_time" | bc)
# Contenu du fichier
echo "Contenu du fichier conducteur_distance.dat :"
echo "distance (km)   Nom"
cat "$output_file"
echo "Temps d'exécution : $elapsed_time secondes"



# Chemin du script Gnuplot pour l'histogramme

gnuplot_script="${home}/temp/conducteur_distance.gnu"



# Écriture du script Gnuplot pour l'histogramme

cat > "$gnuplot_script" <<EOF
set terminal pngcairo size 800,800 enhanced font 'Verdana,11'

set output '${home}/image/opt_d2_tp.png'

# Réglages du style de l'histogramme

set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
unset ytics
set boxwidth 0.2
set xtic rotate by 45
set xtic offset -9, -7
set ytic rotate by 90

#réglage de la marge

set lmargin at screen 0.15
set rmargin at screen 0.95
set tmargin at screen 0.90
set bmargin at screen 0.25

#réglage du nom des axes

set xrange [0:*] reverse  # Inverser l'axe des x
set yrange [0:*] 
set ylabel "Distance (km)" rotate by 90
set xlabel "Conducteurs" offset -5, -7

# Chargement des données
set datafile separator ";"
datafile = "${home}/temp/conducteur_distance.dat"

# Tracé du graphique
plot datafile using 0:1:xtic(2) with boxes notitle 

EOF



# Exécution du script Gnuplot pour générer le graphique

gnuplot "$gnuplot_script"



# Rotation de l'image avec ImageMagick

convert "${home}/image/opt_d2_tp.png" -rotate 90 "${home}/image/opt_d2.png"

rm "${home}/image/opt_d2_tp.png"