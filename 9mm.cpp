#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "headers/animate.hpp"

float animation_parameter;
int animation_ongoing;
int safeguard;
float animation_parameter_abduct;
int animation_ongoing_abduct;
int safeguard_abduct;
int table[24] = {0, 0, -1, 0, 1, -1, -1, 1, 0, -1, 0, 1, 1, 0, 1, 1, -1, 0, 1, 0, 1, 0, 0};

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
    glLineWidth(3);

    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    animation_ongoing = 0;
    animation_parameter = 0;
    safeguard = 0;
    animation_ongoing_abduct = 0;
    animation_parameter_abduct = 0;
    safeguard_abduct = 0;

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

    gluPerspective(60, (float) 1366 / 768, 1, 10);
    glutReshapeWindow(1366, 768);
}

static void on_display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1, 6, 4.1, 0, 0, 0, 0, 1, 0);   


    animate_table(table); 

    get_mouse = 1;
        if(mouse_set) {   
            abduct_player(9);
        }

    glutSwapBuffers();
}