#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "cryptSeq.h"
#include  <string.h>


void convert(char *message, sequence *seq) {
		// Convertit une chaine de caractere (comme la reponse du serveur) en sequence (structure definie dans le fichier "cryptSeq").
		seq->longueur=0;
		for (int i=0; message[i] != 0 ; i++){
				seq->liste[i]=message[i] ;
				seq->longueur++;
				}
}
void reformat(sequence seq, char *reponse){
		// Convertit une sequence en chaine de caractere pour le renvoi au serveur
		seq.liste[seq.longueur] = 0 ;
		strcpy(reponse,seq.liste);
}

void copie(sequence *retour, sequence *seq, int decal){
		// Copie une sequence dans une autre avec si precise un decalage du premier indice (0 pour copie simple)
		char *point = &seq->liste[decal];
		strcpy(retour->liste,point);
		retour->longueur = seq->longueur - decal ;
}

char decalage(char lettreLue, int decal){
    // Applique le décalage de Cesar à une lettre lue
    char lettreSuivante;
    decal=(decal*decal<(26-decal)*(26-decal))?decal:26-decal  ; // Selection de la plus petite valeur en valeur absolue pour un meme decalage
    // (exemple : choisir entre +16 et -10) pour eviter les sorties de la table ASCII
    if ('a' <= lettreLue && lettreLue <= 'z' ) {// Traitement des minuscules
        lettreSuivante = lettreLue + decal;
        if (lettreSuivante > 'z'){
            lettreSuivante =  lettreSuivante-26;
        }
        if (lettreSuivante < 'a'){
            lettreSuivante =  lettreSuivante+26;
        }

    }

    else if ('A' <= lettreLue && lettreLue <= 'Z' ) {// Traitement des majuscules
        lettreSuivante = lettreLue + decal;
        if (lettreSuivante > 'Z'){
            lettreSuivante = lettreSuivante-26;
        }
        if (lettreSuivante < 'A'){
            lettreSuivante = lettreSuivante+26;
        }

    }

    else lettreSuivante = lettreLue;

    return lettreSuivante;
}
void appliquer_decalage(sequence *message, int decal){
		// Applique un decalage de Cesar a une sequence en appelant la fonction decalage 
		for(int i = 0; i<message->longueur;i++){
				message->liste[i] = decalage(message->liste[i], decal);
				}
}
  
int planB(sequence *mess) {
    // Permet de resoudre l'exercice PlanB en recuperant le message necessaire pour l'exercice crypteSeq
    // mess est le pointeur vers le message qu'il faudra stocker pour l'exercice suivant
	
    // Initialisation
    char reponse[100000];
    sequence message;
    message.longueur = 0 ;
    
    
    // Options :
    // Affiche les échanges avec le serveur
    mode_debug(true);
    // Attend après chaque échange l'appui sur entrée
    attente_automatique(false);
    // Connexion au serveur :
    if (! connexion()) {
        fprintf (stderr, "Impossible de se connecter à AppoLab!\n");
        return 1 ;
    }
    envoyer_recevoir("login PEREZ 11713143", reponse);
    
    // Demarrage de l'exercice "planB"
    envoyer_recevoir("load planB", reponse);
    envoyer_recevoir("help", reponse);
    
    
    convert(reponse, &message) ; // Convertion...
    
    // Decryptage du message d'aide (Decalage de Cesar)
    int dc='C'-message.liste[0]; //On trouve la cle en comparant a la premiere lettre qui est un C
    appliquer_decalage(&message,dc);
    reformat(message,reponse);// Convertion...
    envoyer_recevoir(reponse, reponse);// Renvoi
    
    // Renvoi de la reponse au serveur avec le decalage de Cesar correspondant
    sequence has ; 
    convert("hasta la revolucion", &has) ;
    appliquer_decalage(&has,dc);
    envoyer_recevoir("start", reponse);
    reformat(has,reponse);
    envoyer_recevoir(reponse, reponse);
    
    // Decryptage du dernier message et stockage pour l'exercice suivant
    convert(reponse, &message) ;
    int dc2='C'-message.liste[18];
    appliquer_decalage(&message,dc2);
        
   
    /* CODE POUR VALIDER L'EXERCICE PLAN_B SEUL
    
    sequence has2;
    convert("hasta la victoria siempre", has2) ;
    appliquer_decalage(&has2,dc2);
    reformat(has2,reponse);
    envoyer_recevoir(reponse, reponse);
   
    */
		
    reformat(message,reponse);
    envoyer_recevoir(reponse, reponse);
    envoyer_recevoir("\n", reponse);
    copie(mess, &message,18) ; // On enregistre le message important dans mess
    return 0 ; // Deconnexion
   }
int crypteSeq(sequence *mess){
    // Permet de resoudre en entier l'exercice crypteSeq
    // mess contient le dernier message de l'exercice precedant
	
    // Initialisation
    char reponse[100000];
    sequence message;
    message.longueur = 0 ;
	
    // Options
    mode_debug(true);
    attente_automatique(false);

    // Connexion au serveur
    if (! connexion()) {
        fprintf (stderr, "Impossible de se connecter à AppoLab!\n");
        return 1;
    }
    envoyer_recevoir("login PEREZ 11713143", reponse);
	
    // Demarrage de l'exercice
    envoyer_recevoir("load crypteSeq", reponse);
    envoyer_recevoir("start", reponse);
    
    // Premiere partie : cryptage
    cryptseq(mess); // Cryptage du message de l'execice precedant (fonction definie dans le fichier "cryptSeq.c")
    mess->longueur+= -1 ; // Le dernier caractere provoque une erreur
    copie(&message, mess, 0); // Stockage
    reformat(message,reponse); // Convertion...
    envoyer_recevoir(reponse, reponse); // Renvoi
    
    // Deuxieme partie : decryptage 
    convert(reponse, &message) ;
    sequence seqq;
    copie(&seqq, &message, 83);
    decryptseq(&seqq); // Decryptage du message recu (fonction definie dans le fichier "cryptSeq.c")
    reformat(seqq,reponse);// Convertion...
    envoyer_recevoir(reponse, reponse);// Renvoi
	
    return 0; // Deconnexion
}

int main() {
    // Programme principal qui fait le lien entre les deux exercices

   sequence message ;
		
    if (planB(&message) == 1) return 1;
     
    if (crypteSeq(&message) == 1) return 1;
    
    
    
    
    return 0;
}
