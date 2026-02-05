
#include "opengl_buffers.hpp"
#include <stb/stb_image.h>


#include <vector>
#include <list>
#include <string>

#include "Position2D.hpp"
#include "RectManagerClass.hpp"

typedef int* TextureID;

struct TextureVert{
	GLfloat x, y;
	constexpr static GLfloat z = 0.0f;
	
	//texture coord
	GLfloat tx, ty;
	//texture index
	GLuint texture_index;
};





class Texture_Manager : public Render_Rect_Manager<TextureVert>{
	protected:
	using Rect_Manager::new_rect;
	
	using Render_Rect_Manager::ready_for_draw;
	using Render_Rect_Manager::draw_all;
	using Render_Rect_Manager::link_attribute;
	using Render_Rect_Manager::start_init;
	using Render_Rect_Manager::finish_init;
	
	GLuint texture_array;
	int number_offset;
	
	public:
	
	RectangleID new_texture_rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLuint texture_index);
	RectangleID new_texture_rect(Position2D pos, Position2D dimensions, GLuint texture_index);
	
	RectangleID* new_number_rect(Position2D start_pos, GLfloat height, GLfloat spacing, const char* number_string);
	
	void set_rect_texture(RectangleID ID, int new_texture_index);
	
	void set_texture(int index, const char* texture_path);
	void set_number_textures(int offset);
	
	int get_number_offset();
	
	void draw_all_textures();
	
	Texture_Manager(int number_of_textures, int dim);
	~Texture_Manager();
	
	
	
};	




