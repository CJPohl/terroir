#include "Shader.h"
#include "Tpch.h"
#include "core/Assert.h"
#include "renderer/Renderer.h"
#include "renderer/RendererAPI.h"
#include "renderer/opengl/OpenGLShader.h"
namespace Terroir
{

// May be removed
std::shared_ptr<Shader> Shader::Create()
{
    using enum RendererAPI::API;
    switch (Renderer::GetAPI())
    {
    case None: {
        TERR_ENGINE_ASSERT(false, "RendererAPI::None is not supported in Terroir!");
    }
    case OpenGL: {
        return std::make_shared<OpenGLShader>("Default");
    }
    default:

        TERR_ENGINE_ERROR("Unknown RendererAPI");
        return nullptr;
    }
}

std::shared_ptr<Shader> Shader::Create(const std::string_view& name, const std::initializer_list<std::filesystem::path> &paths)
{
    using enum RendererAPI::API;
    switch (Renderer::GetAPI())
    {
    case None: {
        TERR_ENGINE_ASSERT(false, "RendererAPI::None is not supported in Terroir!");
    }
    case OpenGL: {
        return std::make_shared<OpenGLShader>(name, paths);
    }
    default:

        TERR_ENGINE_ERROR("Unknown RendererAPI");
        return nullptr;
    }
}

} // namespace Terroir