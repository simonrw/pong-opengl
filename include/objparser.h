#ifndef OBJPARSER_H_
#define OBJPARSER_H_

#include <string>
#include <vector>
#include <OpenGL/gl.h>

struct ObjModel {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indexes;
    std::vector<GLfloat> normals;
};

class ObjParser {
    public:
        ObjModel model(const std::string &filename);
};

#endif //  OBJPARSER_H_
