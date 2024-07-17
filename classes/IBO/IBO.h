#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IBO{
private:
    GLuint id;
    GLsizei count;
public:
    IBO(GLuint* indices, GLsizei count);
    void bind() const;
    void unbind() const;

    GLsizei getCount() const{ return count;}
    ~IBO();
};