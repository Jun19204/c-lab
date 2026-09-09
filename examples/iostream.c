#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 128
#define STR_SIZE 50

int main(void) {
  
  char buffer[BUFF_SIZE];

  // 1. 문자열 입력
  char str[STR_SIZE];
  printf("문자열 입력: ");
  if (fgets(str, sizeof str, stdin) == NULL) {
    return EXIT_FAILURE;
  }
  str[strcspn(str, "\n")] = '\0';

  // 2. 문자 입력
  char c;
  printf("문자 입력: ");
  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    return EXIT_FAILURE;
  }
  c = buffer[0];

  // 3. 숫자 입력
  long num;
  char* endptr;
  errno = 0;
  printf("정수 입력: ");
  if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    return EXIT_FAILURE;
  }
  num = strtol(buffer, &endptr, 10);


  printf("%s %c %ld", str, c, num);
  return EXIT_SUCCESS;
}
