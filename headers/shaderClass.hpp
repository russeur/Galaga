#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* file_path);

class Shader{
	public:
	GLuint ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	
	void activate();
	void deleteShader();


};

