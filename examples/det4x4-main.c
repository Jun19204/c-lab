#include <stdio.h>
#define N 4

typedef union {
    float m[16];
    struct {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
} Matrix4x4;


// 행렬식 계산 함수
static inline float Det_4x4(const Matrix4x4 *mat) 
{
    // 2x2 소행렬을 행렬 아래쪽(2행, 3행) 성분들로 미리 계산
    float s0 = mat->m20 * mat->m31 - mat->m21 * mat->m30;
    float s1 = mat->m20 * mat->m32 - mat->m22 * mat->m30;
    float s2 = mat->m20 * mat->m33 - mat->m23 * mat->m30;
    float s3 = mat->m21 * mat->m32 - mat->m22 * mat->m31;
    float s4 = mat->m21 * mat->m33 - mat->m23 * mat->m31;
    float s5 = mat->m22 * mat->m33 - mat->m23 * mat->m32;

    // 위쪽(0행, 1행) 성분들로 대응되는 소행렬식을 계산
    float c5 = mat->m02 * mat->m13 - mat->m03 * mat->m12;
    float c4 = mat->m01 * mat->m13 - mat->m03 * mat->m11;
    float c3 = mat->m01 * mat->m12 - mat->m02 * mat->m11;
    float c2 = mat->m00 * mat->m13 - mat->m03 * mat->m10;
    float c1 = mat->m00 * mat->m12 - mat->m02 * mat->m10;
    float c0 = mat->m00 * mat->m11 - mat->m01 * mat->m10;

    // 최종 라플라스 전개 공식 결합
    return (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
}


int main() 
{
    Matrix4x4 my_matrix;

    int i = 0, j = 0;

    printf("\n ************************************************");
    printf("\n ***           행렬식 계산 프로그램           ***");
    printf("\n ************************************************\n");

    printf("\n 행렬의 값 입력: \n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf(" %d행%d열: ", i+1, j+1); fflush(stdout);
            scanf(" %f", &my_matrix.m[i*N+j]);
        }
    }

    printf("\n Matrix A = \n");
    for (i = 0; i < N; i++) {
        printf("\t|  ");
        for (j = 0; j < N; j++) {
            printf("%.2f  ", my_matrix.m[i*N+j]);
        }
        printf("|\n");
    }
 
    float det = Det_4x4(&my_matrix);
    printf("\nDet(A) = %.2f\n\n", det);

    return 0;
}

