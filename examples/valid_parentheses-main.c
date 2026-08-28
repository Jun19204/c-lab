#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define MAX_STACK_SIZE 100

typedef char element;
typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
}StackType;

// 스택 초기화 함수
void init_stack(StackType* s) { s->top = -1; }

// 스택 공백 상태 검사 함수
int is_empty(StackType* s) {
	if (s->top == -1) return 1;
	else return 0;
}

// 스택 포화 상태 검사 함수
int is_full(StackType* s) {
	if (s->top >= (MAX_STACK_SIZE - 1)) return 1;
	else return 0;
}

// 스택 요소 삽입 함수
void push(StackType* s, element newdata) {
	if (is_full(s)) { printf("스택 공간이 부족합니다."); exit(1); }
	else s->data[++(s->top)] = newdata;
}

// 스택 마지막 요소 반환 및 제거 함수
element pop(StackType* s) {
	if (is_empty(s)) { printf("스택이 공백상태입니다."); exit(1); }
	else return s->data[(s->top)--];
}

// 스택 마지막 요소 확인 함수
element peek(StackType* s) {
	if (is_empty(s)) { printf("스택이 공백상태입니다."); exit(1); }
	else return s->data[(s->top)];
}

// 괄호 크기 구분 함수
int prec(char op) {
	switch (op) {
	case '(': case ')': return 0;
	case '{': case '}': return 1;
	case '[': case ']': return 2;
	}
	return -1;
}

// 괄호 검사 함수(이상 없다면 1반환, 이상 있다면 0반환)
int check_matching(const char* exp) {
	StackType stack; // 스택 생성
	init_stack(&stack); // 스택 초기화
	int idx = 0;
	char exp_ch;
	// 인덱스가 수식의 길이보다 작다면 계속 반복
	while (idx < strlen(exp)) {
		exp_ch = exp[idx++]; // 수식의 문자
		switch (exp_ch) {
		// 수식의 문자가 열린 괄호인 경우
		case '(': case '{': case '[':
			if (!is_empty(&stack)) { // 스택에 괄호가 존재하는 경우
				if (prec(peek(&stack)) < prec(exp_ch)) return 0; // 스택의 열린 괄호가 수식의 열린 괄호보다 작은 괄호라면 0반환
				else push(&stack, exp_ch); // 그렇지 않다면 열린 괄호를 스택에 삽입
			}
			// 스택이 공백이라면
			else push(&stack, exp_ch); // 열린 괄호를 괄호스택에 삽입
			break;
		// 수식의 문자가 닫힌 괄호인 경우
		case ')': case '}': case ']':
			if (is_empty(&stack)) return 0; // 스택이 공백이면 잘못된 수식이므로 0반환
			else { // 스택이 공백이 아닌 경우
				char stack_ch = pop(&stack); // 스택 맨 위의 괄호, 스택에서는 삭제
				// 스택의 괄호와 수식의 괄호의 페어 여부 확인
				if ((stack_ch == '(' && exp_ch != ')') ||// 스택의 괄호는 소괄호면서 수식의 괄호가 소괄호가 아닌 경우 or
					(stack_ch == '{' && exp_ch != '}') ||// 스택의 괄호는 중괄호면서 수식의 괄호가 중괄호가 아닌 경우 or
					(stack_ch == '[' && exp_ch != ']'))// 스택의 괄호는 대괄호면서 수식의 괄호가 대괄호가 아닌 경우
					return 0; // 잘못된 수식이므로 0반환
			}
			break;
		// 수식의 문자가 괄호가 아닌 경우
		default: break;
		}
	}
	if (!is_empty(&stack)) return 0; // 스택에 괄호가 남아있다면 0반환
	else return 1; // 남은 괄호가 없다면 1반환
}

// 메인 함수
int main(void) {
	printf("수식 입력:");
	char exp[MAX_STACK_SIZE];
	scanf("%s", exp); // 수식 입력 받기
	if (check_matching(exp)) printf("--> 성공!!!\n");
	else printf("--> 실패\n");
	return 0;
}