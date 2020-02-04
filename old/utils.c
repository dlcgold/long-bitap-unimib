#include "utils.h"

char* charge_file(char* file_name) {

  FILE *fp;
  fp = fopen(file_name, "r"); // apro in lettura
  if (fp == NULL) {
    fprintf(stderr, "Can't open input file\n");
    exit(1);
  }
  
  unsigned int i = 0;
  char c = (char)fgetc(fp); // fgetc legge un char dallo stream
  while (c != EOF) { // itero fino a che ho caratteri 
    c = (char)fgetc(fp); // carico in c il carattere successivo
    i++;
  }
  fclose(fp);
  
  fp = fopen(file_name, "r");
  // dichiaro un array di char lungo quanto il file
  char *str = (char*)malloc(sizeof(char) * (i + 1)); 
  str[0] = '\0';
  char cc = (char)fgetc(fp); // fgetc legge un char dallo stream
  while (cc != EOF) { // itero fino a che ho caratteri 
    strncat(str, &cc, 1); // appendo il carattere alla stringa
    cc = (char)fgetc(fp); // carico in c il carattere successivo
  }
  fclose(fp);
  return str;
}

/*int* charge_u_char(char* pattern, char character){
  //printf("%c", character);
  int* uc = (int*)malloc(sizeof(int) * (strlen(pattern)));
  for(int i = strlen(pattern); i >= 0; i--){
    if(pattern[i] == character)
      uc[i] = 1;
    else
      uc[i] = 0;
  }
  return uc;
  }*/


int charge_u_int(char* pattern, char character){
  int uc = 0;
  // contatore per la potenza per rispettare l'ordine binario  
  unsigned long int l = strlen(pattern) - 1;
  for(unsigned long i = 0; i < strlen(pattern); i++){
    if(pattern[i] == character){
      uc = uc + (int)(pow(2, l) * 1);
    }else
      uc = uc + (int)(pow(2, l) * 0);
    l--;
  }

  return uc;
}


int array_bitn(int num, char* pattern){
  unsigned long int l = strlen(pattern);
  // per un intero in binario servono log_2(num)+1 cifre binarie
  // int bin_l = floor(log(num) / log(2)) + 1;
  int *array_bin = (int*)malloc(sizeof(int) * l);
  // ciclo all'inverso per caricare l'array con il binario
  // printf("%d", l);
  for(unsigned long int i = l-1 ; i != 0; i--){
    array_bin[i] = num % 2;
    num = num / 2;
  }
  /* printf("\n"); */
  /* for(int i = 0 ; i < l; i++){ */
  /*   printf("%d", array_bin[i]); */
  /* } */
  /* printf("\n"); */
  int res = array_bin[l-1];
  free(array_bin);
  return res;
}

/*int* array_bit(int num, char* pattern){
  int l = strlen(pattern);
  // per un intero in binario servono log_2(num)+1 cifre binarie
  // int bin_l = floor(log(num) / log(2)) + 1;
  int *array_bin = (int*)malloc(sizeof(int) * l);
  // ciclo all'inverso per caricare l'array con il binario
  // printf("%d", l);
  for(int i = l-1 ; i >=0; i--){
    array_bin[i] = num % 2;
    num = num / 2;
  }
  // printf("\n"); 
   //for(int i = 0 ; i < l; i++){ 
     //printf("%d", array_bin[i]); 
   //} 
   //printf("\n"); 
  return array_bin;
}*/
