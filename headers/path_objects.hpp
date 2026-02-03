#ifndef PATH_OBJECTS_HPP
#define PATH_OBJECTS_HPP
#include "Position2D.hpp"
#include <vector>
#include <cmath>
#include <iostream>



enum class BugType;
class Bug;
class Bug_Container;

class Path{
	private:
	
	
	
	
	
	public:
	
	std::vector<Position2D> path_points;
	
	float smoothness, speed; 
	
	
	Position2D find_vel(Position2D pos, Position2D vel, int& destination_point, float speed = -1.1);
	
	Path(float smoothness, float speed);
};

class Bug_Spawner{
	Path* target_path;
	Bug_Container* bug_container;
	
	bool is_spawning = false;
	float time_since_last_spawn = 0.0;
	int current_enemies_spawned = 0;
	
	int max_enemy_count;
	float spawn_interval;
	BugType bug_type;
	Position2D offset;
	
	void spawn_bug();
	
	public:
	
	
	void set_max_enemy_count(int new_max_enemy_count);
	void set_spawn_interval(float new_spawn_interval);
	void set_bug_type(BugType bug_type);
	void set_spawn_location(Position2D pos);
	void set_path(Path* target_path);
	
	void start_spawning();
	
	bool next_frame(); // returns if this bug spawner is done
	
	Bug_Spawner(Path* target_path, Bug_Container* bug_container);
	
	
};

Position2D find_vel_to_point(Position2D pos, Position2D target_pos, float speed);

#endif