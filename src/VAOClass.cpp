#include "VAOClass.hpp"
#include <iostream>

VAO::VAO(){
	glGenVertexArrays(1, &ID);
	
}
void VAO::linkAttrib(VBO& targetVBO, GLuint layout, GLenum type, GLint size, GLsizei stride, const void* pointer){
	targetVBO.bind();
	
	if (type == GL_FLOAT){
		
		glVertexAttribPointer(layout, size, type, GL_FALSE, stride, pointer);
	}else if (type == GL_INT){
		glVertexAttribIPointer(layout, size, type, stride, pointer);
	}else{
		std::cout<<"VAO NOT PREPARED FOR ATTRIB TYPE: "<<type<<"\n";
	}
	glEnableVertexAttribArray(layout);
	
	targetVBO.unbind();
}

void VAO::bind(){
	
	glBindVertexArray(ID);
}
void VAO::unbind(){
	glBindVertexArray(0);
}
void VAO::deleteVAO(){
	
	glDeleteVertexArrays(1, &ID);
}
