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
  // copia i caratteri tra gli indici voluti dalla sorgente alla destinazione
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
uint64_t* bitap(char* pattern, char* text, uint64_t m, uint64_t p){

  // inizializzo lunghezza di pattern e testo
  // inizializzo a 0 l'array T con le posizioni dei char nel pattern
  //uint64_t m = strlen(pattern);
  //uint64_t p = strlen(text);
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

  // guardo se ho match o meno guardando l'ultimo bit
  // per comodità setto quella posizione a 1 specificando
  // che lì termina un match

  for(unsigned int i = 0; i < p; i++){
    D[i] = ((D[i] & (one << (m - 1))) != 0) ? 1 : 0;
  }

  return D;
}


// funzione che effettua il bitap su pattern più lunghi della grandezza della
// word
// O(p * ceil(m / w))
void bitapLong(char* pattern, char* text){

  // inizializzo lunghezza di pattern e testo (-1 per terminatore)
  uint64_t m = strlen(pattern) - 1;
  uint64_t p = strlen(text) - 1;

  // inizializzo la grandezza massima della word
  uint64_t w = __WORDSIZE;
  //uint64_t w = 3;
  
  // conto in quanti sottopattern devo dividere il pattern per non eccedere da w
  unsigned int npatterns = ceil((double) m / w);

  
  // alloco un array di stringhe per contenere i vari sottopattern
  char** patterns = (char **)malloc(npatterns * sizeof(char *));
  for (unsigned int i=0; i < npatterns; i++)
    patterns[i] = (char *)malloc(sizeof(char) * w);

  // carico l'array dei sottopattern
  // saranno tutti di lunghezza w tranne l'ultimo che sarà di lunghezza tale da
  // consumare il pattern
  unsigned int begin = 0;
  char* sub;
  for(unsigned int i = 0; i < npatterns; i++){
    if(m - begin < w)
      sub = substr(pattern, begin, m);
    else
      sub = substr(pattern, begin, begin + w);
    strcpy(patterns[i], sub);
    begin += w;
    free(sub);
  }
  
  // tengo un array con i match del sottopattern precedente, corrente e
  // risultante. Ad ogni giro in prev carico il precedente risultante e in curr
  // il bitap del sottapttern attuale, salvando in res i match del complesso dei
  // sottopattern precedenti. A priori so che in ret avrò 0 fino agli indici
  // inferiori alla lunghezza del mio attuale sottopattern.
  uint64_t* prev = (uint64_t*)malloc(sizeof(uint64_t) * p);
  uint64_t* curr;
  uint64_t patternlength = (npatterns != 1) ? w : strlen(patterns[0]);
  uint64_t* res = bitap(patterns[0], text, patternlength, p);
  uint64_t begpat = 0;
  for(unsigned int i = 1; i < npatterns; i++){      
    if(i != npatterns -1){
      patternlength = w;
      begpat += w;
    }else{
      patternlength = strlen(patterns[i]);
      begpat += strlen(patterns[i]);
    }
    
    // copio res in prev a partire dal primo 1
    memcpy(prev, res, p * sizeof(uint64_t));

    // carico curr
    curr = bitap(patterns[i], text, patternlength, p);


    // carico res 
    for(unsigned int j = 0; j < p; j++){
      res[j] = (curr[j] == 1 && prev[j - patternlength] == 1) ? 1 : 0;
    }

  }

  // libero la memoria
  free(prev);

  // inizializzo il count dei match
  unsigned count = 0;


  //stampo occorrenze
  for(unsigned int i = 0; i < p; i++){
    if(res[i] == 1){
      count++;
      printf("occurrance starting at index %ld\n", i - (m - 1));
    }
  }
  
  // stampo conteggio
  printf("total number of occurences: %d\n", count);


  // libero la memoria
  for (unsigned int i = 0; i < npatterns; i++)
    free(patterns[i]);
  free(patterns);
  free(res);
}

char* load_file(char* path)
{
  char* buffer = 0;
  long length;
  FILE * f = fopen (path, "r");
  if (f == NULL) {
    printf("%s\n", path);
    fprintf(stderr, "Can't open input file\n");
    exit(1);
  }else{
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = (char*)malloc ((length+1)*sizeof(char));
    int r;
    if (buffer){
      r = fread (buffer, sizeof(char), length, f);
       if(r == -1){
	fprintf(stderr, "unexpected error %s\n", path);
	exit(1);
      }
    }
    fclose (f);
  }
  free(f);
  return buffer;
}

void print(uint64_t* arr, uint64_t size){
  for(unsigned int i = 0; i < size; i++)
    printf("%ld ", arr[i]);
}
