#include "VBO.h"

VBO :: VBO(GLfloat* vertices, GLsizeiptr size){
    glGenBuffers(1, &this->id);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO :: bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
}


void VBO :: unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


VBO :: ~VBO(){
    glDeleteBuffers(1, &this->id);
}