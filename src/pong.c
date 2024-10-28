#include <stdio.h>

int mainMenu();
int gameProccess(int score_1, int score_2, int field_width, int field_height, int ball_x, int ball_y,
                 int pos_racket_1_y, int pos_racket_2_y, int ball_velocity_x, int ball_velocity_y);
int ballX(int ball_x, int ballVelocityX);
int ballY(int ball_y, int ballVelocityY);
int ballVelocityX(int field_width, int ball_x, int ball_y, int ball_velocity_x, int pos_racket_1_y,
                  int pos_racket_2_y);
int ballVelocityY(int field_width, int field_height, int ball_x, int ball_y, int ball_velocity_y,
                  int pos_racket_1_y, int pos_racket_2_y);
int racket_1_y(int field_height, int pos_racket_1_y, int direction);
int racket_2_y(int field_height, int pos_racket_2_y, int direction);
void greeting();
void fieldGeneration(int score_1, int score_2, int field_width, int field_height, int ball_x, int ball_y,
                     int pos_racket_1_y, int pos_racket_2_y);
void winner(int winner);
void fieldClear();

int main() {
    int score_1 = 0, score_2 = 0, result = 0, init_ball_velocity_x = 1, init_ball_velocity_y = 1;
    int field_width = 80, field_height = 25;
    int init_ball_x = (field_width - 2) / 2, init_ball_y = (field_height - 2) / 2;
    int init_pos_racket_1_y = (field_height - 2) / 2, init_pos_racket_2_y = (field_height - 2) / 2;

    if (mainMenu() == 1) {
        while (!(score_1 == 21) && !(score_2 == 21)) {
            fieldGeneration(score_1, score_2, field_width, field_height, init_ball_x, init_ball_y,
                            init_pos_racket_1_y, init_pos_racket_2_y);
            result = gameProccess(score_1, score_2, field_width, field_height, init_ball_x, init_ball_y,
                                  init_pos_racket_1_y, init_pos_racket_2_y, init_ball_velocity_x,
                                  init_ball_velocity_y);
            score_1 += result == 1 ? 1 : 0;
            score_2 += result == 2 ? 1 : 0;
            init_ball_velocity_x = result == 1 ? -1 : 1;
        }
    }
    fieldClear();
    if (score_1 == 21 && score_2 != 21) {
        winner(0);
    } else if (score_1 != 21 && score_2 == 21) {
        winner(1);
    }
    return 0;
}

int mainMenu() {
    int start;
    char c;

    fieldClear();
    greeting();
    start = scanf("%c", &c);
    fflush(stdin);
    return start;
}

int gameProccess(int score_1, int score_2, int field_width, int field_height, int ball_x, int ball_y,
                 int pos_racket_1_y, int pos_racket_2_y, int ball_velocity_x, int ball_velocity_y) {
    int flag = 0;
    char key;

    while (!flag) {
        fieldClear();
        fieldGeneration(score_1, score_2, field_width, field_height, ball_x, ball_y, pos_racket_1_y,
                        pos_racket_2_y);
        key = getchar();
        fflush(stdin);
        if (key == 'a') {
            pos_racket_1_y = racket_1_y(field_height, pos_racket_1_y, -1);
        } else if (key == 'z') {
            pos_racket_1_y = racket_1_y(field_height, pos_racket_1_y, 1);
        } else if (key == 'k') {
            pos_racket_2_y = racket_2_y(field_height, pos_racket_2_y, -1);
        } else if (key == 'm') {
            pos_racket_2_y = racket_2_y(field_height, pos_racket_2_y, 1);
        } else if (key == ' ') {
        } else {
            continue;
        }

        ball_velocity_x =
            ballVelocityX(field_width, ball_x, ball_y, ball_velocity_x, pos_racket_1_y, pos_racket_2_y);

        if (ball_y == 1 || ball_y == field_height - 2) {
            ball_velocity_y = ballVelocityY(field_width, field_height, ball_x, ball_y, ball_velocity_y,
                                            pos_racket_1_y, pos_racket_2_y);
        }

        ball_x = ballX(ball_x, ball_velocity_x);
        ball_y = ballY(ball_y, ball_velocity_y);

        if (ball_x >= field_width - 1 || ball_x <= 1) {
            if ((ball_y <= pos_racket_1_y + 1 && ball_y >= pos_racket_1_y - 1) ||
                (ball_y <= pos_racket_2_y + 1 && ball_y >= pos_racket_2_y - 1)) {
                ball_velocity_x = ballVelocityX(field_width, ball_x, ball_y, ball_velocity_x, pos_racket_1_y,
                                                pos_racket_2_y);
                ball_velocity_y = ballVelocityY(field_width, field_height, ball_x, ball_y, ball_velocity_y,
                                                pos_racket_1_y, pos_racket_2_y);
            } else {
                flag = ball_x == 1 ? 2 : 1;
            }
        }
    }
    return flag;
}

int ballX(int ball_x, int ballVelocityX) { return ball_x += ballVelocityX; }

int ballY(int ball_y, int ballVelocityY) { return ball_y += ballVelocityY; }

int ballVelocityX(int field_width, int ball_x, int ball_y, int ball_velocity_x, int pos_racket_1_y,
                  int pos_racket_2_y) {
    if (((ball_y <= pos_racket_1_y + 1) && (ball_y >= pos_racket_1_y - 1)) ||
        ((ball_y <= pos_racket_2_y + 1) && (ball_y >= pos_racket_2_y - 1))) {
        if (ball_x == 3 || ball_x == field_width - 4) {
            ball_velocity_x = -1 * ball_velocity_x;
        }
    }
    return ball_velocity_x;
}

int ballVelocityY(int field_width, int field_height, int ball_x, int ball_y, int ball_velocity_y,
                  int pos_racket_1_y, int pos_racket_2_y) {
    if (ball_y == 1 || ball_y == field_height - 2) {
        ball_velocity_y = -1 * ball_velocity_y;
    } else if ((ball_y == pos_racket_1_y && ball_x == 2) ||
               (ball_y == pos_racket_2_y && ball_x == field_width - 3)) {
        ball_velocity_y = -1 * ball_velocity_y;
    } else if ((ball_y == pos_racket_1_y - 1 && ball_x == 2) ||
               (ball_y == pos_racket_2_y - 1 && ball_x == field_width - 3)) {
        ball_velocity_y = (ball_velocity_y > 0) ? (-1 * ball_velocity_y) : (ball_velocity_y);
    } else if ((ball_y == pos_racket_1_y + 1 && ball_x <= 1) ||
               (ball_y == pos_racket_2_y + 1 && ball_x >= field_width)) {
        ball_velocity_y = (ball_velocity_y > 0) ? (ball_velocity_y) : (-1 * ball_velocity_y);
    }
    return ball_velocity_y;
}

int racket_1_y(int field_height, int pos_racket_1_y, int direction) {
    if (pos_racket_1_y > 2 && pos_racket_1_y < field_height - 3) {
        pos_racket_1_y += direction;
    } else if (pos_racket_1_y > 2 && direction == -1) {
        pos_racket_1_y += direction;
    } else if (pos_racket_1_y < field_height - 3 && direction == 1) {
        pos_racket_1_y += direction;
    }
    return pos_racket_1_y;
}

int racket_2_y(int field_height, int pos_racket_2_y, int direction) {
    if (pos_racket_2_y > 2 && pos_racket_2_y < field_height - 3) {
        pos_racket_2_y += direction;
    } else if (pos_racket_2_y > 2 && direction == -1) {
        pos_racket_2_y += direction;
    } else if (pos_racket_2_y < field_height - 3 && direction == 1) {
        pos_racket_2_y += direction;
    }
    return pos_racket_2_y;
}

void greeting() {
    printf("#------------------------------------------------------------------------------#\n");
    printf("#                                                                              #\n");
    printf("#                            Welcome to pong game!                             #\n");
    printf("#                                                                              #\n");
    printf("#                            Controls settings:                                #\n");
    printf("#                     Player 1: a - move racket at the top                     #\n");
    printf("#                               z - move racket at the bottom                  #\n");
    printf("#                     Player 2: k - move racket at the top                     #\n");
    printf("#                               m - move racket at the bottom                  #\n");
    printf("#                                                                              #\n");
    printf("#                    Press any button to strat new game!                       #\n");
    printf("#                                                                              #\n");
    printf("#------------------------------------------------------------------------------#\n");
}

void fieldGeneration(int score_1, int score_2, int field_width, int field_height, int ball_x, int ball_y,
                     int pos_racket_1_y, int pos_racket_2_y) {
    printf("                              1 Score %d : %d Score 2                         \n", score_1,
           score_2);
    printf("\n");
    for (int height = 0; height < field_height; height++) {
        for (int width = 0; width < field_width; width++) {
            if (height == 0 || height == field_height - 1) {
                printf("-");
            } else if (width == 0 || width == field_width - 1) {
                printf("|");
            } else if (height == ball_y && width == ball_x) {
                printf("o");
            } else if ((width == 2) && (height == pos_racket_1_y || height == pos_racket_1_y - 1 ||
                                        height == pos_racket_1_y + 1)) {
                printf("]");
            } else if ((width == field_width - 3) &&
                       ((height == pos_racket_2_y || height == pos_racket_2_y - 1 ||
                         height == pos_racket_2_y + 1))) {
                printf("[");
            } else if (width == field_width / 2) {
                printf(".");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void winner(int winner) {
    printf("#------------------------------------------------------------------------------#\n");
    printf("#                                                                              #\n");
    printf("#                                                                              #\n");
    if (winner == 0) {
        printf("#                                 Player 1 won                                 #\n");
    } else {
        printf("#                                 Player 2 won                                 #\n");
    }
    printf("#                                                                              #\n");
    printf("#                                                                              #\n");
    printf("#                               Congratulations!                               #\n");
    printf("#                                                                              #\n");
    printf("#                                                                              #\n");
    printf("#                                                                              #\n");
    printf("#                              Game made by kept7                              #\n");
    printf("#                                                                              #\n");
    printf("#------------------------------------------------------------------------------#\n");
}

void fieldClear() { printf("\033[0d\033[2J\n"); }
