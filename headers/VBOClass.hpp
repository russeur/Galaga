#include <glad/glad.h>

class VBO{
	public:
	
	GLuint ID;
	void ready(void* vertices, GLsizeiptr size);
	
	void bind();
	void unbind();
	void deleteVBO();
	
};


