#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "cmdline.h"

char* substr(const char *src, int m, int n){
  // lunghezza della stringa di destinazione
  int len = n - m;

  // alloca la stringa di destinazione (più 1 per il carattere nullo)
  char *dest = (char*)malloc(sizeof(char) * (len + 1));

  // extracts characters between m'th and n'th index from source string
  // and copy them into the destination string
  // copia i caratteri tra gli indici volutiu dalla sorgente alla destinazione
  for (int i = m; i < n && (*src != '\0'); i++) {
    *dest = *(src + i);
    dest++;
  }

  // aggiunge il carattere nullo
  *dest = '\0';
  
  // ritorna la stringa della giusta dimensione
  return dest - len;
}

// funzione che effettua la ricerca tramite algoritmo bitap con
// lunghezza del pattern minore della grandezza della word
int* bitap(char* pattern, char* text){
  
  // inizializzo lunghezza di pattern e testo
  // inizializzo a 0 l'array T con le posizioni dei char nel pattern
  unsigned int m = strlen(pattern);
  unsigned int p = strlen(text);
  unsigned int T[CHAR_MAX] = {};

  // inizializzo la mia "matrice" di word
  int* D = (int *)malloc(sizeof(int) * p);
 
  // PREPROCESAMENTO
  // si parte con ogni posizione di T a 0
  // mi muovo sui char del pattern aggiornando T in quella posizione
  // spostando l'indice i lungo il pattern faccio un or di 1 shiftato di i
  // posizioni con il vecchio contenuto di T per un determinato char
  // alla fine del ciclo avrò l'array T caricato
  for(unsigned int i = 0; i < m; i++){
    T[(int)pattern[i]] |= (1<<i);
  }
 
  // seguo l'algoritmo e setto 1 a M[0] sse pattern e text cominciano con lo
  // stesso carattere
  D[0] = (pattern[0] == text[0]) ? 1 : 0;

  // formula dell'algoritmo per calcolare le colonne a partire dalla precedente
  for(unsigned int i = 1; i < p; i++){
    D[i] = ((D[i - 1] << 1) | 1) & T[(int)text[i]];
  }
  
  // guardo se ho match o meno guardando l'ultimo bit
  // per comodità setto quella posizione a 1 specificando
  // che lì termina un match
  for(unsigned int i = 0; i < p; i++){ 
    if((D[i] & (1<<(m-1))) != 0)
      D[i] = 1;
    else
      D[i] = 0;
  }
  
  return D;
}


// struttura per restituire sia l'array delle soluzioni che il numero di esse
/* struct array{ */
/*   int* sol; */
/*   unsigned int count; */
/* }; */

// funzione che effettua il bitap su pattern più lunghi della grandezza della
// word
void bitapLong(char* pattern, char* text){
  // inizializzo lunghezza di pattern e testo
  unsigned int m = strlen(pattern);
  unsigned int p = strlen(text);
 
  // inizializzo la grandezza massima della word
  unsigned int w = __WORDSIZE;

 
  // conto in quanti sottopattern devo dividere il pattern per non eccedere da w
  unsigned int npatterns = ceil((double) m / w);
  
  // alloco un array di stringhe per contenere i vari sottopattern
  char** patterns = (char **)malloc(npatterns * sizeof(char *));
  for (unsigned int i=0; i < npatterns; i++) 
    patterns[i] = (char *)malloc((w + 1) * sizeof(char));

  // carico l'array dei sottopattern
  // saranno tutti di lunghezza w tranne l'ultimo che sarà di lunghezza tale da
  // consumare il pattern
  unsigned int begin = 0;
  char* sub;
  for(unsigned int i = 0; i < npatterns; i++){
    if(m - begin < w)
      w = m - begin;
    sub = substr(pattern, begin, begin + w);
    strcpy(patterns[i], sub);
    begin += w;
    free(sub);
  }
 
  // creo la matrice coi singoli bitap per sottopattern
  int** DL = (int **)malloc(npatterns * sizeof(int *));
  for (unsigned int i=0; i < npatterns; i++)
    DL[i] = (int *)malloc(p * sizeof(int)); 

  // carico le righe con i singoli dei bitap
  int* bitapres;
  for(unsigned int i = 0; i < npatterns; i++){
    bitapres = bitap(patterns[i], text);
    // copio l'array resitutito da bitap nella rige e libero memoria
    memcpy(DL[i], bitapres, p * sizeof(int));
    free(bitapres);
  }
  
  // se ho un pattern minore di w non serve verificare nulla
  // altrimenti verifico di essere nei pattern successivi al primo
  // se nel pattern precedente non ho un match che termina quando
  // dovrebbe iniziare il match che ho col pattern successivo setto a 0
  // in modo che a priori non si abbiano pattern possibili
  // avrò i match complessivi nell'ultima riga della matrice
  for(unsigned int i = 1; i < npatterns; i++){
    for(unsigned int j = 0; j < p; j++){
      if((DL[i][j] == 1 && DL[i-1][j-strlen(patterns[i])]==1))
	DL[i][j]=1;
      else
	DL[i][j]=0;
    }
  }
  for(unsigned int i = 1; i < npatterns; i++){
    for(unsigned int j = 0; j < p; j++){
      printf("%d ", DL[i][j]);
    }
    puts("\n\n\n\n");
  }
 
  // inizializzo il count dei match
  unsigned count = 0;
  
  // guardo quali presentano 1 nell'ultima riga e li conto
  for(unsigned int i = 0; i < p; i++){
    if(DL[npatterns-1][i] == 1)
      count++;
  }

  // inizializzo un array delle soluzioni
  // carico l'array con l'indice di partenza e non di fine
  int j = 0;
  int* sol = (int*)malloc(sizeof(int) * count);
  for(unsigned int i = 0; i < p; i++){
    if(DL[npatterns-1][i] == 1){
      sol[j] = i - (m-1);
      j++;
    }
  }

  // stampo i risultati
  printf("total number of occurences: %d\n", count);
  for(unsigned int i = 0; i < count; i++)
    printf("occurrance at index %d\n", sol[i]);

  // libero la memoria
  free(sol);
  for (unsigned int i=0; i < npatterns; i++)
    free(DL[i]);
  free(DL);
  for (unsigned int i=0; i < npatterns; i++)
    free(patterns[i]);
  free(patterns);
  
}

char* charge_file(char* file_name) {

  FILE *fp;
  fp = fopen(file_name, "r"); // apro in lettura
  if (fp == NULL) {
    fprintf(stderr, "Can't open input file\n");
    exit(1);
  }
  
  unsigned int size = 0;
  char c = (char)fgetc(fp); // fgetc legge un char dallo stream
  while (c != EOF) { // itero fino a che ho caratteri 
    c = (char)fgetc(fp); // carico in c il carattere successivo
    size++;
  }
  fclose(fp);
  
  fp = fopen(file_name, "r");
  // dichiaro un array di char lungo quanto il file
  char *str = (char*)malloc(sizeof(char) * (size + 1)); 
  str[0] = '\0';
  char cc = (char)fgetc(fp); // fgetc legge un char dallo stream
  while (cc != EOF) { // itero fino a che ho caratteri 
    strncat(str, &cc, 1); // appendo il carattere alla stringa
    cc = (char)fgetc(fp); // carico in c il carattere successivo
  }
  fclose(fp);
  return str;
}


int main(int argc, char** argv){
  static struct gengetopt_args_info args_info;
  assert(cmdline_parser(argc, argv, &args_info) == 0);
  char* pattern = args_info.pattern_orig;
  char* text = charge_file(args_info.text_arg);
  
  /* char* pattern = "aacccaa"; */
  /* char* text = "caacccaacaaccccccaac"; */
  
  printf("for pattern %s\n", pattern);
  bitapLong(pattern, text); 
}
