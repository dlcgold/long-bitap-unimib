#include "utils.h"

int main(int argc, char** argv){
  if(argc != 3)
    exit(-1);
  
  char* text = read_text(argv[1]);
  char* pattern = read_text(argv[2]);
  
  
  /* char* pattern = "uic"; */
  /* char* text = "cacacacaccccccacacacacacabbbcbbdbdjcdckdckdckdcbihvvfnvfjvnhrfiurvbuicnhrehgmreouhwovxbuicrrhuxbgrvhmibrx"; */
  
  printf("for pattern: %s\nof length %ld\n", pattern, strlen(pattern) - 1);
  printf("in text: %s\nof length %ld\n", text, strlen(text) - 1);

  bitapLong(pattern, text);
  //free(text);
  //free(pattern);
  return 0;
}
