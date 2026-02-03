#include "Window.hpp"
#include "RectManagerClass.hpp"
#include <type_traits>
#include <assert.h>
#include <cstdint>




class Number_Texture{
	
	protected:
	
	std::vector<RectangleID> texture_digits;//reverse reading order
	std::string current_number_string;//reading order
	
	Position2D dims;
	Position2D position; // bottom right
	float spacing = .5;
	
	Texture_Manager* text_man;
	Window* window;
	bool trim_leading;
	
	void set_texture_numbers();
	Position2D get_number_texture_pos(int digit_index);
	
	public:
	
	
	void update_number(int new_number);
	
	void set_font_size(GLfloat number_height);
	
	void set_pos(Position2D new_pos);
	void modify_pos(Position2D delta);
	
	
	Number_Texture(Texture_Manager* texture_manager, Window* window, Position2D bottom_right_pos, bool trim_leading_zeros);
	~Number_Texture();
	
};


