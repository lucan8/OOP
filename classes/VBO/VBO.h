#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VBO{
private:
    GLuint id;
public:
    VBO(GLsizeiptr size, const GLfloat* vertices = nullptr , GLenum usage = GL_DYNAMIC_DRAW);
    ~VBO();
    void update(GLfloat* vertices, GLsizeiptr size);
    void bind() const;
    void unbind() const;

    //Two VBOs can't have the same id(if one gets deleted, the other will be invalid)
    VBO(const VBO& other) = delete;
    //Moving means we now have a VBO with the same id as the other, which is invalid
    VBO(VBO&& other) = delete;
    VBO& operator=(const VBO& other) = delete;
};
