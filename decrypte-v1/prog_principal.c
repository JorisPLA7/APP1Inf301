#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

/*Les messages ont été correctement décryptés, PEREZ sera ajouté au hall of fame ! ... bzzz.. CRsohucr.ha.CRCz... zzzz
---DEBUG--- crcr   .....  ALICE : MESSAGE DE BOB RETROUVE MOI SUR 'planB' LE MOT DE PASSE EST 'qui vivra verra' LAISSE MOI UN MESS...crcrcrc zbmbbzzzzz bz b zbbb
*/

char decalage(char lettreLue, int decal){
    //Pour decoder le message, on va decaler les lettres en se deplacant du nombre de places indique par "decal" dans la table ASCII
    char lettreSuivante;
    //On doit faire attention a ne pas depasser de l'alphabet dans lequel on est
    if ('a' <= lettreLue && lettreLue <= 'z' ) {
        lettreSuivante = lettreLue + decal;
        if (lettreSuivante > 'z'){
            lettreSuivante =  lettreSuivante-26; //Si l'on arrive a la fin de l'alphabet, on repasse au debut
        }
        if (lettreSuivante < 'a'){
            lettreSuivante =  lettreSuivante+26; //Si l'on arrive au debut de l'alphabet, on repasse a la fin
        }

    }

    else if ('A' <= lettreLue && lettreLue <= 'Z' ) { //Meme chose pour les lettres majuscules
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
            reponse[i] = decalage(reponse[i], -5); //met dans reponse[i] la lettre decodee grace a la fonction decalage
        envoyer_recevoir(reponse, reponse);
    }
    

    return 0;
}
