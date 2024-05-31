#include "shadow_buffer.h"

using namespace bf;

void ShadowBuffer::updateSize(glm::ivec2 size) {
    // Create single-byte texture
    glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
}

ShadowBuffer::ShadowBuffer() {
    glGenFramebuffers(1, &glFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getGLTexture(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowBuffer::~ShadowBuffer() {
    glDeleteFramebuffers(1, &glFramebuffer);
}