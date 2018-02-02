#ifndef WINDOW_H
#define WINDOW_H

enum E_MOUSE
{
	IDLE = 0,
	LEFT_HOLD = 1,
	RIGHT_HOLD = 2
};

class Window
{
private:

public:
	//Window size properties
	static int Width;
	static int Height;

	//Mouse properties
	static int Mouse_Status;
	static glm::vec3 LastPoint;
	static glm::vec3 CurrPoint;

	//Matrix Coordinate Transformation properties
	static glm::mat4 P;
	static glm::mat4 V;

	//Frame Time calculation
	static float LastFrameTime;
	static float DeltaFrameTime;

	// Window functions
	static GLFWwindow* create_window(int width, int height);

	static void Start();
	static void Stop();

	// Window callbacks
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void frame_time_callback();
	static void display_callback(GLFWwindow*);

	// Keyboard and mouse callback functions
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static void cursor_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
