#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VBO{
private:
    GLuint id;
public:
    VBO(const GLfloat* vertices, GLsizeiptr size, GLenum usage);

    void update(GLfloat* vertices, GLsizeiptr size);
    void bind() const;
    void unbind() const;
    ~VBO();
};
