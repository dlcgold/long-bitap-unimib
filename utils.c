#include "utils.h"

// funzione per sottostringa da indice n a indice m
// O(|substr|)
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
// O(m + p)
uint64_t* bitap(char* pattern, char* text){
  
  // inizializzo lunghezza di pattern e testo
  // inizializzo a 0 l'array T con le posizioni dei char nel pattern
  uint64_t m = strlen(pattern);
  uint64_t p = strlen(text);
  uint64_t T[CHAR_MAX] = {};
  // 1 esplcitamente a 64bit
  int64_t one = 1;

  // inizializzo la mia "matrice" di word
  uint64_t* D = (uint64_t *)malloc(sizeof(uint64_t) * p);
 
  // PREPROCESAMENTO
  // si parte con ogni posizione di T a 0
  // mi muovo sui char del pattern aggiornando T in quella posizione
  // spostando l'indice i lungo il pattern faccio un or di 1 shiftato di i
  // posizioni con il vecchio contenuto di T per un determinato char
  // alla fine del ciclo avrò l'array T caricato
  for(unsigned int i = 0; i < m; i++){
    T[(int)pattern[i]] |= (one<<i);
  }
 
  // seguo l'algoritmo e setto 1 a M[0] sse pattern e text cominciano con lo
  // stesso carattere
  D[0] = (pattern[0] == text[0]) ? 1 : 0;

  
  // formula dell'algoritmo per calcolare le colonne a partire dalla precedente
  for(unsigned int i = 1; i < p; i++){
    D[i] = ((D[i - 1] << one) | one) & T[(int)text[i]];
  }

  //int* ret = (int*)malloc(sizeof(int) * p);
  // guardo se ho match o meno guardando l'ultimo bit
  // per comodità setto quella posizione a 1 specificando
  // che lì termina un match
  for(unsigned int i = 0; i < p; i++){
    if((D[i] & (one<<(m-1))) != 0)
      D[i] = 1;
    else
      D[i] = 0;
  }
  
  return D;
}


// funzione che effettua il bitap su pattern più lunghi della grandezza della
// word
// O(p * ceil(m / w))
void bitapLong(char* pattern, char* text){
  
  // inizializzo lunghezza di pattern e testo (-1 per terminatore)
  unsigned int m = strlen(pattern) - 1;
  unsigned int p = strlen(text) - 1;
  
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
  uint64_t** DL = (uint64_t **)malloc(npatterns * sizeof(uint64_t *));
  for (unsigned int i=0; i < npatterns; i++)
    DL[i] = (uint64_t *)malloc(p * sizeof(uint64_t)); 

  // carico le righe con i singoli dei bitap
  uint64_t* bitapres;
  for(unsigned int i = 0; i < npatterns; i++){
    bitapres = bitap(patterns[i], text);
    // copio l'array resitutito da bitap nella rige e libero memoria
    memcpy(DL[i], bitapres, p * sizeof(uint64_t));
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
  // stampa matrici
  /* for(unsigned int i = 0; i < npatterns; i++){ */
  /*   printf("pattern: %s\n", patterns[i]); */
  /*   for(unsigned int j = 0; j < p; j++){ */
  /*     printf("%ld ", DL[i][j]); */
  /*   } */
  /*   puts("\n\n\n\n"); */
  /* } */
 
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
    printf("occurrance starting at index %d\n", sol[i]);

  // libero la memoria
  free(sol);
  for (unsigned int i=0; i < npatterns; i++)
    free(DL[i]);
  free(DL);
  for (unsigned int i=0; i < npatterns; i++)
    free(patterns[i]);
  free(patterns);
}

char* read_file(char* file_name){
  FILE *fp;
  fp = fopen(file_name, "r"); // apro in lettura
  if (fp == NULL) {
    printf("%s\n", file_name);
    fprintf(stderr, "Can't open input file\n");
    exit(1);
  }
  
  unsigned int size = 0;
  char c = (char)fgetc(fp); // fgetc legge un char dallo stream
  while (c != EOF) {
    size++;// itero fino a che ho caratteri
    c = (char)fgetc(fp); // carico in c il carattere successivo
  }
  fclose(fp);
  printf("%d\n", size);
  char *str = (char*)malloc(sizeof(char) * (size));
  unsigned int i=0;
  fp = fopen(file_name, "r");
  // itero caricando i char
  /* while (i < size) { */
  /*   fscanf(fp , "%c" ,&c);  */
  /*   str[i] = c; */
  /*   i++; */
  /* } */
  while (c != EOF) {
    size++;// itero fino a che ho caratteri
    c = (char)fgetc(fp); // carico in c il carattere successivo
    str[++i] = c;  
  }
  printf("%ld\n", strlen(str));
  return str;
}

char* load_file(char* path)
{
    char* buffer = 0;
    long length;
    FILE * f = fopen (path, "rb"); //was "rb"

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = (char*)malloc ((length+1)*sizeof(char));
      if (buffer)
      {
        fread (buffer, sizeof(char), length, f);
      }
      fclose (f);
    }
    //buffer[length] = '\0';

    return buffer;
}
