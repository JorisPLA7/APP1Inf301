#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

/*Les messages ont été correctement décryptés, PEREZ sera ajouté au hall of fame ! ... bzzz.. CRsohucr.ha.CRCz... zzzz
---DEBUG--- crcr   .....  ALICE : MESSAGE DE BOB RETROUVE MOI SUR 'planB' LE MOT DE PASSE EST 'qui vivra verra' LAISSE MOI UN MESS...crcrcrc zbmbbzzzzz bz b zbbb
*/

char decalage(char lettreLue, int decal){
    char lettreSuivante;
    decal=(decal*decal<(26-decal)*(26-decal))?decal:26-decal  ; 
    // Signifie : "si '(decal*decal<(26-decal)*(26-decal))' est vrai, alors 'decal' sinon '26-decal'
    // Selection de la plus petite valeur en valeur absolue pour un meme decalage
    // (exemple : choisir entre +16 et -10) pour eviter les sorties de la table ASCII
    
    if ('a' <= lettreLue && lettreLue <= 'z' ) { //Traitement des minuscules
        lettreSuivante = lettreLue + decal;
        if (lettreSuivante > 'z'){
            lettreSuivante =  lettreSuivante-26;
        }
        if (lettreSuivante < 'a'){
            lettreSuivante =  lettreSuivante+26;
        }
}

    else if ('A' <= lettreLue && lettreLue <= 'Z' ) { //Traitement des majuscules
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


int main() {
    char reponse[100000];
    char message[100000];
    //int taille;

    // Affiche les échanges avec le serveur
    mode_debug(true);

    // Attend après chaque échange l'appui sur entrée
    attente_automatique(false);

    if (! connexion()) {
        fprintf (stderr, "Impossible de se connecter à AppoLab!\n");
        return 1;
    }

    envoyer_recevoir("login PEREZ 11713143", reponse);
    printf("%s", reponse);
    envoyer_recevoir("load planB", reponse);
    envoyer_recevoir("help", message);
    int dc='C'-message[0]; //Permet de calculer la clef
    
    for (int j=0; j<1; j++) {
        for (int i =0; message[i]!=0; i++){
            message[i] = decalage(message[i], dc);
	}
    }    
    
    envoyer_recevoir(message, reponse);
    char has[100] = "hasta la revolucion";
    for (int j=0; j<1; j++) {
        for (int i=0; has[i]!=0; i++){
            has[i] = decalage(has[i], dc);
        }
        printf("%s",has);
    }
    
    envoyer_recevoir("start", reponse);
    envoyer_recevoir(has, message);
    
    int dc2='C'-message[18];
	
    for (int j=0; j<1; j++) {
        for (int i =0; message[i]!=0; i++){
            message[i] = decalage(message[i], dc2);
	}
    }
    
    envoyer_recevoir(message, reponse);
    
    return 0;
}
