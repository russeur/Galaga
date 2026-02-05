#ifndef RECT_MANAGERS_HPP
#define RECT_MANAGERS_HPP
#include "opengl_buffers.hpp"



#include <vector>
#include <list>
#include "Position2D.hpp"


typedef int* RectangleID;
/*
struct Position2D{
	GLfloat x, y;
};
*/
struct RectangleVert{
	//position
	GLfloat x, y;
	constexpr static GLfloat z = 0.0f;
};


template <typename VertType>
class Rect_Manager{
	
	
	public:
	
	std::list<int> rect_start_indices;
	std::vector<VertType> rect_vert_data;
	std::vector<GLuint> indices;
	
	template <typename... VertConstructors> RectangleID new_rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, VertConstructors... rest_args);
	template <typename... VertConstructors> RectangleID new_rect(Position2D pos, Position2D dimensions, VertConstructors... rest_args);
	
	
	
	
	VertType& get_vertex(RectangleID ID, int point_index);
	
	void set_rect_pos(RectangleID ID, GLfloat x, GLfloat y);
	void set_rect_pos(RectangleID ID, Position2D new_pos);
	
	void move_rect(RectangleID ID, GLfloat dx, GLfloat dy);
	void move_rect(RectangleID ID, Position2D delta);
	
	void destroy_rect(RectangleID ID);
	
	
	Rect_Manager();
	~Rect_Manager();
};

template <typename VertType>
class Render_Rect_Manager : public Rect_Manager<VertType>{
	private:
	bool started_init = false;
	bool finished_init = false;
	bool prepared_to_draw = false;
	
	
	public:
	float zoom = 1.0;
	
	Shader shader;
	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
	
	
	
	void ready_for_draw();//gets stuff ready for drawing, must call before every draw_all() call. Allows for you to set uniforms before drawing
	
	void set_uniform(const char* uniform_name, GLint value);
	void set_uniform(const char* uniform_name, GLint value_a, GLint value_b);
	void set_uniform(const char* uniform_name, GLint value_a, GLint value_b, GLint value_c);
	void set_uniform(const char* uniform_name, GLfloat value);
	void set_uniform(const char* uniform_name, GLfloat value_a, GLfloat value_b);
	void set_uniform(const char* uniform_name, GLfloat value_a, GLfloat value_b, GLfloat value_c);
	void draw_all();
	
	
	void modify_zoom(float delta);
	void reset_zoom();
	
	void link_attribute(int attrib_index, GLenum data_type, GLsizei data_count, size_t offset); // must be data from VertType, must be called after constructor and before finish_init()
	
	
	Render_Rect_Manager(const char* vertex_shader_path, const char* fragment_shader_path); 
	void start_init();// after this, link attributes, then call finish_init()
	void finish_init();//only call after calling start_init()
	~Render_Rect_Manager();
	
};


//TEMPLATE DEFINITIONS

template <typename VertType>
template <typename... VertConstructors> 
RectangleID Rect_Manager<VertType>::new_rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, VertConstructors... rest_args){
	int rect_index_start = rect_vert_data.size();
	rect_start_indices.push_back(rect_index_start);
	
	rect_vert_data.push_back(VertType{x,   y,   rest_args...});
	rect_vert_data.push_back(VertType{x+w, y,   rest_args...});
	rect_vert_data.push_back(VertType{x,   y+h, rest_args...});
	rect_vert_data.push_back(VertType{x+w, y+h, rest_args...});
	/*
	rect_vert_data.push_back(VertType{x,   y,   0.0f, 0.0f, rest_args...});
	rect_vert_data.push_back(VertType{x+w, y,   1.0f, 0.0f, rest_args...});
	rect_vert_data.push_back(VertType{x,   y+h, 0.0f, 1.0f, rest_args...});
	rect_vert_data.push_back(VertType{x+w, y+h, 1.0f, 1.0f, rest_args...});
	*/
	indices.push_back(rect_index_start + 0);
	indices.push_back(rect_index_start + 1);
	indices.push_back(rect_index_start + 2);
	indices.push_back(rect_index_start + 3);
	indices.push_back(rect_index_start + 2);
	indices.push_back(rect_index_start + 1);
	
	auto it = rect_start_indices.begin();
	
	
	
	it = rect_start_indices.end();
	std::advance(it, -1);
	return &(*it);
}
template <typename VertType>
template <typename... VertConstructors> 
RectangleID Rect_Manager<VertType>::new_rect(Position2D pos, Position2D dimensions, VertConstructors... rest_args){
	return new_rect(pos.x, pos.y, dimensions.x, dimensions.y, rest_args...);
	
}

#include "RectManagerClass.cpp" // include definitions of template classes inside of hpp file instead of other way around
#endif