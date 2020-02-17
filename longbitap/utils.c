#include "utils.h"

// funzione per sottostringa da indice n a indice m
// O(n - m)
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

  // se ho un match metto 1 altrimenti 0
  for(unsigned int i = 0; i < p; i++){
    D[i] = ((D[i] & (one<<(m-1))) != 0) ? 1 : 0;
  }

  // resituisco l'array
  return D;
}

// funzione che resituisce gli indici delle occorrenze
// da usare per il primo sottopattern
// O(p)
uint64_t* countfirst(uint64_t* array, uint64_t size){
  // inizializzo un counter e conto le occorrenze
  uint64_t count = 0;
  for(unsigned int i = 0; i < size; i++){
    if(array[i] == 1)
      count++;
  }

  // alloco l'array con gli indici, lo carico e lo restituisco
  uint64_t j = 0;
  uint64_t* ret = (uint64_t*)malloc(sizeof(uint64_t) * count);
  for(unsigned int i = 0; i < size; i++){
    if(array[i] == 1){
      ret[j] = i;
      j++;
    }
  }
  return ret;
}
// funzione che effettua il bitap su pattern anche più lunghi della grandezza
// della word
void bitapLong(char* pattern, char* text){

  // inizializzo lunghezza di pattern e testo (-1 per terminatore)
  uint64_t m = strlen(pattern) - 1;
  uint64_t p = strlen(text) - 1;

  // inizializzo la grandezza massima della word
  uint64_t w = __WORDSIZE;
  //uint64_t w = 4;
  
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

  // variabile che contiene la lunghezza del sottopattern analizzato
  // se ho un solo sottopattern vedo quanto è lungo
  // altrimenti so che è lungo una word
  uint64_t patterlength = (npatterns == 1) ? strlen(patterns[0]) : w;

  //variabile per muoversi lungo i match del primo sottopattern
  uint64_t shiftfirst = 0;

  // variabile per il conto dei match
  uint64_t count = 0;

  // vettore in cui carico il bitap del primo sottopattern
  uint64_t* curr = bitap(patterns[0], text, patterlength, p);

  // stringa con la finestra di text da analizzare
  char* window;

  // array con gli indici di fine match 
  uint64_t* firsts = countfirst(curr, p);

  // variabile d'appoggio per spostarsi lungo gli indici di match del
  // primo sottopattern
  uint64_t currentfirst = firsts[0];

  // se ho un solo pattern significa che ho un pattern lungo meno di w
  // quindi il bitap calcolato su patterns[0] è già quello finale
  // e posso vedere le occorrenze
  // altrimenti per ogni occorrenza del primo sottopattern itertaivamente
  // verifico i sottopattern successivi spostanto la finestra nel testo
  // evalutando solo le ultime caselle dei vari bitap
  // se sono 0 mi fermo e passo al match sul primo sottopattern successivo
  // altrimenti itero fino ad esaurimento sottopattern e se all'ultima
  // iterazione ho alla fine ho un match (uso patternlength per tenere conto
  // del numero di caratteri già valutati). Infine se ho altri mathc nel primo
  // ripeto partendo da quel match
  if(npatterns == 1){
    for(unsigned int i = 0; i < p; i++){
      if(curr[i] == 1){
	count++;
	printf("occurrance starting at index %ld\n", i - (m - 1));
      }
    }
    free(curr);
  }else{
    // fino a che ho match col primo sottopattern itero
    while(currentfirst){
      // itero per ogni sottopattern
      for(unsigned int i = 1; i < npatterns; i++){
	// valuto la lunghezza del pattern sapendo che solo l'ultimo potrebbe
	// essere lungo meno di w
	patterlength = (i != npatterns - 1)
	  ? w : strlen(patterns[npatterns - 1]);
	
	// calcolo la finestra di analisi ovvero la sottostringa di text che
	// parte dall'indice del match precedente ed è lunga quanto il
	// sottopattern in analisi
	window = substr(text, currentfirst + 1,
			currentfirst + patterlength + 1);
	
	// chiamo bitap col sottopattern in analisi su quella finestra
	curr = bitap(patterns[i], window, patterlength, patterlength);

	// se alla fine del bitap non ho un match mi fermo
	if(curr[patterlength - 1] != 1){
	  free(curr);
	  break;
	}
	
	// altrimenti aggiorno il conteggio dell'indice dell'ultimo match e
	// itero nuovamente
	currentfirst += patterlength;
      }
      
      // se alla fine ho un 1 alla fine dell'ultimo bitap stampo l'inizio
      // dell'occorrenza sapendo che il pattern nel complesso è lungo m e sommo
      // 1 al counter delle occorrenze
      if(curr[patterlength - 1] == 1){
	printf("occurrance starting at %ld\n", (currentfirst + 1) - m);
	count++;
      }

      // mi sposto al match del primo pattern successivo
      shiftfirst++;
      currentfirst = firsts[shiftfirst];
    }
  }

  // stampoo il numero di occorrenze
  printf("total number of occurences: %ld\n", count);

  // libero la memoria
  if(npatterns != 1){
    free(curr);
    free(window);
  }
  free(firsts);
  for(unsigned int i = 0; i < npatterns; i++)
    free(patterns[i]);
  free(patterns);
}

char* load_file(char* path)
{
  // inizializzo il punattore a char per la stringa
  char* buffer = 0;

  // inizlaizzo la variabile per la lunghezza del file e della stringa
  long length = 0;

  // apro il file in lettura col path passato come argomento
  FILE* f = fopen (path, "r");

  // se non riesco ad aprire un file con quel path lo segnalo
  if (f == NULL) {
    //printf("%s\n", path);
    fprintf(stderr, "Can't open input file %s\n", path);
    exit(1);
  }else{
    
    // sposto il puntatore del filen f alla fine con SEEK_END
    fseek(f, 0, SEEK_END);

    // dato che ftell mi restituisce la posizione attuale del puntatore f che si
    // muove sul file calcolo la lunghezza del file visto che l'ho messo alla
    // fine con fseek
    length = ftell(f);

    // metto il puntatore f all'inizio con SEEK_SET
    fseek(f, 0, SEEK_SET);

    // alloco un array di char di lunghezza adeguata
    buffer = (char*)malloc((length + 1) * sizeof(char));

    // se effettivamente il file contiene qualcosa (e quindi ho allocato buffer)
    // con una certa lunghezza uso fread per leggere il file e caricare la
    // stringa
    if (buffer){
      fread(buffer, sizeof(char), length, f);
    }

    // chiudo il file
    fclose (f);
  }
  // libero la memoria
  free(f);

  // restituisco l'array di char
  return buffer;
}

void print(uint64_t* arr, uint64_t size){
  for(unsigned int i = 0; i < size; i++)
    printf("%ld ", arr[i]);
}
