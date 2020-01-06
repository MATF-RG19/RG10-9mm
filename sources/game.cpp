#include "../headers/draw.hpp"
#include "../headers/animate.hpp"
#include "../headers/game.hpp"
#include "../headers/alphabeta.hpp"
#include "../headers/pravila.hpp"
#include "../headers/heuristike.hpp"
#include <GL/glut.h>
#include <string.h>
#include <iostream>
#include <future>


int move[3] = {-1, -1, -1};
int table1[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int thread_safeguard = 1;

float x_parameter = 0;
float yz_parameter = 0;

int x_mouse_2 = 0;
int y_mouse_2 = 0;
int get_mouse_2 = 0;
int mouse_set_2 = 0;

int animation_first_part = 1;

int legal_move_indicator = 101;
int legal_move_indicator_safeguard = 1;
int position_to_take_safeguard = 1;


void this_is_where_the_magic_happens() {
    gluLookAt(1 + 1 * x_parameter, 6 - 4 * yz_parameter, 4.1 - 7 * yz_parameter, 
              0 + 2 * x_parameter, 0 + 0.5 * yz_parameter, 0 + 3 * yz_parameter, 0, 1, 0);


    if (move_count == 18)
        game_phase = 2;
    

    if (engine_finished) {
        draw_background(0.3);
        animate_table(table); 
    }    

    if (game_phase == 1) {
        if (move_count == 0) {
            //prvi potez engina
            output("Engine is putting a piece.");
            //inicijalizacija prvog poteza
            if (first_move == 0) {
                put_opponent_figure(4, 1);
            }
            else if (first_move == 1) {
                put_opponent_figure(10, 1);
            }
            else if (first_move == 2) {
                put_opponent_figure(19, 1);
            }
            else if (first_move == 3) {
                put_opponent_figure(13, 1);
            }
        }
        else {
            if (next_to_move == -1) {
                //igrac je na potezu            
                if (!mouse_set) {
                    get_mouse = 1;
                    output("Put your piece on an empty spot.");
                }
                else {
                    int position = get_position_from_coordinates(x_mouse, y_mouse);
                    if (position == 100) {
                        output("You need to be more accurate. Click again.");
                        get_mouse = 1;
                    }
                    else {
                        //korisnik je kliknuo na neko polje, proveravamo da li je legalan potez
                        if (legal_move_indicator_safeguard) {
                            legal_move_indicator = legalanPotezPrvaFaza(table, next_to_move, position);
                            //kada korisnik klikne na neko polje i ono je legalno krece se sa animacijom i to polje se ne
                            //proverava ponovo kako bi se izbegli bagovi 
                            //(npr. u jednom trenutku, pre kraja poteza, animacija promeni sadrzaj table i upise tu novu vrednosti.
                            //sada u potezu iako je to bio legalan potez kaze da vise nije jer se tu nalazai neka figura
                            //a to je zapravo ta koju u tom trenutku i postavljamo)
                            legal_move_indicator_safeguard = 0;
                        }
                        if (legal_move_indicator == 101) {
                            //ako to nije legalan potez, ponovo uzmi od korisnika input misem i 
                            //dozvoli evaluiranje legalnog poteza
                            legal_move_indicator_safeguard = 1;

                            output("Select an empty spot.");
                            get_mouse = 1;
                        }
                        else if (legal_move_indicator == 1) {
                            //igrac je zatvorio micu, prvo se postavlja figura pa se nosi protivnikova
                            if (animation_first_part) {
                                put_player_figure(position, 0);
                                if (animation_parameter > 100) {
                                    //kad se zavrsi animacija postavljanja figure vise necemo ulaziti ovde
                                    animation_first_part = 0;
                                }
                            }
                            else {
                                if (!mouse_set_2) {
                                    output("You have closed a mill. Select opponents piece you want to take.");
                                    get_mouse_2 = 1;
                                    get_mouse = 0;
                                }
                                else {
                                    int position_to_take = get_position_from_coordinates(x_mouse_2, y_mouse_2);
                                    if (position_to_take == 100) {
                                        output("You need to be more accurate. Click again.");
                                        get_mouse_2 = 1;
                                        get_mouse = 0;
                                    }
                                    else {
                                        if(table[position_to_take] != 1 && position_to_take_safeguard) {
                                            output("You need to select opponents piece.");
                                            get_mouse_2 = 1;
                                            get_mouse = 0;
                                        }
                                        else {
                                            if (legalnoNosenje(table, next_to_move, position_to_take) && position_to_take_safeguard) {
                                                //kada legalnoNosenje vrati True to nije lagalno nosenje
                                                //postoji logika iza ovoga :D
                                                output("Piece you want to take can't be in a mill, unless all opponent pieces are.");
                                                get_mouse_2 = 1;
                                                get_mouse = 0;
                                            } 
                                            else {
                                                //postavljamo safeguard na nula da bi se izbegao bug da kada animacija
                                                //odbah na pocetku izbrise tu figuru sa table u narednom prolasku kroz f-ju toka igre
                                                //dobijemo poruku da to nosenje nije legalno jer tu nema protivnikove figure
                                                position_to_take_safeguard = 0;
                                                //nosi se protivnikova figura
                                                if (animation_parameter_abduct > 500) {
                                                    //moze da se postavi na 1 a da se ne udje ponovo gore jer
                                                    //kada je animation_parameter_abduct > 500 a pozove se abduct_opponent
                                                    //on ce da ikrementira potez i promeni igraca koji je na potezu jer je zavrsio sa animacijom
                                                    animation_first_part = 1;
                                                    position_to_take_safeguard = 1;
                                                    legal_move_indicator_safeguard = 1;
                                                    //vracaju se i indikatori za mis kako bi se u sledecem potezu ponovo uzimao unos
                                                    mouse_set = 0;
                                                    mouse_set_2 = 0;
                                                    x_mouse = 0;
                                                    y_mouse = 0;
                                                    x_mouse_2 = 0;
                                                    y_mouse_2 = 0;
                                                } 
                                                abduct_opponent(position_to_take);                                            
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            if (animation_parameter > 100) {
                                //smemo da reinicijalizujemo parametre jer kada je animation_parameter > 100
                                //put_player_fugure ce preci na sledeci potez i nece se ponovo uci u ovaj deo f-je
                                //toka igre do narednog igracevog poteza
                                legal_move_indicator_safeguard = 1;
                                mouse_set = 0;
                                x_mouse = 0;
                                y_mouse = 0;
                            }
                            //igrac postavlja figuru koja ne formira micu
                            //na kraju te animacije menjamo igraca na potezu pa se vise ne ulazi tu
                            //tajmer ce sam u jos jednom narednom pozivu da se reinicijalizuje
                            put_player_figure(position, 1);
                        }
                    }
                }              
            }
            else {
                //engine je na potezu
                if (thread_safeguard) {
                    for (int i = 0; i < 24; i++)
                        table1[i] = table[i];
                    std::thread t1(alphabeta, table, move, tree_depth, move_count, cooefs);
                    t1.detach();
                    thread_safeguard = 0;
                    engine_finished = 0;
                }
                if (!engine_finished) {
                    animate_background();
                    animate_table(table1);
                    if (move_count <= 8)
                        output("Engine is thinking... (This may take a while)");
                    else
                        output("Engine is thinking...");                 
                }
                if (engine_finished) {
                    if (move[0] == -1) {
                        //NAPRAVITI ANIMACIJU!!!!!!!!
                        std::cout << "Pobedio je igrac" << std::endl;
                    }
                    else {
                        //provera da li i nosimo neku figuru
                        if (move[2] != -1) {
                            //bitno je da output bude pre animacije kako bi se animirala i pozadina ujedno
                            output("Engine has closed a mill and will take your piece.");
                            
                            if ((animation_parameter_abduct == 0) && (animation_parameter <= 105)) {
                                put_opponent_figure(move[0], 0);
                            }
                            else {
                                if (animation_parameter_abduct > 500) {
                                    //vracamo safeguard na 1 da bi engine mogao da se pokrene u sledecem potezu
                                    //a nema opasnosti da cemo upasti opet u pogresan deo toka igre jer kada je 
                                    //animation_parameter_abduct > 500 abduct_player ce prebaciti na sledeci potez
                                    thread_safeguard = 1;
                                }
                                abduct_player(move[2]);                                
                            }                                                         
                        }
                        else {
                            output("Engine is putting a piece.");
                            if (animation_parameter > 100) {
                                thread_safeguard = 1;
                            }
                            put_opponent_figure(move[0], 1);  
                        }                 
                    }
                }
            }
        }
    }
    else if (game_phase == 2) {
        //zavrseno je postavljane figura i pocinje druga faza igre
        if (next_to_move == -1) {
            //igrac je na potezu
            if(!mouse_set) {
                get_mouse = 1;
                output("Select a piece you want to move.");
            }
            else {
                int position = get_position_from_coordinates(x_mouse, y_mouse);
                if (position == 100) {
                    output("You need to be more accurate. Click again.");
                    get_mouse = 1;
                }
                else if (table[position] != -1) {
                    output("You have to select your piece. Click again.");
                    get_mouse = 1;
                }
                else if (true) {
                    output("There is no legal moves with this piece because all neighbouring spots are taken. Select another piece.");
                    get_mouse = 1;
                }
                else {
                    //igrac je selektovao figuru sa kojom ima legalnih poteza i sada je pomera

                }
            }
        }
        else {
            //engine je na potezu
            if (thread_safeguard) {
                for (int i = 0; i < 24; i++)
                    table1[i] = table[i];
                std::thread t1(alphabeta, table, move, tree_depth, move_count, cooefs);
                t1.detach();
                thread_safeguard = 0;
                engine_finished = 0;
            }
            if (!engine_finished) {
                animate_background();
                animate_table(table1);
                output("Engine is thinking...");                 
            }
            if (engine_finished) {
                if (move[0] == -1) {
                    //NAPRAVITI ANIMACIJU!!!!!!!!
                    std::cout << "Pobedio je igrac" << std::endl;
                }
                else {
                    //provera da li i nosimo neku figuru
                    if (move[2] != -1) {
                        //bitno je da output bude pre animacije kako bi se animirala i pozadina ujedno
                        output("Engine has closed a mill and will take your piece.");
                            
                        if ((animation_parameter_abduct == 0) && (animation_parameter <= 105)) {
                            move_opponent(move[0], move[1], 0);
                        }
                        else {
                            if (animation_parameter_abduct > 500) {
                                //vracamo safeguard na 1 da bi engine mogao da se pokrene u sledecem potezu
                                //a nema opasnosti da cemo upasti opet u pogresan deo toka igre jer kada je 
                                //animation_parameter_abduct > 500 abduct_player ce prebaciti na sledeci potez
                                thread_safeguard = 1;
                            }
                            abduct_player(move[2]);                                
                        }                                                         
                    }
                    else {
                        output("Engine is moving a piece.");
                        if (animation_parameter > 100) {
                            thread_safeguard = 1;
                        }
                        move_opponent(move[0], move[1], 1);  
                    }                 
                }
            }
        }
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
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && get_mouse && !get_mouse_2) {
        x_mouse = x;
        y_mouse = y;
        get_mouse = 0;
        mouse_set = 1;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && get_mouse_2 && !get_mouse) {
        x_mouse_2 = x;
        y_mouse_2 = y;
        get_mouse_2 = 0;
        mouse_set_2 = 1;
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