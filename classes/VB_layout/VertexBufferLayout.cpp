#include "VertexBufferLayout.h"


template<>
void VertexBufferLayout :: addAttribute<float>(uint32_t count){
    if ((count == 0 || count > 4) && count != GL_BGR)
        throw InvalidVertexAttribCount(__func__, count);

    this->attributes.emplace_back(count, GL_FLOAT, GL_FALSE);
    this->stride += count * VertexBufferAttrib :: sizeofType(GL_FLOAT);
}


uint32_t VertexBufferAttrib :: sizeofType(GLenum type){
    switch(type){
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
    }
    return 0;
}