#define PI 3.141592653589
#define EPSILONE 0.01

extern unsigned int texture;

void init_lights();
void set_material(int id, float alpha);
void draw_circle(float centerX, float centerY, float centerZ, float radius, int slices);
void draw_table(float alpha);
void get_coordinates_of_position(int i, float* coordinates);
void draw_player_figure(int position, float radius, int slices, float alpha);
void draw_opponent_figure(int position, float radius, int slices, float alpha);
void draw_cone(int position);
void draw_player_ufo(int position, float angle);
void draw_opponent_ufo(int position, float angle);
void draw_ufo_stabilisators();
void initialize_texture(void);
void draw_background(float lineWidth);
