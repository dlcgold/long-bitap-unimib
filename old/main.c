#include "utils.h"
#include <stdio.h>


int main(){
  char* str = charge_file("./lorem.txt");
  //char str[] = "AGCAGBGCAGCABABAGCABGGCAGCAGBBBCCAAGCA";
  //char* pattern = charge_file("./pattern.txt");
  char pattern[] = "sit";
  //printf("text: %s\n", str);
  //printf("pattern: %s pattern length: %ld\n", pattern, strlen(pattern));
  unsigned long l = strlen(pattern);
  //char character = 'a';
  
  //int* u_char = charge_u_char(pattern, character);
  //int* u_char = (int*)malloc(sizeof(int) * strlen(pattern));
  //int m[strlen(str)];
  int* m =(int*)malloc(sizeof(int) * strlen(str));
  int var = 0;
  // se pattern inizia come text inizializzo a 100... lungo quanto il pattern
  if(pattern[0] == str[0])
    var = (int)(pow(2, strlen(pattern) - 1));

  m[0] = array_bitn(var, pattern);
  for(unsigned long int i = 1; i < strlen(str); i++){
    var = (((var >> 1) | (1 << (l-1))) & charge_u_int(pattern, str[i]));
    m[i] = array_bitn(var, pattern);
  }
  unsigned int count = 0; 
  for(unsigned long i = 0; i < strlen(str); i++) {
    if(m[i] == 1){
      count ++;
    }
  }
  
  int* sol = (int*)malloc(sizeof(int) * count);
  int j = 0;
  for(unsigned long int i = 0; i < strlen(str); i++){
    if(m[i] == 1){
      sol[j] = (int)(i - (strlen(pattern)-1));
      j++;
      //printf("%ld ", i-(strlen(pattern)-1));
    }
  }
  printf("total match: %d\n", count);
  for(unsigned int i = 0; i < count; i++) {
    printf("match at position %d\n", sol[i]);
  }

  //free(pattern);
  free(str);
  free(m);
  free(sol);
  return 0;  
}
