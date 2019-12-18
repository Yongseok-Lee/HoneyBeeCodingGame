#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameApp.h"
#include "SceneManager.h"
#include "Interpolator.h"
#include "SoundManager.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "LoadShaders.h"
#include "LoadTex.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Camera.h"
#include "ShaderManager.h"

extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

void ErrorCallback(int errorCode, const char* errorDescription)
{
	std::cerr<< "Error: "<< errorCode << ", " << errorDescription << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static auto instance = GameApp::GetInstance();
	instance->ProcessKey(key, scancode, action, mods);
}

void CharacterCallback(GLFWwindow* window, unsigned int keyCode)
{
	
}

static bool isClick = false;
static bool isRight = false;
static bool isWheel = false;

static bool joysticAvailable = false;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
	{
		isClick = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		isClick = false;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		isWheel = true;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
	{
		isWheel = false;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		isRight = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		isRight = false;
	}
}

void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	static auto camera = Camera::GetInstance();
	static double mouse_pos[2] = { 0.0 };

	if (isClick)
	{
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		GLfloat dx = 1.f*(xpos - mouse_pos[0]) / w;
		GLfloat dy = -1.f*(ypos - mouse_pos[1]) / h;
		glm::vec4 disp(camera->eye - camera->center, 1);
		GLfloat alpha = 2.0f;
		glm::mat4 V = camera->GetViewing();
		glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), alpha*dy, glm::vec3(transpose(V)[0]));
		glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), -alpha * dx, glm::vec3(0, 1, 0));
		glm::mat4 R = Ry * Rx;
		camera->eye = camera->center + glm::vec3(R*disp);
		camera->up = glm::mat3(R)*camera->up;
	}
	else if (isWheel)
	{
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		GLfloat dx = 1.f*(xpos - mouse_pos[0]) / w;
		GLfloat dy = -1.f*(ypos - mouse_pos[1]) / h;
		glm::mat4 VT = glm::transpose(camera->GetViewing());
		camera->eye += glm::vec3(dx*VT[0] + dy * VT[1]);
		camera->center += glm::vec3(dx*VT[0] + dy * VT[1]);
	}
	mouse_pos[0] = xpos;
	mouse_pos[1] = ypos;
}

void MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	
}

GameApp::GameApp() : _width(1200), _height(800)
{
	glfwSetErrorCallback(ErrorCallback);
	if (!glfwInit()) {
		std::cerr << "Error: GLFW Init failed! " << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	_window = glfwCreateWindow(_width, _height, "Alpha Engine", nullptr, nullptr);
	if (!_window)
		exit(EXIT_FAILURE);

	glfwMakeContextCurrent(_window);

	glewExperimental = GL_TRUE;
	GLenum errorCode = glewInit();
	if (GLEW_OK != errorCode) {
		std::cerr << "Error: GLEW Init failed! " << glewGetErrorString(errorCode) << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	_isExit = false;
	_interpolator = Interpolator::GetInstance();
	_sceneManager = SceneManager::GetInstance();
	_soundManager = SoundManager::GetInstance();
}

GameApp::~GameApp()
{
}

int GameApp::GameLoop()
{
	Init();
	while (!_isExit)
	{
		Update();
		Render();
	}
	Release();
	return 0;
}

void GameApp::Init()
{
	glfwSetKeyCallback(_window, KeyCallback);
	glfwSetCharCallback(_window, CharacterCallback);
	glfwSetMouseButtonCallback(_window, MouseButtonCallback);
	glfwSetScrollCallback(_window, MouseScrollCallback);
	glfwSetCursorPosCallback(_window, MouseCursorPosCallback);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);

	/// Imgui Init
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	ImFont* font = io.Fonts->AddFontFromFileTTF("../Assets/malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	IM_ASSERT(font != NULL);
	io.Fonts->AddFontDefault();
	/// 

	Camera::GetInstance()->Init();
	_interpolator->Init();
	_soundManager->Init();
	_sceneManager->Init();
}

void GameApp::ProcessKey(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, GLFW_TRUE);

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_F1:
			ShaderManager::GetInstance()->SetShader(ShaderManager::Phong);
			std::cout << "Phong Shader Mode" << std::endl;
			break;
		case GLFW_KEY_F2:
			ShaderManager::GetInstance()->SetShader(ShaderManager::Gouraud);
			std::cout << "Gouraud Shader Mode" << std::endl;
			break;
		case GLFW_KEY_F3:
			ShaderManager::GetInstance()->SetShader(ShaderManager::Flat);
			std::cout << "Flat Shader Mode" << std::endl;
			break;
		}
		
	}
}

void GameApp::Update()
{
	glfwPollEvents();
	_isExit = glfwWindowShouldClose(_window);

	_isClick = isClick;
	
	glfwGetCursorPos(_window, &_mouseX, &_mouseY);
	//_mouseX

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		static auto camera = Camera::GetInstance();

		int count;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
			//std::cout << axes[i] << std::endl;
			// 0,1 => 왼쪽 아날로그 조이스틱
			// 2,3 => 오른쪽 아날로그 조이스틱

		float x = abs(axes[2]) > 0.1 ? axes[2] : 0.0f;		// x는 오른쪽이 +임.
		float y = abs(axes[3]) > 0.1 ? axes[3] : 0.0f;		// y는 위로 올리면 -임.
		
		/*
		int w, h;
		glfwGetWindowSize(_window, &w, &h);
		GLfloat dx = 1.f*(xpos - mouse_pos[0]) / w;
		GLfloat dy = -1.f*(ypos - mouse_pos[1]) / h;
		GLfloat alpha = 2.0f;*/
		glm::vec4 disp(camera->eye - camera->center, 1);
		glm::mat4 V = camera->GetViewing();
		glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), 0.05f*y, glm::vec3(transpose(V)[0]));
		glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), 0.05f*x, glm::vec3(0, 1, 0));
		glm::mat4 R = Ry * Rx;
		camera->eye = camera->center + glm::vec3(R*disp);
		camera->up = glm::mat3(R)*camera->up;
	}
	
	_interpolator->Update();
	_sceneManager->Update();
	_soundManager->Update();

#ifdef _DEBUG
	static int fps = 0;
	fps++;
	static clock_t time = 0;
	if (clock() - time > 1000)
	{
		std::cout << "fps: " << fps << std::endl;
		fps = 0;
		time = clock();
	}
#endif
}

void GameApp::Render()
{
	glClearColor(51/255.0f, 204 / 255.0f, 255 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ShaderManager::GetInstance()->GetShader());
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	_sceneManager->Render();

	glDisable(GL_POLYGON_OFFSET_FILL);

	glFlush();

	glfwSwapBuffers(_window);
}

void GameApp::Release()
{
	_soundManager->Release();
	_sceneManager->Release();
	_interpolator->Release();
	glfwTerminate();
}
