#include "client.h"

#if defined (WIN32)
    #include <winsock2.h>
    typedef int socklen_t;
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
#endif

#define TAILLEMESSAGE 32000

typedef struct appolab_data
{
  SOCKET sock;
  char buffer[TAILLEMESSAGE];
  int debut;
  int debug;
  int attente;
} appolab;

char* HOST = "152.77.87.30";
int PORT = 443;


#ifdef __cplusplus
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
#else
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

static appolab a = {0, "", 0, 0, 0};

static void prefix_print(FILE *out, char *prefix, char *message) {
    if (*message != '\0') {
        fprintf(out, "%s", prefix);
        while (*message != '\0') {
            fprintf(out, "%c", *message);
            if ((*message == '\n') && (message[1] != '\0'))
                fprintf(out, "%s", prefix);
            message++;
        }
    }
}

static void debug(char *message) {
    prefix_print(stderr, "---DEBUG--- ", message);
}

static void erreur(char *message) {
    prefix_print(stderr, "***ERREUR*** ", message);
}

static void perreur(char *message) {
    erreur(message);
    perror("Detail");
}

static int insistent_read(SOCKET sock, void *msg, size_t size) {
    ssize_t len=0;
    char *buffer = msg;
    len = recv(sock, buffer + len, size - len, 0);
    while ((len > 0) && (len < size) && ((len != SOCKET_ERROR) || (errno == EINTR))) {
        len += recv(sock, buffer + len, size - len, 0);
    }
    if (len == SOCKET_ERROR) {
        perreur("Erreur de communication");
        return 0;
    }
    if (len == 0) {
        erreur("La connexion avec le serveur semble interrompue\n");
        return 0;
    }
    return 1;
}

static int insistent_write(SOCKET sock, void *msg, size_t size) {
    ssize_t len=0;
    char *buffer = msg;
    len = send(sock, buffer + len, size - len, 0);
    while ((len > 0) && (len < size) && ((len != SOCKET_ERROR) || (errno == EINTR))) {
        len += send(sock, buffer + len, size - len, 0);
    }
    if (len == SOCKET_ERROR) {
        perreur("Erreur de communication");
        return 0;
    }
    // Should not occur...
    if (len == 0) {
        erreur("La connexion avec le serveur semble interrompue\n");
        return 0;
    }
    return 1;
}

void deconnexion() {
    if (closesocket(a.sock) == -1)
        perreur("Erreur à la fermeture de la connexion");
    if (a.debug)
        debug("Vous avez été deconnecté du serveur\n");
#if defined (WIN32)
    WSACleanup();
#endif
}

int connexion() {
#if defined (WIN32)
    WSADATA WSAData;
    erreur = WSAStartup(MAKEWORD(2, 2), &WSAData);
    if (erreur) {
        perreur("Erreur d'initialisation de winsock");
        return 0;
    }
#endif

    /* Création de la socket */
    a.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (a.sock == -1) {
        perreur("Impossible de créer la socket de communication");
        deconnexion();
        return 0;
    }

    SOCKADDR_IN sin;
    /* Configuration de la connexion */
    /* sin.sin_addr.s_addr = inet_addr("152.77.87.30"); */
    sin.sin_addr.s_addr = inet_addr(HOST);
    // sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    //printf("Connexion à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
    /* Si le client arrive à se connecter */
    if (connect(a.sock, (SOCKADDR *) & sin, sizeof(sin)) == SOCKET_ERROR) {
        perreur("Impossible de se connecter\n");
        deconnexion();
        return 0;
    }
    if (a.debug)
        debug("Vous êtes connecté au serveur\n");

    // On récupère le message de bienvenue du serveur
    int taille = recv(a.sock, a.buffer, TAILLEMESSAGE, 0);
    // Le serveur termine son message de bienvenue par "\n\n"
    char end[] = "\n\n";
    int end_length = strlen(end);
    int done = 0;
    while (((taille != -1) || (errno == EINTR)) && !done) {
        a.buffer[taille] = '\0';
        done = !strcmp(a.buffer+taille-end_length, end);
        if (a.debug) {
            debug(a.buffer);
        }
        if (!done)
            taille = recv(a.sock, a.buffer, TAILLEMESSAGE, 0);
    }
    if (taille == -1) {
        perreur("Erreur à la réception du message de bienvenue");
        deconnexion();
        return 0;
    }

    // Commande de passage en mode paquet
    uint32_t mode_paquets = htonl(0xFFFFFFCC);
    send(a.sock, &mode_paquets, 4, 0);
    uint32_t reponse;
    if (!insistent_read(a.sock, &reponse, 4)) {
        deconnexion();
        return 0;
    }
    return 1;
}

void mode_debug(int val) {
    a.debug = val;
}

void attente_automatique(int val) {
    a.attente = val;
}

int envoyer_recevoir(char *message, char *reponse) {
    uint32_t size = strlen(message);
    // header + trailing character
    char buffer[size+6];

    *((uint32_t *) buffer) = htonl(size);
    strcpy(buffer+4, message);

    if (insistent_write(a.sock, &buffer, size+4)) {
        if (a.debug) {
            debug("Message envoyé au serveur :\n");
            // Pretty print
            message = buffer+4;
            message[size] = '\n';
            message[size+1] = '\0';
            debug(message);
        }
        if (!insistent_read(a.sock, &size, 4)) {
            reponse[0] = '\0';
            return -1;
        }
        size = ntohl(size);
        if (insistent_read(a.sock, reponse, size)) {
            reponse[size] = '\0';
            if (a.debug) {
                debug("Message reçu du serveur :\n");
                debug(reponse);
            }
            if (a.attente) {
                fprintf(stderr, "--- appuyez sur entree pour continuer ---\n");
                fgets(a.buffer, TAILLEMESSAGE, stdin);
            }
            return size;
        } else {
            return -1;
        }
    }
    return -1;
}
