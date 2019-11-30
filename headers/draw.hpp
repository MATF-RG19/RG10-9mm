#define PI 3.141592653589
#define EPSILONE 0.01

void init_lights();
void set_material(int id, float alpha);
void draw_circle(float centerX, float centerY, float centerZ, float radius, int slices);
void draw_table(float alpha);
void get_coordinates_of_position(int i, float* coordinates);
void draw_player_figure(int position, float radius, int slices, float alpha);
void draw_opponent_figure(int position, float radius, int slices, float alpha);