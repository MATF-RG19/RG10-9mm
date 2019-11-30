#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "headers/animate.hpp"

float animation_parameter;
int animation_ongoing;
int safeguard;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("9 men's morris");

    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);

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
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, (float) width / height, 1, 10);
}

static void on_display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1, 6, 4.1, 0, 0, 0, 0, 1, 0);   

    int table[24] = {0, 0, -1, 0, 1, -1, -1, 1, 0, 0, 0, 1, 1, 0, 1, 1, -1, 0, 1, 0, 1, 0, 0};

    glutFullScreen();
    animate_table(table); 

    glutSwapBuffers();
}