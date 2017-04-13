#ifndef GLERRORS_H_
#define GLERRORS_H_

#include <OpenGL/gl.h>

GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#endif //  GLERRORS_H_
