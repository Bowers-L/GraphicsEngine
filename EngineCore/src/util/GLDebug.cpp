#include "GLDebug.h"
#include <glad/glad.h>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		PRINTLN("[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line);
		return false;
	}

	return true;
}