#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(int args, char *argv[]) {
  
  // String to hold input buffer
  char buf[BUF_SIZE];

  // Prompt
  printf("$ ");

  // Get input from keyboard
  if (fgets(buf, sizeof(buf), stdin) == NULL) {
    fprintf(stderr, "shell is terminated...\n");
    return EXIT_FAILURE;
  }

  // Run the command
  system(buf);
  return EXIT_SUCCESS;
}
