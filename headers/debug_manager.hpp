#include <glad/glad.h>
#include "../src/config.h"
#include <vector>
#include "Position2D.hpp"
#include "opengl_buffers.hpp"
#include "game_objects.hpp"



class Collision_Debug_Manager{
	private:
	Window* window;
	
	Shader shader;
	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
	
	
	std::vector<Position3D> poly_verts;//x, y
	std::vector<GLuint> poly_indices;
	
	void add_children(Node2D* parent);//recursively add children to poly verts if it the child is a collider
	void clear_vertex_array();
	void add_rect(Position2D pos, Position2D dims);
	
	public:
	float zoom = 1.0;
	
	
	void draw_all(Node2D* root);
	
	void modify_zoom(float delta);
	
	Collision_Debug_Manager(Window* window);
	
	
};

