#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 100
#define INFIX_TO_POSTFIX
#define EVAL_POSTFIX

typedef char operator;
typedef struct {
	operator data[MAX_STACK_SIZE];
	int top;
}StackType; // 수식기호 저장 스택
typedef int operand;
typedef struct {
	operand data[MAX_STACK_SIZE];
	int top;
}OperandStack; // 피연산자 저장 스택

// 스택 초기화 함수
void init_stack_opt(StackType* s) { s->top = -1; }
void init_stack_opd(OperandStack* s) { s->top = -1; }

// 스택 공백 상태 검사 함수
int is_empty_opt(StackType* s) {
	if (s->top == -1) return 1;
	else return 0;
}
int is_empty_opd(OperandStack* s) {
	if (s->top == -1) return 1;
	else return 0;
}

// 스택 포화 상태 검사 함수
int is_full_opt(StackType* s) {
	if (s->top >= (MAX_STACK_SIZE - 1)) return 1;
	else return 0;
}
int is_full_opd(OperandStack* s) {
	if (s->top >= (MAX_STACK_SIZE - 1)) return 1;
	else return 0;
}

// 스택 요소 삽입 함수
void push_opt(StackType* s, operator newdata) {
	if (is_full_opt(s)) { printf("스택 공간이 부족합니다."); exit(1); }
	else s->data[++(s->top)] = newdata;
}
void push_opd(OperandStack* s, operand newdata) {
	if (is_full_opd(s)) { printf("스택 공간이 부족합니다."); exit(1); }
	else s->data[++(s->top)] = newdata;
}

// 스택 마지막 요소 반환 및 제거 함수
operator pop_opt(StackType* s) {
	if (is_empty_opt(s)) { printf("스택이 공백상태입니다."); exit(1); }
	else return s->data[(s->top)--];
}
operand pop_opd(OperandStack* s) {
	if (is_empty_opd(s)) { printf("스택이 공백상태입니다."); exit(1); }
	else return s->data[(s->top)--];
}

// 스택 마지막 요소 확인 함수
operator peek_opt(StackType* s) {
	if (is_empty_opt(s)) { printf("스택이 공백상태입니다."); exit(1); }
	else return s->data[(s->top)];
}
operand peek_opd(OperandStack* s) {
	if (is_empty_opd(s)) { printf("스택이 공백상태입니다."); exit(1); }
	else return s->data[(s->top)];
}

// 연산자의 우선 순위 반환 함수
int prec(char op) {
	switch (op) {
	case '(': case ')': case '{': case '}': case '[': case ']':  return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}

#ifdef INFIX_TO_POSTFIX//==================================================
// 중위 표기 수식 ---> 후위 표기 수식 함수
void infix_to_postfix(char* infix, char* postfix) {
	int idx_infix = 0; // 중위 표기 수식 문자열의 인덱스
	int idx_postfix = 0; // 후위 표기 수식 문자열의 인덱스
	char ch, top_op;
	int len_infix = strlen(infix); // 널문자를 제외한 중위 표기 수식의 길이
	StackType s_opt;  // 수식기호 스택 생성
	init_stack_opt(&s_opt); // 수식기호 스택 초기화

	while (idx_infix < len_infix) { // 수식 길이를 안넘어가는 동안 반복
		ch = infix[idx_infix++]; // 현재 문자 변수, 변수에 값 할당 이후 인덱스 이동
		// 현재 문자가 숫자라면(피연산자 또는 그 일부)
		if (ch >= '0' && ch <= '9') {
			postfix[idx_postfix++] = ch; // postfix문자열에 현재 숫자 추가, 이후 인덱스 이동
			while (idx_infix < len_infix && infix[idx_infix] >= '0' && infix[idx_infix] <= '9') { // 수식 길이 안에서 인덱스를 1씩 이동할 때, 현재 문자가 숫자이면 계속 반복
				postfix[idx_postfix++] = infix[idx_infix++]; // postfix문자열에 숫자 추가, 이후 인덱스 이동
			}
			postfix[idx_postfix++] = ' '; // 마지막에 피연산자들의 구분자로 공백을 추가
		}
		// 현재 문자가 숫자가 아니라면(수식기호라면)
		else {
			switch (ch) {
			// 연산자인 경우
			case '+': case '-': case '*': case '/':
				while (!is_empty_opt(&s_opt) && (prec(ch) <= prec(peek_opt(&s_opt)))) { // 수식기호 스택의 연산자가 현재 연산자보다 우선순위가 높으면 계속 반복
					postfix[idx_postfix++] = pop_opt(&s_opt); // postfix에 추가하고 수식기호 스택에서는 삭제, 이후 인덱스 이동
					postfix[idx_postfix++] = ' '; // 공백 추가, 이후 인덱스 이동
				}
				push_opt(&s_opt, ch); // 현재 연산자를 수식기호 스택에 삽입
				break;
			// 열린 괄호인 경우
			case '(': case '{': case '[':
				push_opt(&s_opt, ch); // 수식기호 스택에 삽입
				break;
			// 닫힌 괄호인 경우
			case ')': case '}': case ']':
				top_op = pop_opt(&s_opt); // 닫힌 괄호 할당 후 수식기호 스택에서 삭제
				while (top_op != '(' && top_op != '{' && top_op != '[') { // 열린 괄호가 나오기 전까지만 실행
					postfix[idx_postfix++] = top_op; // 해당 수식기호를 postfix에 추가, 이후 인덱스 이동
					postfix[idx_postfix++] = ' '; // 공백 추가, 이후 인덱스 이동
					top_op = pop_opt(&s_opt); // 수식기호 스택에서 맨 위의 수식기호를 꺼내오고, 스택에서는 삭제
				}
				break;
			}
		}
	}
	while (!is_empty_opt(&s_opt)) { // 수식기호 스택에 남아있는 모든 연산자 추가
		postfix[idx_postfix++] = pop_opt(&s_opt);
		postfix[idx_postfix++] = ' '; // 공백 추가
	}
	postfix[idx_postfix] = '\0'; // postfix문자열의 가장 마지막에 널문자 추가
	return;
}
#endif//==========================================================

#ifdef EVAL_POSTFIX//=============================================
// 후위 표기 수식 계산 함수
int eval_postfix(char* postfix) {
	int op1, op2, value;
	int len_postfix = strlen(postfix); // 널문자를 제외한 후위 표기 수식의 길이
	char* token = strtok(postfix, " "); // 후위표기수식을 공백 기준으로 토큰 분리
	OperandStack s_opd; // 피연산자 스택 생성
	init_stack_opd(&s_opd); // 피연산자 스택 초기화

	// 토큰이 NULL포인터가 아니라면 계속 반복
	while (token != NULL) {
		// 토큰의 역참조값이 피연산자인 경우
		if (*token != '+' && *token != '-' && *token != '*' && *token != '/') {
			value = atoi(token); // 문자열인 피연산자를 정수로 변환
			push_opd(&s_opd, value); // 피연산자 스택에 정수형인 피연산자 삽입
		}
		// 토큰의 역참조값이 연산자인 경우
		else {
			// 피연산자 스택에서 가장 최근에 삽입된 두개의 피연산자를 꺼내오고 스택에서 삭제
			op2 = pop_opd(&s_opd);
			op1 = pop_opd(&s_opd);
			// 토큰의 역참조값에 해당하는 연산을 수행하고, 그 결과값을 피연산자 스택에 삽입
			switch (*token) {
			case '+': push_opd(&s_opd, op1 + op2); break;
			case '-': push_opd(&s_opd, op1 - op2); break;
			case '*': push_opd(&s_opd, op1 * op2); break;
			case '/': push_opd(&s_opd, op1 / op2); break;
			}
		}
		token = strtok(NULL, " "); // 다음 토큰으로 업데이트
	}
	return pop_opd(&s_opd); // 피연산자 스택에 남아있는 마지막 요소(최종 계산 결과값)를 반환
}
#endif//=======================================================


/*메인 함수*/
int main(void) {
	char infix[MAX_STACK_SIZE];
	printf("입력:"); // 중위 표기 수식 입력받기
	scanf("%s", infix);
	char postfix[MAX_STACK_SIZE] = { NULL }; // postfix문자열 선언및 초기화
	infix_to_postfix(infix, postfix); // 표기 변환 함수 호출
	printf("후위표기수식--> %s\n", postfix);

	int result;
	result = eval_postfix(postfix); // 후위 표기 수식 계산 함수 호출
	printf("결과: %d", result);

	return 0;
}
