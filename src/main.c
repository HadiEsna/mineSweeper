#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <time.h>

FILE *s;
bool win, play, lost, can_I_save, saved;
int value[3], scr_x, scr_y, saved_time;
double start_time, end_time;
SDL_Window *window;
SDL_Renderer *renderer;
int box_w = 40;
const int FPS = 20;
struct bl {
    int st;
    int bomb_num;
};

void new_game() {
    int choose = 0;
    bool f;
    while (1) {
        int start_ticks = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        stringRGBA(renderer, 10, 20, "choose difficulty", 255, 255, 255, 255);
        stringRGBA(renderer, 10, 60, "easy (9*9 & 10 bombs!)", 120 + ((choose % 4) != 0) * 125, 255, 120 + ((choose % 4) != 0) * 125, 255);
        stringRGBA(renderer, 10, 80, "medium (16*16 & 40 bombs!)", 120 + (((choose) % 4) != 1) * 125, 255, 120 + ((choose % 4) != 1) * 125, 255);
        stringRGBA(renderer, 10, 100, "hard (30*30 & 99 bombs!)", 120 + (((choose) % 4) != 2) * 125, 255, 120 + (((choose) % 4) != 2) * 125, 255);
        stringRGBA(renderer, 10, 120, "custom", 120 + ((choose % 4) != 3) * 125, 255, 120 + (((choose) % 4) != 3) * 125, 255);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    if (!f)
                        choose++;
                    f = 1;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    if (!f)
                        choose--;
                    if (choose <= 0)
                        choose += 4;
                    f = 1;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (choose == 3) {
                        double times;
                        int x = 0, f1 = 0;
                        value[2] = 20;
                        value[1] = value[0] = 20;
                        bool wh = 1;
                        while (wh) {
                            start_ticks = SDL_GetTicks();
                            times = (sin(start_ticks)) * (sin(start_ticks));
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                            SDL_RenderClear(renderer);
                            stringRGBA(renderer, 10, 20, "custom:", 255, 255, 255, 255);
                            stringRGBA(renderer, 10, 90, "bombs:", (x != 2) * 255 + (x == 2) * (122), 255, (x != 2) * 255 + (x == 2) * (times * 132 + 122), (x != 2) * 255 + (x == 2) * times * 255);
                            char c[5];
                            sprintf(c, "%d", value[2]);
                            stringRGBA(renderer, 200, 90, c, 255, 255, 255, 255);

                            stringRGBA(renderer, 10, 70, "blocks in row:", (x != 1) * 255 + (x == 1) * (120), 255, (x != 1) * 255 + (x == 1) * (times * 130 + 120), (x != 1) * 255 + (x == 1) * times * 255);
                            sprintf(c, "%d", value[1]);
                            stringRGBA(renderer, 200, 70, c, 255, 255, 255, 255);

                            stringRGBA(renderer, 10, 50, "blocks in colomn:", (x != 0) * 255 + (x == 0) * (120), 255, (x != 0) * 255 + (x == 0) * (times * 130 + 120), (x != 0) * 255 + (x == 0) * times * 255);
                            sprintf(c, "%d", value[0]);
                            stringRGBA(renderer, 200, 50, c, 255, 255, 255, 255);
                            SDL_Event event;
                            while (SDL_PollEvent(&event)) {
                                if (event.type == SDL_QUIT)
                                    return;
                                if (event.type == SDL_KEYDOWN) {
                                    if (event.key.keysym.sym == SDLK_UP) {
                                        if (!f1)
                                            value[x]++;
                                        f1 = 1;
                                    }
                                    if (event.key.keysym.sym == SDLK_DOWN) {
                                        if (!f1)
                                            value[x]--;
                                        f1 = 1;
                                    }
                                    if (event.key.keysym.sym == SDLK_RIGHT) {
                                        if (!f1)
                                            value[x] += 10;
                                        f1 = 1;
                                    }
                                    if (event.key.keysym.sym == SDLK_LEFT) {
                                        if (!f1)
                                            value[x] -= 10;
                                        f1 = 1;
                                    }
                                    if (value[x] < 4)
                                        value[x] = 4;
                                    if (value[2] > value[1] * value[0] - 9) {
                                        value[2] = value[1] * value[0] - 9;
                                    }

                                    if (event.key.keysym.sym == SDLK_RETURN) {
                                        x++;
                                        if (x == 3) {
                                            wh = 0;
                                        }
                                    }
                                } else if (event.type == SDL_KEYUP) {
                                    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT)
                                        f1 = 0;
                                }
                            }
                            SDL_RenderPresent(renderer);
                            while (SDL_GetTicks() - start_ticks < 1000 / FPS);
                        }
                    } else if (choose == 0) {
                        value[2] = 10;
                        value[0] = value[1] = 9;
                    } else if (choose == 1) {
                        value[2] = 40;
                        value[0] = value[1] = 16;
                    } else if (choose == 2) {
                        value[2] = 99;
                        value[0] = value[1] = 30;
                    }
                    scr_y = value[0];
                    scr_x = value[1];
                    return;
                }
            } else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
                    f = 0;
            }
        }
        SDL_RenderPresent(renderer);
        while (SDL_GetTicks() - start_ticks < 1000 / FPS);
    }
}

int load_game_init_value() {
    s = fopen("../save.s", "r");
    fscanf(s, "%d%d%d", &value[0], &value[1], &saved_time);
    if (value[0] == 0) {
        return 0;
    }
    for (int i = 0; i < value[0]; i++) {
        for (int j = 0; j < value[1]; j++) {
            int a;
            fscanf(s, "%d", &a);
            if (a == -1)
                value[2]++;
        }
    }
    scr_x = value[1];
    scr_y = value[0];
    fclose(s);
    return 1;
}

void draw(struct bl blocks[scr_y][scr_x], int bombs, int x, int y) {

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < scr_x; ++i) {
        for (int j = 0; j < scr_y; ++j) {

            thickLineRGBA(renderer, i * box_w + 2, j * box_w + (box_w / 2), (i + 1) * box_w - 2, j * box_w + (box_w / 2), box_w - 4, ((blocks[j][i].bomb_num == -1) && (blocks[j][i].st == 1)) * 135 + (x == i && y == j) * 120 + 100, (x == i && y == j) * 120 + 100, 100, (blocks[j][i].st == 0) * 255 + (blocks[j][i].st == 1) * 100 + (blocks[j][i].st == 2) * 180);
            char a[10] = {0};
            if (blocks[j][i].st == 1) {
                if (blocks[j][i].bomb_num > 0)
                    sprintf(a, "%d", blocks[j][i].bomb_num);
                else if (blocks[j][i].bomb_num == -1)
                    sprintf(a, "*");
            }
            if (blocks[j][i].st == 2)
                sprintf(a, "F");
            if (a[0] != 0)
                stringRGBA(renderer, i * box_w + (box_w / 2) - 5, j * box_w + (box_w / 2), a, 255, 255, 255, 255);
        }
    }
    char buf[50] = {0};

    if (win) {
        stringRGBA(renderer, 20, box_w * scr_y + 10, "you won", 255, 255, 255, 255);
        stringRGBA(renderer, 20, box_w * scr_y + 25, "press any key to exit", 255, 255, 255, 255);

    } else if (lost) {
        stringRGBA(renderer, 20, box_w * scr_y + 10, "you lost", 255, 255, 255, 255);
        stringRGBA(renderer, 20, box_w * scr_y + 25, "press any key to exit", 255, 255, 255, 255);
        thickLineRGBA(renderer, x * box_w + 2, y * box_w + (box_w / 2), (x + 1) * box_w - 2, y * box_w + (box_w / 2), box_w - 4, 255, 100, 100, (sin(SDL_GetTicks())) * (sin(SDL_GetTicks())) * 255);

    } else {
        sprintf(buf, "bombs: %d", bombs);
        stringRGBA(renderer, 20, box_w * scr_y + 20, buf, 255, 255, 255, 255);
    }
    if (can_I_save) {
        stringRGBA(renderer, 200, box_w * scr_y + 30, "save game: press 's'", 255, 255, 255, 100);
    } else if (!win && !lost && play) {
        stringRGBA(renderer, 200, box_w * scr_y + 30, "your game is seccessfully saved", 255, 255, 255, 255);
    }
    sprintf(buf, "time: %d", saved_time + (int) (end_time - start_time) / 1000);
    stringRGBA(renderer, 200, box_w * scr_y + 10, buf, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
}


int init() {
    int choose = 0;
    bool f;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("mines", 10, 10, 300, 300, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    while (1) {
        int start_ticks = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        stringRGBA(renderer, 10, 60, "new game", 120 + (choose % 2) * 135, 255, 120 + (choose % 2) * 135, 255);
        stringRGBA(renderer, 10, 20, "mine sweeper!", 255, 255, 255, 255);
        stringRGBA(renderer, 10, 80, "load game", 120 + ((choose + 1) % 2) * 135, 255, 120 + ((choose + 1) % 2) * 135, 255);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return 1234;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
                    if (!f)
                        choose++;
                    f = 1;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    return choose;
                }
            } else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
                    f = 0;
            }
        }
        SDL_RenderPresent(renderer);
        while (SDL_GetTicks() - start_ticks < 1000 / FPS);
    }

}

int f1;

void save(struct bl blocks[scr_y][scr_x]) {
    s = fopen("../save.s", "w");
    fprintf(s, "%d %d %d ", scr_y, scr_x, (int) (end_time - start_time) / 1000);
    for (int i = 0; i < scr_y; i++) {
        for (int j = 0; j < scr_x; j++) {
            fprintf(s, "%d ", blocks[i][j].bomb_num);
        }
    }
    for (int i = 0; i < scr_y; i++) {
        for (int j = 0; j < scr_x; j++) {
            fprintf(s, "%d ", blocks[i][j].st);
        }
    }
}

int handleEvent(int *y, int *x) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return 12345;
        if (event.type == SDL_KEYDOWN) {
            if (win || lost) {
                play = 0;
                win = 0;
                lost = 0;
                return 12345;
            }
            if (event.key.keysym.sym == SDLK_UP) {
                if (!f1)
                    (*y)--;
                f1 = 1;
            }
            if (event.key.keysym.sym == SDLK_DOWN) {
                if (!f1)
                    (*y)++;
                f1 = 1;
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                if (!f1)
                    (*x)++;
                f1 = 1;
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                if (!f1)
                    (*x)--;
                f1 = 1;
            }
            if (*x < 0)
                (*x)++;
            if (*x >= scr_x)
                (*x)--;
            if (scr_y <= *y)
                (*y)--;
            if (*y < 0)
                (*y)++;

            if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_1) {
                return 11111;
            }
            if (event.key.keysym.sym == SDLK_2) {
                return 2222;
            }
            if (event.key.keysym.sym == SDLK_s && can_I_save) {
                can_I_save = 0;
                return 's';
            }
        } else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT)
                f1 = 0;
        }
    }
}

void make_bomb(int y, int x, struct bl blocks[scr_y][scr_x]) {
    int bomb = value[2];
    int x_b, y_b;
    srand(time(0));
    while (bomb > 0) {
        x_b = rand() % scr_x;
        y_b = rand() % scr_y;
        while (((x_b == x || x_b == x - 1 || x_b == x + 1) && (y_b == y || y_b == y - 1 || y_b == y + 1)) || blocks[y_b][x_b].bomb_num == -1) {
            x_b = rand() % scr_x;
            y_b = rand() % scr_y;
        }
        blocks[y_b][x_b].bomb_num = -1;
        bomb--;
    }
    for (int i = 0; i < scr_x; ++i) {
        for (int j = 0; j < scr_y; ++j) {
            if (blocks[j][i].bomb_num != -1)
                for (int k = -1; k < 2; ++k) {
                    if (j + k == -1 || j + k == scr_y)
                        continue;
                    for (int l = -1; l < 2; ++l) {
                        if (l + i == -1 || l + i == scr_x)
                            continue;
                        if (blocks[j + k][i + l].bomb_num == -1) {
                            blocks[j][i].bomb_num++;
                        }
                    }
                }
        }
    }
}

void curve(int y, int x, struct bl blocks[scr_y][scr_x]) {
    if (y < 0 || y >= scr_y || x < 0 || x >= scr_x || blocks[y][x].st)
        return;
    can_I_save = 1;
    blocks[y][x].st = 1;
    if (blocks[y][x].bomb_num == 0) {
        for (int i = -1; i < 2; ++i) {
            for (int j = -1; j < 2; ++j) {
                curve(y + j, x + i, blocks);
            }
        }
    }

}

int main() {
    int x = 1, y = 1, bomb;
    int choose = init();
    if (choose % 2 == 1) {
        choose = load_game_init_value();
    }
    if (choose % 2 == 0) {
        new_game();
    }
    while (box_w * scr_y > 960) {
        box_w--;
    }
    while (box_w * scr_x > 2160) {
        box_w--;
    }
    while (box_w * scr_x < 300) {
        box_w++;
    }
    SDL_SetWindowSize(window, box_w * scr_x, box_w * scr_y + 50);
    struct bl blocks[scr_y][scr_x];
    for (int i = 0; i < scr_y; ++i) {
        for (int j = 0; j < scr_x; ++j) {
            blocks[i][j].st = 0;
            blocks[i][j].bomb_num = 0;
        }
    }
    bomb = value[2];
    if (choose % 2 == 0) {
        while (1) {
            int start_ticks = SDL_GetTicks();
            int hh = handleEvent(&y, &x);
            if (hh == 12345)
                return 0;
            if (hh == 11111) {
                break;
            }
            draw(blocks, value[2], x, y);
            end_time = start_time = SDL_GetTicks();
            while (SDL_GetTicks() - start_ticks < 1000 / FPS);
        }
        make_bomb(y, x, blocks);
        curve(y, x, blocks);
    } else {
        s = fopen("../save.s", "r");
        int a, b, c;
        fscanf(s, "%d%d%d", &a, &b, &c);
        for (int i = 0; i < scr_y; i++) {
            for (int j = 0; j < scr_x; j++) {
                int a;
                fscanf(s, "%d", &blocks[i][j].bomb_num);
            }
        }
        for (int i = 0; i < scr_y; i++) {
            for (int j = 0; j < scr_x; j++) {
                int a;
                fscanf(s, "%d", &blocks[i][j].st);
                if (blocks[i][j].bomb_num == -1 && blocks[i][j].st == 2) {
                    value[2]--;
                }
            }
        }
        fclose(s);
    }
    bomb = value[2];
    can_I_save = 1;
    play = 1;
    while (play) {
        int start_ticks = SDL_GetTicks();
        if (!win && !lost)
            end_time = SDL_GetTicks();
        int hh = handleEvent(&y, &x);
        if (hh == 's') {
            save(blocks);
        }
        if (hh == 12345)
            return 0;
        if (hh == 11111 && blocks[y][x].st != 2) {
            if (blocks[y][x].bomb_num != -1) {
                curve(y, x, blocks);
            }
            if (blocks[y][x].bomb_num == -1) {
                lost = 1;
                can_I_save = 0;
                for (int i = 0; i < scr_y; ++i) {
                    for (int j = 0; j < scr_x; ++j) {
                        if (blocks[i][j].st != 2)
                            blocks[i][j].st = 1;
                    }
                }
            }
        }
        if (hh == 2222) {
            if (blocks[y][x].st == 0 && bomb) {
                can_I_save = 1;
                blocks[y][x].st = 2;
                bomb--;
            } else if (blocks[y][x].st == 2) {
                can_I_save = 1;
                blocks[y][x].st = 0;
                bomb++;
            }
            if (bomb == 0) {
                bool end = 1;
                for (int i = 0; i < scr_y && end; ++i) {
                    for (int j = 0; j < scr_x && end; ++j) {
                        if (blocks[i][j].st == 0 && blocks[i][j].bomb_num == -1)
                            end = 0;
                    }
                }
                if (end) {
                    can_I_save = 0;
                    win = 1;
                    for (int i = 0; i < scr_y; ++i) {
                        for (int j = 0; j < scr_x; ++j) {
                            blocks[i][j].st = 1;
                        }
                    }
                }
            }
        }
        draw(blocks, bomb, x, y);
        while (SDL_GetTicks() - start_ticks < 1000 / FPS);
    }
    return 0;
}