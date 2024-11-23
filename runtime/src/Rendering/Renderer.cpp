#include "RedEngine/Rendering/Renderer.hpp"

#include "RedEngine/Rendering/RenderingModule.hpp"

#include "RedEngine/Core/Debug/Logger/Logger.hpp"

#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#define CheckGL(expr, ...)          \
    if ((expr) != 0)                \
    {                               \
        RED_LOG_ERROR(__VA_ARGS__); \
    }

#define CheckGLReturn(expr, value, ...) \
    if ((expr) != 0)                    \
    {                                   \
        RED_LOG_ERROR(__VA_ARGS__);     \
        return value;                   \
    }

namespace red
{
ShaderProgram::ShaderProgram() : m_type(ShaderProgramType::Vertex_Pixel), m_handle(0)
{
}

ShaderProgram::~ShaderProgram()
{
    RedAssert(m_handle == 0);
}

bool ShaderProgram::Init(ArrayView<uint8> vertexCode, ArrayView<uint8> pixelCode)
{
    //TODO handle errors
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    {
        char* dataPtr = (char*)vertexCode.data();
        int dataLength = vertexCode.size();
        glShaderSource(vertexShader, 1, &dataPtr, &dataLength);
        glCompileShader(vertexShader);
    }
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    {
        char* dataPtr = (char*)pixelCode.data();
        int dataLength = pixelCode.size();
        glShaderSource(fragmentShader, 1, &dataPtr, &dataLength);
        glCompileShader(fragmentShader);
    }

    m_handle = glCreateProgram();

    glAttachShader(m_handle, vertexShader);
    glAttachShader(m_handle, fragmentShader);
    glLinkProgram(m_handle);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void ShaderProgram::Finalize()
{
    glDeleteProgram(m_handle);
    m_handle = 0;
}

void OpenGlDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/,
                                const GLchar* message, const void* /*userParam*/)
{
    const char* _source;
    const char* _type;
    const char* _severity;
    LogLevel level = LogLevel::LEVEL_INFO;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            _source = "UNKNOWN";
            break;

        default:
            _source = "UNKNOWN";
            break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            level = LogLevel::LEVEL_ERROR;
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            level = LogLevel::LEVEL_ERROR;
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            level = LogLevel::LEVEL_WARNING;
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            level = LogLevel::LEVEL_WARNING;
            break;

        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;

        default:
            _type = "UNKNOWN";
            break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;

        default:
            _severity = "UNKNOWN";
            break;
    }

    RED_LOG(level, "{}: {} of {} severity, raised from {}: {}", id, _type, _severity, _source, message);
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
    {
        RED_LOG_ERROR("Error initializing SDL_Video with error {}", SDL_GetError());
        SDL_Quit();
        RedAbort("Cannot initialize Renderer");
    }

    constexpr int OpenglMajorVersion = 4;
    constexpr int OpenglMinorVersion = 5;

    // Request OpenGL 4.5 context.
    CheckGLReturn(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenglMajorVersion), false,
                  "Error setting gl context major version with error {}", SDL_GetError());
    CheckGLReturn(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenglMinorVersion), false,
                  "Error setting gl context minor version with error {}", SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    uint32 flags;
    flags |= SDL_WindowFlags::SDL_WINDOW_OPENGL;
    flags |= SDL_WindowFlags::SDL_WINDOW_INPUT_FOCUS;

    m_window = SDL_CreateWindow("", 0, 0, 0, 0, flags);
    if (m_openGlContext == nullptr)
    {
        RED_LOG_ERROR("Failed to create window with error '{}'", SDL_GetError());
        return false;
    }

    m_openGlContext = SDL_GL_CreateContext(m_window);
    if (m_openGlContext == nullptr)
    {
        RED_LOG_ERROR(
            "Failed to query openGL context from SDL. Maybe the required Opengl version {}.{} is not supported",
            OpenglMajorVersion, OpenglMinorVersion);
        return false;
    }

    SDL_GL_MakeCurrent(m_window, m_openGlContext);

    if (gl3wInit() != 0)
    {
        RED_LOG_ERROR("Failed to init opengl functions");
        return false;
    }

#ifdef RED_DEVBUILD
    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback((GLDEBUGPROC)&red::OpenGlDebugMessageCallback, nullptr);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_openGlContext);
    ImGui_ImplOpenGL3_Init("#version 130");
#endif

    return true;
}

void Renderer::Finalize()
{
#ifdef RED_DEVBUILD
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
#endif

    SDL_DestroyWindow(m_window);

    SDL_GL_DeleteContext(m_openGlContext);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Renderer::Flip()
{
    // End the current frame
    {
        SDL_GL_SwapWindow(m_window);
    }

    // Start the new frame
    {
        int width = 0, height = 0;
        SDL_GL_GetDrawableSize(m_window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef RED_DEVBUILD
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
#endif
    }
}

}  // namespace red