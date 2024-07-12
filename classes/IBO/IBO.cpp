#include "IBO.h"

IBO :: IBO(GLuint* indices, GLsizeiptr size){
    glGenBuffers(1, &this->id);
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}


void IBO :: bind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}


void IBO :: unbind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


IBO :: ~IBO(){
    glDeleteBuffers(1, &this->id);
}