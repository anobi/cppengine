#ifndef GLUTILS_H
#define GLUTILS_H

#ifdef __APPLE__
//that shit doesnt work
//#include <OpenGL/gl3.h>
#include <GL/glew.h>
#else
#include <GL/glew.h>
#endif


class GLUtils {
public:
	static GLuint LoadShader(const char* vertex_shader_path, const char* fragment_shader_path);
};

#endif // GLUTILS_H