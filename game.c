#include "raylib.h"
#include <stdlib.h>
#include <time.h>

int main(void) {
    // 화면 크기 설정
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Mini Game - Block Dodger");

    // 플레이어 설정
    Vector2 player = { (float)screenWidth / 2.0f, (float)screenHeight - 60.0f };
    float playerWidth = 60.0f;
    float playerHeight = 20.0f;
    float playerSpeed = 7.0f;

    // 장애물(적 블록) 설정
    #define MAX_OBSTACLES 6
    Vector2 obstacles[MAX_OBSTACLES];
    float obstacleSpeeds[MAX_OBSTACLES];
    float obstacleWidth = 40.0f;
    float obstacleHeight = 40.0f;

    srand((unsigned int)time(NULL));
    for(int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i] = (Vector2){ (float)(rand() % (screenWidth - (int)obstacleWidth)), (float)-(rand() % 400 + 50) };
        obstacleSpeeds[i] = (float)(rand() % 3 + 4);
    }

    int score = 0;
    bool gameOver = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!gameOver) {
            // 키보드 입력 (방향키 또는 A, D 키)
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.x -= playerSpeed;
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.x += playerSpeed;

            // 화면 경계 제한
            if (player.x < 0) player.x = 0;
            if (player.x > screenWidth - playerWidth) player.x > screenWidth - playerWidth ? player.x = screenWidth - playerWidth : 0;

            // 장애물 업데이트
            for(int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles[i].y += obstacleSpeeds[i];
                
                // 화면 아래로 떨어지면 위쪽에서 다시 생성
                if (obstacles[i].y > screenHeight) {
                    obstacles[i].y = (float)-(rand() % 200 + 20);
                    obstacles[i].x = (float)(rand() % (screenWidth - (int)obstacleWidth));
                    obstacleSpeeds[i] = (float)(rand() % 4 + 4);
                    score += 10;
                }

                // 충돌 감지
                Rectangle playerRect = { player.x, player.y, playerWidth, playerHeight };
                Rectangle obsRect = { obstacles[i].x, obstacles[i].y, obstacleWidth, obstacleHeight };
                if (CheckCollisionRecs(playerRect, obsRect)) {
                    gameOver = true;
                }
            }
        } else {
            // 재시작 (R 키)
            if (IsKeyPressed(KEY_R)) {
                score = 0;
                gameOver = false;
                for(int i = 0; i < MAX_OBSTACLES; i++) {
                    obstacles[i] = (Vector2){ (float)(rand() % (screenWidth - (int)obstacleWidth)), (float)-(rand() % 400 + 50) };
                }
            }
        }

        // 렌더링 시작
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!gameOver) {
            // 플레이어 그리기 (다크블루)
            DrawRectangleRec((Rectangle){ player.x, player.y, playerWidth, playerHeight }, DARKBLUE);

            // 장애물 그리기 (레드)
            for(int i = 0; i < MAX_OBSTACLES; i++) {
                DrawRectangleRec((Rectangle){ obstacles[i].x, obstacles[i].y, obstacleWidth, obstacleHeight }, RED);
            }

            // 점수 표시
            DrawText(TextFormat("Score: %d", score), 20, 20, 25, DARKGRAY);
            DrawText("Move: Left/Right Arrow or A/D", 20, 50, 18, LIGHTGRAY);
        } else {
            // 게임 오버 화면
            DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 60, 40, RED);
            DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - MeasureText(TextFormat("Final Score: %d", score), 20) / 2, screenHeight / 2 - 10, 20, DARKGRAY);
            DrawText("Press 'R' to Restart", screenWidth / 2 - MeasureText("Press 'R' to Restart", 20) / 2, screenHeight / 2 + 30, 20, DARKBLUE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
