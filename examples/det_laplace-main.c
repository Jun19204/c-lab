#include <stdio.h>
#define N 4

double det(double (*A)[N], int n);

int main(void)
{
    int i = 0, j = 0;
    int n;
    double A[N][N] = {0,};

    printf("\n ************************************************");
    printf("\n ***           행렬식 계산 프로그램           ***");
    printf("\n ************************************************\n");

    printf("\n 정방 행렬 크기: "); fflush(stdout);
    scanf(" %d", &n);

    printf("\n 행렬의 값 입력: \n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf(" %d행%d열: ", i, j); fflush(stdout);
            scanf(" %lf", &A[i][j]);
        }
    }

    printf("\n Matrix A = \n");
    for (i = 0; i < n; i++) {
        printf("\t|  ");
        for (j = 0; j < n; j++) {
            printf("%.f  ", A[i][j]);
        }
        printf("|\n");
    }
    printf("\n Det(A) = %.f\n\n", det(A,n));
    
    return 0;
}

// 행렬식 계산 함수
double det(double (*A)[N], int n)
{
    int r, c, point, sub_c;
    double det_A = 0;
    double Sub[N][N]; // 소행렬

    // 기저 조건 - 1x1 행렬일 때
    if (n == 1) {
        return A[0][0];
    }

    // 0행을 기준으로 각 열(point)에 대해 라플라스 전개
    int sign = 1;
    for (point = 0; point < n; point++) {

        // 소행렬 만들기 - 원래 행렬의 1행부터 n-1행까지 복사
        for (r = 1; r < n; r++) {
            sub_c = 0; // 소행렬의 열 인덱스 초기화
            for (c = 0; c < n; c++) {
                // 기준이 되는 point열은 제외하고 소행렬에 복사
                if (c != point) {
                    Sub[r-1][sub_c] = A[r][c];
                    sub_c++;
                }
            }
        }
        // det_A += (부호) * (기준 원소) * (소행렬식)
        det_A += sign * A[0][point] * det(Sub, n-1);
        sign *= -1;
    }

    return det_A;
}


