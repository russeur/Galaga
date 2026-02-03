#include "BarManager.hpp"


RectangleID Bar_Manager::new_bar(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat fill_percent, Position3D color, int direction){
	RectangleID rect_ID = Rect_Manager<BarVert>::new_rect(x, y, w, h, Position2D{0.0, 0.0}, fill_percent, color);
	
	
	switch (direction){
		case 0:
			get_vertex(rect_ID, 2).is_top = Position2D{0.0, 1.0};
			get_vertex(rect_ID, 3).is_top = Position2D{0.0, 1.0};
			break;
		case 1:
			get_vertex(rect_ID, 1).is_top = Position2D{1.0, 0.0};
			get_vertex(rect_ID, 3).is_top = Position2D{1.0, 0.0};
			break;
		case 2:
			get_vertex(rect_ID, 0).is_top = Position2D{0.0, 1.0};
			get_vertex(rect_ID, 1).is_top = Position2D{0.0, 1.0};
			break;
		case 3:
			get_vertex(rect_ID, 0).is_top = Position2D{1.0, 0.0};
			get_vertex(rect_ID, 2).is_top = Position2D{1.0, 0.0};
			break;
	}	
	
	
	
	
	return rect_ID;
	
}
RectangleID Bar_Manager::new_bar(Position2D pos, Position2D dimensions, GLfloat fill_percent, Position3D color, int direction){
	return new_bar(pos.x, pos.y, dimensions.x, dimensions.y, fill_percent, color, direction);
}

void Bar_Manager::set_fill_percent(RectangleID ID, GLfloat fill_percent){
	
	get_vertex(ID, 0).fill_percent = fill_percent;
	
	get_vertex(ID, 1).fill_percent = fill_percent;
	get_vertex(ID, 2).fill_percent = fill_percent;
	get_vertex(ID, 3).fill_percent = fill_percent;
	
}



void Bar_Manager::draw_all_bars(){
	
	ready_for_draw();
	
	draw_all();
	
	
}

Bar_Manager::Bar_Manager() : Render_Rect_Manager("../src/shaders/bar_vert.txt", "../src/shaders/bar_frag.txt"){
	start_init();
	//VAO1.linkAttrib(VBO1, 1, GL_FLOAT, 2, sizeof(BarVert), (void*)offsetof(BarVert, tx));
	//VAO1.linkAttrib(VBO1, 2, GL_INT, 1, sizeof(BarVert), (void*)offsetof(BarVert, texture_index));
	link_attribute(1, GL_FLOAT, 2, offsetof(BarVert, is_top));
	link_attribute(2, GL_FLOAT, 1, offsetof(BarVert, fill_percent));
	link_attribute(3, GL_FLOAT, 3, offsetof(BarVert, color));
	
	finish_init();
}
Bar_Manager::~Bar_Manager(){
	
	
}

