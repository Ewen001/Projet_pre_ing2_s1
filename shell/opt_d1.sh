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
output_file="${home}/temp/conducteur_trajets.dat"

start_time=$(date +%s.%N)

# Utilisation de awk avec une syntaxe simplifiée
awk -b -F';' '
    NR > 1 {
        # Créer une clé unique pour chaque combinaison de conducteur et de Route ID
        if (!(($6 SUBSEP $1) in unique)) {
            unique[$6 SUBSEP $1]
            count[$6]++
        }
    }
    END {
        # Imprimer les résultats pour chaque conducteur
        for (driver in count) {
            print count[driver],";", driver
        }
    }
' "$csv_file" | sort -nrk1,1 | head -n 10 > "$output_file"
end_time=$(date +%s.%N)
elapsed_time=$(echo "$end_time - $start_time" | bc)
# Contenu du fichier
echo "Contenu du fichier conducteur_trajets.dat :"
echo "Nombre de trajet      Nom"
cat "$output_file"
echo "Temps d'exécution : $elapsed_time secondes"

# Ajout de l'instruction pour générer l'histogramme avec Gnuplot
gnuplot_script="${home}/temp/histogramme_conducteurs.gnu"

# Écriture du script Gnuplot pour l'histogramme
cat > "$gnuplot_script" <<EOF
set terminal pngcairo size 800,800 enhanced font 'Verdana,11'
set output '${home}/image/opt_d1_tp.png'

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
set lmargin at screen 0.25
set rmargin at screen 0.95
set tmargin at screen 0.85
set bmargin at screen 0.25

#réglage du nom des axes
set xrange [0:*] reverse  # Inverser l'axe des x
set yrange [0:*] 
set ylabel "Nombre de trajets" rotate by 90
set xlabel "Conducteurs" offset -5, -7
#ajout du titre
set label 1 "Option d1: Nb routes = f(driver)" at screen 0.1, 0.5 center rotate by 90

# Définition du fichier contenant les données
set datafile separator ";"
datafile = '$output_file'

# Création de l'histogramme vertical avec inversion des axes
plot datafile using 0:1:xtic(2) with boxes notitle
EOF

# Exécution du script Gnuplot pour générer l'histogramme
gnuplot "$gnuplot_script"

# Rotation de l'image avec ImageMagick
convert "${home}/image/opt_d1_tp.png" -rotate 90 "${home}/image/opt_d1.png"

rm "${home}/image/opt_d1_tp.png"






