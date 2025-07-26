#ifndef FBO_H
#define FBO_H

typedef struct rendertarget_t {
    unsigned handle, color_texture; // fbo handle + texture
    unsigned w, h;
} rendertarget_t;

rendertarget_t rendertarget(void);
void           rendertarget_use(rendertarget_t *f, unsigned w, unsigned h);
void           rendertarget_destroy(rendertarget_t *f);

#elif CODE
#pragma once

rendertarget_t rendertarget(void) {
    unsigned int fbo, fboTexture;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        die("!FBO incomplete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return (rendertarget_t){ fbo, fboTexture, 800, 600 };
}

void rendertarget_destroy(rendertarget_t *f) {
    glDeleteTextures(1, &f->color_texture);
    glDeleteFramebuffers(1, &f->handle);
    memset(f, 0, sizeof(rendertarget_t));
}

void rendertarget_use(rendertarget_t *f, unsigned w, unsigned h) {
    if (f) {
        // Resize rendertarget dimensions if viewport changes
        if (f->w != w || f->h != h) {
            glBindTexture(GL_TEXTURE_2D, f->color_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            f->w = w;
            f->h = h;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, f->handle);
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

#endif
