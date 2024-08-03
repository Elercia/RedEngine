#pragma once

#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Utils/Types.hpp"
#include "RedEngine/Utils/Uncopyable.hpp"

struct SDL_Window;

namespace red
{
enum class ShaderProgramType
{
    Vertex_Pixel,
    Compute
};

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    bool Init(ArrayView<uint8> vertexCode, ArrayView<uint8> pixelCode);
    void Finalize();

private:
    ShaderProgramType m_type;
    uint32 m_handle;
};

class MaterialParameters
{

};

class Material
{
public:
    Material();
    ~Material();
};

using OpenglContext = void*;

class Renderer : Uncopyable, Unmovable
{
public:
    Renderer();
    ~Renderer();

    bool Init();
    void Finalize();

    void Flip();

private:
    OpenglContext m_openGlContext;
    SDL_Window* m_window;
};

}  // namespace red