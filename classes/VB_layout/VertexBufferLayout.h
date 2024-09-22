#pragma once
#include <vector>
#include <GL/glew.h>
#include <cstdint>

struct VertexBufferAttrib{
    uint32_t count;
    GLenum type;
    GLboolean normalized;
    //Maybe throw exception if count is invalid
    static uint32_t sizeofType(GLenum type);
};
class VertexBufferLayout{
private:
    uint32_t stride;
    std::vector<VertexBufferAttrib> attributes;
public:
    VertexBufferLayout() : stride(0){};

    template<typename T>
    void addAttribute(uint32_t count);

    const std::vector<VertexBufferAttrib>& getAttributes() const{return attributes;}
    unsigned int getStride() const{return stride;}
};