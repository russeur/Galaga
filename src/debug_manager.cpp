#include "debug_manager.hpp"





void Collision_Debug_Manager::add_rect(Position2D pos, Position2D dims){
	
	int indice_size = poly_verts.size();
	poly_indices.push_back(indice_size + 0);
	poly_indices.push_back(indice_size + 1);
	poly_indices.push_back(indice_size + 2);
	poly_indices.push_back(indice_size + 3);
	poly_indices.push_back(indice_size + 2);
	poly_indices.push_back(indice_size + 1);
	
	poly_verts.push_back(Position3D{pos.x, pos.y, 1.0});
	poly_verts.push_back(Position3D{pos.x + dims.x, pos.y, 1.0});
	poly_verts.push_back(Position3D{pos.x, pos.y + dims.y, 1.0});
	poly_verts.push_back(Position3D{pos.x + dims.x, pos.y + dims.y, 1.0});
	
	
	
	
}
void Collision_Debug_Manager::clear_vertex_array(){
	poly_verts.clear();
	poly_indices.clear();
}
void Collision_Debug_Manager::add_children(Node2D* parent){
	for (int i = 0; i<parent->get_child_count(); i++){
		add_children(parent->get_child<Node2D>(i));//recursivly add_children
		Collider* collider_pointer = dynamic_cast<Collider*>(parent->get_child<Node2D>(i));
		if (collider_pointer != nullptr){//child is a collider
			add_rect(window->screen_to_UV_pos(collider_pointer->get_pos()), window->screen_to_UV_vector(collider_pointer->dims));
		}
	}
}

void Collision_Debug_Manager::draw_all(Node2D* root){
	clear_vertex_array();
	add_children(root);
	/*
	for (size_t i = 0; i <poly_verts.size(); i++){
		std::cout<<"("<<poly_verts[i].x<<", "<<poly_verts[i].y<<"), ";
	}
	std::cout<<"\n";
	*/
	
	
	
	
	shader.activate();
	
	glUniform1f(glGetUniformLocation(shader.ID, "zoom"), zoom);
	
	
	
	
	VAO1.bind();
	VBO1.bind();
	EBO1.bind();
	glBufferData(GL_ARRAY_BUFFER, poly_verts.size()*sizeof(Position3D), poly_verts.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, poly_indices.size()*sizeof(GLuint), poly_indices.data(), GL_DYNAMIC_DRAW);
	
	glDrawElements(GL_TRIANGLES, poly_indices.size(), GL_UNSIGNED_INT, 0);
	
	VAO1.unbind();
	VBO1.unbind();
	EBO1.unbind();
	
}
void Collision_Debug_Manager::modify_zoom(float delta){
	zoom += delta*zoom;
	
	if (zoom < .005){
		zoom = .005;
	}
}
Collision_Debug_Manager::Collision_Debug_Manager(Window* window) : shader("../src/shaders/debug_colliders_vert.txt", "../src/shaders/debug_colliders_frag.txt"){
	this->window = window;
	VAO1.bind();
	VBO1.ready(poly_verts.data(), 20*sizeof(Position3D));
	EBO1.ready(poly_indices.data(), 20*sizeof(GLuint));
	
	EBO1.bind();
	
	VAO1.linkAttrib(VBO1, 0, GL_FLOAT, 3, sizeof(Position3D), (void*)offsetof(Position3D, x));
	
	VAO1.unbind();
	VBO1.unbind();
	EBO1.unbind();
	
}


