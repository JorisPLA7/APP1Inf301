#ifndef __CLIENT_H__
#define __CLIENT_H__

char* HOST;
int   PORT;

/*
 * Permet l'ouverture d'une connection avec le serveur
 * Les paramètres pour accéder au bon serveur sont déjà défini dans la fonction
 * renvoie une valeur nulle (faux) en cas d'échec
 */
int connexion(void);
/*
 * Permet la fermeture de la connection avec le serveur donné en argument
 */
void deconnexion();
/*
 * Permet d'activer (val != 0) ou non (val == 0) le mode debug
 */
void mode_debug(int val);
/*
 * Permet d'activer (val != 0) ou non (val == 0) l'attente automatique après un échange
 */
void attente_automatique(int val);
/*
 * Permet d'envoyer un message au serveur et de recevoir la réponse.
 * Renvoie le nombre d'octets de la réponse
 */
int envoyer_recevoir(char *message, char *reponse);

#endif
