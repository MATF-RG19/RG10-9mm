extern float animation_parameter;
extern int animation_ongoing;

//sprecava da timer sve vise i vise poziva sam sebe
extern int safeguard; 


extern float animation_parameter_abduct;
extern int animation_ongoing_abduct;
extern int safeguard_abduct; 

extern int get_mouse;
extern int mouse_set;
extern int x_mouse;
extern int y_mouse;

extern int table[24];
extern int depth;
extern int tree_depth;
extern int next_to_move;
extern int move_count;
extern int game_phase;
extern int cooefs[19];
extern int first_move;

extern int move[3];

extern float x_parameter;
extern float yz_parameter;

extern int animataion_parameter_ending;
extern float animation_parameter_background_ending;


void animate_table(int table[24]);
void put_player_figure(int position, int go_to_next_move);
void put_opponent_figure(int position, int go_to_next_move);
void lift_player_figure(int position);
void lift_opponent_figure(int position);
void jump_player(int position1, int position2, int go_to_next_move);
void jump_opponent(int position1, int position2, int go_to_next_move);
void move_player(int position1, int position2, int go_to_next_move);
void move_opponent(int position1, int position2, int go_to_next_move);
void abduct_player(int position);
void abduct_opponent(int position);
void animate_background();
void end_game(int who_won);
void on_timer(int id);
void on_click(int button, int state, int x, int y);