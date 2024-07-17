#pragma once
#include <vector>
#include <GL/glew.h>
#include "../exceptions/InvalidVertexAttribCount.h"

struct VertexBufferAttrib{
    uint32_t count;
    GLenum type;
    GLboolean normalized;

    static uint32_t sizeofType(GLenum type);
};


class VertexBufferLayout{
private:
    unsigned int stride;
    std::vector<VertexBufferAttrib> attributes;
public:
    VertexBufferLayout() : stride(0){};
    
    template<typename T>
    void addAttribute(unsigned int count);
    const std::vector<VertexBufferAttrib>& getAttributes() const{return attributes;}
    unsigned int getStride() const{return stride;}

};