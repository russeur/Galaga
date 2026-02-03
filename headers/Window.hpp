#ifndef WINDOW_CLASS
#define WINDOW_CLASS
#include <glad/glad.h>
#include "../src/config.h"
#include <iostream>
#include <memory>
#include "opengl_buffers.hpp"
#include "TextureUtil.hpp"


struct Color{
	GLfloat r, g, b, a;
};



class Window{
	private:
	inline static Window* static_window = nullptr;
	Window(int width, int height, const char* title);
	protected:
	
	GLFWwindow* window_pntr;
	
	//for user input
	int cursor_inside_window;
	constexpr static int key_buffer_size = 32;
	int last_key_index = -1;
	int keys[key_buffer_size];
	
	//for fps tracking
	double last_time;
	int frame_count = 0;
	int last_fps = 0;
	
	
	
	public:
	int width, height;
	double cursor_x, cursor_y;
	GLFWwindow* get_window();
	void update_frame();//swap frame buffer, poll events
	void clear_window();
	void set_clear_color(Color color);
	bool window_should_close();
	
	int get_mouse_button_state(int target_button);//target_button is equal to a mouse button(like GLFW_MOUSE_BUTTON_LEFT), output is either GLFW_PRESS or GLFW_RELEASE
	int get_key_button_state(int target_key);//target_key is equal to a key button(like GLFW_KEY_LEFT), output is either GLFW_PRESS or GLFW_RELEASE
	
	int get_fps();
	
	Position2D screen_to_UV_pos(Position2D screen_pos); // returns num linearaly normilized(between -1 and 1) by range, will not clip values outside of -1 and 1
	
	Position2D screen_to_UV_vector(Position2D vector);
	
	
	
	void terminate();
	
	
	static Window* init_window(int width, int height, const char* title);
	static Window* get_static_window();
	
	
	
};
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);

#endif