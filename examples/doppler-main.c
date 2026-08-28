// 도플러 효과 랜더링 따라해보기
#include <raylib.h>
#include <string.h>

#define WIDTH 900
#define HEIGHT 600
#define MAX_WAVES 50
#define WAVE_SPEED 100 
#define MOVING_SPEED 1.0f
#define WAVE_EMISSION_FREQUENCY 6

int current_waves = 0;

struct MovingObject {
    float x,y;
};

struct SoundWave {
    float x,y,r;
};

struct MovingObject one;
struct SoundWave waves[MAX_WAVES];


void draw_car()
{
    DrawCircle(one.x, one.y, 20, WHITE);
}

void emit_new_wave()
{
    struct SoundWave copy[MAX_WAVES];
    memset(copy, 0, MAX_WAVES*sizeof(struct SoundWave));
    for (int i = 0; i < MAX_WAVES; i++){
        copy[i] = waves[i];
    }
    for (int i = 0; i < MAX_WAVES; i++){
        if (i < MAX_WAVES - 1)
            waves[i+1] = copy[i];
    }

    waves[0] = (struct SoundWave) {one.x, one.y, 0};
    if (current_waves < MAX_WAVES) current_waves++;
}

void draw_waves()
{
    for (int i = 0; i < current_waves; i++)
    {
        DrawCircleLines(waves[i].x, waves[i].y, waves[i].r, WHITE);
    }
}

void propagate_waves(float dt)
{
    for (int i = 0; i < current_waves; i++)
    {
        waves[i].r += WAVE_SPEED * dt;
    }
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Doppler Simulator");

    one = (struct MovingObject){(float)WIDTH/2, (float)HEIGHT/2};

    SetTargetFPS(60);
    float interval = 0;
    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();

        interval += dt;
        if (interval > 1.0f / WAVE_EMISSION_FREQUENCY)
        {
            emit_new_wave();
            interval = 0.0f;
        }
        propagate_waves(dt);
        
        // 독립적인 속도로 만들기
        if (IsKeyDown(KEY_RIGHT)) one.x += MOVING_SPEED;
        if (IsKeyDown(KEY_LEFT)) one.x -= MOVING_SPEED;
        if (IsKeyDown(KEY_UP)) one.y -= MOVING_SPEED;
        if (IsKeyDown(KEY_DOWN)) one.y += MOVING_SPEED;


        BeginDrawing();
                
        ClearBackground(BLACK);
        
        draw_car();
        draw_waves();

        EndDrawing();
    }
    return 0;
}
