#ifndef TAILLE_MAX
#define TAILLE_MAX 10000
#endif
int isAlpha(char c){
  //return (('a'<=c && 'z'>=c)||('A'<=c && c<='Z')) ; 
  return true ;
  }
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




