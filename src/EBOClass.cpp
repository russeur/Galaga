#include "EBOClass.hpp"
void EBO::ready(GLuint* indices, GLsizeiptr size){
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
}

void EBO::bind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}
void EBO::unbind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::deleteEBO(){
	glDeleteBuffers(1, &ID);
}