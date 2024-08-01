#include "FBO.h"
#include "../textures/Textures.h"

FBO :: FBO(){
    glGenFramebuffers(1, &fbo);
}


void FBO :: bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}


void FBO :: bindTexture(const Textures& texture){
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.get(), 0);
}


void FBO :: unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FBO :: ~FBO(){
    glDeleteFramebuffers(1, &fbo);
}