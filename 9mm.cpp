#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "headers/game.hpp"
#include "headers/animate.hpp"
#include "headers/draw.hpp"


int depth = 7; // 7 - easy, 8 - normal (~30 sec per move), 9 - hard (~2-3 min per move)


float animation_parameter = 0;
int animation_ongoing = 0;
int safeguard = 0;
float animation_parameter_abduct = 0;
int animation_ongoing_abduct = 0;
int safeguard_abduct = 0;
int table[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int tree_depth = depth;
int next_to_move = 1; 
int move_count = 0;
int game_phase = 1;
int cooefs[19] = {10, 10, 2, 8, 9, 25, 6, 15, 17, 8, 21, 28, 48, 3, 5779,
                  23, 27, 31, 5949};
int first_move;

float parameter = 0;
int parameter_indicator = 1;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1366, 768);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("9 men's morris");

    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutMouseFunc(on_click);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2);

    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(time(NULL));
    first_move = std::rand() % 4;

    initialize_texture();

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}

static void on_reshape(int width, int height){
    
    glViewport(0, 0, 1366, 768);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, (float) 1366 / 768, 1, 15);
    glutReshapeWindow(1366, 768);
}

static void on_display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    this_is_where_the_magic_happens();

    glutSwapBuffers();
}