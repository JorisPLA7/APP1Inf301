#ifndef TAILLE_MAX
#define TAILLE_MAX 10000
#endif

typedef struct { //Type sequence
  char liste[TAILLE_MAX];
  int longueur;
} sequence ;


int indexa(char c, sequence seq) {
  // Renvoie l'indice de la premiere occurence du caractere c dans la sequence fournie seq
  int i;
  for(i=0; seq.liste[i] != c; i++) {
    if (i >= seq.longueur) return -1; 
  }
  return i;
}

void ajouter_fin(char c , sequence *seq) {
  //ajoute un element a la fin d'une sequence
  seq->liste[seq->longueur] = c; // ajout de l'element
  seq->longueur++ ; //mise a jour de la longueur
}

void decaseq(char c, sequence *seq) {
  //deplace la premiere occurence du caractere c vers la fin de la sequence fournie en decalant les valeurs 
  int i; 
  int l_trouvee = 0; //drapeau (VRAI si on a trouve le caractere a deplacer)
  for (i=0; i< seq->longueur;i++) {
    l_trouvee =( l_trouvee || (seq->liste[i]==c) );// mise a jour du drapeau a chaque caractere lu
    if (l_trouvee){//si on a depasse le caractere, on decale les valeurs
      seq->liste[i] = (i < seq->longueur-1)? seq->liste[i+1]:c; //si on arrive en bout de sequence, on replace le caractere
    }
  }
}

void cryptseq(sequence *message) {
  //Permet de crypter un message par la derniere methode de cryptage par sequence
  sequence seq;
  seq.longueur=0 ;
  
  for(int i=0; i < message->longueur; i++) { // pour chaque caractere du message...
    char c = message->liste[i];
    char cp=c;
    
    int index_c = indexa(c, seq);//...on regarde sa presence et sa place dans la sequence de cryptage
    
    if (index_c == -1) {//si il manque, on l'ajoute
      ajouter_fin(c,&seq);
    }
    else {//sinon, on applique la transformation
      
      if (index_c == 0) index_c = seq.longueur ;
        
      cp=seq.liste[index_c-1]; //on recupere alors le caractere precedant dans la sequence
      
      decaseq(c,&seq);//on deplace le caractere lu a la fin de la sequence
        
     
    }
    message->liste[i] = cp; // on ecrit le nouveau caractere a la place de l'ancien dans le message
  }
  
}

void decryptseq(sequence *message) {
  //Decrypte un message code selon le cryptage par sequence
  sequence seq;
  seq.longueur=0 ;
  for(int i=0; i < message->longueur; i++) {// pour chaque caractere du message crypte...
    char c = message->liste[i];
    
    int index_c = indexa(c, seq);//...on regarde sa presence et sa place dans la sequence de cryptage
    
    if (index_c == -1) {//si il manque, on l'ajoute
      ajouter_fin(c,&seq);
    }
    else {//sinon, on applique la transformation
      
      index_c = (index_c + 1) % seq.longueur ;
        
      message->liste[i] = seq.liste[index_c];//on ecrit alors a la place du caractere lu le caractere precedant dans la sequence
      decaseq(seq.liste[index_c],&seq);//on deplace ensuite le nouveau caractere a la fin de la sequence
        
       
    }
  }
  
}

