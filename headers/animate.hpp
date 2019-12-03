extern float animation_parameter;
extern int animation_ongoing;

//sprecava da timer sve vise i vise poziva sam sebe
extern int safeguard; 


extern float animation_parameter_abduct;
extern int animation_ongoing_abduct;
extern int safeguard_abduct; 



void animate_table(int table[24]);
void put_player_figure(int position);
void put_opponent_figure(int position);
void lift_player_figure(int position);
void lift_opponent_figure(int position);
void jump_player(int position1, int position2);
void jump_opponent(int position1, int position2);
void move_player(int position1, int position2);
void move_opponent(int position1, int position2);
void abduct_player(int position);
void abduct_opponent(int position);
void on_timer(int id);