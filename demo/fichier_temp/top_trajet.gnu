set terminal pngcairo size 800,800 enhanced font 'Verdana,12'
set output '/home/e_679/Projet/image/opt_l.png'

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

# Définition du fichier contenant les données
datafile = '/home/e_679/Projet/temp/top_trajet.dat'

# Création de l'histogramme vertical avec inversion des axes
plot datafile using 0:1:xtic(2) with boxes notitle
