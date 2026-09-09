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
  if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    buffer[strcspn(buffer, "\n")] = '\0';
    snprintf(str, sizeof(str), "%s", buffer);
  }

  // 2. 문자 입력
  char c;
  printf("문자 입력: ");
  if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    c = buffer[0];
  }

  // 3. 숫자 입력
  int num;
  printf("정수 입력: ");
  if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    if (sscanf(buffer, "%d", &num) != 1) {
      fprintf(stderr, "숫자 파싱 실패");
      return EXIT_FAILURE;
    }
  }


  printf("%s %c", str, c);
  return EXIT_SUCCESS;
}
