#!/bin/bash

# Vérification de la présence de deux arguments
if [ $# -ne 2 ]; then
    echo "Utilisation: $0 <C PROGRAM> <csv_file>"
    exit 1
fi

exec=$1
csv_file=$2
l=$(wc -l "$csv_file")
# compilation du fichier c
gcc $1 -o code_t

# Exécution de l'exécutable pour traiter le fichier CSV et création de temp.dat
./code_t $l $csv_file t > temp/temp_t.dat 

# Utilisation de awk pour transformer les données en un format attendu par Gnuplot
awk -F '|' '{print $2, $4, $6}' temp/temp_t.dat > temp/option_t.dat
# Suppression du fichier temporaire
rm temp/temp_t.dat

sed -i '/^ *$/d' temp/option_t.dat

# Vérification de l'existence de données dans option_s.dat
if [ ! -s temp/option_t.dat ]; then
    echo "Erreur : Le fichier de données est vide ou n'existe pas."
    exit 2
fi

cat "temp/option_t.dat"

gnuplot -persist <<-EOF
    set terminal png size 800,800  
    set output 'image/option_t.png'
    set ylabel 'Nombre d étapes'
    set xlabel 'Nom de la ville'
    set style data histograms
    set style histogram cluster gap 1
    set style fill solid 1.0 border -1
    set boxwidth 0.6
    set datafile separator ";"
    set xtics rotate by -35 font ",11" 
    #réglage de la marge 
    set lmargin 10  
    set rmargin 10  
    set tmargin 5   
    set bmargin 9   
    set grid y
    set yrange [0:*]
    plot 'temp/option_t.dat' using 2:xticlabels(1) title 'Villes traversées' lc rgb "red", '' using 3 title 'Ville de départ' lc rgb "black"
EOF