#include "VAO.h"

VAO :: VAO(){
    glGenVertexArrays(1, &this->id);
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