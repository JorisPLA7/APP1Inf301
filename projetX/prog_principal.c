#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

/*Les messages ont été correctement décryptés, PEREZ sera ajouté au hall of fame ! ... bzzz.. CRsohucr.ha.CRCz... zzzz
---DEBUG--- crcr   .....  ALICE : MESSAGE DE BOB RETROUVE MOI SUR 'planB' LE MOT DE PASSE EST 'qui vivra verra' LAISSE MOI UN MESS...crcrcrc zbmbbzzzzz bz b zbbb
*/

char decalage(char lettreLue, int decal)
{
    char lettreSuivante;
    decal=(decal*decal<(26-decal)*(26-decal))?decal:26-decal  ; 
    // Signifie : "si '(decal*decal<(26-decal)*(26-decal))' est vrai, alors 'decal' sinon '26-decal'
    // Selection de la plus petite valeur en valeur absolue pour un meme decalage
    // (exemple : choisir entre +16 et -10) pour eviter les sorties de la table ASCII
    if ('a' <= lettreLue && lettreLue <= 'z' ) { //Traitement des minuscules
        lettreSuivante = lettreLue + decal;
        if (lettreSuivante > 'z'){
            lettreSuivante =  lettreSuivante-26; //Si l'on arrive a la fin de l'alphabet, on repasse au debut
        }
        if (lettreSuivante < 'a'){
            lettreSuivante =  lettreSuivante+26; //Si l'on arrive au debut de l'alphabet, on repasse a la fin
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
    
    int dc='C'-message[0]; //met dans la variable 'dc' la valeur du decalage des lettres
    for (int j=0; j<1; j++) {
        for (int i =0; message[i]!=0; i++){
            message[i] = decalage(message[i], dc); //appelle la fonction decalage avec la clef calculee au dessus
        }
    }    
    
    envoyer_recevoir(message, reponse);
    char has[100] = "hasta la revolucion";
    for (int j=0; j<1; j++) {
        for (int i=0; has[i]!=0; i++){
            has[i] = decalage(has[i], dc);
        }
    }
    
    envoyer_recevoir("start", reponse);
    envoyer_recevoir(has, message);
    
    int dc2='C'-message[18]; //On prend message[18] car on ne doit pas prendre le "bonne reponse !" qui a ete envoye par le serveur
    for (int j=0; j<1; j++) {
        for (int i =0; message[i]!=0; i++){
            message[i] = decalage(message[i], dc2);
        }
    }
    char has2[100] = "hasta la victoria siempre";
    for (int j=0; j<1; j++) {
        for (int i=0; has2[i]!=0; i++){
            has2[i] = decalage(has2[i], dc2);
        }
    }
    
    envoyer_recevoir(has2, reponse);
    
    return 0;
}
