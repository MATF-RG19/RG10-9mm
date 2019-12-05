#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../headers/draw.hpp"
#include "../headers/animate.hpp"


int get_mouse = 0;
int mouse_set = 0;
int x_mouse;
int y_mouse;


void animate_table(int table[24]) {

    init_lights();
    draw_table(1);

    //postavljamo figure
    for (int i = 0; i < 24; i++) {
        if (table[i] == 1)
            draw_opponent_figure(i, 0.2, 40, 1);
        else if (table[i] == -1)
            draw_player_figure(i, 0.2, 40, 1);
    }       
}

//animacija postavljanja igraceve figure na tablu
//izvodi se u opsegu tajmera 0-100
void put_player_figure(int position) {
    animation_ongoing = 1;
    if (animation_parameter <= 100) {
        glPushMatrix();
        glTranslatef(0, 1 - animation_parameter / 100, 0);
        draw_player_figure(position, 0.2, 40, animation_parameter / 100.0);
        glPopMatrix();
        if (safeguard == 0) { 
            glutTimerFunc(100, on_timer, 1);
            safeguard = 1;
        }
    }
    else {
        safeguard = 0;
    }       
}

//animacija postavljanja protivnikove figure na tablu
//izvodi se u opsegu tajmera 0-100
void put_opponent_figure(int position) {
    animation_ongoing = 1;
    if (animation_parameter <= 100) {
        glPushMatrix();
        glTranslatef(0, 1 - animation_parameter / 100.0, 0);
        draw_opponent_figure(position, 0.2, 40, animation_parameter / 100.0);
        glPopMatrix();
        if (safeguard == 0) { 
            glutTimerFunc(100, on_timer, 1);
            safeguard = 1;
        }
    }
    else {
        safeguard = 0;
    }
}

//animacija podizanja igraceve figure sa table
//izvodi se u opsegu tajmera 0-100
void lift_player_figure(int position) {
    animation_ongoing = 1;
    if (animation_parameter <= 100) {
        glPushMatrix();
        glTranslatef(0, animation_parameter / 100.0, 0);
        draw_player_figure(position, 0.2, 40,1 - animation_parameter / 100.0);
        glPopMatrix();
        if (safeguard == 0) { 
            glutTimerFunc(100, on_timer, 1);
            safeguard = 1;
        }
    }
    else {
        safeguard = 0;
    }
}

//animacija podizanja protivnikove figure sa table
//izvodi se u opsegu tajmera 0-100
void lift_opponent_figure(int position) {
    animation_ongoing = 1;
    if (animation_parameter <= 100) {
        glPushMatrix();
        glTranslatef(0, animation_parameter / 100.0, 0);
        draw_opponent_figure(position, 0.2, 40,1 - animation_parameter / 100.0);
        glPopMatrix();
        if (safeguard == 0) { 
            glutTimerFunc(100, on_timer, 1);
            safeguard = 1;
        }
    }
    else {
        safeguard = 0;
    }
}

//animacija skoka igraceve figure sa pozicije na poziciju (koristi se u trecoj fazi igre)
void jump_player(int position1, int position2) {
    lift_player_figure(position1);
    put_player_figure(position2);
}

//animacija skoka protivnikove figure sa pozicije na poziciju (koristi se u trecoj fazi igre)
void jump_opponent(int position1, int position2) {
    lift_opponent_figure(position1);
    put_opponent_figure(position2);
}

void move_player(int position1, int position2) {
    float coordinates1[3];
    float coordinates2[3];
    get_coordinates_of_position(position1, coordinates1);
    get_coordinates_of_position(position2, coordinates2);

    animation_ongoing = 1;
    if (animation_parameter <= 100) {
        glPushMatrix();
        glTranslatef((animation_parameter / 100.0) * (coordinates2[0] - coordinates1[0]), 0,
         (animation_parameter / 100.0) * (coordinates2[2] - coordinates1[2]));
        draw_player_figure(position1, 0.2, 40, 1);
        glPopMatrix();
        if (safeguard == 0) { 
            glutTimerFunc(100, on_timer, 1);
            safeguard = 1;
        }
    }
    else {
        safeguard = 0;
    }
}

void move_opponent(int position1, int position2) {
    float coordinates1[3];
    float coordinates2[3];
    get_coordinates_of_position(position1, coordinates1);
    get_coordinates_of_position(position2, coordinates2);

    animation_ongoing = 1;
    if (animation_parameter <= 100) {
        glPushMatrix();
        glTranslatef((1 - animation_parameter / 100.0) * (coordinates1[0] - coordinates2[0]), 0,
         (1 - animation_parameter / 100.0) * (coordinates1[2] - coordinates2[2]));
        draw_opponent_figure(position2, 0.2, 40, 0.5 + animation_parameter / 200);
        glPopMatrix();
        if (safeguard == 0) { 
            glutTimerFunc(100, on_timer, 1);
            safeguard = 1;
        }
    }
    else {
        safeguard = 0;
    }
}

//animacija nosenja igraceve figure kada protivnik zatvori micu
void abduct_player(int position) {
    float coordinates[3];
    get_coordinates_of_position(position, coordinates);

    animation_ongoing_abduct = 1;
    if (animation_parameter_abduct <= 100) {
        glPushMatrix();
        glTranslatef(0, (1 - animation_parameter_abduct / 100.0) * (3 - coordinates[2]), 0);
        draw_opponent_ufo(position, 0);
        glPopMatrix();
        if (safeguard_abduct == 0) { 
            glutTimerFunc(100, on_timer, 2);
            safeguard_abduct = 1;
        }
    }
    else if (animation_parameter_abduct > 100 && animation_parameter_abduct <= 200) {
        draw_opponent_ufo(position, 0);
        glPushMatrix();
        double clip_plane[] = {0, 1, 0, -1.74 + 1.74 * (animation_parameter_abduct - 100) / 100};
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);
        set_material(3, 0.2);
        draw_cone(position);
        glDisable(GL_CLIP_PLANE0);
        glPopMatrix();        
    }
    else if (animation_parameter_abduct > 200 && animation_parameter_abduct <= 300) {
        draw_opponent_ufo(position, animation_parameter_abduct - 200);
        set_material(3, 0.2);
        draw_cone(position);
        table[position] = 0; //izbrisemo tu figuru sa table u tom trenutku
        lift_player_figure(position);               
    }
    else if (animation_parameter_abduct > 300 && animation_parameter_abduct <= 400) {
        draw_opponent_ufo(position, 100);
        set_material(3, 0.2);
        glPushMatrix();
        double clip_plane[] = {0, 1, 0, -1.74 * ((animation_parameter_abduct - 300) / 100)};
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);
        set_material(3, 0.2);
        draw_cone(position);
        glDisable(GL_CLIP_PLANE0);
        glPopMatrix();         
    }
    else if (animation_parameter_abduct > 400 && animation_parameter_abduct <= 500) {
        glPushMatrix();
        glTranslatef(0, ((animation_parameter_abduct-400) / 100.0) * (3 - coordinates[2]), 0);
        draw_opponent_ufo(position, 100);
        glPopMatrix();        
    }
    else {
        safeguard_abduct = 0;
    }

}

void abduct_opponent(int position) {
    float coordinates[3];
    get_coordinates_of_position(position, coordinates);

    animation_ongoing_abduct = 1;
    if (animation_parameter_abduct <= 100) {
        glPushMatrix();
        glTranslatef(0, (1 - animation_parameter_abduct / 100.0) * (3 - coordinates[2]), 0);
        draw_player_ufo(position, 0);
        glPopMatrix();
        if (safeguard_abduct == 0) { 
            glutTimerFunc(100, on_timer, 2);
            safeguard_abduct = 1;
        }
    }
    else if (animation_parameter_abduct > 100 && animation_parameter_abduct <= 200) {
        draw_player_ufo(position, 0);
        glPushMatrix();
        double clip_plane[] = {0, 1, 0, -1.74 + 1.74 * (animation_parameter_abduct - 100) / 100};
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);
        set_material(2, 0.2);
        draw_cone(position);
        glDisable(GL_CLIP_PLANE0);
        glPopMatrix();        
    }
    else if (animation_parameter_abduct > 200 && animation_parameter_abduct <= 300) {
        draw_player_ufo(position, animation_parameter_abduct - 200);
        set_material(2, 0.2);
        draw_cone(position);
        lift_opponent_figure(position);               
    }
    else if (animation_parameter_abduct > 300 && animation_parameter_abduct <= 400) {
        draw_player_ufo(position, 100);
        set_material(2, 0.2);
        glPushMatrix();
        double clip_plane[] = {0, 1, 0, -1.74 * ((animation_parameter_abduct - 300) / 100)};
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);
        set_material(2, 0.2);
        draw_cone(position);
        glDisable(GL_CLIP_PLANE0);
        glPopMatrix();         
    }
    else if (animation_parameter_abduct > 400 && animation_parameter_abduct <= 500) {
        glPushMatrix();
        glTranslatef(0, ((animation_parameter_abduct-400) / 100.0) * (3 - coordinates[2]), 0);
        draw_player_ufo(position, 100);
        glPopMatrix();        
    }
    else {
        safeguard_abduct = 0;
    }
}

void on_timer(int id) {
    if (id == 1) {
        animation_parameter += 5;    

        if (animation_ongoing && animation_parameter <= 100) {
            glutPostRedisplay();
            glutTimerFunc(100, on_timer, 1);
        }
        else {
            animation_parameter = 0;
            animation_ongoing = 0;
        }  
    }
    else if (id == 2) {
        animation_parameter_abduct += 5;    

        if (animation_ongoing_abduct && animation_parameter_abduct <= 500) {
            glutPostRedisplay();
            glutTimerFunc(100, on_timer, 2);
        }
        else {
            animation_parameter_abduct = 0;
            animation_ongoing_abduct = 0;
        }  
    }  
    else
        return;    
}

void on_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && get_mouse) {
        x_mouse = x;
        y_mouse = y;
        get_mouse = 0;
        mouse_set = 1;
    }
}