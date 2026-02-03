#include "path_objects.hpp"
#include "galaga_objects.hpp"

Position2D Path::find_vel(Position2D pos, Position2D vel, int& destination_point, float speed ){
	
	
	
	if (speed == -1.1f){
		
		speed = this->speed;
		
		
		if (destination_point == path_points.size()){
			return Position2D{0, 0};
		}
		
	}
	
	Position2D delta_to_dest = Position2D{pos.x - path_points[destination_point].x, pos.y - path_points[destination_point].y};
	
	GLfloat dist_to_dest = sqrt((delta_to_dest.x * delta_to_dest.x) + (delta_to_dest.y * delta_to_dest.y));
	
	
	if (dist_to_dest <= speed * 1.5){
		
		destination_point++;
	}
	
	Position2D actual_vel = Position2D{(delta_to_dest.x / dist_to_dest) * speed, (delta_to_dest.y / dist_to_dest) * speed};
	if (vel.x == 0 && vel.y == 0){
		return actual_vel;
	}
	
	float current_influence = (dist_to_dest * smoothness) / speed;
	
	return Position2D{(vel.x * current_influence + actual_vel.x)/(current_influence+1), (vel.y*current_influence + actual_vel.y)/(current_influence+1)};
}

Path::Path(float smoothness, float speed){
	this->smoothness = smoothness;
	this->speed = speed;
}


void Bug_Spawner::set_max_enemy_count(int new_max_enemy_count){
	this->max_enemy_count = new_max_enemy_count;
}
void Bug_Spawner::set_spawn_interval(float new_spawn_interval){
	this->spawn_interval = new_spawn_interval;
}
void Bug_Spawner::set_path(Path* target_path){
	this->target_path = target_path;
}
void Bug_Spawner::set_bug_type(BugType bug_type){
	this->bug_type = bug_type;
}
void Bug_Spawner::set_spawn_location(Position2D pos){
	this->offset = pos;
}
void Bug_Spawner::start_spawning(){
	if (is_spawning) return;
	
	current_enemies_spawned = 0;
	time_since_last_spawn = 0;
	
	
	is_spawning = true;
	
	
}

void Bug_Spawner::spawn_bug(){
	if (!is_spawning) return;
	Bug* new_bug = bug_container->add_bug_type(offset, "bug2", bug_type);
	new_bug->set_path(target_path);
	time_since_last_spawn = 0.0;
	current_enemies_spawned++;
}
bool Bug_Spawner::next_frame(){
	if (!is_spawning) return false;
	if (time_since_last_spawn == spawn_interval){
		spawn_bug();
	}
	if (current_enemies_spawned >= max_enemy_count){
		is_spawning = false;
	}
	
	
	time_since_last_spawn++;
	return is_spawning;
}

Bug_Spawner::Bug_Spawner(Path* target_path, Bug_Container* bug_container){
	this->target_path = target_path;
	this->bug_container = bug_container;
}



Position2D find_vel_to_point(Position2D pos, Position2D target_pos, float speed){
	Position2D delta_to_dest = pos - target_pos;
	
	GLfloat dist_to_dest = sqrt((delta_to_dest.x * delta_to_dest.x) + (delta_to_dest.y * delta_to_dest.y));
	
	Position2D actual_vel = Position2D{(delta_to_dest.x / dist_to_dest) * speed, (delta_to_dest.y / dist_to_dest) * speed};
	
	if (dist_to_dest <= speed){
		
		return delta_to_dest;
	}
	
	return actual_vel;
	
}