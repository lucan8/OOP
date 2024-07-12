#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VBO{
private:
    GLuint id;
public:
    VBO(GLfloat* vertices, GLsizeiptr size);
    void bind() const;
    void unbind() const;
    ~VBO();
};
