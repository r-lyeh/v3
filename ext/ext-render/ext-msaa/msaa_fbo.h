#ifndef MSAA_FBO_H
#define MSAA_FBO_H

typedef enum {
    MSAA_0X  = 1,  // no AA
    MSAA_1X  = 1,  // no AA
    MSAA_2X  = 2,  // 2X AA (requires AA context >= 2)
    MSAA_4X  = 4,  // 4X AA (requires AA context >= 4)
    MSAA_8X  = 8,  // 8X AA (requires AA context >= 8)
    MSAA_MAX = -1, // AA in use (requires AA context > 0)
} MSAA_Level;

typedef struct {
    GLuint fbo[3];      // 1x, 2x, 4x FBOs
    GLuint color[3];    // color: texture for 1x, renderbuffers for 2x/4x
    GLuint depth[3];    // depth renderbuffers
    int width, height, level;

    // single-sample resolve target (texture + fbo)
    GLuint resolve_fbo;
    GLuint resolve_tex;
} MSAA_FBO;

void msaa_fbo_init(MSAA_FBO* fbo);
void msaa_fbo_bind(MSAA_FBO* fbo, MSAA_Level level);
void msaa_fbo_blit(MSAA_FBO* fbo);
void msaa_fbo_destroy(MSAA_FBO* fbo);

#endif

#ifdef CODE
#pragma once

static int msaa_max(void) {
    return app_state().aa;
}

static void msaa_query_fb_size(int* w, int* h) {
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    *w = vp[2];
    *h = vp[3];
}

void msaa_fbo_init(MSAA_FBO* fbo) {
    int w, h;
    msaa_query_fb_size(&w, &h);

    fbo->width = w;
    fbo->height = h;
    fbo->resolve_fbo = 0;
    fbo->resolve_tex = 0;

    glGenFramebuffers(3, fbo->fbo);
    glGenTextures(1, &fbo->color[0]);
    glGenRenderbuffers(2, &fbo->color[1]);
    glGenRenderbuffers(3, fbo->depth);

    // create resolve texture + FBO
    glGenTextures(1, &fbo->resolve_tex);
    glBindTexture(GL_TEXTURE_2D, fbo->resolve_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &fbo->resolve_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->resolve_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->resolve_tex, 0);
    GLenum db = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &db);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Resolve FBO incomplete\n");
        fbo->resolve_fbo = 0;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int samples[3] = {1, 2, 4};
    for (int i = 0; i < 3; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo[i]);

        if (i == 0) {
            // 1x: color texture
            glBindTexture(GL_TEXTURE_2D, fbo->color[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->color[i], 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            // depth single-sample
            glBindRenderbuffer(GL_RENDERBUFFER, fbo->depth[i]);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depth[i]);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        } else {
            // 2x/4x: color renderbuffer multisample
            glBindRenderbuffer(GL_RENDERBUFFER, fbo->color[i]);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples[i], GL_RGBA8, w, h);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fbo->color[i]);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            // depth multisample
            glBindRenderbuffer(GL_RENDERBUFFER, fbo->depth[i]);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples[i], GL_DEPTH_COMPONENT24, w, h);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depth[i]);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, &draw_buffer);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("FBO %dx incomplete\n", samples[i]);
            fbo->fbo[i] = 0;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void msaa_fbo_destroy(MSAA_FBO* fbo) {
    glDeleteFramebuffers(3, fbo->fbo);
    glDeleteTextures(1, &fbo->color[0]);
    glDeleteRenderbuffers(2, &fbo->color[1]);
    glDeleteRenderbuffers(3, fbo->depth);
    if (fbo->resolve_fbo) glDeleteFramebuffers(1, &fbo->resolve_fbo);
    if (fbo->resolve_tex) glDeleteTextures(1, &fbo->resolve_tex);
}

void msaa_fbo_bind(MSAA_FBO* fbo, MSAA_Level level) {
    if( level >= msaa_max() ) level = MSAA_MAX;
    fbo->level = level;

    if (level < 0) glEnable(GL_MULTISAMPLE), glBindFramebuffer(GL_FRAMEBUFFER, 0);
    else
    if (level <= 1) glDisable(GL_MULTISAMPLE), glBindFramebuffer(GL_FRAMEBUFFER, 0);
    else {
        glEnable(GL_MULTISAMPLE);
        int idx = (level == MSAA_1X) ? 0 : (level == MSAA_2X) ? 1 : 2;
        if (fbo->fbo[idx]) glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo[idx]);
        else glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glViewport(0, 0, fbo->width, fbo->height);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void msaa_fbo_blit(MSAA_FBO* fbo) {
    if (fbo->level <= 1) return;

    int level = fbo->level;
    int idx = (level == MSAA_1X) ? 0 : (level == MSAA_2X) ? 1 : 2;
    if (!fbo->fbo[idx]) return;

    // control visual sharpness when upsampling: GL_NEAREST (faster) or GL_LINEAR (nicer)
    int filtering = GL_NEAREST; // use GL_LINEAR only when the fbo is smaller than the framebuffer (upsampling)

    GLint viewport[4]; glGetIntegerv(GL_VIEWPORT, viewport);
    int dst_w = viewport[2], dst_h = viewport[3];

    if (level == MSAA_1X) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->fbo[idx]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glDrawBuffer(GL_BACK);
        glBlitFramebuffer(0,0,fbo->width,fbo->height,0,0,dst_w,dst_h,GL_COLOR_BUFFER_BIT,filtering);
    } else {
        // MSAA resolve -> single-sample
        if (!fbo->resolve_fbo) return;
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->fbo[idx]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->resolve_fbo);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBlitFramebuffer(0,0,fbo->width,fbo->height,0,0,fbo->width,fbo->height,GL_COLOR_BUFFER_BIT,GL_NEAREST); // note: do not use GL_LINEAR here

        // resolve -> default framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->resolve_fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0,0,fbo->width,fbo->height,0,0,dst_w,dst_h,GL_COLOR_BUFFER_BIT,filtering);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
#endif
