#include "game_objects.hpp"




void Node2D::set_pos(Position2D new_pos){
	new_pos.x -= pos.x;
	new_pos.y -= pos.y;
	modify_pos(new_pos);
	
}
void Node2D::modify_pos(Position2D delta){
	pos.x += delta.x;
	pos.y += delta.y;
	
	
	for (size_t i = 0; i< children.size(); i++){
		children[i]->modify_pos(delta);
	}
}
Position2D Node2D::get_pos(){
	return pos;
}
void Node2D::destroy(){
	if (parent != nullptr){//if it has a parent
		parent->destroy_child(this);
	}
}
void Node2D::proccess(){
	for (size_t i = 0; i < children.size(); i++){
		children[i]->proccess();
	}
}
int Node2D::get_child_count(){
	return children.size();
}
void Node2D::destroy_child(Node2D* child_pointer){
	for (size_t i = 0; i < children.size(); i++){
		if (children[i] == child_pointer){
			delete child_pointer;//destroy child
			children.erase(children.begin() + i);//free up space in children vector
		}
	}
}
Node2D::Node2D(Position2D pos, const char* name, Node2D* parent){
	this->pos = pos;
	
	this->name = name;
	
	this->parent = parent;
	
}
Node2D::~Node2D(){
	for (size_t i = 0; i< children.size(); i++){
		delete children[i];
	}
	
	
}




Position2D Sprite::get_pos(){
	return pos;
}
void Sprite::set_pos(Position2D new_pos){
	Node2D::set_pos(new_pos);
	texture_manager->set_rect_pos(texture_ID, window->screen_to_UV_pos(pos));
}
void Sprite::modify_pos(Position2D delta){
	if (delta.x == 0 && delta.y == 0) return;
	Node2D::modify_pos(delta);
	
	texture_manager->move_rect(texture_ID, window->screen_to_UV_vector(delta));
	
}

void Sprite::set_texture(int texture_index){
	texture_manager->set_rect_texture(texture_ID, texture_index);
}

Sprite::Sprite(Position2D pos, const char* name, Node2D* parent, Position2D dimensions, int texture_index, Texture_Manager* texture_manager, Window* window) : Node2D(pos, name, parent){
	this->texture_manager = texture_manager;
	this->window = window;
	
	
	this->dims = dimensions;
	
	texture_ID = texture_manager->new_texture_rect(window->screen_to_UV_pos(pos), window->screen_to_UV_vector(dims), texture_index);
	
	
}
Sprite::~Sprite(){
	texture_manager->destroy_rect(texture_ID);
	
}


bool Collider::is_colliding_with(Collider* other_collider){
	
	
	
	//if (othercollider's layer is part of this->mask && boxes intersect)
	if (static_cast<bool>(layer_mask & other_collider->layer) && !(pos.x > other_collider->get_pos().x + other_collider->dims.x || pos.y > other_collider->get_pos().y - other_collider->dims.y || other_collider->get_pos().x > pos.x + dims.x || other_collider->get_pos().y > pos.y - dims.y)){
		return true;
	}
	return false;
	
}

Collider::Collider(Position2D pos, const char* name, Node2D* parent, Position2D dimensions, Layer_Flags layer, Layer_Flags layer_mask) : Node2D(pos, name, parent){
	this->dims = dimensions;
	this->layer = layer;
	this->layer_mask = layer_mask;
	
}