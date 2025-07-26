#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242

#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_DEBUG_SOURCE_API               0x8246
#define GL_DEBUG_TYPE_ERROR               0x824C
//
void glDebugCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char * message, void * userdata) {
    // whitelisted codes (also: 131169, 131204).
    if( id == 131154 ) return; // Pixel-path performance warning: Pixel transfer is synchronized with 3D rendering.
    if( id == 131185 ) return; // Buffer object 2 (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations
    if( id == 131218 ) return; // Program/shader state performance warning: Vertex shader in program 9 is being recompiled based on GL state.
    if( id == 2 ) return; // INFO: API_ID_RECOMPILE_FRAGMENT_SHADER performance warning has been generated. Fragment shader recompiled due to state change. [ID: 2]

    const char * GL_ERROR_SOURCE[] = { "API", "WINDOW SYSTEM", "SHADER COMPILER", "THIRD PARTY", "APPLICATION", "OTHER" };
    const char * GL_ERROR_SEVERITY[] = { "HIGH", "MEDIUM", "LOW", "NOTIFICATION" };
    const char * GL_ERROR_TYPE[] = { "ERROR", "DEPRECATED BEHAVIOR", "UNDEFINED DEHAVIOUR", "PORTABILITY", "PERFORMANCE", "OTHER" };

    severity = severity == GL_DEBUG_SEVERITY_NOTIFICATION ? 3 : severity - GL_DEBUG_SEVERITY_HIGH;
    source = source - GL_DEBUG_SOURCE_API;
    type = type - GL_DEBUG_TYPE_ERROR;

    //printf("[SEVERITY: %s] [SOURCE: %s] [TYPE: %s] ", GL_ERROR_SEVERITY[severity], GL_ERROR_SOURCE[source], GL_ERROR_TYPE[type]);
    lputs( va("!%s [ID: %u]\n", message, id ) );
}
void glDebugEnable(void* arg) {
    ONCE {
    if(!glDebugMessageCallback) { lputs("glDebugMessageCallback not available (OpenGL >= 4.3)"); return; }

    typedef void (*GLDEBUGPROC)(uint32_t, uint32_t, uint32_t, uint32_t, int32_t, const char *, const void *);
    //typedef void (*GLDEBUGMESSAGECALLBACKPROC)(GLDEBUGPROC, const void *);
    // void (*glDebugMessageCallback)(GLDEBUGPROC, const void *) = (GLDEBUGMESSAGECALLBACKPROC)app_proc("glDebugMessageCallback");
    // if(!glDebugMessageCallback) return;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((GLDEBUGPROC)glDebugCallback, NULL);
    //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    }

    static unsigned frame = -1; ++frame;
    for(int error = glGetError(); error != GL_NO_ERROR; error = glGetError()) {
        const char* str = "Unknown error";
        /**/ if( error == GL_INVALID_OPERATION ) str = "GL_INVALID_OPERATION";
        else if( error == GL_INVALID_ENUM ) str = "GL_INVALID_ENUM";
        else if( error == GL_INVALID_VALUE ) str = "GL_INVALID_VALUE";
        else if( error == GL_OUT_OF_MEMORY ) str = "GL_OUT_OF_MEMORY";
        else if( error == GL_INVALID_FRAMEBUFFER_OPERATION ) str = "GL_INVALID_FRAMEBUFFER_OPERATION";
        lputs(va("OpenGL Error, Frame #%u: %s (%#x)", frame, str, error));
    }
}

AUTORUN {
    on_frame_begin(glDebugEnable, NULL);
}
