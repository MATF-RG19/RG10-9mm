#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "../headers/draw.hpp"
#include "../headers/animate.hpp"


int get_mouse = 0;
int mouse_set = 0;
int x_mouse;
int y_mouse;

int go_to_the_next_move_guard = 0;
int safeguard_lift = 0;

//za koliko cemo povecati debljinu linije dok racunar razmislja i da li cemo je povecati ili samnjiti
float background = 0;
int background_indicator = 1;

float reverse_camera_parameter = 0;


void animate_table(int table[24]) {

    init_lights();    
    draw_table(1 - reverse_camera_parameter);    

    //postavljamo figure
    for (int i = 0; i < 24; i++) {
        if (table[i] == 1)
            draw_opponent_figure(i, 0.2, 40, 1 - reverse_camera_parameter);
        else if (table[i] == -1)
            draw_player_figure(i, 0.2, 40, 1 - reverse_camera_parameter);
    }       
    
}

//animacija postavljanja igraceve figure na tablu
//izvodi se u opsegu tajmera 0-100
void put_player_figure(int position, int go_to_next_move) {
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
        //postavljam figuru na tablu i pre nego sto se animacija zavrsi kako se ne bi desavalo
        //da na sekund nestane figura pre nego sto se ponovo iscrta
        if (animation_parameter == 100)
            table[position] = -1;
    }
    else {
        if (go_to_next_move && (go_to_the_next_move_guard == 0)) {
            move_count++;
            next_to_move = -next_to_move;
            move[0] = -1;
            move[1] = -1;
            move[2] = -1;
            go_to_the_next_move_guard++;
        }
    }       
}

//animacija postavljanja protivnikove figure na tablu
//izvodi se u opsegu tajmera 0-100
void put_opponent_figure(int position, int go_to_next_move) {
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
        //postavljam figuru na tablu i pre nego sto se animacija zavrsi kako se ne bi desavalo
        //da na sekund nestane figura pre nego sto se ponovo iscrta
        if (animation_parameter == 100)
            table[position] = 1;
    }
    else {      
        if (go_to_next_move && (go_to_the_next_move_guard == 0)) {
            move_count++;
            next_to_move = -next_to_move;
            move[0] = -1;
            move[1] = -1;
            move[2] = -1;
            go_to_the_next_move_guard++;
        }
    }
}

//animacija podizanja igraceve figure sa table
//izvodi se u opsegu tajmera 0-100
//ne koristiti samu f-ju jer ne vracam animacione parametre u njoj
//koristiti iskljucivo u okviru jump jer ce u put_figure biti resetovani
void lift_player_figure(int position) {
    table[position] = 0;
    animation_ongoing = 1;
    if (animation_parameter <= 90) {
        glPushMatrix();
        glTranslatef(0, animation_parameter / 90.0, 0);
        draw_player_figure(position, 0.2, 40,1 - animation_parameter / 90.0);
        glPopMatrix();
        //poseban safeguard kada se poziva u abduct
        //zajednicki safeguard kada se pozove u jump da se timer ne pozove dva puta
        //i u lift i u put
        if (safeguard_lift == 0 && safeguard == 0) { 
            glutTimerFunc(100, on_timer, 1);
            safeguard_lift = 1;
            safeguard = 1;
        }
    }
    else {
        safeguard_lift = 0;
    }
}

//animacija podizanja protivnikove figure sa table
//izvodi se u opsegu tajmera 0-100
//ne koristiti samu f-ju jer ne vracam animacione parametre u njoj
//koristiti iskljucivo u okviru jump jer ce u put_figure biti resetovani
void lift_opponent_figure(int position) {
    table[position] = 0;
    animation_ongoing = 1;
    /*
    * do 90 umesto 100 jer se iz abduct poziva svega 95 puta
    * dakle 90 traje animacija, a na 95 reinicijalizujemo safeguard_lift za sledeci potez
    * u jump ce brzina biti blago neuskladjena ali posto figura nestaje kako se podize nece biti uocljivo
    */
    if (animation_parameter <= 90) {
        glPushMatrix();
        glTranslatef(0, animation_parameter / 90.0, 0);
        draw_opponent_figure(position, 0.2, 40,1 - animation_parameter / 90.0);
        glPopMatrix();
        if (safeguard_lift == 0 && safeguard == 0) { 
            glutTimerFunc(100, on_timer, 1);
            safeguard_lift = 1;
            safeguard = 1;
        }
    }
    else {
        safeguard_lift = 0;
    }
}

//animacija skoka igraceve figure sa pozicije na poziciju (koristi se u trecoj fazi igre)
void jump_player(int position1, int position2, int go_to_next_move) {
    lift_player_figure(position1);
    put_player_figure(position2, go_to_next_move);
}

//animacija skoka protivnikove figure sa pozicije na poziciju (koristi se u trecoj fazi igre)
void jump_opponent(int position1, int position2, int go_to_next_move) {
    lift_opponent_figure(position1);
    put_opponent_figure(position2, go_to_next_move);
}

void move_player(int position1, int position2, int go_to_next_move) {
    table[position1] = 0;

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

        if (animation_parameter >= 94)
            table[position2] == -1;
    }
    else {
       if (go_to_next_move && (go_to_the_next_move_guard == 0)) {
            move_count++;
            next_to_move = -next_to_move;
            move[0] = -1;
            move[1] = -1;
            move[2] = -1;
            go_to_the_next_move_guard++;
        }
    }
}

void move_opponent(int position1, int position2, int go_to_next_move) {
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

        if (animation_parameter >= 94)
            table[position2] == 1;
    }
    else {
        if (go_to_next_move && (go_to_the_next_move_guard == 0)) {
            move_count++;
            next_to_move = -next_to_move;
            move[0] = -1;
            move[1] = -1;
            move[2] = -1;
            go_to_the_next_move_guard++;
        }
    }
}

//animacija nosenja igraceve figure kada protivnik zatvori micu
void abduct_player(int position) {
    float coordinates[3];
    get_coordinates_of_position(position, coordinates);

    animation_ongoing_abduct = 1;
    if (animation_parameter_abduct <= 100) {
        if (animation_parameter_abduct < 60 && (position == 0 ||  position == 1 || position == 2 ||  position == 3 || position == 4 ||  position == 5 ||  position == 6 || position == 7 ||  position == 8)) {
            //ako se nosi figura dole desno kamera ide gore levo
            //ako se nosi figura dole levo kamera ide gore desno
            yz_parameter = animation_parameter_abduct / 60;
            if (position == 0 ||  position == 3 || position == 6)
                //na levoj strani table je x-osa negativna, na desnoj je pozitivna
                x_parameter = -yz_parameter;
            else if (position == 2 || position == 5 || position == 8) {
                x_parameter = yz_parameter;
                yz_parameter = yz_parameter * 1.1;
            }
            else
                yz_parameter = yz_parameter * 1.1;
            
        }
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
        if (position == 0 ||  position == 1 || position == 2 ||  position == 3 || position == 4 ||  position == 5 ||  position == 6 || position == 7 ||  position == 8) {
            if (animation_parameter_abduct > 400 && animation_parameter_abduct < 470)
                reverse_camera_parameter = (animation_parameter_abduct - 400) / 70;
            else if (animation_parameter_abduct > 470 && animation_parameter_abduct <= 500)
                reverse_camera_parameter = (1 - (animation_parameter_abduct - 470) / 30);
            if (animation_parameter_abduct == 470) {
                x_parameter = 0;
                yz_parameter = 0;
            }
        }
        glPushMatrix();
        glTranslatef(0, ((animation_parameter_abduct-400) / 100.0) * (3 - coordinates[2]), 0);
        draw_opponent_ufo(position, 100);
        glPopMatrix();        
    }
    else {
        reverse_camera_parameter = 0;
        safeguard_abduct = 0;
        animation_parameter_abduct = 0;
        animation_ongoing_abduct = 0;
        move_count++;
        next_to_move = -next_to_move;
        move[0] = -1;
        move[1] = -1;
        move[2] = -1;
    }

}

void abduct_opponent(int position) {
    float coordinates[3];
    get_coordinates_of_position(position, coordinates);

    animation_ongoing_abduct = 1;
    if (animation_parameter_abduct <= 100) {
        if (animation_parameter_abduct < 60 && (position == 0 ||  position == 1 || position == 2 ||  position == 3 || position == 4 ||  position == 5 ||  position == 6 || position == 7 ||  position == 8)) {
            //ako se nosi figura dole desno kamera ide gore levo
            //ako se nosi figura dole levo kamera ide gore desno
            yz_parameter = animation_parameter_abduct / 60;
            if (position == 0 ||  position == 3 || position == 6)
                //na levoj strani table je x-osa negativna, na desnoj je pozitivna
                x_parameter = -yz_parameter;
            else if (position == 2 || position == 5 || position == 8) {
                x_parameter = yz_parameter;
                yz_parameter = yz_parameter * 1.1;
            }
            else
                yz_parameter = yz_parameter * 1.1;
            
        }
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
        table[position] = 0;
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
        if (position == 0 ||  position == 1 || position == 2 ||  position == 3 || position == 4 ||  position == 5 ||  position == 6 || position == 7 ||  position == 8) {
            if (animation_parameter_abduct > 400 && animation_parameter_abduct < 470)
                reverse_camera_parameter = (animation_parameter_abduct - 400) / 70;
            else if (animation_parameter_abduct > 470 && animation_parameter_abduct <= 500)
                reverse_camera_parameter = (1 - (animation_parameter_abduct - 470) / 30);
            if (animation_parameter_abduct == 470) {
                x_parameter = 0;
                yz_parameter = 0;
            }
        }
        glPushMatrix();
        glTranslatef(0, ((animation_parameter_abduct-400) / 100.0) * (3 - coordinates[2]), 0);
        draw_player_ufo(position, 100);
        glPopMatrix();        
    }
    else {
        reverse_camera_parameter = 0;
        safeguard_abduct = 0;
        animation_parameter_abduct = 0;
        animation_ongoing_abduct = 0;
        move_count++;
        next_to_move = -next_to_move;
        move[0] = -1;
        move[1] = -1;
        move[2] = -1;
    }
}

//animacija pulsirajuce pozadine
void animate_background() {
    set_material(3, 1);
    draw_background(0.3 + background);
    if (background_indicator) {
        background += 0.05;
        if (background >= 2.2)
            background_indicator = 0;
    }
    else {
        background -= 0.05;
        if (background <= 0)
            background_indicator = 1;
    }
}

void on_timer(int id) {
    if (id == 1) {
        animation_parameter += 5; 
        
        if (animation_ongoing && animation_parameter <= 100) {
            glutPostRedisplay();
            glutTimerFunc(100, on_timer, 1);
        }
        else if (animation_ongoing && animation_parameter > 100 && animation_parameter < 115) {
            //potrebna su nam jos dva otkucaja tajmera zbog sinhronizacije animacija
            //medjutim, kada engine odigra potez za brze od 100 milisekundi dolazi do greske u sinhronizaciji
            //zbog toga sinhronizacione otkucaje dajemo na 1 milisekund
            glutPostRedisplay();
            glutTimerFunc(1, on_timer, 1);

        }
        else {
            safeguard = 0;
            animation_parameter = 0;
            animation_ongoing = 0;
            go_to_the_next_move_guard = 0;
        } 
    }
    else if (id == 2) {    

        if (animation_ongoing_abduct) {
            //inkrementacija obavezno unutar if-a (ako je van onda se jos jednom
            //poveca pa za sledeci put ostane 5 umesto 0), a iznad PostRedisplaya 
            //kako bi odma krenulo sa animacijom, a ne cekalo jos 1 otkucaj
            animation_parameter_abduct += 5;

            glutPostRedisplay();
            glutTimerFunc(100, on_timer, 2);            
        }
        

    }  
    else
        return;    
}