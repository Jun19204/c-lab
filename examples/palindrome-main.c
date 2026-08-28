// 회문 검사 프로그램
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define MAX_SIZE 100

typedef char element;
typedef struct {
	element str[MAX_SIZE];
	int top;
}StackType;

// 스택 초기화
void init_stack(StackType* stack) {
	stack->top = -1;
	return;
}

// 스택이 가득 찼는지 검사
int is_full(StackType* stack) {
	if (stack->top >= (MAX_SIZE - 1)) return 1;
	else return 0;
}

// 스택이 비었는지 검사
int is_emty(StackType* stack) {
	if (stack->top <= -1)return 1;
	else return 0;
}

// 스택에 요소를 추가하는 함수
void push(StackType* stack, element ch) {
	if (is_full(stack)) printf("스택이 포화 상태입니다\n");
	stack->str[++(stack->top)] = ch;
	return;
}

// 스택의 마지막 요소를 반환하고 삭제하는 함수
element pop(StackType* stack){
	if (is_emty(stack)) exit(1);
	else return stack->str[(stack->top)--];
}

// 스택의 마지막 요소를 반환만 하는 함수
element peek(StackType* stack) {
	if (is_emty(stack)) exit(1);
	else return stack->str[(stack->top)];
}

// 소문자로 변환하는 함수
void to_lower(char* str) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) { 
		if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32; // 대문자인경우 소문자로 변환
		else continue;
	}
	return;
}

// 회문 확인 함수
int is_palindorme(char* str) {
	StackType s; // 스택 생성
	init_stack(&s); // 스택 초기화

	to_lower(str); // 문자열의 문자를 전부 소문자로 변환

	int len = strlen(str); // 문자열의 길이

	// 문자열의 알파벳만 스택에 넣기
	for (int i = 0; i < len; i++) {
		if (str[i] >= 'a' && str[i] <= 'z') push(&s, str[i]);
	}

	// 회문 검사
	for (int j = 0; j < len; j++) {
		if (str[j] >= 'a' && str[j] <= 'z') { // 원문의 문자가 알파벳이면
			char open_ch = pop(&s);
			if (open_ch != str[j]) return 0;
		}
	}
	return 1;
}

// 메인함수 : 
int main(void) {
	printf("문자열을 입력하시오:");
	char str[MAX_SIZE];
	scanf("%s", str);
	if(is_palindorme(str)) printf("회문이 맞습니다.\n"); // 회문 검사 함수 호출
	else printf("회문이 아닙니다.\n");

	return 0;
}