
#include "galaga_objects.hpp"
#include "path_objects.hpp"
#include "path_developer.hpp"
#include "debug_manager.hpp"
#include <ctime>
#include <fstream>
#include <string>

const bool debug_collisions = false; // set true if you want to see all collision shapes

const Position2D play_text_pos = {500, 1000};
const Position2D play_text_dims = {500, -500};

const Position2D play_button_pos = {500, 656};
const Position2D play_button_dims = {500, 186};


void save_high_score(int);

void get_high_score(int&);

void reset_high_score();

void run_menu(Window* window, Galaga_Root* root, Texture_Manager* menu_text_man, Texture_Manager* menu_num_man, Number_Texture* high_score); // make the menu screen and procces that

void run_simulation(Window* window,  Galaga_Root* root, Texture_Manager* text_man, Collision_Debug_Manager* collider_manager, Bar_Manager* bar_man); // run the real game


bool clicking_left = false;
bool clicking_right = false;
bool is_in_menu = true;
bool was_in_menu = false;



int main() {
	
	
	stbi_set_flip_vertically_on_load(true);

	
	Window* window = Window::init_window(640, 640, "Galaga");
	window->set_clear_color(Color{0.25f, 0.5f, 0.75f, 1.0f}); // set background color
	
	{
	Bar_Manager bar_man;
	
	Texture_Manager text_man(18, 16); // 18 slots for 16x16 textures, these textures are used in the simulation(the real game), 8 for real textures and 10 for numbers
	
	text_man.set_texture(0, "../textures/Ship/ship_base.png"); //player
	text_man.set_texture(1, "../textures/Bugs/Mother_Bug1.png"); // bug
	text_man.set_texture(2, "../textures/Bullets/basic_bullet.png"); // player_bullet
	text_man.set_texture(3, "../textures/Bugs/fly.png"); // bug
	text_man.set_texture(4, "../textures/Bullets/dumb.png"); // bug bullet
	text_man.set_texture(5, "../textures/x.png"); // multiply symbol
	text_man.set_texture(6, "../textures/kill_bar.png"); // #kills bar
	text_man.set_texture(7, "../textures/time_bar.png"); // time remaining bar
	text_man.set_number_textures(8);// fill 8-17
	
	
	
	
	
	
	Texture_Manager menu_num_man(10, 16); // 10 slots(for 10 numbers) 
	menu_num_man.set_number_textures(0); // fill menu_num_manager with numbers
	
	
	Texture_Manager menu_text_man(14, 32); // 14 slots for 32x32 textures
	menu_text_man.set_texture(0, "../textures/Menu/play.png"); // play button
	menu_text_man.set_texture(1, "../textures/Menu/High.png"); // high score label 1
	menu_text_man.set_texture(2, "../textures/Menu/Score.png"); // high score label 2
	
	menu_text_man.new_texture_rect(window->screen_to_UV_pos(play_text_pos), window->screen_to_UV_vector(play_text_dims), 0); // make play button texture
	menu_text_man.new_texture_rect(window->screen_to_UV_pos({100, 500}), window->screen_to_UV_vector({500, -500}), 1); // make high score label 1 texture
	menu_text_man.new_texture_rect(window->screen_to_UV_pos({380, 500}), window->screen_to_UV_vector({500, -500}), 2); // make high score label 2 texture
	
	
	Number_Texture high_score(&menu_num_man, window, Position2D{static_cast<float>(window->width-75), 310}, false); // create new number attached to the menu_num_man for the high score
	high_score.set_font_size(75);
	high_score.update_number(0);
	
	
	Collision_Debug_Manager collider_manager(window); // only used if debug_collisions == true
	
	
	
	
	Galaga_Root root(&text_man, window, &bar_man, false); // make the simulation root
	
	
	
	
	
	
	
	
	while(!window->window_should_close()){
		
		if (is_in_menu){
			run_menu(window, &root, &menu_text_man, &menu_num_man, &high_score);
			
			
		}else{
			
			run_simulation(window, &root, &text_man, &collider_manager, &bar_man);
			
		}
		
		
		
	}
	}
	
	
	window->terminate();
	
	return 0;
}




void run_menu(Window* window, Galaga_Root* root, Texture_Manager* menu_text_man, Texture_Manager* menu_num_man, Number_Texture* high_score){
	
	if (!was_in_menu){
		save_high_score(root->get_score());
		
		
		int hs; get_high_score(hs);
		high_score->update_number(hs);
	}
	
	if (!clicking_left && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		
		clicking_left=true;
		
	}
	if (clicking_left && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
		std::cout<<"x: "<<window->cursor_x<<", y: "<<window->cursor_y<<"\n";
		clicking_left = false;
		if ((window->cursor_x > play_button_pos.x && window->cursor_x < play_button_pos.x+play_button_dims.x) &&
			(window->cursor_y > play_button_pos.y && window->cursor_y < play_button_pos.y+play_button_dims.y)){
				
			
			is_in_menu = false;
			
			root->reset_game();
			
			
			root->next_stage();
		}
		
		
		
		
	}
	
	
	window->clear_window();
	menu_text_man->draw_all_textures();
	menu_num_man->draw_all_textures();
	window->update_frame();
	was_in_menu = true;
	
	
}

void run_simulation(Window* window,  Galaga_Root* root, Texture_Manager* text_man, Collision_Debug_Manager* collider_manager, Bar_Manager* bar_man){
	
	if (!clicking_left && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		root->add_bug<Bug>(Position2D{static_cast<float>(window->cursor_x), static_cast<float>(window->cursor_y)}, "bug1");
		clicking_left=true;
		
	}
	if (clicking_left && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
		
		clicking_left = false;
		//Position2D pos = window->screen_to_UV_pos(Position2D{(float)window->cursor_x, (float)window->cursor_y});
		
		
		
	}
	
	if (!clicking_right && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		
		clicking_right=true;
		
	}
	if (clicking_right && window->get_mouse_button_state(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
		
		clicking_right = false;
		//text_man.destroy_texture_rect(dummy);
		
	}
		
	
	
	
	
	
	
	
	if (window->get_key_button_state(GLFW_KEY_3) == GLFW_PRESS){
		text_man->modify_zoom(-.005f);
		
		
		
		if (debug_collisions){
			collider_manager->modify_zoom(-.005f);
		}
	}
	if (window->get_key_button_state(GLFW_KEY_4) == GLFW_PRESS){
		text_man->modify_zoom(.005f);
		
		if (debug_collisions){
			collider_manager->modify_zoom(.005f);
		}
	}
	if (window->get_key_button_state(GLFW_KEY_2) == GLFW_PRESS){
		text_man->reset_zoom();
		
		
		
		if (debug_collisions){
			collider_manager->modify_zoom(-.005f);
		}
	}
	if (root->is_game_lost()){
		is_in_menu = true;
		
	}
	if (root->is_stage_done()){
		root->next_stage();
	}
	
	
	
	
	//run the game simulation
	root->proccess();
	
	//start drawing
	window->clear_window(); // blank slate
	
	bar_man->draw_all_bars(); // multiply bars
	text_man->draw_all_textures(); // all textures(bugs, players, bullets, numbers)
	
	
	if (debug_collisions){
		collider_manager->draw_all(root); // draw collision shapes
	}
	
	
	window->update_frame(); // render it
	
	was_in_menu = false;
	
}




void save_high_score(int new_score){
	//make sure current high score isn't higher than new score
	int current_high_score; get_high_score(current_high_score);
	if (current_high_score >= new_score) return;
	
	
	
	std::ofstream ofs("../save/high_score.bin", std::fstream::out | std::fstream::binary);
	if (ofs.is_open()){
		
		ofs.write(reinterpret_cast<const char*>(&new_score), sizeof(new_score));
	}else{
		std::cout<<"failed to open high_score save file\n";
	}
	ofs.close();
	
}

void get_high_score(int& high_score){
	
	std::ifstream ifs("../save/high_score.bin", std::fstream::in | std::fstream::binary); 
	
	if (ifs.is_open()){
		
		int score;
		ifs.read(reinterpret_cast<char*>(&score), sizeof(score));
		high_score = score;
		
	}else{
		std::cout<<"failed to open high_score load file\n";
	}
	ifs.close();
	
	
}

void reset_high_score(){
	std::ofstream ofs("../save/high_score.bin", std::fstream::out | std::fstream::binary);
	if (ofs.is_open()){
		int score = 0;
		ofs.write(reinterpret_cast<const char*>(&score), sizeof(score));
	}else{
		std::cout<<"failed to open high_score save file\n";
	}
	ofs.close();
}

