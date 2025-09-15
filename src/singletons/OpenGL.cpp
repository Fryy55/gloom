#include "OpenGL.hpp"

#include "prelude.hpp"

#include <GLFW/glfw3.h>

#include <print>
#include <iostream>

using namespace gloom::prelude;


OpenGL* OpenGL::get() noexcept {
	static OpenGL instance;

	return &instance;
}


// Init/Termination
bool OpenGL::init() const noexcept {
	if (!glfwInit()) {
		std::print(std::cerr, "Failed to initialize GLFW.");
		return false;
	}

	return true;
}

void OpenGL::terminate() const noexcept {
	glfwTerminate();

	return;
}

OpenGL::Version OpenGL::getGLFWVersion() const noexcept {
	Version ver;
	glfwGetVersion(&ver.major, &ver.minor, &ver.patch);

	return ver;
}

// Hints
void OpenGL::resetWindowHints() const noexcept {
	glfwDefaultWindowHints();

	return;
}