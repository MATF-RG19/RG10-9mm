#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../headers/draw.hpp"

int ufo_sphere_material = 4;

void init_lights() {
    GLfloat light0_position[] = {-3, 4.5, 2.2, 0};
    GLfloat light1_position[] = {3, 4.5, 3.1, 0};
    GLfloat light2_position[] = {2.2, 4.5, -3, 0};


    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};
    GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};
    GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};

    glEnable(GL_LIGHTING);
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);    
}

void set_material(int id, float alpha) {
    GLfloat ambient_coeffs[] = {0.1, 0.1, 0.1, alpha};    
    GLfloat diffuse_coeffs[] = {0.2, 0.2, 0.2, alpha};
    GLfloat specular_coeffs[] = {0.8, 0.8, 0.8, alpha};
    GLfloat shininess = 20;

    if (id == 1) {
      diffuse_coeffs[0] = 0.9;
      diffuse_coeffs[1] = 0.9;
      diffuse_coeffs[2] = 0.9;
    }
    else if (id == 2) {
      diffuse_coeffs[2] = 1.0;
    }
    else if (id == 3) {
      diffuse_coeffs[0] = 1.0;
    }
    else if (id == 4) {
      diffuse_coeffs[1] = 1.0;
    }

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void draw_circle(float centerX, float centerY, float centerZ, float radius, int slices) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(centerX, centerY, centerZ);
  for (int i = 0; i <= slices; i++) 
    glVertex3f(centerX + radius * cos(2*PI / slices * i), centerY, centerZ + radius * sin(2*PI / slices * i));
  glEnd();
}

 void set_normal_and_vertex(float u, float v, float centerX, float centerY, float centerZ, float radius) {
  glNormal3f(
    sin(u) * sin(v) * radius + centerX,
    cos(u) * radius + centerY,
    sin(u) * cos(v) * radius + centerZ
  );
  glVertex3f(
    sin(u) * sin(v) * radius + centerX,
    cos(u) * radius + centerY,
    sin(u) * cos(v) * radius + centerZ
  );
}

void draw_opponent_figure(int position, float radius, int slices, float alpha) {

  //na osnovu polja na tabli dobijamo koordinate figure
  float coordinates[3];
  get_coordinates_of_position(position, coordinates);
  float centerX = coordinates[0];
  //posto je Y koordinata centra igraceve figure 0.3 kada se figura nalazi na tabli
  //oduzimamo 0.1 zbog oblika i pozicije protivnicke figure kako ne bi lebdela iznad table
  //kada je u tom neutralnom polozaju
  float centerY = coordinates[1] - 0.1;
  float centerZ = coordinates[2];

  //iscrtavamo figuru
  float u, v;
  
  set_material(3, alpha);
  glPushMatrix();

  draw_circle(centerX, centerY, centerZ, radius, 20);

  for (u = 0; u < PI/2; u += PI / slices) {
    glBegin(GL_TRIANGLE_STRIP);
    for (v = 0; v < 2*PI + EPSILONE; v += PI / slices) {
      set_normal_and_vertex(u, v, centerX, centerY, centerZ, radius);
      set_normal_and_vertex(u + PI / slices, v, centerX, centerY, centerZ, radius);
    }
    glEnd();
  }
  glPopMatrix();
}

//f-ja crta igracevu figuru koja predstavlja sredisnji isecak sfere plave boje
//(sferu kojoj su skinute gornja i donja "kapa" odredjene velicine)
void draw_player_figure(int position, float radius, int slices, float alpha) {

  //na osnovu polja na tabli dobijamo koordinate figure
  float coordinates[3];
  get_coordinates_of_position(position, coordinates);
  float centerX = coordinates[0];
  float centerY = coordinates[1];
  float centerZ = coordinates[2];

  //iscrtavamo figuru
  float u, v;

  //visina sferne kape koja je odsecena
  float h = 3 * radius / 3.5;

  //polova visine tog segmenta sfere koji je ostao kada se odseku gornja i donja kapa
  //za toliko treba da pomerimo krug po Y-u iz centra da bi smo zatvorili vrh i dno figure
  float half_of_whats_left = radius - h;

  //poluprecnik kruga koji predstavlja bazu odsecene sferne kape
  float r = sqrt(radius * 2 * h - h * h);
  
  set_material(2, alpha);
  glPushMatrix();

  draw_circle(centerX, centerY + half_of_whats_left, centerZ, r, 20);
  draw_circle(centerX, centerY - half_of_whats_left, centerZ, r, 20);

  for (u = 3*PI/7; u < 4*PI/7; u += PI / slices) {
    glBegin(GL_TRIANGLE_STRIP);
    for (v = 0; v < 2*PI + EPSILONE; v += PI / slices) {
      set_normal_and_vertex(u, v, centerX, centerY, centerZ, radius);
      set_normal_and_vertex(u + PI / slices, v, centerX, centerY, centerZ, radius);
    }
    glEnd();
  }
  glPopMatrix();
}

void set_normal_and_vertex_cone(float u, float v, float centerX, float centerY, float centerZ, float radius) {
  glNormal3f(
    u * sin(v) * radius * 0.4 + centerX,
    (2*PI - u) * radius + centerY,
    u * cos(v) * radius * 0.4 + centerZ
  );

  glVertex3f(
    u * sin(v) * radius * 0.4 + centerX,
    (2*PI - u) * radius + centerY,
    u * cos(v) * radius * 0.4 + centerZ
  );
}

void draw_cone(int position) {
  float coordinates[3];
  get_coordinates_of_position(position, coordinates);

  float u, v;

  glPushMatrix();
  for (u = 2*PI; u >= 0; u -= PI / 80) {
    glBegin(GL_LINES);
    for (v = 0; v < 2 * PI + EPSILONE; v += PI / 7) {
      set_normal_and_vertex_cone(u, v, coordinates[0], coordinates[1], coordinates[2], 0.23);
      set_normal_and_vertex_cone(u + PI/40, v, coordinates[0], coordinates[1], coordinates[2], 0.23);
    }
    glEnd();
  }
  glPopMatrix();
}

void set_normal_and_vertex_ufo(float u, float v, float centerX, float centerY, float centerZ, float radius) {
  glNormal3f(
    u * sin(v) * radius + centerX,
    u * 1.3 * radius + centerY,
    u * cos(v) * radius + centerZ
  );

  glVertex3f(
    u * sin(v) * radius + centerX,
    u * 1.3 * radius + centerY,
    u * cos(v) * radius + centerZ
  );
}

void draw_player_ufo(int position, float angle) {
  float coordinates[3];
  get_coordinates_of_position(position, coordinates);

  float u, v, coef;

  set_material(4,1);
  // y = PI * 1.3 * 0.23 + 1.74 + 0.025
  //radius = 0.23 * PI (poluprecnik baze kupe nacrtane na onakav nacin)
  draw_circle(coordinates[0], 2.703, coordinates[2], 0.23 * PI, 40);
  glPushMatrix();
  for (u = 0; u <= PI; u += PI / 60) {
    glBegin(GL_TRIANGLE_STRIP);
    for (v = 0; v < 2 * PI + EPSILONE; v += PI / 40) {
      coef = ((float)rand() / (float)RAND_MAX + u / PI) / 2;
      if (coef < 0.5) 
        set_material(2, 1);
      else
        set_material(4, 1);      
      set_normal_and_vertex_ufo(u, v, coordinates[0], coordinates[1] + 1.44, coordinates[2], 0.23);
      set_normal_and_vertex_ufo(u + PI/40, v, coordinates[0], coordinates[1] + 1.44, coordinates[2], 0.23);
    }
    glEnd();
  }
  glPopMatrix();

  //da bi se boja sfere na ufo-u redje menjala
  coef = (float)rand() / (float)RAND_MAX;
  if (coef > 0.9) 
    if (ufo_sphere_material == 4 || ufo_sphere_material == 3) {
      set_material(2, 1);
      ufo_sphere_material = 2;
    }
    else {
      set_material(4, 1);
      ufo_sphere_material = 4;
    }
  else 
    if (ufo_sphere_material == 4 || ufo_sphere_material == 3) {
      set_material(4, 1);
      ufo_sphere_material == 4;
    }
    else
      set_material(2, 1);
  
  
  glPushMatrix();
  glTranslatef(coordinates[0], 2.703, coordinates[2]);
  glutSolidSphere(0.3, 40, 40);


  set_material(2, 1);
  glPushMatrix();
  glRotatef(-angle, 0, 1, 0);
  draw_ufo_stabilisators();
  glPopMatrix();
  glPopMatrix();
}

void draw_opponent_ufo(int position, float angle) {
  float coordinates[3];
  get_coordinates_of_position(position, coordinates);

  float u, v, coef;

  set_material(4,1);
  // y = PI * 1.3 * 0.23 + 1.74 + 0.025
  //radius = 0.23 * PI (poluprecnik baze kupe nacrtane na onakav nacin)
  draw_circle(coordinates[0], 2.703, coordinates[2], 0.23 * PI, 40);
  glPushMatrix();
  for (u = 0; u <= PI; u += PI / 60) {
    glBegin(GL_TRIANGLE_STRIP);
    for (v = 0; v < 2 * PI + EPSILONE; v += PI / 40) {
      coef = ((float)rand() / (float)RAND_MAX + u / PI) / 2;
      if (coef < 0.5) 
        set_material(3, 1);
      else
        set_material(4, 1);      
      set_normal_and_vertex_ufo(u, v, coordinates[0], coordinates[1] + 1.44, coordinates[2], 0.23);
      set_normal_and_vertex_ufo(u + PI/40, v, coordinates[0], coordinates[1] + 1.44, coordinates[2], 0.23);
    }
    glEnd();
  }
  glPopMatrix();

  //da bi se boja sfere na ufo-u redje menjala
  coef = (float)rand() / (float)RAND_MAX;
  if (coef > 0.9) 
    if (ufo_sphere_material == 4 || ufo_sphere_material == 2) {
      set_material(3, 1);
      ufo_sphere_material = 3;
    }
    else {
      set_material(4, 1);
      ufo_sphere_material = 4;
    }
  else 
    if (ufo_sphere_material == 4 || ufo_sphere_material == 2) {
      set_material(4, 1);
      ufo_sphere_material == 4;
    }
    else
      set_material(3, 1);
  
  glPushMatrix();
  glTranslatef(coordinates[0], 2.703, coordinates[2]);
  glutSolidSphere(0.3, 40, 40);

  set_material(3, 1);
  glPushMatrix();
  glRotatef(angle, 0, 1, 0);
  draw_ufo_stabilisators();
  glPopMatrix();
  glPopMatrix();
}

void draw_ufo_stabilisators() {
  glBegin(GL_QUADS);    
  //prvi stabilizator
  //gornji pravougaonik
  glVertex3f(1.3, -0.01, 0.05);
  glVertex3f(1.3, -0.01, -0.05);
  glVertex3f(-1.3, -0.01, -0.05);
  glVertex3f(-1.3, -0.01, 0.05); 

  //donji pravougaonik
  glVertex3f(1.3, -0.08, 0.05);
  glVertex3f(1.3, -0.08, -0.05);
  glVertex3f(-1.3, -0.08, -0.05);
  glVertex3f(-1.3, -0.08, 0.05);

  //ostale ivice stabilizatora
  glVertex3f(-1.3, -0.08, -0.05);
  glVertex3f(-1.3, -0.01, -0.05);
  glVertex3f(-1.3, -0.01, 0.05);
  glVertex3f(-1.3, -0.08, 0.05);

  glVertex3f(-1.3, -0.08, -0.05);
  glVertex3f(-1.3, -0.01, -0.05);
  glVertex3f(1.3, -0.01, -0.05);
  glVertex3f(1.3, -0.08, -0.05);

  glVertex3f(1.3, -0.08, -0.05);
  glVertex3f(1.3, -0.01, -0.05);
  glVertex3f(1.3, -0.01, 0.05);
  glVertex3f(1.3, -0.08, 0.05);

  glVertex3f(1.3, -0.08, 0.05);
  glVertex3f(1.3, -0.01, 0.05);
  glVertex3f(1.3, -0.01, -0.05);
  glVertex3f(1.3, -0.08, -0.05);


  //drugi stabilizator
  //gornji pravougaonik
  glVertex3f(0.05, -0.01, 1.3);
  glVertex3f(0.05, -0.01, -1.3);
  glVertex3f(-0.05, -0.01, -1.3);
  glVertex3f(-0.05, -0.01, 1.3); 

  //donji pravougaonik
  glVertex3f(0.05, -0.08, 1.3);
  glVertex3f(0.05, -0.08, -1.3);
  glVertex3f(-0.05, -0.08, -1.3);
  glVertex3f(-0.05, -0.08, 1.3);

  //ostale ivice stabilizatora
  glVertex3f(-0.05, -0.08, -1.3);
  glVertex3f(-0.05, -0.01, -1.3);
  glVertex3f(-0.05, -0.01, 1.3);
  glVertex3f(-0.05, -0.08, 1.3);

  glVertex3f(-0.05, -0.08, -1.3);
  glVertex3f(-0.05, -0.01, -1.3);
  glVertex3f(0.05, -0.01, -1.3);
  glVertex3f(0.05, -0.08, -1.3);

  glVertex3f(0.05, -0.08, -1.3);
  glVertex3f(0.05, -0.01, -1.3);
  glVertex3f(0.05, -0.01, 1.3);
  glVertex3f(0.05, -0.08, 1.3);

  glVertex3f(0.05, -0.08, 1.3);
  glVertex3f(0.05, -0.01, 1.3);
  glVertex3f(0.05, -0.01, -1.3);
  glVertex3f(0.05, -0.08, -1.3);
  glEnd();
}

void draw_table(float alpha) {

  //sama tabla
  set_material(1, alpha);
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
  set_material(1, alpha);
  //spoljasnji prsten linija na tabli
  glVertex3f(-2.8, 0.2, 2.8);
  glVertex3f(2.8, 0.2, 2.8);
  glVertex3f(2.8, 0.2, 2.8);
  glVertex3f(2.8, 0.2, -2.8);
  glVertex3f(2.8, 0.2, -2.8);
  glVertex3f(-2.8, 0.2, -2.8);
  glVertex3f(-2.8, 0.2, -2.8);
  glVertex3f(-2.8, 0.2, 2.8);

  //srednji prsten linija na tabli
  glVertex3f(-2.3, 0.2, 2.3);
  glVertex3f(2.3, 0.2, 2.3);
  glVertex3f(2.3, 0.2, 2.3);
  glVertex3f(2.3, 0.2, -2.3);
  glVertex3f(2.3, 0.2, -2.3);
  glVertex3f(-2.3, 0.2, -2.3);
  glVertex3f(-2.3, 0.2, -2.3);
  glVertex3f(-2.3, 0.2, 2.3);

  //unutrasnji prsten linija na tabli
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
  set_material(0, alpha);     
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

void get_coordinates_of_position(int i, float* coordinates) {
  if (i < 0 || i > 23 ) {
    fprintf(stderr, "You asked for coordinates of invalid position\n");
    exit(1);
  }
  else if (i == 0) {
    *coordinates = -2.8;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 2.8;
  }
  else if (i == 1) {
    *coordinates = 0;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 2.8;
  }
  else if (i == 2) {
    *coordinates = 2.8;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 2.8;
  }
  else if (i == 3) {
    *coordinates = -2.3;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 2.3;
  }
  else if (i == 4) {
    *coordinates = 0;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 2.3;
  }
  else if (i == 5) {
    *coordinates = 2.3;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 2.3;
  }
  else if (i == 6) {
    *coordinates = -1.7;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 1.7;
  }
  else if (i == 7) {
    *coordinates = 0;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 1.7;
  }
  else if (i == 8) {
    *coordinates = 1.7;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 1.7;
  }
  else if (i == 9) {
    *coordinates = -2.8;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 0;
  }
  else if (i == 10) {
    *coordinates = -2.3;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 0;
  }
  else if (i == 11) {
    *coordinates = -1.7;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 0;
  }
  else if (i == 12) {
    *coordinates = 1.7;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 0;
  }
  else if (i == 13) {
    *coordinates = 2.3;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 0;
  }
  else if (i == 14) {
    *coordinates = 2.8;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = 0;
  }
  else if (i == 15) {
    *coordinates = -1.7;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -1.7;
  }
  else if (i == 16) {
    *coordinates = 0;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -1.7;
  }
  else if (i == 17) {
    *coordinates = 1.7;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -1.7;
  }
  else if (i == 18) {
    *coordinates = -2.3;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -2.3;
  }
  else if (i == 19) {
    *coordinates = 0;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -2.3;
  }
  else if (i == 20) {
    *coordinates = 2.3;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -2.3;
  }
  else if (i == 21) {
    *coordinates = -2.8;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -2.8;
  }
  else if (i == 22) {
    *coordinates = 0;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -2.8;
  }
  else if (i == 23) {
    *coordinates = 2.8;
    *(coordinates + 1) = 0.3;
    *(coordinates + 2) = -2.8;
  }
}