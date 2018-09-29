#ifndef TAILLE_MAX
#define TAILLE_MAX 10000
#endif

typedef struct { 
  char liste[TAILLE_MAX];
  int longueur;
} sequence ;


int indexa(char c, sequence seq) {
  int i;
  for(i=0; seq.liste[i] != c; i++) {
    if (i >= seq.longueur) return -1; 
  }
  return i;
}

void ajouter_fin(char c , sequence *seq) {
  seq->liste[seq->longueur] = c;
  seq->longueur++ ;
}

void decaseq(char c, sequence *seq) {
  int i; 
  int l_trouvee = 0;
  for (i=0; i< seq->longueur;i++) {
    l_trouvee =( l_trouvee || (seq->liste[i]==c) );
    if (l_trouvee){
      seq->liste[i] = (i < seq->longueur-1)? seq->liste[i+1]:c;
    }
  }
}

void cryptseq(sequence *message) {
  sequence seq;
  seq.longueur=0 ;
  for(int i=0; i < message->longueur; i++) {
    char c = message->liste[i];
    char cp=c;
    
    int index_c = indexa(c, seq);
    if (index_c == -1) {
      ajouter_fin(c,&seq);
    }
    else {
      if (index_c == 0) index_c = seq.longueur ;
        
      cp=seq.liste[index_c-1];
      decaseq(c,&seq);
        
     
    }
    message->liste[i] = cp;
  }
  
}

void decryptseq(sequence *message) {
  sequence seq;
  seq.longueur=0 ;
  for(int i=0; i < message->longueur; i++) {
    char c = message->liste[i];
    
    int index_c = indexa(c, seq);
    if (index_c == -1) {
      ajouter_fin(c,&seq);
    }
    else {
      index_c = (index_c + 1) % seq.longueur ;
        
      message->liste[i] = seq.liste[index_c];
      decaseq(seq.liste[index_c],&seq);
        
       
    }
  }
  
}

