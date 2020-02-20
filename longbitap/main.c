#include "utils.h"

int main(int argc, char** argv){
  // inizializzo input con gengetopt 
  static struct gengetopt_args_info args_info;
  assert(cmdline_parser(argc, argv, &args_info) == 0);

  // leggo i due file
  char* pattern = load_file(args_info.pattern_arg);
  char* text = load_file(args_info.text_arg);

  // stampo pattern e testo (con le rispettive lunghezze)
  printf("pattern length: %s%ld%s\n", GREEN, strlen(pattern) - 1, RESET);
  printf("text length: %s%ld%s\n\n", GREEN, strlen(text) - 1, RESET);

  // chiamo l'algoritmo
  bitapLong(pattern, text);
  // libero la memoria
  /* free(text); */
  /* free(pattern); */
  return 0;
}
