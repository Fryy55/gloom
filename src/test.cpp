#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gloom.hpp>

#include <print>
#include <iostream>
#include <array>

using namespace gloom::prelude;


void verifyShaderComp(std::uint32_t shader) {
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	char log[256];
	if (!status) {
		glGetShaderInfoLog(shader, sizeof(log) / sizeof(log[0]), nullptr, log);
		std::print(std::cerr, "SHADER COMP ERROR: {}\n", log);
	} else {
		std::print(std::cerr, "Shader {} successfully compiled.\n", shader);
	}

	return;
}

void verifyProgramLink(std::uint32_t program) {
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	char log[256];
	if (!status) {
		glGetProgramInfoLog(program, sizeof(log) / sizeof(log[0]), nullptr, log);
		std::print(std::cerr, "PROGRAM LINK ERROR: {}\n", log);
	} else {
		std::print(std::cerr, "Shader program {} successfully linked.\n", program);
	}

	return;
}

int main() {
	if (!OpenGL::get()->init())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	try {
		OpenGL::get()->setHint<OpenGL::Context_Profile, OpenGL::Profile::Core>();
	} catch (std::exception const& e) {
		std::print("{}", e.what());
	}

	constexpr int winWidth = 800;
	constexpr int winHeight = 600;
	auto window = glfwCreateWindow(winWidth, winHeight, "cool thingy", nullptr, nullptr);
	if (!window) {
		std::print("Failed to create a window");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	std::print("{}\n", glfwGetWindowAttrib(window, GLFW_FOCUS_ON_SHOW));

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::print("Failed to initialize GLAD");
		return 1;
	}
	glViewport(0, 0, winWidth, winHeight);

	// Callbacks
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
		glViewport(0, 0, width, height);
	});
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			switch (key) {
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(window, true);
					break;

				default:
					// i blame msvc
					if (GLFW_KEY_A <= key && key <= GLFW_KEY_Z)
						std::print("{}", static_cast<char>('a' + key - GLFW_KEY_A));
					else if (key == GLFW_KEY_SPACE)
						std::print(" ");
					else if (key == GLFW_KEY_ENTER)
						std::print("\n");

					break;
			}
		}
	});

	std::array vertices{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	std::uint32_t vbo;
	glGenBuffers(1, &vbo);
	std::array indices{
		0, 1, 3,
		1, 2, 3
	};
	std::uint32_t ebo;
	glGenBuffers(1, &ebo);
	std::uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;

		void main() {
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);
		}
	)";
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	verifyShaderComp(vertexShader);

	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	auto fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;

		void main() {
			FragColor = vec4(0.77f, 0.26f, 0.96f, 1.f);
		}
	)";
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	verifyShaderComp(fragmentShader);


	auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glDeleteShader(vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glDeleteShader(fragmentShader);
	glLinkProgram(shaderProgram);
	verifyProgramLink(shaderProgram);

	glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
	glEnableVertexAttribArray(0);


	// Render loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.35f, 0.04f, 0.35f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	OpenGL::get()->terminate();

	return 0;
}