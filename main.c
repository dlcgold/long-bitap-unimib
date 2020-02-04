#include "utils.h"

int main(int argc, char** argv){
  if(argc != 3)
    exit(-1);
  
  char* text = read_file(argv[1]);
  char* pattern = read_file(argv[2]);
  
  
  /* char* pattern = "uic"; */
  /* char* text = "cacacacaccccccacacacacacabbbcbbdbdjcdckdckdckdcbihvvfnvfjvnhrfiurvbuicnhrehgmreouhwovxbuicrrhuxbgrvhmibrx"; */
  
  printf("for pattern: %s\n", pattern);
  printf("in text: %s\n", text);

  bitapLong(pattern, text);
  //free(text);
  //free(pattern);
  return 0;
}
