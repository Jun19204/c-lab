#include <stdio.h>
#include <math.h>
#define ROW 4
#define COL 4

typedef struct {
    float x, y, z, w;
} Vector;

typedef union {
    float m[COL][ROW];
    struct {
        float m00, m10, m20, m30;
        float m01, m11, m21, m31;
        float m02, m12, m22, m32;
        float m03, m13, m23, m33;
    };
} Matrix4x4;

typedef struct {
    float w; // 실수부 (cos(theta/2))
    float x, y, z; // 허수부 (sin(theta/2) * 축)
} Quaternion;


// 항등 행렬
Matrix4x4 identity() 
{
    Matrix4x4 I = {
        .m00=1.0f, .m01=0.0f, .m02=0.0f, .m03=0.0f,
        .m10=0.0f, .m11=1.0f, .m12=0.0f, .m13=0.0f,
        .m20=0.0f, .m21=0.0f, .m22=1.0f, .m23=0.0f,
        .m30=0.0f, .m31=0.0f, .m32=0.0f, .m33=1.0f
    };
    return I;
}


// 행렬 출력 함수
void print_matrix(const Matrix4x4 A)
{
    printf(" [ %6.2f %6.2f %6.2f %6.2f ]\n",A.m00,A.m01,A.m02,A.m03);
    printf(" [ %6.2f %6.2f %6.2f %6.2f ]\n",A.m10,A.m11,A.m12,A.m13);
    printf(" [ %6.2f %6.2f %6.2f %6.2f ]\n",A.m20,A.m21,A.m22,A.m23);
    printf(" [ %6.2f %6.2f %6.2f %6.2f ]\n",A.m30,A.m31,A.m32,A.m33);
}


// 이동 변환 행렬
Matrix4x4 T_translate(float x, float y, float z)
{
    Matrix4x4 A = identity();
    A.m03 = x;
    A.m13 = y;
    A.m23 = z;
    return A;
}


// 크기(Scale) 변환 행렬
Matrix4x4 T_scale(float (scala))
{
    Matrix4x4 A = identity();
    A.m00 *= (scala);
    A.m11 *= (scala);
    A.m22 *= (scala);
    return A;
}


// 1. 임의의 축과 각도로부터 사원수 생성
Quaternion quat_from_axis_angle(float axis_x, float axis_y, float axis_z, float degree)
{
    Quaternion q;
    float radian = degree * (M_PI / 180.0f);
    float half_angle = radian * 0.5f;
    float sin_half = sinf(half_angle);

    // 축 벡터 정규화 (크기를 1로 만듦)
    float length = sqrtf(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
    if (length > 0.00001f) {
        axis_x /= length;
        axis_y /= length;
        axis_z /= length;
    }

    q.w = cosf(half_angle);
    q.x = axis_x * sin_half;
    q.y = axis_y * sin_half;
    q.z = axis_z * sin_half;
    return q;
}


// 2. 사원수끼리의 곱셈 (회전 누적)
Quaternion quat_multiply(Quaternion a, Quaternion b)
{
    Quaternion q;
    q.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    q.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    q.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    q.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    return q;
}


// 3. 사원수를 4x4 회전 행렬로 변환 (렌더링 파이프라인 연동용)
Matrix4x4 quat_to_matrix(Quaternion q)
{
    // 정규화 (오차 방지)
    float magnitude = sqrtf(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
    if (magnitude > 0.0f) {
        q.w /= magnitude; q.x /= magnitude; q.y /= magnitude; q.z /= magnitude;
    }

    Matrix4x4 M = identity(); // 항등행렬로 초기화 후 회전성분 대입

    float xx = q.x * q.x; float yy = q.y * q.y; float zz = q.z * q.z;
    float xy = q.x * q.y; float xz = q.x * q.z; float yz = q.y * q.z;
    float wx = q.w * q.x; float wy = q.w * q.y; float wz = q.w * q.z;

    // 행렬 성분 채우기 (열 우선/행 우선 구조에 맞게 매핑)
    M.m00 = 1.0f - 2.0f * (yy + zz); M.m01 = 2.0f * (xy - wz);        M.m02 = 2.0f * (xz + wy);
    M.m10 = 2.0f * (xy + wz);        M.m11 = 1.0f - 2.0f * (xx + zz); M.m12 = 2.0f * (yz - wx);
    M.m20 = 2.0f * (xz - wy);        M.m21 = 2.0f * (yz + wx);        M.m22 = 1.0f - 2.0f * (xx + yy);

    return M;
}


// 원근 투영 전처리 행렬
Matrix4x4 T_preprocess_projection()
{
    Matrix4x4 A = identity();
    A.m32 = -1.0f;
    A.m33 = 0.0f;
    return A;
}


// 변환 행렬 합성
Matrix4x4 T_combine(Matrix4x4 A, Matrix4x4 B)
{
    Matrix4x4 T = {0};
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            T.m[col][row] = A.m[0][row] * B.m[col][0] +
                            A.m[1][row] * B.m[col][1] +
                            A.m[2][row] * B.m[col][2] +
                            A.m[3][row] * B.m[col][3];
        }
    }
    return T;
}


// 아핀 변환 함수
Vector affine_transformation(Matrix4x4 T, Vector v)
{
    Vector vt;
    vt.x = T.m00*v.x + T.m01*v.y + T.m02*v.z + T.m03*v.w;
    vt.y = T.m10*v.x + T.m11*v.y + T.m12*v.z + T.m13*v.w;
    vt.z = T.m20*v.x + T.m21*v.y + T.m22*v.z + T.m23*v.w;
    vt.w = T.m30*v.x + T.m31*v.y + T.m32*v.z + T.m33*v.w;
    return vt;
}


// 아핀 변환 + 원근 투영 변환 함수
Vector projection_transformation(Matrix4x4 T, Vector v)
{
    Vector vt = affine_transformation(T, v);
    if (vt.w != 0.0f && vt.w != 1.0f) {
        vt.x /= vt.w;
        vt.y /= vt.w;
        vt.z /= vt.w;
        vt.w = 1.0f;
    }
    return vt;
}


// 행렬식 계산 함수
static inline float det_4x4(Matrix4x4 T) 
{
    // 아래쪽 2x2 소행렬식 계산
    float b1 = T.m20*T.m31 - T.m21*T.m30;
    float b2 = T.m20*T.m32 - T.m22*T.m30;
    float b3 = T.m20*T.m33 - T.m23*T.m30;
    float b4 = T.m21*T.m32 - T.m22*T.m31;
    float b5 = T.m21*T.m33 - T.m23*T.m31;
    float b6 = T.m22*T.m33 - T.m23*T.m32;

    // 위쪽 2x2 소행렬식 계산
    float t6 = T.m02*T.m13 - T.m03*T.m12;
    float t5 = T.m01*T.m13 - T.m03*T.m11;
    float t4 = T.m01*T.m12 - T.m02*T.m11;
    float t3 = T.m00*T.m13 - T.m03*T.m10;
    float t2 = T.m00*T.m12 - T.m02*T.m10;
    float t1 = T.m00*T.m11 - T.m01*T.m10;

    // 최종 라플라스 전개
    return (b1*t6 - b2*t5 + b3*t4 + b4*t3 - b5*t2 + b6*t1);
}


// main함수
int main(void)
{
    float value;
    char componants[4] = {'x', 'y', 'z', 'w'};
    Vector v = {0};

    printf("\n *************************************************");
    printf("\n **                                             **");
    printf("\n **            MVP 변환 파이프라인              **");
    printf("\n **                                             **");
    printf("\n *************************************************\n");

    printf("\n x:가로, y:세로(높이), z:깊이(앞뒤), w:(점or벡터)\n");
    printf("            [ 변환시킬 점or벡터 입력 ]\n");
    int i = 0;
    do {
        printf("  -> %c: ", componants[i]);
        scanf(" %f", &value);
        if (i == 0) v.x = value;
        else if (i == 1) v.y = value;
        else if (i == 2) v.z = value;
        else if (i == 3 && (value == 0 || value == 1)) v.w = value;
        else {
            printf(" 유효하지 않은 입력입니다.\n"); 
            continue;
        }
        i++;
    } while (i < 4);
    printf(" v = (%.2f, %.2f, %.2f, %.2f)\n", v.x, v.y, v.z, v.w);
   
    int choice;
    char ch;
    Matrix4x4 A = identity();
    Quaternion q_total_local = {1.0f, 0.0f, 0.0f, 0.0f};
    Quaternion q_total_world = {1.0f, 0.0f, 0.0f, 0.0f};
    Matrix4x4 T_local = identity();
    Matrix4x4 T_world = identity();
    float x, y, z, degree, scala;
    short run = 1;
    int use_projection = 0;
    float z_camera = 0;

    printf("\n                        [ 변환 선택 ]");
    while (run) {
    printf("\n (1)이동변환 (2)크기변환 (3)회전변환 (4)변환행렬출력 (5)변환적용: ");
    scanf(" %d", &choice);
        switch(choice) {
            case 1: {
                printf(" x += "); scanf(" %f", &x);
                printf(" y += "); scanf(" %f", &y);
                printf(" z += "); scanf(" %f", &z);
                A = T_translate(x, y, z);
                T_local = T_combine(T_local, A); // 우곱
                T_world = T_combine(A, T_world); // 좌곱
                break;
            }
            case 2: {
                printf(" scala: "); scanf(" %f", &scala);
                A = T_scale(scala);
                T_local = T_combine(T_local, A);
                T_world = T_combine(A, T_world);
                printf(" 크기 %.2f배 적용완료..\n", scala);
                break;
            }
            case 3: {
                do {
                    printf("  -> 회전축(x/y/z): ");
                    scanf(" %c", &ch);
                    if (ch != 'z' && ch != 'y' && ch != 'x') {
                        printf(" 유효하지 않은 입력입니다.\n");
                    }
                } while (ch != 'z' && ch != 'y' && ch != 'x');
                printf(" degree: "); scanf(" %f", &degree);
                Quaternion q_new;
                if (ch == 'x') q_new = quat_from_axis_angle(1.0f, 0.0f, 0.0f, degree);
                else if (ch == 'y') q_new = quat_from_axis_angle(0.0f, 1.0f, 0.0f, degree);
                else q_new = quat_from_axis_angle(0.0f, 0.0f, 1.0f, degree);
                q_total_local = quat_multiply(q_total_local, q_new);
                q_total_world = quat_multiply(q_new, q_total_world);
                break;
            }
            case 4: {
                Matrix4x4 R_local = quat_to_matrix(q_total_local);
                Matrix4x4 T_to_print_local = T_combine(T_local, R_local);
                Matrix4x4 R_world = quat_to_matrix(q_total_world);
                Matrix4x4 T_to_print_world = T_combine(R_world, T_world);
                printf(" -> 로컬 변환 행렬\n");
                print_matrix(T_to_print_local);
                printf("\n -> 월드 변환 행렬\n");
                print_matrix(T_to_print_world);
                break;
            }
            case 5:
            default: {
                run = 0;
                printf("  -> 뷰 행렬(V) & 원근 투영 행렬(P) 적용?(Y/n): ");
                fflush(stdout);
                scanf(" %c", &ch);
                if (ch == 'y' || ch == 'Y') { 
                    use_projection = 1;
                    printf("  -> 카메라의 z좌표: ");
                    scanf(" %f", &z_camera);
                }
                break;
            }
        }
    }

    Matrix4x4 R_final_local = quat_to_matrix(q_total_local);
    Matrix4x4 R_final_world = quat_to_matrix(q_total_world);
    Matrix4x4 MVP_local = T_combine(T_local, R_final_local); // M 적용
    Matrix4x4 MVP_world = T_combine(R_final_world, T_world);

    if (use_projection) {
        Matrix4x4 V = T_translate(0.0f, 0.0f, -z_camera);
        Matrix4x4 P = T_preprocess_projection();

        MVP_local = T_combine(V, MVP_local); // V x M
        MVP_local = T_combine(P, MVP_local); // pre_P x V X M

        MVP_world = T_combine(V, MVP_world);
        MVP_world = T_combine(P, MVP_world);
    }

    Vector vt_local;
    Vector vt_world;

    if (use_projection) {
        vt_local = projection_transformation(MVP_local, v); // P x V x M x v
        vt_world = projection_transformation(MVP_world, v);
        printf("\n\n     [ 카메라 뷰 & 화면 원근 투영(MVP) 결과 ]\n");
    }
    else {
        vt_local = affine_transformation(MVP_local, v); // M x v
        vt_world = affine_transformation(MVP_world, v); // M x v
        printf("\n\n            [ 아핀 변환(M) 결과 ]\n");
    }

    printf(" ==================================================\n");
    printf(" 입력 원본 좌표:\n");
    printf("    v = (%.2f, %.2f, %.2f, %.2f)\n", v.x, v.y, v.z, v.w);
    printf(" --------------------------------------------------\n");
    printf(" 로컬 좌표계 기준 변환:\n");
    printf("    v' = (%.2f, %.2f, %.2f, %.2f)\n", \
                                    vt_local.x, vt_local.y, vt_local.z, vt_local.w);
    printf("    Matrix Det = %.2f\n", det_4x4(MVP_local));
    printf(" --------------------------------------------------\n");
    printf(" 월드 좌표계 기준 변환:\n");
    printf("    v' = (%.2f, %.2f, %.2f, %.2f)\n", \
                                    vt_world.x, vt_world.y, vt_world.z, vt_world.w);
    printf("    Matrix Det = %.2f\n", det_4x4(MVP_world));
    printf(" ==================================================\n\n");
    
    return 0;
}

