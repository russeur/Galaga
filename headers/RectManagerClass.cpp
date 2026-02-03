//do not include RectManagerClass.hpp because that file includes this in order to define all template classes in the hpp folder.

template <typename VertType>
VertType& Rect_Manager<VertType>::get_vertex(RectangleID ID, int point_index){
	int rect_index = *ID;
	return rect_vert_data[rect_index + point_index];
}
template <typename VertType>
void Rect_Manager<VertType>::set_rect_pos(RectangleID ID, GLfloat x, GLfloat y){
	int index = *ID;
	
	GLfloat w = rect_vert_data[index + 3].x - rect_vert_data[index].x;
	GLfloat h = rect_vert_data[index + 3].y - rect_vert_data[index].y;
	
	
	rect_vert_data[index].x = x;
	rect_vert_data[index].y = y;
	
	rect_vert_data[index+1].x = x+w;
	rect_vert_data[index+1].y = y;
	
	rect_vert_data[index+2].x = x;
	rect_vert_data[index+2].y = y+h;
	
	rect_vert_data[index+3].x = x+w;
	rect_vert_data[index+3].y = y+h;
}
template <typename VertType>
void Rect_Manager<VertType>::set_rect_pos(RectangleID ID, Position2D new_pos){
	this->set_rect_pos(ID, new_pos.x, new_pos.y);
}
template <typename VertType>
void Rect_Manager<VertType>::move_rect(RectangleID ID, GLfloat dx, GLfloat dy){
	int index = *ID;
	rect_vert_data[index].x += dx;
	rect_vert_data[index].y += dy;
	
	rect_vert_data[index+1].x += dx;
	rect_vert_data[index+1].y += dy;
	
	rect_vert_data[index+2].x += dx;
	rect_vert_data[index+2].y += dy;
	
	rect_vert_data[index+3].x += dx;
	rect_vert_data[index+3].y += dy;
}
template <typename VertType>
void Rect_Manager<VertType>::move_rect(RectangleID ID, Position2D delta){
	this->move_rect(ID, delta.x, delta.y);
}
template <typename VertType>
void Rect_Manager<VertType>::destroy_rect(RectangleID ID){
	int index = *ID;
	
	
	auto ID_iter = rect_start_indices.begin();
	for (; ID_iter != rect_start_indices.end(); ID_iter++){
		if (*ID_iter == index){
			break;
		}
	}
	
	
	
	
	
	rect_start_indices.erase(ID_iter);
	
	auto it = rect_start_indices.begin();
	
	
	rect_vert_data.erase(rect_vert_data.begin() + index, rect_vert_data.begin() + index + 4);
	indices.erase(indices.begin() + index, indices.begin() + index + 6);
	
	
	it = ID_iter;
	for (; it != rect_start_indices.end(); it++){
		(*it)-=4;
	}
	
	
	for (size_t i = index; i < indices.size(); i++){
		indices[i] -= 4;
	}
}



template <typename VertType>
Rect_Manager<VertType>::Rect_Manager(){
	static_assert(std::is_base_of<RectangleVert, VertType>::value, "VertType must be derived from RectangleVert!");
	
	
}
template <typename VertType>
Rect_Manager<VertType>::~Rect_Manager(){
	
}


template <typename VertType>
void Render_Rect_Manager<VertType>::ready_for_draw(){
	
	
	
	
	
	if (prepared_to_draw || !finished_init)return;
	
	shader.activate();
	
	glUniform1f(glGetUniformLocation(shader.ID, "zoom"), zoom);
	//set_uniform("zoom", zoom);
	
	prepared_to_draw = true;
	
}


template <typename VertType>
void Render_Rect_Manager<VertType>::draw_all(){
	
	
	if (!prepared_to_draw || !finished_init){
		std::cout<<"not ready to draw_all\n";
		return;
	}
	
	
	
	
	
	VAO1.bind();
	
	VBO1.bind();
	
	EBO1.bind();
	
	glBufferData(GL_ARRAY_BUFFER, this->rect_vert_data.size()*sizeof(VertType), this->rect_vert_data.data(), GL_DYNAMIC_DRAW);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()*sizeof(GLuint), this->indices.data(), GL_DYNAMIC_DRAW);
	
	
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	
	VAO1.unbind();
	VBO1.unbind();
	EBO1.unbind();
	prepared_to_draw = false;
	
	
	
	/*
	shader.activate();
	
	glUniform1f(glGetUniformLocation(shader.ID, "zoom"), zoom);
	
		
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
	
	glUniform1i(glGetUniformLocation(shader.ID, "textures"), 0);
	
	
	
	VAO1.bind();
	VBO1.bind();
	EBO1.bind();
	glBufferData(GL_ARRAY_BUFFER, texture_vert_data.size()*sizeof(TextureVert), texture_vert_data.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);
	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	VAO1.unbind();
	VBO1.unbind();
	EBO1.unbind();
	*/
}



template <typename VertType>
void Render_Rect_Manager<VertType>::set_uniform(const char* uniform_name, GLint value){
	if (!finished_init) return;
	glUniform1i(glGetUniformLocation(shader.ID, uniform_name), value);
}
template <typename VertType>
void Render_Rect_Manager<VertType>::set_uniform(const char* uniform_name, GLint value_a, GLint value_b){
	if (!finished_init) return;
	glUniform2i(glGetUniformLocation(shader.ID, uniform_name), value_a, value_b);
}
template <typename VertType>
void Render_Rect_Manager<VertType>::set_uniform(const char* uniform_name, GLint value_a, GLint value_b, GLint value_c){
	if (!finished_init) return;
	glUniform3i(glGetUniformLocation(shader.ID, uniform_name), value_a, value_b, value_c);
}
template <typename VertType>
void Render_Rect_Manager<VertType>::set_uniform(const char* uniform_name, GLfloat value){
	if (!finished_init) return;
	glUniform1f(glGetUniformLocation(shader.ID, uniform_name), value);
}
template <typename VertType>
void Render_Rect_Manager<VertType>::set_uniform(const char* uniform_name, GLfloat value_a, GLfloat value_b){
	if (!finished_init) return;
	glUniform2f(glGetUniformLocation(shader.ID, uniform_name), value_a, value_b);
}
template <typename VertType>
void Render_Rect_Manager<VertType>::set_uniform(const char* uniform_name, GLfloat value_a, GLfloat value_b, GLfloat value_c){
	if (!finished_init) return;
	glUniform3f(glGetUniformLocation(shader.ID, uniform_name), value_a, value_b, value_c);
}


template <typename VertType>
void Render_Rect_Manager<VertType>::modify_zoom(float delta){
	if (!finished_init) return;
	zoom += delta*zoom;
	
	if (zoom < .005){
		zoom = .005;
	}
}
template <typename VertType>
void Render_Rect_Manager<VertType>::reset_zoom(){
	zoom = 1.0f;
}
template <typename VertType>
void Render_Rect_Manager<VertType>::link_attribute(int attrib_index, GLenum data_type, GLsizei data_count, size_t offset){
	if (finished_init || !started_init) return;
	
	VAO1.linkAttrib(VBO1, attrib_index, data_type, data_count, sizeof(VertType), (void*)offset);
}
template <typename VertType>
Render_Rect_Manager<VertType>::Render_Rect_Manager(const char* vertex_shader_path, const char* fragment_shader_path) : shader(vertex_shader_path, fragment_shader_path){
	
	
	
}
template <typename VertType>
void Render_Rect_Manager<VertType>::start_init(){
	if (finished_init || started_init) return;
	VAO1.bind();
	VBO1.ready(this->rect_vert_data.data(), 20*sizeof(VertType));
	EBO1.ready(this->indices.data(), 20*sizeof(GLuint));
	
	EBO1.bind();
	
	//VAO1.linkAttrib(VBO1, 0, GL_FLOAT, 3, sizeof(VertType), (void*)offsetof(VertType, x));
	finished_init = false;
	started_init = true;
	
	
	link_attribute(0, GL_FLOAT, 2, offsetof(RectangleVert, x));
	
}
template <typename VertType>
void Render_Rect_Manager<VertType>::finish_init(){
	
	if (finished_init || !started_init) return;
	VAO1.unbind();
	VBO1.unbind();
	EBO1.unbind();
	finished_init = true;
}
template <typename VertType>
Render_Rect_Manager<VertType>::~Render_Rect_Manager(){
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
	
	VAO1.deleteVAO();
	VBO1.deleteVBO();
	EBO1.deleteEBO();
}

