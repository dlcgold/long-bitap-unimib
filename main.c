#include "utils.h"

int main(int argc, char** argv){
  static struct gengetopt_args_info args_info;
  assert(cmdline_parser(argc, argv, &args_info) == 0);

  /* if(argc != 3) */
  /*   exit(-1); */

  // carico i file nei rispettivi array di char
  /* char* text = load_file(argv[1]); */
  /* char* pattern = load_file(argv[2]); */

  char* pattern = load_file(args_info.pattern_arg);
  char* text = load_file(args_info.text_arg);

  // stampo pattern e testo (con le rispettive lunghezze)
  printf("pattern length %ld\n", strlen(pattern) - 1);
  printf("text length %ld\n", strlen(text) - 1);

  // chiamo l'algoritmo
  bitapLong(pattern, text);

  //free(text);
  //free(pattern);
  return 0;
}
