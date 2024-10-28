#include <ncurses.h>
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

int main() {
    int score_1 = 0, score_2 = 0, result = 0, init_ball_velocity_x = 1, init_ball_velocity_y = 1;
    int field_width = 80, field_height = 25;
    int init_ball_x = (field_width - 2) / 2, init_ball_y = (field_height - 2) / 2;
    int init_pos_racket_1_y = (field_height - 2) / 2, init_pos_racket_2_y = (field_height - 2) / 2;

    initscr();

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

    if (score_1 == 21 && score_2 != 21) {
        winner(0);
    } else if (score_1 != 21 && score_2 == 21) {
        winner(1);
    }

    endwin();

    return 0;
}

int mainMenu() {
    int start;
    char c;

    greeting();
    start = scanf("%c", &c);

    return start;
}

int gameProccess(int score_1, int score_2, int field_width, int field_height, int ball_x, int ball_y,
                 int pos_racket_1_y, int pos_racket_2_y, int ball_velocity_x, int ball_velocity_y) {
    int flag = 0;
    char key;

    while (!flag) {
        fieldGeneration(score_1, score_2, field_width, field_height, ball_x, ball_y, pos_racket_1_y,
                        pos_racket_2_y);
        cbreak();
        noecho();
        halfdelay(2);

        key = getch();
        if (key == 'a') {
            pos_racket_1_y = racket_1_y(field_height, pos_racket_1_y, -1);
        } else if (key == 'z') {
            pos_racket_1_y = racket_1_y(field_height, pos_racket_1_y, 1);
        } else if (key == 'k') {
            pos_racket_2_y = racket_2_y(field_height, pos_racket_2_y, -1);
        } else if (key == 'm') {
            pos_racket_2_y = racket_2_y(field_height, pos_racket_2_y, 1);
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
    clear();
    printw("#------------------------------------------------------------------------------#\n");
    printw("#                                                                              #\n");
    printw("#                            Welcome to pong game!                             #\n");
    printw("#                                                                              #\n");
    printw("#                            Controls settings:                                #\n");
    printw("#                     Player 1: a - move racket at the top                     #\n");
    printw("#                               z - move racket at the bottom                  #\n");
    printw("#                     Player 2: k - move racket at the top                     #\n");
    printw("#                               m - move racket at the bottom                  #\n");
    printw("#                                                                              #\n");
    printw("#                    Press any button to strat new game!                       #\n");
    printw("#                                                                              #\n");
    printw("#------------------------------------------------------------------------------#\n");
    refresh();
}

void fieldGeneration(int score_1, int score_2, int field_width, int field_height, int ball_x, int ball_y,
                     int pos_racket_1_y, int pos_racket_2_y) {
    clear();
    printw("                              1 Score %d : %d Score 2                         \n", score_1,
           score_2);
    printw("\n");
    for (int height = 0; height < field_height; height++) {
        for (int width = 0; width < field_width; width++) {
            if (height == 0 || height == field_height - 1) {
                printw("-");
            } else if (width == 0 || width == field_width - 1) {
                printw("|");
            } else if (height == ball_y && width == ball_x) {
                printw("o");
            } else if ((width == 2) && (height == pos_racket_1_y || height == pos_racket_1_y - 1 ||
                                        height == pos_racket_1_y + 1)) {
                printw("]");
            } else if ((width == field_width - 3) &&
                       ((height == pos_racket_2_y || height == pos_racket_2_y - 1 ||
                         height == pos_racket_2_y + 1))) {
                printw("[");
            } else if (width == field_width / 2) {
                printw(".");
            } else {
                printw(" ");
            }
        }

        printw("\n");
        refresh();
    }
}

void winner(int winner) {
    clear();

    printw("#------------------------------------------------------------------------------#\n");
    printw("#                                                                              #\n");
    printw("#                                                                              #\n");
    if (winner == 0) {
        printw("#                                 Player 1 won                                 #\n");
    } else {
        printw("#                                 Player 2 won                                 #\n");
    }
    printw("#                                                                              #\n");
    printw("#                                                                              #\n");
    printw("#                               Congratulations!                               #\n");
    printw("#                                                                              #\n");
    printw("#                                                                              #\n");
    printw("#                                                                              #\n");
    printw("#                              Game made by kept7                              #\n");
    printw("#                                                                              #\n");
    printw("#------------------------------------------------------------------------------#\n");

    refresh();
    getchar();
}
