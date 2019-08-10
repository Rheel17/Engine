#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Engine.h"
#include "../PerspectiveCamera.h"

namespace rheel {

bool Window::_is_glfw_initialized = false;

inline static GLFWwindow *getWindow(void *handle) {
	return static_cast<GLFWwindow *>(handle);
}

static void glfw_KeyCallback(GLFWwindow *glfw_window, int key, int scancode, int action, int mods);
static void glfw_CharCallback(GLFWwindow *glfw_window, unsigned int codepoint);
static void glfw_MouseMoveCallback(GLFWwindow *glfw_window, double xpos, double ypos);
static void glfw_MouseButtonCallback(GLFWwindow *glfw_window, int button, int action, int mods);
static void glfw_ScrollCallback(GLFWwindow *glfw_window, double x, double y);

static void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar *message, const void *userParam);

Window::Window(DisplayConfiguration configuration) :
		_configuration(configuration) {}

Window::~Window() {
	if (_window_handle) {
		GLFWwindow *window = getWindow(_window_handle);
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

void Window::Show() {
	// apply the window parameters
	glfwDefaultWindowHints();
	GLFWmonitor *monitor = nullptr;

	switch (_configuration.window_mode) {
		case DisplayConfiguration::WindowMode::FULLSCREEN:
			glfwWindowHint(GLFW_RESIZABLE, false);
			monitor = glfwGetPrimaryMonitor();
			break;
		case DisplayConfiguration::WindowMode::WINDOWED_UNRESIZABLE:
			glfwWindowHint(GLFW_RESIZABLE, false);
			break;
		default:
			throw std::runtime_error("window mode not supported by OpenGLWindow.");
	}

	glfwWindowHint(GLFW_SAMPLES, _configuration.SampleCount());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef RE_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	// create the window and set the window handle for later user
	GLFWwindow *window = glfwCreateWindow(
			_configuration.resolution.width,
			_configuration.resolution.height,
			_configuration.title.c_str(),
			monitor, nullptr);

	_window_handle = window;

	// initialize callbacks
	glfwSetKeyCallback(window, glfw_KeyCallback);
	glfwSetCharCallback(window, glfw_CharCallback);
	glfwSetCursorPosCallback(window, glfw_MouseMoveCallback);
	glfwSetMouseButtonCallback(window, glfw_MouseButtonCallback);
	glfwSetScrollCallback(window, glfw_ScrollCallback);

	// enable or disable vsync
	if (!_configuration.vsync) {
		glfwSwapInterval(0);
	}

	// initialize OpenGL
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW.");
	}

#ifdef RE_DEBUG
	// initialize OpenGL debugging
	GLint debugFlags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &debugFlags);

	if (debugFlags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Window::Loop() {
	GLFWwindow *window = getWindow(_window_handle);

	// run the window
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Engine::UpdateScenes();
		Engine::GetUI().Draw();

		glfwSwapBuffers(window);
	}
}

void Window::InitializeDisplaySystems() {
	// initialize GLFW
	if (!_is_glfw_initialized) {
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW.");
		}

		_is_glfw_initialized = true;
	}
}

void Window::DestroyDisplaySystems() {
	// terminate GLFW
	if (_is_glfw_initialized) {
		glfwTerminate();
	}
}

static void glfw_KeyCallback(GLFWwindow *glfw_window, int key, int scancode, int action, int mods) {
	Engine::GetUI().OnKey(static_cast<Input::Key>(key), scancode, static_cast<Input::Action>(action), mods);
}

static void glfw_CharCallback(GLFWwindow *glfw_window, unsigned int codepoint) {
	Engine::GetUI().OnCharacter(codepoint);
}

static void glfw_MouseMoveCallback(GLFWwindow *glfw_window, double xpos, double ypos) {
	Engine::GetUI().OnMouseMove(xpos, ypos);
}

static void glfw_MouseButtonCallback(GLFWwindow *glfw_window, int button, int action, int mods) {
	Engine::GetUI().OnMouseButton(static_cast<Input::MouseButton>(button), static_cast<Input::Action>(action), mods);
}

static void glfw_ScrollCallback(GLFWwindow *glfw_window, double x, double y) {
	Engine::GetUI().OnScroll(x, y);
}

static void glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar *message, const void *userParam) {

	if (id == 131169 || id == 131204) {
		return;
	}

	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
		return;
	}

	std::cout << "[Debug][OpenGL] " << id << ": ";

	switch (source) {
		case GL_DEBUG_SOURCE_API:			  std::cout << "source=API"; 			 break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "source=WINDOW_SYSTEM";   break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "source=SHADER_COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:	  std::cout << "source=THIRD_PARTY";	 break;
		case GL_DEBUG_SOURCE_APPLICATION:	  std::cout << "source=APPLICATION";	 break;
		case GL_DEBUG_SOURCE_OTHER:			  std::cout << "source=OTHER";			 break;
	}

	std::cout << ", ";

	switch (type) {
		case GL_DEBUG_TYPE_ERROR: 				std::cout << "type=ERROR"; 				 break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "type=DEPRECATED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: 	std::cout << "type=UNDEFINED_BEHAVIOR";  break;
		case GL_DEBUG_TYPE_PORTABILITY: 		std::cout << "type=PORTABILITY";		 break;
		case GL_DEBUG_TYPE_PERFORMANCE:		 	std::cout << "type=PERFORMANCE";		 break;
		case GL_DEBUG_TYPE_MARKER: 				std::cout << "type=MARKER";				 break;
		case GL_DEBUG_TYPE_PUSH_GROUP: 			std::cout << "type=PUSH_GROUP";			 break;
		case GL_DEBUG_TYPE_POP_GROUP: 			std::cout << "type=POP_GROUP";			 break;
		case GL_DEBUG_TYPE_OTHER: 				std::cout << "type=OTHER";				 break;
	}

	std::cout << ", ";

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "severity=HIGH";         break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "severity=MEDIUM";       break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "severity=LOW";          break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "severity=NOTIFICATION"; break;
	}

	std::cout << ", " << std::string(message, length) << std::endl;
}

}
