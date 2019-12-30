#include "utils.h"
#include <stdio.h>


int main(){
  //char* str = charge_file("./lorem.txt");
  char str[] = "AGCAGBGCAGCABABAGCABGGCAGCAGBBBCCAAGCA";
  //char* pattern = charge_file("./pattern.txt");
  char pattern[] = "GCA";
  printf("text: %s\n", str);
  printf("pattern: %s pattern length: %ld\n", pattern, strlen(pattern));
  int l = strlen(pattern);
  //char character = 'a';
  
  //int* u_char = charge_u_char(pattern, character);
  //int* u_char = (int*)malloc(sizeof(int) * strlen(pattern));
  int m[strlen(str)];

  int var = 0;
  // se pattern inizia come text inizializzo a 100... lungo quanto il pattern
  if(pattern[0] == str[0])
    var = (int)pow(2, strlen(pattern) - 1);

  m[0] = array_bitn(var, pattern);
  for(unsigned long int i = 1; i < strlen(str); i++){
    var = (((var >> 1) | (0b1 << (l-1))) & charge_u_int(pattern, str[i]));
    m[i] = array_bitn(var, pattern);
  }
  int count = 0; 
  for(int i = 0; i < strlen(str); i++) {
    if(m[i] == 1){
      count ++;
    }
  }
  int sol[count];
  int j = 0;
  for(unsigned long int i = 0; i < strlen(str); i++){
    if(m[i] == 1){
      sol[j] = i - (strlen(pattern)-1);
      j++;
    }
  }
  printf("total match: %ld\n", sizeof(sol) / sizeof(sol[0]));
  for(int i = 0; i < count; i++) {
    printf("match at position %d\n", sol[i]);
  }

  //free(pattern);
  //free(str);

  return 0;  
}
