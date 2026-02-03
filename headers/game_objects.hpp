#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP


#include "Window.hpp"
#include "game_ui.hpp"
#include "BarManager.hpp"
#include <type_traits>
#include <assert.h>
#include <cstdint>


typedef std::uint8_t Layer_Flags;

#define LAYERNONE 0
#define LAYER0 1 << 0 // player
#define LAYER1 1 << 1 //bugs
#define LAYER2 1 << 2 //player bullets
#define LAYER3 1 << 3 //bug bullets
#define LAYER4 1 << 4
#define LAYER5 1 << 5
#define LAYER6 1 << 6
#define LAYER7 1 << 7
#define LAYERALL 255

class Node2D{
	protected:
	Node2D* parent;
	std::vector<Node2D*> children;
	Position2D pos;
	
	
	
	public:
	std::string name;
	
	
	template <typename T, typename... Args> T* add_child(Position2D pos, const char* name, Args... constructor_args);
	template <typename T> T* get_child(int index);
	template <typename T> T* get_child(const char* name);
	
	
	virtual void set_pos(Position2D new_pos);
	virtual void modify_pos(Position2D delta);
	virtual Position2D get_pos();
	virtual void destroy();
	
	virtual void proccess();
	
	
	
	
	
	int get_child_count();
	void destroy_child(Node2D* child_pntr);
	
	
	
	Node2D(Position2D pos, const char* name, Node2D* parent);
	virtual ~Node2D();
	
	
};

class Sprite : public Node2D{
	
	Position2D dims;
	TextureID texture_ID;
	
	
	Texture_Manager* texture_manager;
	Window* window;
	
	public:
	
	
	void set_pos(Position2D new_pos);
	void modify_pos(Position2D delta);
	Position2D get_pos();
	
	void set_texture(int texture_index);
	
	
	Sprite(Position2D pos, const char* name, Node2D* parent, Position2D dimensions, int texture_index, Texture_Manager* texture_manager, Window* window);
	~Sprite();
};

class Collider : public Node2D{
	public:
	
	Position2D dims;
	
	Layer_Flags layer;
	Layer_Flags layer_mask;
	
	
	
	bool is_colliding_with(Collider* other_collider);
	
	Collider(Position2D pos, const char* name, Node2D* parent, Position2D dimensions, Layer_Flags layer, Layer_Flags layer_mask);
	
};








//TEMPLATE FUNCTION DEFINITIONS




template <typename T, typename... Args>
T* Node2D::add_child(Position2D pos, const char* name, Args... constructor_args){
	//make sure target type is derived from Node2D
	static_assert(std::is_base_of<Node2D, T>::value, "T must be derived from Node2D!");
	//make new Object of target type and add it to this nodes children
	pos.x += this->pos.x;
	pos.y += this->pos.y;
	T* new_child = new T(pos, name, this, constructor_args...);
	children.push_back(new_child);
	return new_child;
}

template <typename T> T* Node2D::get_child(int index){
	
	//make sure target type is derived from Node2D
	static_assert(std::is_base_of<Node2D, T>::value, "T must be derived from Node2D!");
	//make sure index is in bounds
	if (index == -1) index = get_child_count()-1;
	assert(index < children.size());
	//cast the child to the target type
	T* child = dynamic_cast<T*>(children[index]);
	//test if the target type is the same as 
	assert(child != nullptr);
	
	return child;
	
	
}
template <typename T> T* Node2D::get_child(const char* name){
	//make sure target type is derived from Node2D
	static_assert(std::is_base_of<Node2D, T>::value, "T must be derived from Node2D!");
	T* child = nullptr;
	for (size_t i = 0; i < children.size(); i++){
		if (children[i]->name == name){
			child = dynamic_cast<T*>(children[i]);
		}
	}
	return child;
	
	
	
}
#endif