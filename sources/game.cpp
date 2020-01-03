#include "../headers/draw.hpp"
#include "../headers/animate.hpp"
#include "../headers/game.hpp"
#include "../headers/alphabeta.hpp"
#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <future>


int move[3] = {-1, -1, -1};
int thread_safeguard = 1;
int table1[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



void this_is_where_the_magic_happens() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1, 6, 4.1, 0, 0, 0, 0, 1, 0);   

    if (engine_finished) {
        draw_background(0.3);
        animate_table(table); 
        output((char*)"Hello world"); 
    }
    
    if (move_count == 0) {
         //inicijalizacija prvog poteza
        if (first_move == 0) {
            put_opponent_figure(4);
        }
        else if (first_move == 1) {
            put_opponent_figure(10);
        }
        else if (first_move == 2) {
            put_opponent_figure(19);
        }
        else if (first_move == 3) {
            put_opponent_figure(13);
        }
    }   

    if (next_to_move == -1) {
        //igrac je na potezu
    }
    else {
        if (thread_safeguard) {
            for (int i = 0; i < 24; i++)
                table1[i] = table[i];
            std::thread t1(alphabeta, table, move, tree_depth, move_count+1, cooefs);
            t1.detach();
            thread_safeguard = 0;
            engine_finished = 0;
        }
        if (!engine_finished) {
            animate_table(table1); 
        }
        if (engine_finished)
            put_opponent_figure(move[0]);
    }


}

int get_position_from_coordinates(int x, int y) {
    if ((x <= 325 && x >= 285) && (y <= 545 && y >= 515))
        return 0;
    else if ((x <= 625 && x >= 580) && (y <= 625 && y >= 590)) 
        return 1;
    else if ((x <= 975 && x >= 915) && (y <= 725 && y >= 660)) 
        return 2;
    else if ((x <= 405 && x >= 365) && (y <= 505 && y >= 470)) 
        return 3;
    else if ((x <= 645 && x >= 590) && (y <= 575 && y >= 530)) 
        return 4;
    else if ((x <= 915 && x >= 860) && (y <= 640 && y >= 580)) 
        return 5;
    else if ((x <= 495 && x >= 440) && (y <= 470 && y >= 420)) 
        return 6;
    else if ((x <= 670 && x >= 610) && (y <= 510 && y >= 460)) 
        return 7;
    else if ((x <= 840 && x >= 805) && (y <= 545 && y >= 515)) 
        return 8;
    else if ((x <= 460 && x >= 420) && (y <= 310 && y >= 275)) 
        return 9;
    else if ((x <= 505 && x >= 465) && (y <= 320 && y >= 285)) 
        return 10;
    else if ((x <= 560 && x >= 510) && (y <= 335 && y >= 300)) 
        return 11;
    else if ((x <= 865 && x >= 820) && (y <= 395 && y >= 355)) 
        return 12;
    else if ((x <= 915 && x >= 880) && (y <= 405 && y >= 370)) 
        return 13;
    else if ((x <= 965 && x >= 930) && (y <= 420 && y >= 370)) 
        return 14;
    else if ((x <= 605 && x >= 560) && (y <= 230 && y >= 195)) 
        return 15;
    else if ((x <= 740 && x >= 695) && (y <= 245 && y >= 220)) 
        return 16;
    else if ((x <= 875 && x >= 240) && (y <= 270 && y >= 245)) 
        return 17;
    else if ((x <= 570 && x >= 535) && (y <= 190 && y >= 155)) 
        return 18;
    else if ((x <= 740 && x >= 710) && (y <= 215 && y >= 190)) 
        return 19;
    else if ((x <= 930 && x >= 885) && (y <= 250 && y >= 215)) 
        return 20;
    else if ((x <= 545 && x >= 515) && (y <= 155 && y >= 135)) 
        return 21;
    else if ((x <= 750 && x >= 715) && (y <= 185 && y >= 165)) 
        return 22;
    else if ((x <= 965 && x >= 935) && (y <= 225 && y >= 200)) 
        return 23;
    else 
        return 100;
}

void on_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && get_mouse) {
        x_mouse = x;
        y_mouse = y;
        get_mouse = 0;
        mouse_set = 1;
    }
}

void output(char const *string) {
  set_material(5, 1);
  glRasterPos2f(-4.8, 3);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]); 
  }
}