#include "VAO.h"

VAO :: VAO(){
    glGenVertexArrays(1, &this->id);
}


void VAO :: addBuffer(const VBO& vbo, const VertexBufferLayout& layout){
    this->bind();
    vbo.bind();

    const auto& attributes = layout.getAttributes();
    uint32_t offset = 0;

    for (uint32_t i = 0; i < attributes.size(); i++){
        const auto& attrib = attributes[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attrib.count, attrib.type, attrib.normalized,
                              layout.getStride(), (const void*)offset);
        offset += attrib.count * VertexBufferAttrib :: sizeofType(attrib.type);
    }
}
void VAO :: bind() const{
    glBindVertexArray(this->id);
}


void VAO :: unbind() const{
    glBindVertexArray(0);
}


VAO :: ~VAO(){
    glDeleteVertexArrays(1, &this->id);
}