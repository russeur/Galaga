#include "Window.hpp"




Window::Window(int width, int height, const char* title){
	
	//set up glfw
	if (!glfwInit()) {
		std::cout<< "GLFW could not start\n";
		return ;
	}
	//create window
	window_pntr = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height, title, NULL, NULL);
	
	//make window as target window for all operations in the future
	glfwMakeContextCurrent(window_pntr);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		glfwTerminate();
		return;
	}
	//turn on VSync
	glfwSwapInterval(1);
	//ensure window is correct size
	glfwGetFramebufferSize(window_pntr, &this->width, &this->height);
	std::cout<<"w: "<<this->width<<", h: "<<this->height<<"\n";
	
	glViewport(0.0, 0.0, this->width, this->height);
	
	//enable alpha chanell blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//bind cursor position callback
	glfwSetCursorPosCallback(window_pntr, cursorPositionCallback);//bind
	glfwSetInputMode(window_pntr, GLFW_STICKY_MOUSE_BUTTONS, 1);//ensure all mouse events are proccessed
	//set cursor to normal
	glfwSetInputMode(window_pntr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//cursor is visable like normal
	
	
}



GLFWwindow* Window::get_window(){
	return window_pntr;
}

void Window::update_frame(){
	
	//find fps
	frame_count++;
	double current_time = glfwGetTime();
	if (current_time - last_time >= 1.0d){
		last_fps = frame_count;
		std::cout<<"fps: "<<last_fps<<"\n";
		frame_count = 0;
		last_time = current_time;
		
	}
	
	//update screen
	glfwSwapBuffers(window_pntr);
	//proccess inputs
	glfwPollEvents();
	
	//remove inputs from event buffer
	for (int i = 0; i <= last_key_index; i++){
		keys[i] = -1;
	}
	last_key_index = -1;
}
void Window::clear_window(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::set_clear_color(Color color){
	glClearColor(color.r, color.g, color.b, color.a);
}

bool Window::window_should_close(){
	return glfwWindowShouldClose(window_pntr);
}

int Window::get_mouse_button_state(int target_button){
	return glfwGetMouseButton(window_pntr, target_button);
}

int Window::get_key_button_state(int target_key){
	return glfwGetKey(window_pntr, target_key);
}

Window* Window::init_window(int width, int height, const char* title){
	
	if (static_window == nullptr){
		static_window = new Window(width, height, title);
	}
	
	return static_window;
}
Window* Window::get_static_window(){
	return static_window;
}
int Window::get_fps(){
	return last_fps;
}

Position2D Window::screen_to_UV_pos(Position2D screen_pos){
	return Position2D{(screen_pos.x - width/2.0f) / (width/2.0f), -(screen_pos.y - height/2.0f) / (height/2.0f)};
}

Position2D Window::screen_to_UV_vector(Position2D vector){
	return Position2D{2*vector.x / width, -2*vector.y / height};
}



void Window::terminate(){
	glfwTerminate();
}


void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos){
	Window::get_static_window()->cursor_x = xPos;
	Window::get_static_window()->cursor_y = yPos;
}
