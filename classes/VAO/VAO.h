#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class VAO{
private:
    GLuint id;
public:
    VAO();
    ~VAO();
    void bind() const;
    void unbind() const;
};