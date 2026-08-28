// Real-TIme Fluid Dynamics for Games논문의 소스코드를 사용해서 콘솔창에 물결 띄우기
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 시뮬레이션 그리드 크기 정의
#define WIDTH 80
#define HEIGHT 40
#define IX(i, j) ((i) + (WIDTH + 2) * (j))
#define SWAP(x0, x) {float *tmp=x0;x0=x;x=tmp;}

/* Real-TIme Fluid Dynamics for Games 의 소스코드 */

typedef struct {
    int size;
    float dt, diff, visc;
    float *s, *density;
    float *vx, *vy, *vx0, *vy0;
} FluidCube;

// 경계 조건 설정 함수
void set_bnd(int b, float *x) {
    for (int i = 1; i <= WIDTH; i++) {
        x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, HEIGHT + 1)] = b == 2 ? -x[IX(i, HEIGHT)] : x[IX(i, HEIGHT)];
    }
    for (int j = 1; j <= HEIGHT; j++) {
        x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
        x[IX(WIDTH + 1, j)] = b == 1 ? -x[IX(WIDTH, j)] : x[IX(WIDTH, j)];
    }
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, HEIGHT + 1)] = 0.5f * (x[IX(1, HEIGHT + 1)] + x[IX(0, HEIGHT)]);
    x[IX(WIDTH + 1, 0)] = 0.5f * (x[IX(WIDTH, 0)] + x[IX(WIDTH + 1, 1)]);
    x[IX(WIDTH + 1, HEIGHT + 1)] = 0.5f * (x[IX(WIDTH, HEIGHT + 1)] + x[IX(WIDTH + 1, HEIGHT)]);
}

void lin_solve(int b, float *x, float *x0, float a, float c) {
    for (int k = 0; k < 40; k++) {
        for (int j = 1; j <= HEIGHT; j++) {
            for (int i = 1; i <= WIDTH; i++) {
                x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] + x[IX(i, j - 1)] + x[IX(i, j + 1)])) / c;
            }
        }
        set_bnd(b, x);
    }
}

void diffuse(int b, float *x, float *x0, float diff, float dt) {
    float a = dt * diff * WIDTH * HEIGHT;
    lin_solve(b, x, x0, a, 1 + 4 * a);
}

void project(float *vx, float *vy, float *p, float *div) {
    for (int j = 1; j <= HEIGHT; j++) {
        for (int i = 1; i <= WIDTH; i++) {
            div[IX(i, j)] = -0.5f * (vx[IX(i + 1, j)] - vx[IX(i - 1, j)] + vy[IX(i, j + 1)] - vy[IX(i, j - 1)]) / WIDTH;
            p[IX(i, j)] = 0;
        }
    }
    set_bnd(0, div); set_bnd(0, p);
    lin_solve(0, p, div, 1, 4);

    for (int j = 1; j <= HEIGHT; j++) {
        for (int i = 1; i <= WIDTH; i++) {
            vx[IX(i, j)] -= 0.5f * WIDTH * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
            vy[IX(i, j)] -= 0.5f * WIDTH * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
        }
    }
    set_bnd(1, vx); set_bnd(2, vy);
}

void advect(int b, float *d, float *d0, float *vx, float *vy, float dt) {
    float dt0 = dt * WIDTH;
    for (int j = 1; j <= HEIGHT; j++) {
        for (int i = 1; i <= WIDTH; i++) {
            float x = i - dt0 * vx[IX(i, j)];
            float y = j - dt0 * vy[IX(i, j)];
            if (x < 0.5f) x = 0.5f; if (x > WIDTH + 0.5f) x = WIDTH + 0.5f;
            if (y < 0.5f) y = 0.5f; if (y > HEIGHT + 0.5f) y = HEIGHT + 0.5f;
            int i0 = (int)x, i1 = i0 + 1, j0 = (int)y, j1 = j0 + 1;
            float s1 = x - i0, s0 = 1 - s1, t1 = y - j0, t0 = 1 - t1;
            d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) + s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
        }
    }
    set_bnd(b, d);
}

FluidCube* create_fluid(float dt, float diff, float visc) {
    FluidCube *cube = malloc(sizeof(FluidCube));
    int size = (WIDTH + 2) * (HEIGHT + 2);
    cube->dt = dt; cube->diff = diff; cube->visc = visc;
    cube->s = calloc(size, sizeof(float));
    cube->density = calloc(size, sizeof(float));
    cube->vx = calloc(size, sizeof(float));
    cube->vy = calloc(size, sizeof(float));
    cube->vx0 = calloc(size, sizeof(float));
    cube->vy0 = calloc(size, sizeof(float));
    return cube;
}

void fluid_step(FluidCube *cube) {
    diffuse(1, cube->vx0, cube->vx, cube->visc, cube->dt);
    diffuse(2, cube->vy0, cube->vy, cube->visc, cube->dt);
    project(cube->vx0, cube->vy0, cube->vx, cube->vy);
    advect(1, cube->vx, cube->vx0, cube->vx0, cube->vy0, cube->dt);
    advect(2, cube->vy, cube->vy0, cube->vx0, cube->vy0, cube->dt);
    project(cube->vx, cube->vy, cube->vx0, cube->vy0);
    diffuse(0, cube->s, cube->density, cube->diff, cube->dt);
    advect(0, cube->density, cube->s, cube->vx, cube->vy, cube->dt);
}

// ncurses를 사용하여 콘솔에서 실시간으로 물 시뮬레이션을 렌더링
int main() {
    initscr(); raw(); keypad(stdscr, TRUE); noecho(); curs_set(0); nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    printf("\033[?1003h\n");

    FluidCube *cube = create_fluid(0.1f, 0.0001f, 0.0001f);
    MEVENT event;
    int prev_x = 0, prev_y = 0;

    while (getch() != 'q') {
        if (getmouse(&event) == OK) {
            if (event.bstate & (BUTTON1_PRESSED | BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED | BUTTON1_TRIPLE_CLICKED)) {
                int i = event.x; int j = event.y;
                if (i > 0 && i <= WIDTH && j > 0 && j <= HEIGHT) {
                    cube->density[IX(i, j)] += 100.0f;
                    cube->vx[IX(i, j)] += (i - prev_x) * 5.0f;
                    cube->vy[IX(i, j)] += (j - prev_y) * 5.0f;
                }
            }
            prev_x = event.x; prev_y = event.y;
        }

        fluid_step(cube);

        // 렌더링 파트
        const char *water_chars = " :~=+oa#@";
        for (int j = 1; j <= HEIGHT; j++) {
            for (int i = 1; i <= WIDTH; i++) {
                float d = cube->density[IX(i, j)];

                // 밀도가 너무 낮으면 공백으로 출력해서 물방울이 맺힌 느낌을 줌
                if (d < 0.1f) {
                    mvaddch(j - 1, i - 1, ' ');
                } else {
					// 농도가 높을수록 더 진한 문자로 표현
                    int idx = (int)(d * 10);
                    if (idx > 8) idx = 8;
                    mvaddch(j - 1, i - 1, water_chars[idx]);
                }
            }
        }
        refresh();
        usleep(10000);
    }

    printf("\033[?1003l\n"); endwin();
    return 0;
}
