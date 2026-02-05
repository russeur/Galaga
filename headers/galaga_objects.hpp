#ifndef GALAGA_OBJECTS_HPP
#define GALAGA_OBJECTS_HPP
#include "game_objects.hpp"


enum class BugType{
	COMMON,
	SHOOTER
};




struct Coord2D{
	int x, y;
};


class Path;
class Bug_Spawner;
class Player_Ship;
class Bug_Container;
class Bullet;
class Bug;


class Galaga_Root : public Node2D{
	private:
	
	bool is_stage_editor;
	
	RectangleID time_bar_id;
	RectangleID mult_bar_id;
	Number_Texture mult_text;
	
	int current_stage = 0;
	bool game_lost = false;
	bool stage_done = true;
	int score = 0;
	Number_Texture score_text; //current score
	Number_Texture stage_text; //current stage
	
	Texture_Manager* tex_man;
	Bar_Manager* bar_man;
	Window* window;
	
	//multiplier stuff
	static constexpr int drain_mult_time = 120; // amount of frames inside of bar that counts frames and decreases mult bar.
	static constexpr int number_of_mults = 4;
	static constexpr int mult_kill_thresholds[number_of_mults] = {0, 4, 8, 18};
	static constexpr int mult_multipliers[number_of_mults] = {1, 2, 3, 5};
	int drain_mult_time_left = 0;
	int number_of_current_kills = 0;
	int current_multiplier = 0;
	
	bool collide_bullet_and_bug(Bullet*, Bug*);
	
	
	public:
	Node2D* bullet_container;
	Bug_Container* bug_container;
	Player_Ship* player_ship;
	Node2D* score_container;
	
	template <typename T, typename... Args> T* add_bug_bullet(Position2D pos, const char* name, Position2D velocity, Args... constructor_args);
	template <typename T, typename... Args> T* add_player_bullet(Position2D pos, const char* name, Position2D velocity, Args... constructor_args);
	template <typename T, typename... Args> T* add_bug(Position2D pos, const char* name, Args... constructor_args);
	
	bool is_stage_done();
	
	void next_stage();
	
	void reset_game();
	
	bool is_game_lost();
	int get_score();
	
	void proccess();
	
	Galaga_Root(Texture_Manager* texture_manager, Window* window, Bar_Manager* bar_man, bool is_stage_editor); // if you are using Galaga_Root for stage editor is_stage_editor = true and bar_man will equal nullptr
};

class Player_Ship : public Node2D{
	private:
	GLfloat speed;
	int lives_left;
	Window* window;
	Texture_Manager* tex_man;
	Galaga_Root* galaga_root;
	
	bool attack_button_pressed = false;
	
	
	public:
	int number_of_bullets = 0;
	
	void reset_game();
	
	void collided_with_weapon();
	
	virtual void proccess();
	
	Player_Ship(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, GLfloat speed, int starting_lives);
};

class Bug : public Node2D{
	protected:
	Coord2D form_coord;
	bool is_pathing = true;
	Bug_Container* bug_container;
	
	int point_value = 12;
	
	Texture_Manager* tex_man;
	Window* window;
	Player_Ship* player;
	
	Path* target_path = nullptr;
	Position2D path_offset;
	int next_path_point = 1;
	
	Position2D velocity = {0.0, 0.0};
	
	
	bool is_diving = false;
	float get_dive_speed();
	
	
	public:
	
	int get_point_value();
	
	bool dive();
	
	virtual void proccess();
	virtual int collided_with_bullet();//returns point value
	void set_path(Path* target_path);
	
	Bug(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Player_Ship* player);
	
};
class Bug_Shooter : public Bug{
	protected:
	Galaga_Root* galaga_root;
	int fire_chance = 1; // every frame and out of 100
	
	public:
	
	virtual void proccess();
	
	Bug_Shooter(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Player_Ship* player, Galaga_Root* galaga_root);
	
};
class Bug_Container : public Node2D{
	
	Position2D current_formation_spacing;
	Position2D current_formation_pos;
	int formation_width_count;
	
	
	
	static constexpr Position2D formation_spacing = {55, 60};
	static constexpr float formation_speed = 10;  
	static constexpr float formation_width = 700;
	static constexpr Position2D formation_tl_pos = {0, 100};
	std::vector<Bug*> bug_formation;
	
	
	std::vector<Bug_Spawner*> bug_spawners;
	std::vector<Path*> paths;
	
	Texture_Manager* tex_man;
	Window* window;
	Player_Ship* player_ship;
	Galaga_Root* galaga_root;
	
	int frame_count = 0;
	
	
	
	public:
	bool stage_is_complete = false;
	void reset_game();
	
	virtual void proccess();
	
	void add_path(float smoothness, float speed, Position2D* path_points, int number_of_points);
	void add_spawner(int target_path, int max_bugs, float spawn_interval, BugType bug_type, Position2D spawn_pos);
	
	Coord2D enlist_bug_to_formation(Bug* target_bug);
	void unenlist_bug_from_formation(Coord2D bug_coord);
	Position2D get_formation_velocity(Coord2D bug_coord);
	Position2D get_formation_position(Coord2D coord);
	
	
	void start_stage();
	void end_stage();
	void kill_all_bugs();
	
	template <typename T, typename... Args> T* add_bug(Position2D pos, const char* name, Args... constructor_args);
	template <typename... Args> Bug* add_bug_type(Position2D pos, const char* name, BugType bug_type, Args... constructor_args);
	
	Bug_Container(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Player_Ship* player_ship);
	
	~Bug_Container();
	
};

class Bullet : public Node2D{
	
	protected:
	
	Position2D velocity;
	Window* window;
	
	public:
	
	virtual void destroy();
	
	virtual void collided_with_bug();
	virtual void proccess();
	
	Bullet(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Position2D velocity);
	
	
};
class Player_Bullet : public Bullet{
	Player_Ship* player;
	
	
	public:
	
	virtual void destroy();
	
	
	Player_Bullet(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Position2D velocity, Player_Ship* player);
};
class Bug_Bullet : public Bullet{
	
	public:
	
	Bug_Bullet(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Position2D velocity);
	
	
};

class Score_Label : public Node2D{
	protected:
	static constexpr int lifespan = 20;
	static constexpr Position2D velocity = {0, -8};
	static constexpr int font_size = 50;
	
	Number_Texture label;
	
	int age = 0;
	
	
	public:
	
	virtual void proccess();
	
	
	Score_Label(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Position2D bottom_right_pos, int score);
	
	
};

void load_stage(int stage, Bug_Container* bug_container);  






//TEMPLATE DEFINITIONS


template <typename T, typename... Args> T* Galaga_Root::add_bug_bullet(Position2D pos, const char* name, Position2D velocity, Args... constructor_args){
	static_assert(std::is_base_of<Bug_Bullet, T>::value, "T must be derived from Bullet!");
	return get_child<Node2D>("Bullet_Container")->add_child<T>(pos, name, tex_man, window, velocity, constructor_args...);
}
template <typename T, typename... Args> T* Galaga_Root::add_player_bullet(Position2D pos, const char* name, Position2D velocity, Args... constructor_args){
	static_assert(std::is_base_of<Player_Bullet, T>::value, "T must be derived from Bullet!");
	return get_child<Node2D>("Bullet_Container")->add_child<T>(pos, name, tex_man, window, velocity, player_ship, constructor_args...);
}


template <typename T, typename... Args> T* Galaga_Root::add_bug(Position2D pos, const char* name, Args... constructor_args){
	
	static_assert(std::is_base_of<Bug, T>::value, "T must be derived from Bullet!");
	return get_child<Node2D>("Bug_Container")->add_child<T>(pos, name, tex_man, window, player_ship, constructor_args...);
	
}

template <typename T, typename... Args> T* Bug_Container::add_bug(Position2D pos, const char* name, Args... constructor_args){
	
	static_assert(std::is_base_of<Bug, T>::value, "T must be derived from Bullet!");
	
	return add_child<T>(pos, name, tex_man, window, player_ship, constructor_args...);
	
}
template <typename... Args> Bug* Bug_Container::add_bug_type(Position2D pos, const char* name, BugType bug_type, Args... constructor_args){
	std::cout<<static_cast<int>(bug_type)<<", adding bug\n";
	switch (bug_type){
		case BugType::COMMON:
			return add_bug<Bug>(pos, name, constructor_args...);
			
			break;
		case BugType::SHOOTER:
			return add_bug<Bug_Shooter>(pos, name, galaga_root, constructor_args...);
			break;
		default:
			return nullptr;
			break;
	}
	
	
}

#endif
