#include "VBOClass.hpp"
void VBO::ready(void* vertices, GLsizeiptr size){
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

void VBO::bind(){
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::unbind(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::deleteVBO(){
	glDeleteBuffers(1, &ID);
}