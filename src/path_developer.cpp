#include "path_developer.hpp"


/*
Position2D Path_Developer::find_vel(Position2D pos, Position2D vel, int& destination_point){
	
	Position2D delta_to_dest = Position2D{pos.x - path_points[destination_point].x, pos.y - path_points[destination_point].y};
	GLfloat dist_to_dest = sqrt((delta_to_dest.x * delta_to_dest.x) + (delta_to_dest.y * delta_to_dest.y));
	
	
	if (dist_to_dest <= speed * 1.5){
		destination_point++;
	}
	
	Position2D actual_vel = Position2D{(delta_to_dest.x / dist_to_dest) * speed, (delta_to_dest.y / dist_to_dest) * speed};
	
	float current_influence = (dist_to_dest * smoothness) / speed;
	
	return Position2D{(vel.x * current_influence + actual_vel.x)/(current_influence+1), (vel.y*current_influence + actual_vel.y)/(current_influence+1)};
}
*/
void Path_Developer::add_point(Position2D pos){
	
	if (path_points.size() == 0){
		path_points.push_back(Position2D{0.0, 0.0});
		
	}else{
		
		path_points.push_back(pos);
		
	}
	
	update_curve();
	
	point_renderer.new_rect(window->screen_to_UV_pos(Position2D{pos.x - point_size/2, pos.y - point_size/2}), window->screen_to_UV_vector(Position2D{point_size, point_size}), (path_points.size() != 1));
	
}
void Path_Developer::insert_point(Position2D pos, int index){
	path_points.insert(path_points.begin()+index, pos);
	update_curve();
}

void Path_Developer::remove_point(int index){
	path_points.erase(path_points.begin() + index);
	update_curve();
}
void Path_Developer::remove_point(Position2D pos){
	
	for (size_t i = 0; i < path_points.size(); i++){
		
		if (path_points[i].x == pos.x && path_points[i].y == pos.y){
			remove_point(i);
		}
	}
}
void Path_Developer::draw_curve(Position2D offset, bool is_selected){
	
	if (path_points.size() < 1) return;
	if (is_selected){
		point_renderer.zoom = zoom;
		point_renderer.ready_for_draw();
		point_renderer.set_uniform("color", color.x, color.y, color.z);
		Position2D scaled_offset = window->screen_to_UV_vector(offset);
		point_renderer.set_uniform("offset", scaled_offset.x, scaled_offset.y);
		point_renderer.draw_all();
	}
	
	
	if (path_points.size() < 2) return;
	
	curve_shader.activate();
	
	glUniform1f(glGetUniformLocation(curve_shader.ID, "zoom"), zoom);
	glUniform2f(glGetUniformLocation(curve_shader.ID, "window_size"), window->width, window->height);
	
	glUniform2f(glGetUniformLocation(curve_shader.ID, "offset"), offset.x, offset.y);
	
	if (is_selected){
		glUniform3f(glGetUniformLocation(curve_shader.ID, "color"), selected_color.x, selected_color.y, selected_color.z);
	}else{
		glUniform3f(glGetUniformLocation(curve_shader.ID, "color"), color.x, color.y, color.z);
	}
	
	
	
	curve_VAO1.bind();
	curve_VBO1.bind();
	curve_EBO1.bind();
	
	glBufferData(GL_ARRAY_BUFFER, line_points.size()*sizeof(Position2D), line_points.data(), GL_DYNAMIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, rect_indices.size()*sizeof(GLuint), rect_indices.data(), GL_DYNAMIC_DRAW);
	
	//glDrawElements(GL_TRIANGLES, rect_indices.size(), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINE_STRIP, 0, line_points.size());
	
	curve_VAO1.unbind();
	curve_VBO1.unbind();
	curve_EBO1.unbind();
	
	return;
	
	
}

void Path_Developer::update_curve(){
	line_points.clear();
	if (path_points.size() < 2) return;
	
	
	Position2D last_point = path_points[0];
	line_points.push_back(last_point);
	Position2D last_vel = Position2D{0, 0};
	
	for (int destination_point = 1; destination_point < path_points.size(); ){
		
		last_vel = find_vel(last_point, last_vel, destination_point, 30);
		
		last_point = Position2D{last_point.x - last_vel.x, last_point.y - last_vel.y};
		
		line_points.push_back(last_point);
		//std::cout<<last_point.x<<", "<<last_point.y<<"\n";
	}
	
	
	
	
}
float Path_Developer::distance_from(Position2D pos){
	float shortest_dist = 10000.0;
	for (size_t p = 0; p < line_points.size(); p++){
		Position2D delta = Position2D{pos.x - line_points[p].x, pos.y - line_points[p].y};
		float current_dist = sqrt((delta.x * delta.x) + (delta.y * delta.y));
		
		if (shortest_dist > current_dist){
			
			shortest_dist = current_dist;
		}
	
	}
	
	return shortest_dist;
}
Path_Developer::Path_Developer(float smoothness, float speed, Window* window) : Path(smoothness, speed), point_renderer("../src/shaders/path_points_vert.txt", "../src/shaders/path_points_frag.txt"), curve_shader("../src/shaders/path_curve_vert.txt", "../src/shaders/path_curve_frag.txt"){
	
	point_renderer.start_init();
	point_renderer.link_attribute(1, GL_INT, 1, offsetof(Path_Point_Vert, point_location));
	point_renderer.finish_init();
	
	
	this->window = window;
	
	curve_VAO1.bind();
	curve_VBO1.ready(line_points.data(), 20*sizeof(Position2D));
	//EBO1.ready(rect_indices.data(), 20*sizeof(GLuint));
	
	//EBO1.bind();
	
	curve_VAO1.linkAttrib(curve_VBO1, 0, GL_FLOAT, 2, sizeof(Position2D), (void*)offsetof(Position2D, x));//position
	
	curve_VAO1.unbind();
	curve_VBO1.unbind();
	//EBO1.unbind();
	
	
	
	
}
void Path_Developer_Manager::add_point_to_spawner(int target_bug_spawner, Position2D pos){// adds point to end of path
	if (target_bug_spawner >= bug_spawners.size()) return;
	
	
	add_point_to_path(bug_spawners[target_bug_spawner]->target_path, pos - bug_spawners[target_bug_spawner]->offset);
	
	
}
void Path_Developer_Manager::insert_point_to_spawner(int target_bug_spawner, Position2D pos, int index){
	if (target_bug_spawner >= bug_spawners.size()) return;
	
	//paths[target_path]->insert_point(pos, index);
	
}
void Path_Developer_Manager::remove_point_to_spawner(int target_bug_spawner, int index){
	if (target_bug_spawner >= bug_spawners.size()) return;
	
	//paths[target_path]->remove_point(index);
}
void Path_Developer_Manager::remove_point_to_spawner(int target_bug_spawner, Position2D pos){
	if (target_bug_spawner >= bug_spawners.size()) return;
	
	//paths[target_path]->remove_point(pos);
}

void Path_Developer_Manager::add_point_to_path(int target_path, Position2D pos){// adds point to end of path
	if (target_path >= paths.size()) return;
	
	paths[target_path]->add_point(pos);
	
}

Position3D Path_Developer_Manager::get_unique_color(){
	
	bool unused_colors[unique_color_count];
	for (int i = 0; i<unique_color_count; i++){
		unused_colors[i] = true;
	}
	
	for (int p = 0; p < paths.size(); p++){
		for (int i = 0; i<unique_color_count; i++){
			if (unique_colors[i].x == paths[p]->color.x && unique_colors[i].y == paths[p]->color.y && unique_colors[i].z == paths[p]->color.z){
				unused_colors[i] = false;
				break;
			}
		}
	}
	
	
	for (int i = 0; i<unique_color_count; i++){
		
		if (unused_colors[i]){
			return unique_colors[i];
		}
	}
	
	return unique_colors[rand() % unique_color_count];
}
void Path_Developer_Manager::add_path(Position3D color, float smoothness, float speed){
	
	paths.push_back(new Path_Developer(smoothness, speed, window));
	
	paths[paths.size()-1]->color = color;
	paths[paths.size()-1]->zoom = zoom;
	
	
	
	
}
void Path_Developer_Manager::add_path(Position3D color, float smoothness, float speed, Position2D* points, int number_of_points){
	add_path(color, smoothness, speed);
	
	for (int i = 0; i < number_of_points; i++){
		add_point_to_path(paths.size()-1, points[i]);
	}
	
}

void Path_Developer_Manager::add_spawner(int target_path, int max_enemy_count, float spawn_interval, BugType bug_type, Position2D offset){
bug_spawners.push_back(new Bug_Spawner_Dev{target_path, max_enemy_count, spawn_interval, bug_type, offset});
}

void Path_Developer_Manager::destroy_spawner(int target_spawner){
	if (target_spawner == -1) return;
	delete bug_spawners[target_spawner];
	bug_spawners.erase(bug_spawners.begin() + target_spawner);
	update_selected_spawner(-1);
}
	
void Path_Developer_Manager::destroy_all_paths(){
	for (int i = 0; i < paths.size(); i++){
		delete paths[i];
	}
	paths.clear();
}
void Path_Developer_Manager::destroy_all_spawners(){
	for (int i = 0; i < bug_spawners.size(); i++){
		delete bug_spawners[i];
	}
	bug_spawners.clear();
}

void Path_Developer_Manager::draw_all_paths(){
	for (size_t i = 0; i < bug_spawners.size(); i++){
		paths[bug_spawners[i]->target_path]->draw_curve(bug_spawners[i]->offset, (i == selected_spawner));
	}
}
void Path_Developer_Manager::select_new_spawner(Position2D pos){
	
	
	float closest_dist = 10000.0;
	int closest_path = -1;
	
	for (int s = 0; s < bug_spawners.size(); s++){
		float current_dist = paths[bug_spawners[s]->target_path]->distance_from(pos - bug_spawners[s]->offset);
		
		if (current_dist < path_dist_thres && current_dist < closest_dist){
			closest_dist = current_dist;
			closest_path = s;
		}
	}
	
	
	
	update_selected_spawner(closest_path);
	
}
int Path_Developer_Manager::get_selected_spawner(){
	return selected_spawner;
}
void Path_Developer_Manager::update_selected_spawner(int new_selection){
	selected_spawner = new_selection;
	
	
}
void Path_Developer_Manager::modify_zoom(float delta){
	zoom += delta*zoom;
	
	if (zoom < .005){
		zoom = .005;
	}
	for (size_t i = 0; i < paths.size(); i++){
		paths[i]->zoom = zoom;
		
	}
}
void Path_Developer_Manager::reset_zoom(){
	zoom = 1.0f;
	for (size_t i = 0; i < paths.size(); i++){
		paths[i]->zoom = zoom;
		
	}
}
Path_Developer_Manager::Path_Developer_Manager(Window* window){
	
	this->window = window;
	
	
	
	
}

Path_Developer_Manager::~Path_Developer_Manager(){
	destroy_all_paths();
	destroy_all_spawners();
}

