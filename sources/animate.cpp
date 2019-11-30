#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../headers/draw.hpp"
#include "../headers/animate.hpp"


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

    move_opponent(9, 21);
    move_player(10, 9);
    
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

void on_timer(int id) {
    if (id != 1) 
        return;

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