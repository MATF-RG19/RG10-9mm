#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.141592653589
#define EPSILONE 0.01

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int id);


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("9 men's morris");

    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(3);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

static void init_lights() {
    GLfloat light_position[] = {1, 3, 1, 0};

    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};
    GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};
    GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

static void set_material(int id) {
    GLfloat ambient_coeffs[] = {0.1, 0.1, 0.1, 1};

    
    GLfloat diffuse_coeffs[] = {0.2, 0.2, 0.2, 1};
    GLfloat specular_coeffs[] = {1, 1, 1, 1};
    GLfloat shininess = 5;

    if (id == 1) {
      diffuse_coeffs[0] = 1.0;
      diffuse_coeffs[1] = 1.0;
      diffuse_coeffs[2] = 1.0;
    }

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

static void draw_circle(float centerX, float centerY, float centerZ, float radius, int slices) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(centerX, centerY, centerZ);
  for (int i = 0; i <= slices; i++) 
    glVertex3f(centerX + radius * cos(2*PI / slices * i), centerY, centerZ + radius * sin(2*PI / slices * i));
  glEnd();
}

static void draw_table() {

  set_material(1);
  //kruzici koji obelezavaju pozicije na kojima se postavljaju figure
  //pozicije redom: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
  draw_circle(-2.8, 0.201, 2.8, 0.1, 20);
  draw_circle(0, 0.201, 2.8, 0.1, 20);
  draw_circle(2.8, 0.201, 2.8, 0.1, 20);
  draw_circle(-2.3, 0.201, 2.3, 0.1, 20);
  draw_circle(0, 0.201, 2.3, 0.1, 20);
  draw_circle(2.3, 0.201, 2.3, 0.1, 20);
  draw_circle(-1.7, 0.201, 1.7, 0.1, 20);
  draw_circle(0, 0.201, 1.7, 0.1, 20);
  draw_circle(1.7, 0.201, 1.7, 0.1, 20);
  draw_circle(-2.8, 0.201, 0, 0.1, 20);
  draw_circle(-2.3, 0.201, 0, 0.1, 20);
  draw_circle(-1.7, 0.201, 0, 0.1, 20);  
  draw_circle(1.7, 0.201, 0, 0.1, 20);
  draw_circle(2.3, 0.201, 0, 0.1, 20);
  draw_circle(2.8, 0.201, 0, 0.1, 20);
  draw_circle(-1.7, 0.201, -1.7, 0.1, 20);
  draw_circle(0, 0.201, -1.7, 0.1, 20);
  draw_circle(1.7, 0.201, -1.7, 0.1, 20);
  draw_circle(-2.3, 0.201, -2.3, 0.1, 20);
  draw_circle(0, 0.201, -2.3, 0.1, 20);
  draw_circle(2.3, 0.201, -2.3, 0.1, 20);
  draw_circle(-2.8, 0.201, -2.8, 0.1, 20);
  draw_circle(0, 0.201, -2.8, 0.1, 20);
  draw_circle(2.8, 0.201, -2.8, 0.1, 20);


  glBegin(GL_LINES);
  set_material(1);
  //spoljasnji prsten
  glVertex3f(-2.8, 0.2, 2.8);
  glVertex3f(2.8, 0.2, 2.8);
  glVertex3f(2.8, 0.2, 2.8);
  glVertex3f(2.8, 0.2, -2.8);
  glVertex3f(2.8, 0.2, -2.8);
  glVertex3f(-2.8, 0.2, -2.8);
  glVertex3f(-2.8, 0.2, -2.8);
  glVertex3f(-2.8, 0.2, 2.8);

  //srednji prsten 
  glVertex3f(-2.3, 0.2, 2.3);
  glVertex3f(2.3, 0.2, 2.3);
  glVertex3f(2.3, 0.2, 2.3);
  glVertex3f(2.3, 0.2, -2.3);
  glVertex3f(2.3, 0.2, -2.3);
  glVertex3f(-2.3, 0.2, -2.3);
  glVertex3f(-2.3, 0.2, -2.3);
  glVertex3f(-2.3, 0.2, 2.3);

  //unutrasnji prsten
  glVertex3f(-1.7, 0.2, 1.7);
  glVertex3f(1.7, 0.2, 1.7);
  glVertex3f(1.7, 0.2, 1.7);
  glVertex3f(1.7, 0.2, -1.7);
  glVertex3f(1.7, 0.2, -1.7);
  glVertex3f(-1.7, 0.2,- 1.7);
  glVertex3f(-1.7, 0.2, -1.7);
  glVertex3f(-1.7, 0.2, 1.7);

  //poprecne linije na tabli
  glVertex3f(2.8, 0.2, 0);
  glVertex3f(1.7, 0.2, 0);
  glVertex3f(0, 0.2, 2.8);
  glVertex3f(0, 0.2, 1.7);
  glVertex3f(0, 0.2, -2.8);
  glVertex3f(0, 0.2,- 1.7);
  glVertex3f(-2.8, 0.2, 0);
  glVertex3f(-1.7, 0.2, 0);
  glEnd();

  glBegin(GL_QUADS);
  set_material(0);     
  //gornji pravougaonik
  glVertex3f(3, 0.2, 3);
  glVertex3f(3, 0.2, -3);
  glVertex3f(-3, 0.2, -3);
  glVertex3f(-3, 0.2, 3);

  //donji pravougaonik
  glVertex3f(3, -0.2, 3);
  glVertex3f(3, -0.2, -3);
  glVertex3f(-3, -0.2, -3);
  glVertex3f(-3, -0.2, 3);

  //ostale ivice table
  glVertex3f(-3, -0.2, -3);
  glVertex3f(-3, 0.2, -3);
  glVertex3f(-3, 0.2, 3);
  glVertex3f(-3, -0.2, 3);

  glVertex3f(-3, -0.2, -3);
  glVertex3f(-3, 0.2, -3);
  glVertex3f(3, 0.2, -3);
  glVertex3f(3, -0.2, -3);

  glVertex3f(3, -0.2, -3);
  glVertex3f(3, 0.2, -3);
  glVertex3f(3, 0.2, 3);
  glVertex3f(3, -0.2, 3);

  glVertex3f(3, -0.2, 3);
  glVertex3f(3, 0.2, 3);
  glVertex3f(3, 0.2, -3);
  glVertex3f(3, -0.2, -3);
  glEnd();
}

static void on_display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1, 6, 4.1, 0, 0, 0, 0, 1, 0);   

    init_lights();

    
    draw_table();
      

    glutSwapBuffers();
}



