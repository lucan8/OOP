#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IBO{
private:
    GLuint id;
    GLsizei count;
public:
    IBO(GLuint* indices, GLsizei count);
    ~IBO();

    void bind() const;
    void unbind() const;
    GLsizei getCount() const{ return count;}

    //Two IBOs can't have the same id(if one gets deleted, the other will be invalid)
    IBO(const IBO& other) = delete;
    //Moving means we now have a IBO with the same id as the other, which is invalid
    IBO(IBO&& other) = delete;
    IBO& operator=(const IBO& other) = delete;
};