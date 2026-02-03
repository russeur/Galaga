
#include "galaga_objects.hpp"
#include "path_objects.hpp"
#include "path_developer.hpp"
#include "debug_manager.hpp"
#include <ctime>
#include <fstream>
#include <string>



void save_stage(int stage, Path** stage_paths, int path_count, Bug_Spawner_Dev** stage_bug_spawners, int spawner_count);

void load_stage(int stage, Path_Developer_Manager* path_man);







/*
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
*/
int main() {
	srand(time(0));
	const bool debug_collisions = false;
	
	
	
	stbi_set_flip_vertically_on_load(true);

	
	Window* window = Window::init_window(640, 640, "My Window");
	window->set_clear_color(Color{0.25f, 0.5f, 0.75f, 1.0f});
	glDisable(GL_CULL_FACE);
	{
	Texture_Manager text_man(16, 16); 
	
	text_man.set_texture(0, "../textures/Ship/ship_base.png");
	text_man.set_texture(1, "../textures/Bugs/Mother_Bug1.png");
	text_man.set_texture(2, "../textures/Bullets/basic_bullet.png");
	text_man.set_texture(3, "../textures/Bugs/fly.png");
	text_man.set_number_textures(4);//4-13
	text_man.set_texture(15, "../textures/editor_window_area.png");
	
	
	text_man.new_texture_rect(window->screen_to_UV_pos(Position2D{0, 0}), window->screen_to_UV_vector(Position2D{window->width, window->height}), 15);
	
	
	Collision_Debug_Manager collider_manager(window);
	
	Path_Developer_Manager path_manager(window);
	
	//Path_Developer path1(window);
	//path1.smoothness = .5;
	//path1.color = {1.0, 0.0, 1.0};
	
	
	//path1.add_point(Position2D{.1, .1});
	
	
	Galaga_Root root(&text_man, window, nullptr, true);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	double lastTime = glfwGetTime();
	int frameCount = 0;
	
	Number_Texture stage_label(&text_man, window, Position2D{50, 50}, false);
	stage_label.set_font_size(50);
	
	
	
	bool clicking_left = false;
	bool clicking_right = false;
	bool pressing_8 = false;
	bool pressing_9 = false;
	bool pressing_left = false;
	bool pressing_right = false;
	bool pressing_d = false;
	bool pressing_enter = false;
	bool started_path = false;
	
	
	int current_stage = 0;
	stage_label.update_number(current_stage);
	
	path_manager.update_selected_spawner(-1);
	load_stage(current_stage, &path_manager);
	
	
	
	while(!window->window_should_close()){
		
		
		//std::cout<<"is_colliding"<<ship.get_child<Collider>("SCollider")->is_colliding_with(bug.get_child<Collider>("BCollider"))<<"\n";
		
		
		if (!clicking_left && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
			//root.add_bug<Bug>(Position2D{window->cursor_x, window->cursor_y}, "bug1");
			//root.add_bug<Bug>(path_manager.paths[0].path_points[0], "bug1");
			//bug1 = root.get_child<Node2D>("Bug_Container")->get_child<Bug>("bug1");
			//std::cout<<"made bug\n";
			//std::cout<<"dist = "<<path_manager.paths[0].distance_from(Position2D{window->cursor_x, window->cursor_y})<<"\n";
			clicking_left=true;
			//started_path = true;
			
		}
		if (clicking_left && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
			
			if (!started_path){
				
				if (window->get_key_button_state(GLFW_KEY_P) == GLFW_PRESS){
					
					//root.bug_container->add_path(.5, 60, nullptr, 0);
					
					path_manager.add_path(path_manager.get_unique_color(), .5, 20);
					
					
					
					path_manager.add_spawner(path_manager.paths.size()-1, 10, 5, BugType::COMMON, Position2D{((float)window->cursor_x - window->width/2) / path_manager.zoom + window->width/2, ((float)window->cursor_y - window->height/2) / path_manager.zoom  + window->height/2});
					
					path_manager.update_selected_spawner(path_manager.bug_spawners.size()-1);
					
					path_manager.add_point_to_spawner(path_manager.get_selected_spawner(), Position2D{((float)window->cursor_x - window->width/2) / path_manager.zoom + window->width/2, ((float)window->cursor_y - window->height/2) / path_manager.zoom  + window->height/2});
					
					//root.bug_container->add_spawner(0, 10, 60, Position2D{0, 0});
				}else{
					path_manager.select_new_spawner(Position2D{((float)window->cursor_x - window->width/2) / path_manager.zoom + window->width/2, ((float)window->cursor_y - window->height/2) / path_manager.zoom  + window->height/2});	
					if (path_manager.get_selected_spawner() >= 0){
						std::cout<<"enemy_count: "<<path_manager.bug_spawners[path_manager.get_selected_spawner()]->max_enemy_count<<"\n";
						std::cout<<"spawn frequency: "<<path_manager.bug_spawners[path_manager.get_selected_spawner()]->spawn_interval<<"\n";
						std::cout<<"bug type: "<<static_cast<int>(path_manager.bug_spawners[path_manager.get_selected_spawner()]->bug_type)<<"\n";
					}

					
				}
			}				
			clicking_left = false;
			//Position2D pos = window->screen_to_UV_pos(Position2D{(float)window->cursor_x, (float)window->cursor_y});
			
			
			
		}
		
		if (!clicking_right && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
			
			clicking_right=true;
			
		}
		if (clicking_right && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
			if (!started_path){
				
				
				
				path_manager.add_point_to_spawner(path_manager.get_selected_spawner(), Position2D{((float)window->cursor_x - window->width/2) / path_manager.zoom + window->width/2, ((float)window->cursor_y - window->height/2) / path_manager.zoom  + window->height/2});
				//path1.add_point(Position2D{window->cursor_x, window->cursor_y});
				
			}
			clicking_right = false;
		}
	
		
		if (!started_path && window->get_key_button_state(GLFW_KEY_S) == GLFW_PRESS){
			
			for (int i= 0; i < path_manager.paths.size(); i++){
				
				root.bug_container->add_path(path_manager.paths[i]->smoothness, path_manager.paths[i]->speed, path_manager.paths[i]->path_points.data(), path_manager.paths[i]->path_points.size());
				
			}
			for (int i = 0; i< path_manager.bug_spawners.size(); i++){
				
				root.bug_container->add_spawner(path_manager.bug_spawners[i]->target_path, path_manager.bug_spawners[i]->max_enemy_count, path_manager.bug_spawners[i]->spawn_interval, path_manager.bug_spawners[i]->bug_type, path_manager.bug_spawners[i]->offset);
			}
			started_path = true;
			root.bug_container->start_stage();
			
		}else if (started_path && window->get_key_button_state(GLFW_KEY_E) == GLFW_PRESS){
			
			root.bug_container->end_stage();
			root.bug_container->kill_all_bugs();
			
			
			started_path = false;
			root.reset_game();
		}
		
		
		
		if(!started_path && !pressing_9 && window->get_key_button_state(GLFW_KEY_9) == GLFW_PRESS){
			std::cout<<"load\n";
			path_manager.update_selected_spawner(-1);
			load_stage(current_stage, &path_manager);
			
			pressing_9 = true;
		}
		if (window->get_key_button_state(GLFW_KEY_9) == GLFW_RELEASE){
			pressing_9 = false;
		}
		
		if (!started_path && !pressing_8 && window->get_key_button_state(GLFW_KEY_8) == GLFW_PRESS){
			std::cout<<"save\n";
			
			save_stage(current_stage, (Path**)path_manager.paths.data(), path_manager.paths.size(), path_manager.bug_spawners.data(), path_manager.bug_spawners.size());
			pressing_8 = true;
		}if (window->get_key_button_state(GLFW_KEY_8) == GLFW_RELEASE){
			pressing_8 = false;
		}
		if (!started_path && !pressing_d && window->get_key_button_state(GLFW_KEY_D) == GLFW_PRESS){
			pressing_d = true;
			
			
		}
		if (!started_path && pressing_d && window->get_key_button_state(GLFW_KEY_D) == GLFW_RELEASE){
			pressing_d = false;
			
			path_manager.destroy_spawner(path_manager.get_selected_spawner());
			
		}
		if (!started_path && !pressing_enter && window->get_key_button_state(GLFW_KEY_ENTER) == GLFW_PRESS){
			pressing_enter = true;
		}
		if (!started_path && pressing_enter && window->get_key_button_state(GLFW_KEY_ENTER) == GLFW_RELEASE){
			pressing_enter = false;
			if (window->get_key_button_state(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
				float smoothness, speed;
				
				std::cout<<path_manager.paths[path_manager.bug_spawners[path_manager.get_selected_spawner()]->target_path]->smoothness<<", enter smoothness: ";
				std::cin >> smoothness;
				std::cout<<path_manager.paths[path_manager.bug_spawners[path_manager.get_selected_spawner()]->target_path]->speed<<", enter speed: ";
				std::cin >>speed;
				
				path_manager.paths[path_manager.bug_spawners[path_manager.get_selected_spawner()]->target_path]->smoothness = smoothness;
				path_manager.paths[path_manager.bug_spawners[path_manager.get_selected_spawner()]->target_path]->speed = speed;
				path_manager.paths[path_manager.bug_spawners[path_manager.get_selected_spawner()]->target_path]->update_curve();
				
			}else{
				int enemy_count;
				int spawn_freq;
				int bug_type;
				std::cout<<path_manager.bug_spawners[path_manager.get_selected_spawner()]->max_enemy_count<<", enter enemy_count: ";
				std::cin >> enemy_count;
				std::cout<<path_manager.bug_spawners[path_manager.get_selected_spawner()]->spawn_interval<<", enter spawn_freq: ";
				std::cin >> spawn_freq;
				std::cout<<static_cast<int>(path_manager.bug_spawners[path_manager.get_selected_spawner()]->bug_type)<<", enter bug_type: ";
				std::cin >> bug_type;
				path_manager.bug_spawners[path_manager.get_selected_spawner()]->max_enemy_count = enemy_count;
				path_manager.bug_spawners[path_manager.get_selected_spawner()]->spawn_interval = spawn_freq;
				path_manager.bug_spawners[path_manager.get_selected_spawner()]->bug_type = static_cast<BugType>(bug_type);
			}
			
			
			
		}
		
		if (window->get_key_button_state(GLFW_KEY_LEFT) == GLFW_PRESS && !pressing_left){
			pressing_left=true;
		}
		if (window->get_key_button_state(GLFW_KEY_LEFT) == GLFW_RELEASE && pressing_left){
			pressing_left = false;
			if (window->get_key_button_state(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
				if (current_stage != 0){
					
					current_stage--;
					std::cout<<"current_stage: "<<current_stage<<"\n";
					path_manager.update_selected_spawner(-1);
					load_stage(current_stage, &path_manager);
					stage_label.update_number(current_stage);
				}
			}
		}
		
		
		if (window->get_key_button_state(GLFW_KEY_RIGHT) == GLFW_PRESS && !pressing_right){
			pressing_right=true;
		}
		if (window->get_key_button_state(GLFW_KEY_RIGHT) == GLFW_RELEASE && pressing_right){
			pressing_right = false;
			if (window->get_key_button_state(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
				
					
				current_stage++;
				std::cout<<"current_stage: "<<current_stage<<"\n";
				path_manager.update_selected_spawner(-1);
				load_stage(current_stage, &path_manager);
				stage_label.update_number(current_stage);
				
			}
		}
		
		
		if (started_path){
			
		
			
			root.proccess();
			
			
		}
		
		
		
		
		
		if (window->get_key_button_state(GLFW_KEY_3) == GLFW_PRESS){
			text_man.modify_zoom(-.005f);
			
			path_manager.modify_zoom(-.005f);
			
			if (debug_collisions){
				collider_manager.modify_zoom(-.005f);
			}
		}
		if (window->get_key_button_state(GLFW_KEY_4) == GLFW_PRESS){
			text_man.modify_zoom(.005f);
			path_manager.modify_zoom(.005f);
			if (debug_collisions){
				collider_manager.modify_zoom(.005f);
			}
		}
		if (window->get_key_button_state(GLFW_KEY_2) == GLFW_PRESS){
			text_man.reset_zoom();
			
			path_manager.reset_zoom();
			
			if (debug_collisions){
				collider_manager.modify_zoom(-.005f);
			}
		}
		
		
		
		
		//if (root.get_child<Player_Ship>("Player_Ship")->get_child<Collider>("ship_collider")->is_colliding_with(root.get_child<Sprite>("Bug")->get_child<Collider>("BCollider"))){
		//	std::cout<<"collided\n";
		//}
		
		//root.get_child<Player_Ship>("Player_Ship")->proccess_ui();
		//root.get_child<Player_Bullet>("Player_Bullet")->update_pos();
		//root.proccess();
		window->clear_window();
		
		
		
		
		
		text_man.draw_all_textures();
		
		if (debug_collisions){
			collider_manager.draw_all(&root);
			
		}
		path_manager.draw_all_paths();
		
		window->update_frame();
		
		
		
		
	}
	}
	
	window->terminate();
	
	return 0;
}





void save_stage(int stage, Path** stage_paths, int path_count, Bug_Spawner_Dev** stage_bug_spawners, int spawner_count){
	//open file/create file named as '93.bin' for stage 93
	std::string file_name = "../stages/";
	file_name += std::to_string(stage);
	file_name += ".bin";
	
	std::ofstream ofs(file_name, std::fstream::out | std::fstream::binary);
	if (ofs.is_open()){
		ofs.write(reinterpret_cast<const char*>(&path_count), sizeof(path_count));
		//write all paths first
		Path** current_path = stage_paths;
		for (int p = 0; p < path_count; p++){
			//write smoothness, speed, and number of points
			ofs.write(reinterpret_cast<const char*>(&(*current_path)->smoothness), sizeof((*current_path)->smoothness));
			ofs.write(reinterpret_cast<const char*>(&(*current_path)->speed), sizeof((*current_path)->speed));
			size_t path_size = (*current_path)->path_points.size();
			ofs.write(reinterpret_cast<const char*>(&path_size), sizeof(size_t));
			
			ofs.write(reinterpret_cast<const char*>((*current_path)->path_points.data()), sizeof(Position2D) * (*current_path)->path_points.size());
			current_path++;
			
		}
		
		//bug_spawner
		ofs.write(reinterpret_cast<const char*>(&spawner_count), sizeof(spawner_count));
		Bug_Spawner_Dev** current_spawner = stage_bug_spawners;
		for (int s = 0; s < spawner_count; s++){
			
			ofs.write(reinterpret_cast<const char*>(&(*current_spawner)->target_path), sizeof((*current_spawner)->target_path));
			ofs.write(reinterpret_cast<const char*>(&(*current_spawner)->max_enemy_count), sizeof((*current_spawner)->max_enemy_count));
			ofs.write(reinterpret_cast<const char*>(&(*current_spawner)->spawn_interval), sizeof((*current_spawner)->spawn_interval));
			ofs.write(reinterpret_cast<const char*>(&(*current_spawner)->bug_type), sizeof((*current_spawner)->bug_type));
			ofs.write(reinterpret_cast<const char*>(&(*current_spawner)->offset), sizeof((*current_spawner)->offset));
		
			
			current_spawner++;
		}
		
	}else{
		std::cout<<"failed to open file name: "<<"../stages/"<<stage<<".bin"<<"\n";
	}
	ofs.close();
	return;
	
}

void load_stage(int stage, Path_Developer_Manager* path_man){
	path_man->destroy_all_paths();
	path_man->destroy_all_spawners();
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
			
			path_man->add_path(path_man->get_unique_color(), smoothness, speed);
			
			Position2D path_point;
			for (int i = 0; i < number_of_points; i++){
				ifs.read(reinterpret_cast<char*>(&path_point), sizeof(Position2D));
				path_man->add_point_to_path(p, path_point);
			}
			
			
			
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
			
			path_man->add_spawner(target_path, max_enemy_count, spawn_interval, bug_type, offset);
			
			
		
			//ofs.write(reinterpret_cast<const char*>(&layer_count), sizeof(layer_count));
		}
		
		
	}else{
		std::cout<<"failed to open file name: "<<"../stages/"<<stage<<".bin"<<"\n";
	}
	ifs.close();
	return;
}
