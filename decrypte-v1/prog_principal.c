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
    
    if ('a' <= lettreLue && lettreLue <= 'z' ) {
        lettreSuivante = lettreLue + decal;
        if (lettreSuivante > 'z'){
            lettreSuivante =  lettreSuivante-26;
        }
        if (lettreSuivante < 'a'){
            lettreSuivante =  lettreSuivante+26;
        }

    }

    else if ('A' <= lettreLue && lettreLue <= 'Z' ) {
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
    //int taille;

    // Affiche les échanges avec le serveur
    mode_debug(true);

    // Attend après chaque échange l'appui sur entrée
    attente_automatique(false);

    // Commentez ou passez à 'false' les lignes ci-dessus pour en désactiver 
    // les effets.

    if (! connexion()) {
        fprintf (stderr, "Impossible de se connecter à AppoLab!\n");
        return 1;
    }

    envoyer_recevoir("login PEREZ 11713143", reponse);
    /* printf("%s", reponse);  // décommentez pour afficher la réponse du serveur */
    envoyer_recevoir("load decrypte-v1", reponse);
    envoyer_recevoir("start", reponse);
    
    // Il est possible de travailler sur la réponse du serveur, par exemple
    // ici on passe tout en majuscules.
    for (int j=0; j<5; j++) {
        for (int i =0; reponse[i]!=0; i++)
            reponse[i] = decalage(reponse[i], -5);
        envoyer_recevoir(reponse, reponse);
    }
    

    return 0;
}
