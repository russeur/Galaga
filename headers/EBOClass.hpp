#include <glad/glad.h>

class EBO{
	public:
	
	GLuint ID;
	void ready(GLuint* indices, GLsizeiptr size);
	
	void bind();
	void unbind();
	void deleteEBO();
	
};


