#include "game_ui.hpp"
void Number_Texture::set_texture_numbers(){
	if (current_number_string.length() != texture_digits.size()) return;
	
	
	for (int i = 0; i < texture_digits.size(); i++){
		
		text_man->set_rect_texture(texture_digits[i], std::stoi(std::string(1, current_number_string[texture_digits.size()-i-1])) + text_man->get_number_offset());
		
	}
	
}
Position2D Number_Texture::get_number_texture_pos(int digit_index){
	return {position.x - ((digit_index+1) * (dims.x * spacing)), position.y};
}

void Number_Texture::update_number(int new_number){
	std::string new_number_string = std::to_string(abs(new_number));
	int digit_count_diff = new_number_string.length() - current_number_string.length();
	
	if (digit_count_diff == 0){//don't do anything
		
		
	}else if (digit_count_diff < 0){//remove excess number textures
		for (int i = 0; i > digit_count_diff; i--){
			
			text_man->destroy_rect(texture_digits[texture_digits.size()-1]);//destroy rect
			
			texture_digits.erase(texture_digits.end()); // destroy digit rectangleID
			
		}
		
	}else{ // add new textures
		for (int i = 0; i < digit_count_diff; i++){
			
			texture_digits.push_back(text_man->new_texture_rect(window->screen_to_UV_pos(get_number_texture_pos(i + current_number_string.length())), window->screen_to_UV_vector(dims), 0)); // set texture to 0 because it will be set later
		}
	}
	
	current_number_string = new_number_string;
	set_texture_numbers();
	
	
}
	
void Number_Texture::set_font_size(GLfloat number_height){
	dims.x = number_height;
	dims.y = -number_height;
}

void Number_Texture::set_pos(Position2D new_pos){
	modify_pos(new_pos - position);
	
}
void Number_Texture::modify_pos(Position2D delta){
	position += delta;
	for (int i = 0; i < texture_digits.size(); i++){
		
		text_man->move_rect(texture_digits[i], window->screen_to_UV_vector(delta));
	}
}
	
Number_Texture::Number_Texture(Texture_Manager* texture_manager, Window* window, Position2D bottom_right_pos, bool trim_leading_zeros){
	this->text_man = texture_manager;
	this->window = window;
	this->position = bottom_right_pos;
	this->trim_leading = trim_leading_zeros;
}
Number_Texture::~Number_Texture(){
	for (int i = 0; i< texture_digits.size(); i++){
		text_man->destroy_rect(texture_digits[i]);
	}
	
}

