#include "shaderClass.hpp"


std::string get_file_contents(const char* file_path){
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;
	file.open(file_path);
	if (!file.is_open()){
		std::cout<<"ERROR: file '"<<file_path<<"' could not be found\n";
	}
	while(std::getline(file, line)){
		
		bufferedLines << line << "\n";
		
		
	}
	
	std::string shaderSource = bufferedLines.str();
	file.close();
	return shaderSource;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath){
	//vertex shader
	
	
	std::string vertexString = get_file_contents(vertexPath);
	const char* vertexSrc = vertexString.c_str();
	
	unsigned int vertexModule = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexModule, 1, &vertexSrc, NULL);
	glCompileShader(vertexModule);
	
	int vert_success;
	glGetShaderiv(vertexModule, GL_COMPILE_STATUS, &vert_success);
	if (!vert_success){
		char errorLog[1024];
		glGetShaderInfoLog(vertexModule, 1024, NULL, errorLog);
		std::cout<<"Vertex Module compilation error in '"<<vertexPath<<"': \n"<<errorLog<<"\n";
		exit(0);
	}
	
	
	//fragment shader
	std::string fragmentString = get_file_contents(fragmentPath);
	const char* fragmentSrc = fragmentString.c_str();
	
	unsigned int fragmentModule = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(fragmentModule, 1, &fragmentSrc, NULL);
	glCompileShader(fragmentModule);
	
	int frag_success;
	glGetShaderiv(fragmentModule, GL_COMPILE_STATUS, &frag_success);
	if (!frag_success){
		char errorLog[1024];
		glGetShaderInfoLog(fragmentModule, 1024, NULL, errorLog);
		std::cout<<"Fragment Module compilation error in '"<<fragmentPath<<"': \n"<<errorLog<<"\n";
		exit(0);
	}
	
	//shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexModule);
	glAttachShader(ID, fragmentModule);
	
	glLinkProgram(ID);
	
	int shader_success;
	glGetProgramiv(ID, GL_LINK_STATUS, &shader_success);
	if (!shader_success){
		char errorLog[1024];
		GLsizei length_error;
		glGetProgramInfoLog(ID, 1024, &length_error, &errorLog[0]);
		
		std::cout<<"Shader linking compilation error in '"<<vertexPath<<"': \n"<<errorLog<<"(size: "<<length_error<<")"<<"\n";
		exit(0);
	}
	glDeleteShader(vertexModule);
	glDeleteShader(fragmentModule);
	
	
	
}


void Shader::activate(){
	glUseProgram(ID);
}
void Shader::deleteShader(){
	glDeleteProgram(ID);
}
