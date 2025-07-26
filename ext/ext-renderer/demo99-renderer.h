// this is a wip renderer

typedef union r_state {
    struct {
    uint16_t blend:3;       // 0(off) 1(alpha) 2(premul) 3(add) 4(mul)
    uint16_t depthrd:3;     // 0(off) 1(<) 2(<=) 3(>) 4(>=)
    uint16_t depthwr:1;     // 0(off) 1(on)
    uint16_t depthclamp:1;  // 0(off) 1(on)
    uint16_t msaa:1;        // 0(off) 1(on) requires samples/msaa context enabled
    uint16_t ccw:1;         // 0(off) 1(on)
    uint16_t cull:2;        // 0(off) 1(front) 2(back) 3(front-and-back)
    uint16_t scissor:1;     // 0(off) 1(on)
    };
    uint16_t mask:13;
    uint16_t bits;
} r_state;

STATIC_ASSERT( sizeof(r_state) == 2 );

r_state r_state_set(r_state old_state, r_state new_state) {
    if( old_state.mask != new_state.mask ) {
        int mode;
        if( old_state.blend != (mode = new_state.blend) ) {
            int modes[] = { 0, 0,
                GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, // Alpha Blending: standard blend
                GL_ONE, GL_ONE_MINUS_SRC_ALPHA, // Premultiplied Alpha: faster and more correct than above as long textures use premultiplied alpha
                GL_SRC_ALPHA, GL_ONE, // Additive Blending: accumulate colors
                GL_DST_COLOR, GL_ZERO, // Multiplicative Blending: darkening: ignores alpha (srcColor * dstColor)
            };
            if( !mode ) glDisable(GL_BLEND);
            else glEnable(GL_BLEND), glBlendFunc(modes[mode*2+0], modes[mode*2+1]);
        }
        if( old_state.depthrd != (mode = new_state.depthrd) ) {
            int modes[] = { 0, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL }; // GL_NEVER, GL_ALWAYS, GL_NOTEQUAL, GL_EQUAL
            if( !mode ) glDisable(GL_DEPTH_TEST);
            else glEnable(GL_DEPTH_TEST), glDepthFunc(modes[mode]);
        }
        if( old_state.depthwr != (mode = new_state.depthwr) ) {
            glDepthMask(mode ? GL_TRUE : GL_FALSE);
        }
        if( old_state.depthclamp != (mode = new_state.depthclamp) ) {
            (mode ? glEnable : glDisable)(GL_DEPTH_CLAMP);
        }
        if( old_state.ccw != (mode = new_state.ccw) ) {
            int modes[] = { GL_CW, GL_CCW };
            glFrontFace(modes[mode]);
        }
        if( old_state.cull != (mode = new_state.cull) ) {
            int modes[] = { 0, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK };
            if( !mode ) glDisable(GL_CULL_FACE);
            else glEnable(GL_CULL_FACE), glCullFace(modes[mode]);
        }
        if( old_state.scissor != (mode = new_state.scissor) ) {
            (mode ? glEnable : glDisable)(GL_SCISSOR_TEST);
        }
        if( old_state.msaa != (mode = new_state.msaa) ) {
            (mode ? glEnable : glDisable)(GL_MULTISAMPLE);
        }
    }
    return new_state;
}

void r_default(void) {
    r_state r = { .blend=0, .depthrd=1, .depthwr=1, .ccw=1, .cull=2, .scissor=1 };
    r_state_set( ((r_state){.mask = ~r.mask }), r );
}

// other glEnable/glDisable booleans:
// GL_FRAMEBUFFER_SRGB
// GL_TEXTURE_CUBE_MAP_SEAMLESS
// GL_MULTISAMPLE, see also: GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_ALPHA_TO_ONE
// GL_STENCIL_TEST, see also: glStencilOpSeparate, glStencilFuncSeparate, glStencilMaskSeparate
// GL_POLYGON_OFFSET_FILL, see also: glPolygonOffset
// GL_COLOR_LOGIC_OP, see also: glLogicOp
// GL_DEBUG_OUTPUT, GL_DEBUG_OUTPUT_SYNCHRONOUS
// see also: glColorMask
// see also: glBlendColor, glBlendFuncSeparate, glBlendEquationSeparate

void glWireframe(int on) {
    if(on)
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}
