#include "galaga_objects.hpp"
#include "path_objects.hpp"




bool Galaga_Root::collide_bullet_and_bug(Bullet* target_bullet, Bug* target_bug){
	if ((target_bullet->get_child<Collider>("bullet_collider")->layer == LAYER2 && target_bullet->get_child<Collider>("bullet_collider")->layer_mask == LAYER1) && // first check if the bullet even wants to collide with a bug
		(target_bullet->get_child<Collider>("bullet_collider")->is_colliding_with(target_bug->get_child<Collider>("bug_collider")))){ // next check if they are colliding physically / overlapping
		
		
		
		// TODO: make a seperate funtion for this
		if (!is_stage_editor){ 
		
			drain_mult_time_left = drain_mult_time;
			number_of_current_kills++;
			if (current_multiplier < number_of_mults-1 && number_of_current_kills >= mult_kill_thresholds[current_multiplier+1]){
				current_multiplier++;
				mult_text.update_number(mult_multipliers[current_multiplier]);
			}
		}
		
		
		
		
		//make a floating label to show how many points the bug is worth
		score_container->add_child<Score_Label>(target_bug->get_pos(), "score", tex_man, window, target_bug->get_pos(), target_bug->get_point_value() * mult_multipliers[current_multiplier]);
		//then kill bug and store its point value
		int point_reward = target_bug->collided_with_bullet();
		
		
		// add point value to score
		if (!is_stage_editor){ 
			score += point_reward * mult_multipliers[current_multiplier];
			score_text.update_number(score);
		}
		
		
		
		
		
		target_bullet->collided_with_bug(); // destroy bullet
		return true; // true = collided
	}else return false; // false = didn't collide
	
}




bool Galaga_Root::is_stage_done(){
	return stage_done;
}

void Galaga_Root::next_stage(){
	if (!stage_done) return;
	std::cout<<"s :"<<current_stage<<"\n";
	load_stage(++current_stage, bug_container);
	bug_container->start_stage();
	stage_done = false;
	stage_text.update_number(current_stage);
	
	
}

void Galaga_Root::reset_game(){
	current_stage = -1;
	stage_done = true;
	game_lost = false;
	score = 0;
	score_text.update_number(score);
	
	int child_count = bullet_container->get_child_count();
	for (int i = 0; i < child_count; i++){
		bullet_container->get_child<Bullet>(0)->destroy();
	}
	child_count = score_container->get_child_count();
	for (int i = 0; i < child_count; i++){
		score_container->get_child<Score_Label>(0)->destroy();
	}
	
	
	bug_container->reset_game();
	player_ship->reset_game();
	
	
}

bool Galaga_Root::is_game_lost(){
	return game_lost;
}
int Galaga_Root::get_score(){
	if (!is_stage_editor){ 
	
		return score;
	}else return 0;
}

void Galaga_Root::proccess(){
	if (game_lost) return;
	Node2D::proccess();
	
	if (bug_container->get_child_count() == 0 && bug_container->stage_is_complete) stage_done = true;
	
	if (bar_man != nullptr){ //bar_man is nullptr if it is being used by stage editor
		bar_man->set_fill_percent(time_bar_id, ((float)drain_mult_time_left/drain_mult_time));
		bar_man->set_fill_percent(mult_bar_id, (float)number_of_current_kills/(mult_kill_thresholds[number_of_mults-1]));
	
	
		if (number_of_current_kills > 0){
			
			drain_mult_time_left--;
			if (drain_mult_time_left <= 0){
				if (current_multiplier != 0){
					current_multiplier--;
					mult_text.update_number(mult_multipliers[current_multiplier]);
				}
				number_of_current_kills = mult_kill_thresholds[current_multiplier];
				drain_mult_time_left = drain_mult_time;
			}
		}
	}
	
	
	
	for (size_t bullet_i = 0; bullet_i < bullet_container->get_child_count(); bullet_i++){
		
		Bullet* current_bullet = bullet_container->get_child<Bullet>(bullet_i);
		
		if (current_bullet->get_child<Collider>("bullet_collider")->layer == LAYER2 && current_bullet->get_child<Collider>("bullet_collider")->layer_mask == LAYER1){ // bullet wants to hit enemies
			
				for (size_t bug_i = 0; bug_i < bug_container->get_child_count(); bug_i++){ // loop through bugs because the bullet want s to hit a bug
					if (collide_bullet_and_bug(current_bullet, bug_container->get_child<Bug>(bug_i))){ // return true if they collided and destroyed
					
						bug_i--; // need to decrease current bug index, because we just killed one bug
						bullet_i--; //Same reason, but for a bullet
						
						break; // no need to search for if this bullet is hitting multiple bugs
					}
				}
			
		}else if (current_bullet->get_child<Collider>("bullet_collider")->layer_mask == LAYER0){ // bullet is wanting to hit player
		
				if (current_bullet->get_child<Collider>("bullet_collider")->is_colliding_with(player_ship->get_child<Collider>("ship_collider"))){
					if (!is_stage_editor){ 
					
						current_bullet->collided_with_bug();
						
						player_ship->collided_with_weapon();
						game_lost = true;
					}
					break;
					
				}
			}
		
	}
	
	for (int bug_i = 0; bug_i < bug_container->get_child_count(); bug_i++){
		
		Bug* current_bug = bug_container->get_child<Bug>(bug_i);
		
		if (current_bug->get_child<Collider>("bug_collider")->layer == LAYER1 && current_bug->get_child<Collider>("bug_collider")->layer_mask == LAYER0){
			
			
			if (current_bug->get_child<Collider>("bug_collider")->is_colliding_with(player_ship->get_child<Collider>("ship_collider"))){
				if (!is_stage_editor){ 
					current_bug->collided_with_bullet();
					
					bug_i--;
				
				
					player_ship->collided_with_weapon();
					game_lost = true;
				}
				break;
			}
			
		}
		
		
	}
	
	
	
	
}

Galaga_Root::Galaga_Root(Texture_Manager* texture_manager, Window* window, Bar_Manager* bar_man, bool is_stage_editor) : Node2D(Position2D{0, 0}, "Galaga_Root", nullptr) , score_text(texture_manager, window, Position2D{1000, 150}, false), stage_text(texture_manager, window, Position2D{1855, 968}, false), mult_text(texture_manager, window, Position2D{1225, 150}, false){
	
	this->is_stage_editor = is_stage_editor; // removes many parts of game if true, like points, multipliers, and losing
	
	//set up multiply label
	mult_text.set_font_size(75);
	mult_text.update_number(1);
	
	//set up score label
	score_text.set_font_size(100);
	score_text.update_number(score);
	
	//set up stage label
	stage_text.set_font_size(50);
	stage_text.update_number(0);
	
	this->tex_man = texture_manager;
	this->window = window;
	this->bar_man = bar_man;
	if (is_stage_editor) this->bar_man == nullptr;
	
	if (!is_stage_editor){ // used in stage editor because it doesn't tally up points 
		//set up multiplier bars
		add_child<Sprite>(Position2D{100, 116}, "time_bar", Position2D{50, -50}, 7, tex_man, window);
		time_bar_id = bar_man->new_bar(window->screen_to_UV_pos(Position2D{100, 100}), window->screen_to_UV_vector(Position2D{50, -21}), 0.0, Position3D{0.1, .8, .8}, 1);
		
		add_child<Sprite>(Position2D{137, 100}, "kill_bar", Position2D{50, -50}, 6, tex_man, window);
		mult_bar_id = bar_man->new_bar(window->screen_to_UV_pos(Position2D{150, 100}), window->screen_to_UV_vector(Position2D{25, -50}), 0.0, Position3D{.9, .2, .1}, 0);
	}
	// add player, bullet container, score label containers(for when a bug dies)
	add_child<Player_Ship>(Position2D{window->width/2, window->height - 100}, "Player_Ship", tex_man, window, 12, 1);
	add_child<Node2D>(Position2D{0, 0}, "Bullet_Container");
	add_child<Node2D>(Position2D{0, 0}, "Score_container");
	add_child<Sprite>(Position2D{1150, 150}, "Times", Position2D{50, -50}, 5, tex_man, window);
	
	//set up player ship
	player_ship = get_child<Player_Ship>("Player_Ship");
	//add bug container
	add_child<Bug_Container>(Position2D{0, 0}, "Bug_Container", tex_man, window, player_ship);
	
	//set up containers
	bullet_container = get_child<Node2D>("Bullet_Container");
	bug_container = get_child<Bug_Container>("Bug_Container");
	score_container = get_child<Node2D>("Score_container");
	
}

void Player_Ship::reset_game(){
	number_of_bullets = 0;
}

void Player_Ship::collided_with_weapon(){
	std::cout<<"collide\n";
}

void Player_Ship::proccess(){
	Node2D::proccess();
	
	//move ship based on ui
	Position2D ship_vel = {0, 0};
	if (window->get_key_button_state(GLFW_KEY_LEFT) == GLFW_PRESS && get_pos().x > 0){
		ship_vel.x -=speed;
	}
	if (window->get_key_button_state(GLFW_KEY_RIGHT) == GLFW_PRESS && get_pos().x < window->width-50){
		ship_vel.x +=speed;
	}
	//uncomment if you want up and down player movement
	/*
	if (window->get_key_button_state(GLFW_KEY_DOWN) == GLFW_PRESS){
		ship_vel.y +=speed;
	}
	if (window->get_key_button_state(GLFW_KEY_UP) == GLFW_PRESS){
		ship_vel.y -=speed;
	}
	*/
	modify_pos(ship_vel);
	
	//fire bullet based on ui
	if (!attack_button_pressed && window->get_key_button_state(GLFW_KEY_SPACE) == GLFW_PRESS){ // only fire if is pressed and it wasn't pressed last frame(in other words was just pressed)
		attack_button_pressed = true;
		
		if (number_of_bullets < 2){
			galaga_root->add_player_bullet<Player_Bullet>(Position2D{get_pos().x + 25, get_pos().y - 45}, "bullet", Position2D{0, -30});
			number_of_bullets++; 
		}
	}
	
	if (window->get_key_button_state(GLFW_KEY_SPACE) == GLFW_RELEASE){ // mark space as unpressed
		attack_button_pressed = false;
	}
	
	
}
	
Player_Ship::Player_Ship(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, GLfloat speed, int starting_lives) : Node2D(pos, name, parent){
	
	
	galaga_root = dynamic_cast<Galaga_Root*>(parent);
	assert(galaga_root != nullptr);
	
	//add ship texture and collider
	add_child<Sprite>(Position2D{0, 0}, "ship_sprite", Position2D{50, -50}, 0, texture_manager, window);
	add_child<Collider>(Position2D{15, -15}, "ship_collider", Position2D{15, -15}, LAYER0, LAYERNONE);
	
	this->speed = speed;
	
	this->window = window;
	this->tex_man = texture_manager;
	
	lives_left = starting_lives;
}


float Bug::get_dive_speed(){
	return 15;
}

int Bug::get_point_value(){
	return point_value;
}

bool Bug::dive(){
	if (is_diving || is_pathing) return false;
	
	Path* dive_path = new Path(.5, get_dive_speed());
	dive_path->path_points.push_back({0, 0});
	if (rand()%10 < 5){
		dive_path->path_points.push_back({70, 50});
	}else{
		dive_path->path_points.push_back({-70, 50});
	}
	
	
	
	dive_path->path_points.push_back(Position2D{rand()%window->width, window->height+50} - get_pos());
	dive_path->path_points.push_back(Position2D{rand()%window->width, window->height+100} - get_pos());
	
	set_path(dive_path);
	is_diving = true;
	is_pathing = true;
	
	return true;
	
}

void Bug::proccess(){
	Node2D::proccess();
	
	
	if (target_path != nullptr && is_pathing == true){//if is following path, then get next velocity in order to follow it
		is_pathing = true;
		velocity = target_path->find_vel(pos - path_offset, velocity, next_path_point);
		if (velocity == Position2D{0, 0}){
			if (is_diving){
				velocity = {0, 0};
				set_pos(Position2D{bug_container->get_formation_position(form_coord).x, -50});
				is_diving = false;
				delete target_path;
				is_pathing = false;
			}
			target_path = nullptr;
			
		}
		
	}
	if(target_path == nullptr && is_pathing == true){
		form_coord = bug_container->enlist_bug_to_formation(this);
		
		is_pathing = false;
		
		
	}
	
	
	//move
	
	if (!is_pathing){
		velocity = bug_container->get_formation_velocity(form_coord);
		//if (rand() % 10000 < 2){
			//dive();
		//}
	}
	
	modify_pos(velocity * Position2D{-1, -1});
	
}
int Bug::collided_with_bullet(){ 
	if (!is_pathing){
		
		bug_container->unenlist_bug_from_formation(form_coord);
	}
	destroy();
	return point_value;
}
void Bug::set_path(Path* target_path){
	this->target_path = target_path;
	next_path_point = 1;
	path_offset = pos;
	is_pathing = true;
}


Bug::Bug(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Player_Ship* player) : Node2D(pos, name, parent){
	bug_container = dynamic_cast<Bug_Container*>(parent);
	assert(bug_container != nullptr);
	
	
	this->player = player;
	
	add_child<Sprite>(Position2D{0, 0}, "bug_sprite", Position2D{50, -50}, 1, texture_manager, window);
	add_child<Collider>(Position2D{0, 0}, "bug_collider", Position2D{50, -50}, LAYER1, LAYER0);
	
	this->window = window;
	this->tex_man = texture_manager;
	
	
}

void Bug_Shooter::proccess(){
	Bug::proccess();
	if (rand()%100 < fire_chance){
		galaga_root->add_bug_bullet<Bug_Bullet>(Position2D{get_pos().x + 25, get_pos().y+45}, "bullet", Position2D{0, 7});
	}
	
}

Bug_Shooter::Bug_Shooter(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Player_Ship* player, Galaga_Root* galaga_root) : Bug(pos, name, parent, texture_manager, window, player){
	
	get_child<Sprite>("bug_sprite")->set_texture(3);
	this->galaga_root = galaga_root;
	
	point_value = 20;
	
}

void Bug_Container::reset_game(){
	current_formation_pos = formation_tl_pos;
	current_formation_spacing = formation_spacing;
	frame_count = 0;
	int count = get_child_count();
	for (int i = 0; i<count; i++){
		get_child<Bug>(0)->destroy();
	}
	bug_formation.clear();
	end_stage();
}

void Bug_Container::proccess(){ 
	
	Node2D::proccess();
	
	frame_count++;
	
	
	
	//spawn bugs
	stage_is_complete = true;
	for (int i = 0; i < bug_spawners.size(); i++){
		
		stage_is_complete = (!bug_spawners[i]->next_frame()) && stage_is_complete;
		
	}
	
	
	
	
	//dive bugs
	if (rand()%400 < 2 && get_child_count() != 0){
		int diver_count = (rand()%7) + 1;
		int current_divers = 0;
		int random_bug = rand()%get_child_count();
		
		int checked = 0;
		std::cout<<"DIVE\n";
		while (current_divers < diver_count){
			if (checked >100) break;
			if (get_child<Bug>(random_bug)->dive()){
				current_divers++;
			}
			checked ++;
			random_bug = rand()%get_child_count();
		}
		
		
	}
	
	const int change_direction_interval = 500;
	//move formation
	if (frame_count % change_direction_interval){
		if ((frame_count%change_direction_interval) - (change_direction_interval/2) <= 0){
			current_formation_pos += Position2D{(window->width-formation_width) / change_direction_interval * 2, 0};
		}else{
			current_formation_pos -= Position2D{(window->width-formation_width) / change_direction_interval *2, 0};
		}
	}
	/*
	if (frame_count % 230){
		if ((frame_count%230) - (230/2) <= 0){
			current_formation_spacing += Position2D{formation_speed/50, formation_speed/50};
		}else{
			current_formation_spacing -= Position2D{formation_speed/50, formation_speed/50};
		}
	}
	*/
	
	
}
void Bug_Container::add_path(float smoothness, float speed, Position2D* path_points, int number_of_points){
	//make new path and add it to vector, then set attributes
	Path* new_path = new Path(smoothness, speed);
	paths.push_back(new_path);
	
	//add points to path
	for (int p = 0; p < number_of_points; p++){
		new_path->path_points.push_back(*(path_points + p));
	}
}
void Bug_Container::add_spawner(int target_path, int max_bugs, float spawn_interval, BugType bug_type, Position2D spawn_pos){
	
	//add new spawner to vector, then set attributes of it
	Bug_Spawner* new_spawner = new Bug_Spawner(paths[target_path], this);
	bug_spawners.push_back(new_spawner);
	new_spawner->set_max_enemy_count(max_bugs);
	new_spawner->set_spawn_interval(spawn_interval);
	new_spawner->set_bug_type(bug_type);
	new_spawner->set_spawn_location(spawn_pos);
}

Coord2D Bug_Container::enlist_bug_to_formation(Bug* target_bug){
	Coord2D bug_coord = {bug_formation.size()%formation_width_count, 0};
	bug_coord.y = (bug_formation.size()-bug_coord.x) / formation_width_count;
	//find bug_coord
	for (int i = 0; i < bug_formation.size(); i++){
		if (bug_formation[i] == nullptr){
			bug_coord = {i%formation_width_count, 0};
			bug_coord.y = (i-bug_coord.x) / formation_width_count;
			
			bug_formation[i] = target_bug;
			return bug_coord;
			break;
		}
	}
	
	
	
	
	
	bug_formation.push_back(target_bug);
	return bug_coord;
	
	
	
}
void Bug_Container::unenlist_bug_from_formation(Coord2D bug_coord){
	int bug_index = bug_coord.y * formation_width_count + bug_coord.x;
	bug_formation[bug_index] = nullptr;
	
}
Position2D Bug_Container::get_formation_velocity(Coord2D bug_coord){
	int bug_index = bug_coord.y * formation_width_count + bug_coord.x;
	Position2D target_point = get_formation_position(bug_coord);
	
	return find_vel_to_point(bug_formation[bug_index]->get_pos(), target_point, formation_speed);
}
Position2D Bug_Container::get_formation_position(Coord2D coord){
	return current_formation_pos + current_formation_spacing* Position2D{coord.x, coord.y};
}


void Bug_Container::start_stage(){
	for (int i = 0; i < bug_spawners.size(); i++){
		bug_spawners[i]->start_spawning();
	}
}
void Bug_Container::end_stage(){
	
	for (int i = 0; i < bug_spawners.size(); i++){
		delete bug_spawners[i];
		
	}
	bug_spawners.clear();
	
	for (int i = 0; i < paths.size(); i++){
		delete paths[i];
	}
	paths.clear();
	
	for (int i = 0; i< get_child_count(); i++){
		get_child<Bug>(i)->destroy();
	}
	bug_formation.clear();
	
	
}
void Bug_Container::kill_all_bugs(){
	std::cout<<"n: "<<get_child_count()<<"\n";
	for (int i = 0; i < get_child_count();){
		
		destroy_child(get_child<Bug>(0));
	}
	std::cout<<"na: "<<get_child_count()<<"\n";
}
Bug_Container::Bug_Container(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Player_Ship* player_ship) : Node2D(pos, name, parent){
	
	galaga_root = dynamic_cast<Galaga_Root*>(parent);
	assert(galaga_root != nullptr);
	
	this->tex_man = texture_manager;
	this->window = window;
	
	this->player_ship = player_ship;
	formation_width_count = formation_width / formation_spacing.x;
	
	current_formation_pos = formation_tl_pos;
	current_formation_spacing = formation_spacing;
}
Bug_Container::~Bug_Container(){
	end_stage();
}

void Bullet::destroy(){
	
	Node2D::destroy();
}

void Bullet::collided_with_bug(){
	destroy();
}
void Bullet::proccess(){
	Node2D::proccess();
	modify_pos(velocity);
	
	if (get_pos().x < 0 || get_pos().y < 0 || get_pos().x > window->width || get_pos().y > window->height){
		
		destroy();//make sure bullet is destroyed when getting out of screen
		
	}
	
}
	
Bullet::Bullet(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Position2D velocity) : Node2D(pos, name, parent){
	
	add_child<Sprite>(Position2D{-25, 0}, "bullet_sprite", Position2D{50, -50}, 2, texture_manager, window);
	add_child<Collider>(Position2D{-8, 0}, "bullet_collider", Position2D{12, -50}, LAYER2, LAYER1);
	
	
	
	this->velocity = velocity;
	
	this->window = window;
}

void Player_Bullet::destroy(){
	player->number_of_bullets--;
	Bullet::destroy();
}
Player_Bullet::Player_Bullet(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Position2D velocity, Player_Ship* player) : Bullet(pos, name, parent, texture_manager, window, velocity){
	this->player = player;
	
	get_child<Collider>("bullet_collider")->layer = LAYER2;
	get_child<Collider>("bullet_collider")->layer_mask = LAYER1;
}
Bug_Bullet::Bug_Bullet(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Position2D velocity) : Bullet(pos, name, parent, texture_manager, window, velocity){
	get_child<Collider>("bullet_collider")->layer = LAYER4;
	get_child<Collider>("bullet_collider")->layer_mask = LAYER0;
	
	get_child<Sprite>("bullet_sprite")->set_texture(4);
}

void Score_Label::proccess(){
	Node2D::proccess();
	
	
	age++;
	if (age >= lifespan){

		destroy();
		return;
	}
	
	
	
	modify_pos(velocity);
	label.modify_pos(velocity);

	
	
}
	
	
Score_Label::Score_Label(Position2D pos, const char* name, Node2D* parent, Texture_Manager* texture_manager, Window* window, Position2D bottom_right_pos, int score) : Node2D(pos, name, parent), label(texture_manager, window, bottom_right_pos, false){
	label.set_font_size(font_size);
	label.update_number(score);
	
	
	
	
	
}




void load_stage(int stage, Bug_Container* bug_container){
	bug_container->end_stage(); // clear all paths and spawners
	//open file/create file named as '93.bin' for stage 93
	std::string file_name = "../stages/";
	file_name += std::to_string(stage);
	file_name += ".bin";
	std::ifstream ifs(file_name, std::fstream::in | std::fstream::binary); 
	if (ifs.is_open()){
		int path_count;
		ifs.read(reinterpret_cast<char*>(&path_count), sizeof(path_count));
		//write all paths first
		
		for (int p = 0; p < path_count; p++){
			float smoothness, speed;
			size_t number_of_points;
			//write smoothness, speed, and number of points
			ifs.read(reinterpret_cast<char*>(&smoothness), sizeof(smoothness));
			ifs.read(reinterpret_cast<char*>(&speed), sizeof(speed));
			ifs.read(reinterpret_cast<char*>(&number_of_points), sizeof(size_t));
			
			
			Position2D* path_points = new Position2D[number_of_points];
			ifs.read(reinterpret_cast<char*>(path_points), sizeof(Position2D) * number_of_points);
			bug_container->add_path(smoothness, speed, path_points, number_of_points);
			
			
			
			
			
		}
		
		
		int spawner_count;
		
		ifs.read(reinterpret_cast<char*>(&spawner_count), sizeof(spawner_count));
		
		for (int s = 0; s < spawner_count; s++){
			
			int target_path, max_enemy_count;
			float spawn_interval;
			BugType bug_type;
			Position2D offset;
			
			ifs.read(reinterpret_cast<char*>(&target_path), sizeof(target_path));
			ifs.read(reinterpret_cast<char*>(&max_enemy_count), sizeof(max_enemy_count));
			ifs.read(reinterpret_cast<char*>(&spawn_interval), sizeof(spawn_interval));
			ifs.read(reinterpret_cast<char*>(&bug_type), sizeof(bug_type));
			ifs.read(reinterpret_cast<char*>(&offset), sizeof(offset));
			
			bug_container->add_spawner(target_path, max_enemy_count, spawn_interval, bug_type, offset);
			
			
		
			//ofs.write(reinterpret_cast<const char*>(&layer_count), sizeof(layer_count));
		}
		
		
	}else{
		std::cout<<"failed to open file name: "<<"../stages/"<<stage<<".bin"<<"\n";
	}
	ifs.close();
	return;
}