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
output_file="${home}/temp/top_trajet.dat"

# Mesure du temps de début
start_time=$(date +%s.%N)

# Utilisation de awk
awk -b -F';' '
    NR > 1 {
        key = $1;
        distance = $5;  # La distance se trouve dans le cinquième champ

        # Vérifier si la distance est un nombre
        if (distance ~ /^[0-9]+(\.[0-9]+)?$/) {
            total_distance[key] += distance;
        }
    }
    END {
        # Trier les trajets par distance parcourue
        PROCINFO["sorted_in"] = "@val_num_desc";
        for (key in total_distance) {
            print total_distance[key], key;
        }
    }
' "$csv_file" | head -n 10 > "$output_file"

# Mesure du temps de fin
end_time=$(date +%s.%N)

# Calcul du temps écoulé
elapsed_time=$(echo "$end_time - $start_time" | bc)

# Contenu du fichier
echo "Contenu du fichier top_trajet.dat :"
echo "Distance (km)     Id trajet"
cat "$output_file"

# Affichage du temps d'exécution
echo "Temps d'exécution : $elapsed_time secondes"

# Ajout de l'instruction pour générer l'histogramme avec Gnuplot
gnuplot_script="${home}/temp/top_trajet.gnu"

# Écriture du script Gnuplot pour l'histogramme
cat > "$gnuplot_script" <<EOF
set terminal pngcairo size 800,800 enhanced font 'Verdana,12'
set output '${home}/image/opt_l.png'

# Réglages du style de l'histogramme

set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
unset ytics
set boxwidth 0.2
set xtic rotate by 45
set xtic offset -1, -2.5
set ytic rotate by 0
set yrange [0:*]
#réglage de la marge
set lmargin at screen 0.15
set rmargin at screen 0.95
set tmargin at screen 0.90
set bmargin at screen 0.15

#réglage du nom des axes
set ylabel "Distance totale parcourue (km)" rotate by 90
set xlabel "Id trajet" offset 0, -2
set title "Option l: Distance = f(route)"

# Définition du fichier contenant les données
datafile = '$output_file'

# Création de l'histogramme vertical avec inversion des axes
plot datafile using 0:1:xtic(2) with boxes notitle
EOF

# Exécution du script Gnuplot pour générer l'histogramme
gnuplot "$gnuplot_script"
