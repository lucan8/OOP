#pragma once
#include "../VB_layout/VertexBufferLayout.h"
#include "../VBO/VBO.h"
class VAO{
private:
    GLuint id;
public:
    VAO();
    ~VAO();
    void addBuffer(const VBO& vbo, const VertexBufferLayout& layout);
    void bind() const;
    void unbind() const;
};