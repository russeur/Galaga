#include <vector>
#include "Position2D.hpp"
#include <fstream>
#include "opengl_buffers.hpp"
#include "Window.hpp"
#include <cmath>
#include "RectManagerClass.hpp"
#include "path_objects.hpp"

struct Path_Point_Vert{
	GLfloat x, y;
	constexpr static GLfloat z = 0.0f;
	//0 for beginning, 1 for middle, 2 for end
	int point_location;
};

struct Bug_Spawner_Dev{
	
	int target_path;
	
	int max_enemy_count;
	float spawn_interval;
	
	BugType bug_type;
	
	Position2D offset;
};


class Path_Developer : public Path{
	constexpr static Position3D selected_color = {0.0, 0.0, 1.0};
	
	Render_Rect_Manager<Path_Point_Vert> point_renderer;
	constexpr static int point_size = 6;
	
	
	
	
	public:
	
	
	
	
	float zoom = 1.0;
	
	Shader curve_shader;
	VAO curve_VAO1;
	VBO curve_VBO1;
	EBO curve_EBO1;
	
	Position3D color;
	
	Window* window;
	
	//std::vector<Position2D> path_points;
	
	std::vector<Position2D> line_points;
	
	
	
	
	//Position2D find_vel(Position2D pos, Position2D vel, int& destination_point);
	
	void add_point(Position2D);
	void insert_point(Position2D, int index);
	
	void remove_point(int index);
	void remove_point(Position2D);
	
	void draw_curve(Position2D offset, bool is_selected);
	
	void update_curve();
	
	float distance_from(Position2D pos);
	
	Path_Developer(float smoothness, float speed, Window* window);
	
	
	
	
	
};




class Path_Developer_Manager{
	private:
	Window* window;
	int selected_spawner = -1;
	
	public:
	constexpr static int unique_color_count = 7;
	constexpr static Position3D unique_colors[unique_color_count] = {
			{0.0, 0.0, 0.0},
			{1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 1.0},
			{0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
			{1.0, 1.0, 1.0}
	};
	constexpr static float path_dist_thres = 30.0; 
	
	float zoom = 1.0;
	
	std::vector<Path_Developer*> paths;
	std::vector<Bug_Spawner_Dev*> bug_spawners;
	
	
	
	void add_point_to_spawner(int target_spawner, Position2D pos); // adds point to end of path
	void insert_point_to_spawner(int target_spawner, Position2D pos, int index);
	void remove_point_to_spawner(int target_spawner, int index);
	void remove_point_to_spawner(int target_spawner, Position2D pos);
	
	void add_point_to_path(int target_path, Position2D pos);
	//void move_entire_path(int target_path, Position2D delta);
	
	Position3D get_unique_color();
	
	void add_path(Position3D color, float smoothness, float speed);
	void add_path(Position3D color, float smoothness, float speed, Position2D* points, int num_points);
	
	void add_spawner(int target_spawner, int max_enemy_count, float spawn_interval, BugType bug_type, Position2D offset);
	
	void destroy_spawner(int target_spawner);
	
	void destroy_all_paths();
	void destroy_all_spawners();
	
	void draw_all_paths();
	
	void select_new_spawner(Position2D pos);
	int get_selected_spawner();
	void update_selected_spawner(int new_selection);
	
	void modify_zoom(float delta);
	void reset_zoom();
	
	
	Path_Developer_Manager(Window* window);
	
	~Path_Developer_Manager();
	
};
