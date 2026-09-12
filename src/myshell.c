#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(int args, char *argv[]) {
  
  // String to hold input buffer
  char buf[BUF_SIZE];

  // Prompt
  fgets(buf, sizeof(buf), stdin);

  // Run the command
  system(buf);
  return EXIT_SUCCESS;
}
