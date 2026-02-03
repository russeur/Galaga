#include "TextureUtil.hpp"


RectangleID Texture_Manager::new_texture_rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLuint texture_index){
	RectangleID rect_ID = Rect_Manager<TextureVert>::new_rect(x, y, w, h, 0.0f, 0.0f, texture_index);
	
	get_vertex(rect_ID, 0).tx = 0.0;
	get_vertex(rect_ID, 0).ty = 0.0;
	
	get_vertex(rect_ID, 1).tx = 1.0;
	get_vertex(rect_ID, 1).ty = 0.0;
	
	get_vertex(rect_ID, 2).tx = 0.0;
	get_vertex(rect_ID, 2).ty = 1.0;
	
	get_vertex(rect_ID, 3).tx = 1.0;
	get_vertex(rect_ID, 3).ty = 1.0;
	
	
	return rect_ID;
	
}
RectangleID Texture_Manager::new_texture_rect(Position2D pos, Position2D dimensions, GLuint texture_index){
	return new_texture_rect(pos.x, pos.y, dimensions.x, dimensions.y, texture_index);
}

void Texture_Manager::set_rect_texture(RectangleID ID, int new_texture_index){
	get_vertex(ID, 0).texture_index = new_texture_index;
	get_vertex(ID, 1).texture_index = new_texture_index;
	get_vertex(ID, 2).texture_index = new_texture_index;
	get_vertex(ID, 3).texture_index = new_texture_index;
}

void Texture_Manager::set_texture(int index, const char* texture_path){
	int imgWidth, imgHeight, imgNumColorChan;
	unsigned char* img_bytes = stbi_load(texture_path, &imgWidth, &imgHeight, &imgNumColorChan, 0);
	std::cout<<imgWidth<<", "<<imgHeight<<"\n";
	
	
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
	
	
	GLenum errorCode;
	if ((errorCode = glGetError()) != GL_NO_ERROR){
		std::string error;
		switch (errorCode){
			case GL_INVALID_ENUM: error = "INVALID ENUM"; break;
			case GL_INVALID_VALUE: error = "INVALID VALUE"; break;
			case GL_INVALID_OPERATION: error = "INVALID OPERATION"; break;
		}
		std::cout<<error<<"\n";
	}
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, imgWidth, imgHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, img_bytes);
	//glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	std::cout<<"index: "<<index<<"\n";
	errorCode;
	if ((errorCode = glGetError()) != GL_NO_ERROR){
		std::string error;
		switch (errorCode){
			case GL_INVALID_ENUM: error = "INVALID ENUM"; break;
			case GL_INVALID_VALUE: error = "INVALID VALUE"; break;
			case GL_INVALID_OPERATION: error = "INVALID OPERATION"; break;
		}
		std::cout<<error<<"\n";
	}
	
	stbi_image_free(img_bytes);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Texture_Manager::set_number_textures(int offset){
	for (int i = 0; i < 10; i++){
		set_texture(offset+i, ("../textures/numbers/"+std::to_string(i)+".png").c_str());
	}
	
	this->number_offset = offset;
}

int Texture_Manager::get_number_offset(){
	return number_offset;
}

void Texture_Manager::draw_all_textures(){
	
	ready_for_draw();
	
	
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
	
	glUniform1i(glGetUniformLocation(shader.ID, "textures"), 0);
	
	
	draw_all();
	
	
}

Texture_Manager::Texture_Manager(int number_of_textures, int dim) : Render_Rect_Manager("../src/shaders/vertex.txt", "../src/shaders/fragment.txt"){
	start_init();
	//VAO1.linkAttrib(VBO1, 1, GL_FLOAT, 2, sizeof(TextureVert), (void*)offsetof(TextureVert, tx));
	//VAO1.linkAttrib(VBO1, 2, GL_INT, 1, sizeof(TextureVert), (void*)offsetof(TextureVert, texture_index));
	link_attribute(1, GL_FLOAT, 2, offsetof(TextureVert, tx));
	link_attribute(2, GL_INT, 1, offsetof(TextureVert, texture_index));
	
	finish_init();
	
	
	//generate and bind to new texture array
	glGenTextures(1, &texture_array);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
	//set how to extend textures
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	
	//
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, dim, dim, number_of_textures, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}
Texture_Manager::~Texture_Manager(){
	
	glDeleteTextures(2, &texture_array);
}

