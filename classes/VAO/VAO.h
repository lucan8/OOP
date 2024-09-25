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

    //Two VAOs can't have the same id(if one gets deleted, the other will be invalid)
    VAO(const VAO& other) = delete;
    //Moving the VAO means we now have a VAO with the same id as the other, which is invalid
    VAO(VAO&& other) = delete;
    VAO& operator=(const VAO& other) = delete;
};