# Projet_pre_ing2_s1

Description du Projet

Ce projet vise à développer un programme de gestion des données de transport routier, comprenant un script Shell et un programme C. Les données, issues du fichier CSV “data.csv”, seront traitées pour générer des graphiques résumant l’activité de la société nationale de transport routier.

I Structure du Projet

Script Shell: 
 
	•	Vérifie la présence et la compilation de l’exécutable C
	•	Gère la création des dossiers temp, images, et demo
	•	Affiche la durée de chaque traitement
	•	Crée des graphiques en utilisant GNUPlot et ImageMagick 
 
Programme C:
 
	• Compilation via le Makefile
	•	Effectue des opérations de filtrage, tri, et/ou calcul en fonction des besoins du script Shell
	•	Retourne un code d’erreur en cas de problème, libère les allocations mémoires, et limite la taille mémoire utilisée.


II Instructions d’Utilisation

	1.	Compilation du Programme C:
	•	Naviguez vers le dossier progc.
	•	Exécutez make pour compiler le programme.
	2.	Exécution du Script Shell:
	•	Placez le fichier de données CSV “data.csv” dans le dossier ‘data’.
	•	Exécutez le script Shell avec les arguments appropriés.

	3.	Options du Script Shell:
	•	-d1: Conducteurs avec le plus de trajets
	•	-d2: Conducteurs et la plus grande distance
	•	-l: Les 10 trajets les plus longs
	•	-t: Les 10 villes les plus traversées
	•	-s: Statistiques sur les étapes
	•	-h: Affiche les options disponibles
	4.	Fichiers Résultats:
	•	Les graphiques seront stockés dans le dossier ‘images’.
	•	Les fichiers intermédiaires seront placés dans ‘temp’.
	•	Les résultats des exécutions précédentes seront dans ‘demo’.
