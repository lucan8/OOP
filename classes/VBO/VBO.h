#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VBO{
private:
    GLuint id;
public:
    VBO(GLsizeiptr size, const GLfloat* vertices = nullptr , GLenum usage = GL_DYNAMIC_DRAW);

    void update(GLfloat* vertices, GLsizeiptr size);
    void bind() const;
    void unbind() const;
    ~VBO();
};
