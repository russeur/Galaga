#include <glad/glad.h>

#ifndef VBO_CLASS_HPP
#define VBO_CLASS_HPP
#include "VBOClass.hpp"
#endif


class VAO{
	GLuint ID;
	public:
	
	
	VAO();
	void linkAttrib(VBO& targetVBO, GLuint layout, GLenum type, GLint size, GLsizei stride, const void* pointer);
	
	void bind();
	void unbind();
	void deleteVAO();
};
