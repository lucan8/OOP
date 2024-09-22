#include "VBO.h"
#include "../exceptions/MyRuntimeException.h"

VBO :: VBO(GLsizeiptr size, const GLfloat* vertices, GLenum usage){
    glGenBuffers(1, &this->id);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

void VBO :: update(GLfloat* vertices, GLsizeiptr size){
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
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