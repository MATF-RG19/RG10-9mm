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

int x_mouse_1 = 0;
int y_mouse_1 = 0;
int get_mouse_1 = 0;
int mouse_set_1 = 0;

int x_mouse_2 = 0;
int y_mouse_2 = 0;
int get_mouse_2 = 0;
int mouse_set_2 = 0;

int animation_first_part = 1;

int legal_move_indicator = 101;
int legal_position_to_select_safeguard = 1;
int legal_move_indicator_safeguard = 1;
int position_to_take_safeguard = 1;

int move_will_close_a_mill = 0;
int move_will_close_a_mill_safeguard = 1;

int end_indicator = 0;

int game_phase_for_player = 2;
int game_phase_for_engine = 2;
int numberOfPieces[2] = {0, 0};


void this_is_where_the_magic_happens() {
    if(end_indicator == 0)
        gluLookAt(1 + 1 * x_parameter, 6 - 4 * yz_parameter, 4.1 - 7 * yz_parameter, 
                  0 + 2 * x_parameter, 0 + 0.5 * yz_parameter, 0 + 3 * yz_parameter, 0, 1, 0);

    //provera da li je doslo do zavrsetka partije

    /*
    * igrac je izgubio ukoliko je prosla prva faza igre a on ili je ostao sa 2 figure ili
    * su mu sve figure blokirane a nema ih samo tri (ukoliko ima tri onda mozes da skoci na bilo
    * koje polje na tabli),
    * ili ukoliko u toku prve faze igre njegov protivnik postavljajuci poslednju figuru blokira
    * sve igraceve (ili igrac postavi svoju poslednju figuru tako da nece imati legalnih poteza u narednom potezu)
    */
    if (engine_finished && !animation_ongoing && !animation_ongoing_abduct && animation_first_part) {
        int result[4] = {0, 0, 0, 0};
        brojBlokiranih(table, result);
        if (game_phase != 1 && (result[3] == 2 || (result[3] == result[1] && result[3] != 3))) 
            end_indicator = 1;
        else if (game_phase != 1 && (result[2] == 2 || (result[2] == result[0] && result[2] != 3))) 
            end_indicator = -1;
        else if (result[3] == 9 && result[3] == result[1])
            end_indicator = 1;
        else if (result[2] == 9 && result[2] == result[0]) 
            end_indicator = -1;

        //promena dubine stabla pretrage po potrebi
        if (move_count >= 18 && ((result[2] + result[3]) > 14)) {
            tree_depth = depth + 1;
        }
        else if (move_count < 8) {
            tree_depth = depth - 1;
        }
	else if (result[2] == 3 || result[3] == 3) {
            tree_depth = depth - 2;
        }
        else {
            tree_depth = depth;
        }
    }
    /*
    PROBLEMI:
    - lift u okviru abduct uopste ne radi nakon prve faze
    - kada ostanu 4 figure krene jump umesto move iako ne mozes da skocis nigde osim na susednu
    - kad ostanu tri figure ne mozes ni da ih selektujes. stoji you need to be more accurate
    */

    //provera po kojim pravirima igre igraju u sledecem potezu
    if (engine_finished && !animation_ongoing && !animation_ongoing_abduct && animation_first_part) {
        brojFigura(table, numberOfPieces);
        if (numberOfPieces[0] == 3 && game_phase == 2)
            game_phase_for_engine = 3;
        else
            game_phase_for_engine = 2;
        
        
        if (numberOfPieces[1] == 3 && game_phase == 2)
            game_phase_for_player = 3;
        else
            game_phase_for_player = 2;
        
    }

    if (move_count == 18)
        game_phase = 2;    

    if (engine_finished && end_indicator == 0) {
        draw_background(0.3);
        animate_table(table); 
    }

    if (end_indicator != 0) {
        end_game(end_indicator);
    }
    else if (game_phase == 1) {
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
                    if (move_count < 10)
                        output("Engine is thinking... (This may take a while)");
                    else
                        output("Engine is thinking...");                 
                }
                if (engine_finished) {
                    if (move[0] == -1) {
                        end_indicator = -1;
                    }
                    else {
                        //provera da li i nosimo neku figuru
                        if (move[2] != -1) {
                            //bitno je da output bude pre animacije kako bi se animirala i pozadina ujedno
                            output("Engine has closed a mill and will take your piece.");
                            
                            if ((animation_parameter_abduct == 0) && (animation_parameter <= 105) && animation_first_part) {
                                if (animation_parameter == 105)
                                    animation_first_part = 0;

                                put_opponent_figure(move[0], 0);
                            }
                            else {
                                if (animation_parameter_abduct > 500) {
                                    //vracamo safeguard na 1 da bi engine mogao da se pokrene u sledecem potezu
                                    //a nema opasnosti da cemo upasti opet u pogresan deo toka igre jer kada je 
                                    //animation_parameter_abduct > 500 abduct_player ce prebaciti na sledeci potez
                                    thread_safeguard = 1;
                                    animation_first_part = 1;
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
    //kraj prve faze igre
    else if (next_to_move == -1) {
        //na potezu je igrac

        if (game_phase_for_player == 2) {
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
                else if (table[position] != -1 && legal_position_to_select_safeguard) {
                    output("You have to select your piece. Click again.");
                    get_mouse = 1;
                }
                else if (slobodnaMestaOkoPozicije(table, position) == 0 && legal_position_to_select_safeguard) {
                    output("There is no legal moves with this piece because all neighbouring spots are taken. Select another piece.");
                    get_mouse = 1;
                }
                else {
                    legal_position_to_select_safeguard = 0;
                    //igrac je selektovao figuru sa kojom ima legalnih poteza i sada je pomera
                    if (!mouse_set_1) {
                        get_mouse_1 = 1;
                        output("Select an empty neighbouring spot.");
                    }
                    else {
                        int position1 = get_position_from_coordinates(x_mouse_1, y_mouse_1);
                        if (position1 == 100) {
                            output("You need to be more accurate. Click again.");
                            get_mouse_1 = 1;
                        }
                        else if (table[position1] == -1 && legal_move_indicator_safeguard) {
                            output("By the touch-move rule you have to play with previously selected piece. Select an empty neighbouring spot.");
                            get_mouse_1 = 1;
                        }
                        else {
                            int legal_moves[4] = {-1, -1, -1, -1};
                            int legal_move_tmp_indicator = 0;
                            moguciPoteziDrugaFaza(table, position, legal_moves);
                            for (int i = 0; i < 4; i++) {
                                if (position1 == legal_moves[i]) {
                                    legal_move_tmp_indicator = 1;
                                }
                            }
                            if (!legal_move_tmp_indicator && legal_move_indicator_safeguard) {
                                output("That is not an empty neighbouring spot. Click again.");
                                get_mouse_1 = 1;
                            }
                            else {
                                legal_move_indicator_safeguard = 0;
                                //selecktovano je legalno polje na koje moze da se pomeri figura
                                if (move_will_close_a_mill_safeguard) {
                                    int tmp_move[3] = {position, position1, -1};
                                    move_will_close_a_mill = zatvorenaMicaDrugaITrecaFaza(table, next_to_move, tmp_move);
                                    move_will_close_a_mill_safeguard = 0;
                                }
                                if(move_will_close_a_mill) {
                                    //igrac je tim pomeranjem figure zatvorio micu
                                    if (animation_first_part) {
                                        move_player(position, position1, 0);
                                        if (animation_parameter > 100) {
                                            animation_first_part = 0;
                                        }
                                    }
                                    else {
                                        if (!mouse_set_2) {
                                            output("You have closed a mill. Select opponents piece you want to take.");
                                            get_mouse_2 = 1;
                                            get_mouse = 0;
                                            get_mouse_1 = 0;
                                        }
                                        else {
                                            int position_to_take = get_position_from_coordinates(x_mouse_2, y_mouse_2);
                                            if (position_to_take == 100) {
                                                output("You need to be more accurate. Click again.");
                                                get_mouse_2 = 1;
                                                get_mouse_1 = 0;
                                                get_mouse = 0;
                                            }
                                            else if (table[position_to_take] != 1 && position_to_take_safeguard) {
                                                output("You need to select opponents piece");
                                                get_mouse_2 = 1;
                                                get_mouse = 0;
                                                get_mouse_1 = 0;
                                            }
                                            else if (legalnoNosenje(table, next_to_move, position_to_take) && position_to_take_safeguard) {
                                                output("Piece you want to take can't be in a mill, unless all opponent pieces are.");
                                                get_mouse_2 = 1;
                                                get_mouse = 0;
                                                get_mouse_1 = 0;
                                            }
                                            else {
                                                //igrac je odabrao protivnikovu figuru koja je legalna za nosenje
                                                //postavljamo safeguard na nula da bi da kraja animacije direktno upadali ovde
                                                //i izbegli ranije navedene bugove
                                                position_to_take_safeguard = 0;

                                                if (animation_parameter_abduct > 500) {
                                                    animation_first_part = 1;
                                                    position_to_take_safeguard = 1;
                                                    legal_move_indicator_safeguard = 1;
                                                    legal_position_to_select_safeguard = 1;

                                                    mouse_set = 0;
                                                    mouse_set_1 = 0;
                                                    mouse_set_2 = 0;
                                                    x_mouse = 0;
                                                    y_mouse = 0;
                                                    x_mouse_1 = 0;
                                                    y_mouse_1 = 0;
                                                    x_mouse_2 = 0;
                                                    y_mouse_2 = 0;

                                                    move_will_close_a_mill_safeguard = 1;
                                                }
                                                abduct_opponent(position_to_take);
                                            }
                                        }
                                    }
                                }
                                else {
                                    //to pomeranje figure ne zatvara micu
                                    if (animation_parameter > 100) {
                                        legal_move_indicator_safeguard = 1;
                                        legal_position_to_select_safeguard = 1;

                                        mouse_set = 0;
                                        mouse_set_1 = 0;

                                        x_mouse = 0;
                                        y_mouse = 0;
                                        x_mouse_1 = 0;
                                        y_mouse_1 = 0;

                                        move_will_close_a_mill_safeguard = 1;
                                    }
                                    move_player(position, position1, 1);
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (game_phase_for_player == 3) {
            //igrac igra po pravilima trece faze igre (moze figurom da skoci na bilo koju slobodnu poziciju)
            if (!mouse_set) {
                get_mouse = 1;
                output("You can jump with any piece to any empty spot on the board. Select a piece.");
            }
            else {
                int position = get_position_from_coordinates(x_mouse, y_mouse);
                if (position == 100) {
                    output("You need to be more accurate. Click again.");
                }
                else if (table[position] != -1 && legal_position_to_select_safeguard) {
                    output("You have to select your piece. Click again.");
                    get_mouse = 1;
                }
                else {
                    //igrac je selektovao svoju figuru
                    legal_position_to_select_safeguard = 0;
                    if (!mouse_set_1) {
                        get_mouse_1 = 1;
                        output("Select an empty spot on the board.");
                    }
                    else {
                        int position1 = get_position_from_coordinates(x_mouse_1, y_mouse_1);
                        if (position1 == 100) {
                            output("You need to be more accurate. Click again.");
                            get_mouse_1 = 1;
                        }
                        else if (table[position1] == -1 && legal_move_indicator_safeguard) {
                            output("By the touch-move rule you have to play with previously selected piece. Select an empty spot on the board.");
                            get_mouse_1 = 1;
                        }
                        else if (table[position1] != 0 && legal_move_indicator_safeguard) {
                            output("That is not an empty spot. Click again.");
                            get_mouse_1 = 1;
                        }
                        else {
                            //igrac je izabrao legalno polje na koje moze da skoci
                            legal_move_indicator_safeguard = 0;
                            if (move_will_close_a_mill_safeguard) {
                                int tmp_move[3] = {position, position1, 0};
                                move_will_close_a_mill = zatvorenaMicaDrugaITrecaFaza(table, next_to_move, tmp_move);
                                move_will_close_a_mill_safeguard = 0;
                            }
                            if (move_will_close_a_mill) {
                                //tim skokom igrac zatvara micu
                                if (animation_first_part) {
                                    jump_player(position, position1, 0);
                                    if (animation_parameter > 100) {
                                        animation_first_part = 0;
                                    }
                                }
                                else {
                                    if (!mouse_set_2) {
                                        output("You have closed a mill. Select opponents piece you want to take.");
                                        get_mouse_2 = 1;
                                        get_mouse = 0;
                                        get_mouse_1 = 0;
                                    }
                                    else {
                                        int position_to_take = get_position_from_coordinates(x_mouse_2, y_mouse_2);
                                        if (position_to_take == 100) {
                                            output("You need to be more accurate. Click again.");
                                            get_mouse_2 = 1;
                                            get_mouse_1 = 0;
                                            get_mouse = 0;
                                        }
                                        else if (table[position_to_take] != 1 && position_to_take_safeguard) {
                                            output("You need to select opponents piece");
                                            get_mouse_2 = 1;
                                            get_mouse = 0;
                                            get_mouse_1 = 0;
                                        }
                                        else if (legalnoNosenje(table, next_to_move, position_to_take) && position_to_take_safeguard) {
                                            output("Piece you want to take can't be in a mill, unless all opponent pieces are.");
                                            get_mouse_2 = 1;
                                            get_mouse = 0;
                                            get_mouse_1 = 0;
                                        }
                                        else {
                                            //odabrana figura je legalna za nosenje
                                            position_to_take_safeguard = 0;
                                            if (animation_parameter_abduct > 500) {
                                                animation_first_part = 1;
                                                position_to_take_safeguard = 1;
                                                legal_move_indicator_safeguard = 1;
                                                legal_position_to_select_safeguard = 1;

                                                mouse_set = 0;
                                                mouse_set_1 = 0;
                                                mouse_set_2 = 0;
                                                x_mouse = 0;
                                                y_mouse = 0;
                                                x_mouse_1 = 0;
                                                y_mouse_1 = 0;
                                                x_mouse_2 = 0;
                                                y_mouse_2 = 0;

                                                move_will_close_a_mill_safeguard = 1;
                                            }
                                            abduct_opponent(position_to_take);
                                        }
                                    }
                                }
                            }
                            else {
                                //taj skok ne zatvara micu
                                if (animation_parameter > 100) {
                                    legal_move_indicator_safeguard = 1;
                                    legal_position_to_select_safeguard = 1;

                                    mouse_set = 0;
         	                        mouse_set_1 = 0;

                                    x_mouse = 0;
                                    y_mouse = 0;
                                    x_mouse_1 = 0;
                                    y_mouse_1 = 0;

                                    move_will_close_a_mill_safeguard = 1;
                                    }
                                    jump_player(position, position1, 1);
                            }
                        }
                    }
                }
            }
        }
    }
    else if (next_to_move == 1) {
        //na potezu je engine

        if (game_phase_for_engine == 2) {
            //engine igra po pravilima druge faze igre
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
                    end_indicator = -1;
                }
                else {
                    //provera da li i nosimo neku figuru
                    if (move[2] != -1) {
                        //bitno je da output bude pre animacije kako bi se animirala i pozadina ujedno
                        output("Engine has closed a mill and will take your piece.");
                            
                        if ((animation_parameter_abduct == 0) && (animation_parameter <= 105) && animation_first_part) {
                            if (animation_parameter == 105)
                                animation_first_part = 0;
                            move_opponent(move[0], move[1], 0);                            
                        }
                        else {
                            if (animation_parameter_abduct > 500) {
                                //vracamo safeguard na 1 da bi engine mogao da se pokrene u sledecem potezu
                                //a nema opasnosti da cemo upasti opet u pogresan deo toka igre jer kada je 
                                //animation_parameter_abduct > 500 abduct_player ce prebaciti na sledeci potez
                                thread_safeguard = 1;
                                animation_first_part = 1;
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
        else if (game_phase_for_engine == 3) {
            //engine igra po pravilima trece faze igre
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
                    end_indicator = -1;
                }
                else {
                    //provera da li i nosimo neku figuru
                    if (move[2] != -1) {
                        //bitno je da output bude pre animacije kako bi se animirala i pozadina ujedno
                        output("Engine has closed a mill and will take your piece.");
                            
                        if ((animation_parameter_abduct == 0) && (animation_parameter <= 105) && animation_first_part) {
                            if (animation_parameter == 105)
                                animation_first_part = 0;
                            jump_opponent(move[0], move[1], 0);                            
                        }
                        else {
                            if (animation_parameter_abduct > 500) {
                                //vracamo safeguard na 1 da bi engine mogao da se pokrene u sledecem potezu
                                //a nema opasnosti da cemo upasti opet u pogresan deo toka igre jer kada je 
                                //animation_parameter_abduct > 500 abduct_player ce prebaciti na sledeci potez
                                thread_safeguard = 1;
                                animation_first_part = 1;
                            }
                            abduct_player(move[2]);                                
                        }                                                         
                    }
                    else {
                        output("Engine is jumping.");
                        if (animation_parameter > 100) {
                            thread_safeguard = 1;
                        }
                        jump_opponent(move[0], move[1], 1);  
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
    else if ((x <= 875 && x >= 840) && (y <= 270 && y >= 245)) 
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
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && get_mouse && !get_mouse_2 && !get_mouse_1) {
        x_mouse = x;
        y_mouse = y;
        get_mouse = 0;
        mouse_set = 1;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && get_mouse_2 && !get_mouse && !get_mouse_1) {
        x_mouse_2 = x;
        y_mouse_2 = y;
        get_mouse_2 = 0;
        mouse_set_2 = 1;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && get_mouse_1 && !get_mouse && !get_mouse_2) {
        x_mouse_1 = x;
        y_mouse_1 = y;
        get_mouse_1 = 0;
        mouse_set_1 = 1;
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
