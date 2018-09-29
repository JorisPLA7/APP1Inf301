#!/bin/bash
if [ $# -eq 0 ]
then
	echo "saisir un nom/ensemble de noms de dossier à traiter"
	for dossier in *
	do 
		$0 $dossier
	done
fi

dossiers=$(echo $*) 

for dossier in $dossiers
do
	if [ -r $dossier ]
	then
		#echo Pressez une touche pour lancer l'execution des programmes dans $dossier...
		read -p "Pressez une touche pour lancer la compilation & l'execution des programmes dans $dossier..."
		echo "Compilation des éléments de $dossier"
		cd $dossier
		make clean
		make
		echo "Execution des éléments de $dossier"
		$(find . -executable -type f) #execute tout les executables du dossier courant
		#./main executer le programme
		cd ..
		
		echo " "
		echo Executions dans $dossier terminées.
		echo " " 
	fi
done


