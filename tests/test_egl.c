#include <stdio.h>
#include <EGL/egl.h>

int result = 1; // Success
#define assert(x) do { if (!(x)) {result = 0; printf("Assertion failure: %s in %s:%d!\n", #x, __FILE__, __LINE__); } } while(0)

int main(int argc, char *argv[])
{
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(display != EGL_NO_DISPLAY);
    assert(eglGetError() == EGL_SUCCESS);

    EGLint major = 0, minor = 0;
    EGLBoolean ret = eglInitialize(display, &major, &minor);
    assert(eglGetError() == EGL_SUCCESS);
    assert(ret == EGL_TRUE);
    assert(major * 10000 + minor >= 10004);

    EGLint numConfigs;
    ret = eglGetConfigs(display, NULL, 0, &numConfigs);
    assert(eglGetError() == EGL_SUCCESS);
    assert(ret == EGL_TRUE);

    EGLint attribs[] = {
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_BLUE_SIZE, 5,
        EGL_NONE
    };
    EGLConfig config;
    ret = eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    assert(eglGetError() == EGL_SUCCESS);
    assert(ret == EGL_TRUE);

    EGLNativeWindowType dummyWindow;
    EGLSurface surface = eglCreateWindowSurface(display, config, dummyWindow, NULL);
    assert(eglGetError() == EGL_SUCCESS);
    assert(surface != 0);

    EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLContext context = eglCreateContext(display, config, NULL, contextAttribs);
    assert(eglGetError() == EGL_SUCCESS);
    assert(context != 0);

    assert(eglGetCurrentContext() == 0); // Creating a context does not yet activate it.
    assert(eglGetError() == EGL_SUCCESS);

    ret = eglMakeCurrent(display, surface, surface, context);
    assert(eglGetError() == EGL_SUCCESS);
    assert(ret == EGL_TRUE);
    assert(eglGetCurrentContext() == context);
    assert(eglGetCurrentSurface(EGL_READ) == surface);
    assert(eglGetCurrentSurface(EGL_DRAW) == surface);

    ret = eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    assert(eglGetError() == EGL_SUCCESS);
    assert(ret == EGL_TRUE);
    assert(eglGetCurrentContext() == EGL_NO_CONTEXT);
    assert(eglGetCurrentSurface(EGL_READ) == EGL_NO_SURFACE);
    assert(eglGetCurrentSurface(EGL_DRAW) == EGL_NO_SURFACE);

    ret = eglTerminate(display);
    assert(eglGetError() == EGL_SUCCESS);
    assert(ret == EGL_TRUE);
    
#ifdef REPORT_RESULT
    REPORT_RESULT();
#endif
}
