#!/bin/bash

# Vérification de la présence de deux arguments
if [ $# -ne 2 ]; then
    echo "Utilisation: $0 <programme c> <fichier csv>"
    exit 1
fi

exec=$1
csv_file=$2
l=$(wc -l "$csv_file")

# compilation du fichier c
make progc/opts
gcc $1 -o code_s

# Exécution de l'exécutable pour traiter le fichier CSV et création de temp.dat
./code_s $l $csv_file > temp/temp_s.dat 

# Utilisation de awk pour transformer les données en un format attendu par Gnuplot
awk -F '|' '{print $3, $7, $11, $13 }' temp/temp_s.dat > temp/option_s.dat
# Suppression du fichier temporaire
rm temp/temp_s.dat

sed -i '/^ *$/d' temp/option_s.dat

# Vérification de l'existence de données dans option_s.dat
if [ ! -s temp/option_s.dat ]; then
    echo "Erreur : Le fichier de données est vide ou n'existe pas."
    exit 2
fi

cat "temp/option_s.dat"

# Script Gnuplot
gnuplot -persist <<-EOF
    set terminal png size 1400, 800 enhanced font 'Verdana,11'
    set output 'image/option_s.png'
    set ylabel 'Distance (km)'
    set xlabel 'Id du trajet'
    set title "Option s: Distance = f(route)"
    set xtics rotate by -75
    set key outside
    set yrange [0:*]
    set datafile separator " "
        # Ajout des styles pour spécifier la couleur, le type de ligne et l'épaisseur
    set style line 1 lc rgb 'red' lt 1 lw 2
    set style line 2 lc rgb 'blue' lt 1 lw 3
    set style line 3 lc rgb 'blue' lt 1 lw 3

    plot 'temp/option_s.dat' using 2:xtic(1) title 'Distance moyenne' with lines linestyle 1, \
         'temp/option_s.dat' using 3:xtic(1) title 'Distance max' with lines linestyle 2, \
         'temp/option_s.dat' using 4:xtic(1) title 'Distance min' with lines linestyle 3
EOF

exit 0