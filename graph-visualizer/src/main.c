#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <csv.h>

#include "raylib.h"
#include "rlgl.h"

#define OFFSET_Y -10
#define OFFSET_X -15
#define STEP 25

struct Obs {
    int wp_num;
    double x;
    double y;
    double c;
};

int arr_len = 0;
int obs_len = 0;
int state = 1;

void cb1(void *s, size_t len, void *data) {
    arr_len++; 
    ((double*)data)[arr_len-1] = *((double*)s);
}

void cb2(void *s, size_t len, void *data) {
    switch (state) {
        case 0:
            ((struct Obs*)data)[obs_len].wp_num = *((int*)s);
            break;
        case 1:
            ((struct Obs*)data)[obs_len].x = *((double*)s);
            break;
        case 2:
            ((struct Obs*)data)[obs_len].y = *((double*)s);
            break;
        default:
            ((struct Obs*)data)[obs_len].c = *((double*)s);
            break;
    }
    state++;
}

void cb3(int c, void *data) {
    obs_len++;
    state = 1;
}

double* parse_file() {
    FILE *fd;
    struct csv_parser p;
    char buf[1024];
    size_t bytes;

    double* data = (double*) malloc(sizeof(double) * 1024);
    if (data == NULL) {
        printf("Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    if (csv_init(&p, CSV_EMPTY_IS_NULL) != 0) {
        printf("Error initializing parser!\n");
        exit(EXIT_FAILURE);
    }

    fd = fopen("wps.csv", "rb+");
    if (fd == NULL) {
        printf("Error in opening file!\n");
        exit(EXIT_FAILURE);
    }

    while ((bytes = fread(buf, 1, 1024, fd)) > 0) {
        if (csv_parse(&p, buf, bytes, cb1, NULL, data) != bytes) {
            printf("Failed to parse data!\n");
            csv_strerror(csv_error(&p));
            exit(EXIT_FAILURE);
        }
    }

    csv_fini(&p, cb1, NULL, data);
    fclose(fd);
    csv_free(&p);

    return data;
}

struct Obs* parse_obs_file() {
    FILE *fd;
    struct csv_parser p;
    char buf[1024];
    size_t bytes;

    struct Obs *data = (struct Obs*) malloc(sizeof(struct Obs) * 1024);
    if (data == NULL) {
        printf("Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    if (csv_init(&p, 0) != 0) {
        printf("Error initializing parser\n");
        exit(EXIT_FAILURE);
    }

    fd = fopen("obs.csv", "rb+");
    if (fd == NULL) {
        printf("Error in opening file!\n");
        exit(EXIT_FAILURE);
    }

    while ((bytes = fread(buf, 1, 1024, fd)) > 0) {
        if (csv_parse(&p, buf, bytes, cb2, cb3, data) != bytes) {
            printf("Failed to parse data!\n");
            csv_strerror(csv_error(&p));
            exit(EXIT_FAILURE);
        }
    }

    csv_fini(&p, cb1, NULL, data);
    fclose(fd);
    csv_free(&p);

    return data;
}

int main(int argc, char *argv[])
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    double* data;
    struct Obs* obs;
    int max = 1;
    Color color, text_color;

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;


    InitWindow(screenWidth, screenHeight, "graph-visualizer");

    SetTargetFPS(60);

    data = parse_file();
    obs = parse_obs_file();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE) && max < arr_len) {
            max++;
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                rlPushMatrix();
                    rlTranslatef(50, 25*50, 0);
                    rlRotatef(90, 1, 0, 0);
                    DrawGrid(100, 25);
                rlPopMatrix();
                for (int i = 0; i < obs_len; i++) {
                    if (obs[i].c == 4) {
                        color = GRAY;
                    }
                    else {
                        color = LIGHTGRAY;
                    }
                    DrawRectangle(GetScreenWidth()/2.0f + OFFSET_X + STEP * obs[i].x, GetScreenHeight() / 2.0f + OFFSET_Y - STEP * obs[i].y, 25, 25, color);
                }
                for (int i = 0; i < max; i += 2) {
                    DrawCircle(GetScreenWidth()/2.0f + OFFSET_X + STEP * data[i], GetScreenHeight() / 2.0f + OFFSET_Y - STEP * data[i+1], 5, RED);
                    if (i > 0) {
                        DrawLine(GetScreenWidth()/2.0f + OFFSET_X + STEP * data[i-2], GetScreenHeight() / 2.0f + OFFSET_Y - STEP * data[i-1],
                             GetScreenWidth()/2.0f + OFFSET_X + STEP * data[i], GetScreenHeight() / 2.0f + OFFSET_Y - STEP * data[i+1], RED);
                    }
                }
                
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}
