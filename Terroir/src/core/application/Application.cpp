//
// Created by cjp on 1/4/23.
//

#include "Application.h"
#include "Tpch.h"
#include "core/Assert.h"
#include "core/event/ApplicationEvent.h"
#include "core/event/KeyEvent.h"
#include "core/event/MouseEvent.h"
#include "core/event/WindowEvent.h"
#include "dear-imgui/DearImGuiLayer.h"
#include "log/Log.h"
#include "platform/Input.h"
#include "renderer/VertexArray.h"
#include "renderer/buffer/BufferLayout.h"
#include <glad/glad.h>

namespace Terroir
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define SET_EVENT_CB_LAMBDA(x) SetEventCallback([this](auto &&PH1) { x(std::forward<decltype(PH1)>(PH1)); })
// #define EVENT_LAMBDA(x) [this](auto && PH1) { x(std::forward<decltype(PH1)>(PH1)); }

Application *Application::s_Instance{nullptr};

void Application::Run()
{

    TERR_ENGINE_INFO("Terroir Application Initialized");

    while (m_Running)
    {
        glClearColor(.2, .5, .4, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Shader->Bind();
        m_VertexArray->Bind();

        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

        for (auto &layer : m_LayerStack)
        {
            layer->OnUpdate();
        }
        m_DearImGuiLayer->Begin();
        for (auto &layer : m_LayerStack)
        {
            layer->OnDearImGuiRender();
        }
        m_DearImGuiLayer->End();

        m_Window->OnUpdate();
    }
}

Application::Application()
{
    TERR_ENGINE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    constexpr u32 defaultWidth{1000}, defaultHeight{1000};
    auto window{Window::Create({"Terroir Engine", defaultWidth, defaultHeight})};
    m_Window = std::unique_ptr<Window>(window);
    m_Window->SET_EVENT_CB_LAMBDA(OnEvent);

    auto dearImGuiLayer{std::make_unique<DearImGuiLayer>()};
    m_DearImGuiLayer = dearImGuiLayer.get();
    PushOverlay(std::move(dearImGuiLayer));

    std::array<f32, 3 * 7> vertices{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.0f, 0.0f,
                                    1.0f,  0.0f,  1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f};
    std::array<u32, 3> indices{0, 1, 2};

    m_VertexArray = std::shared_ptr<VertexArray>(VertexArray::Create());
    m_VertexBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(&vertices[0], sizeof(vertices)));

    BufferLayout layout{{"a_Pos", ShaderDataType::Vec3}, {"a_Color", ShaderDataType::Vec4}};
    m_VertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    m_IndexBuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(&indices[0], indices.size()));
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_VertexBuffer->Unbind();
    m_VertexArray->Unbind();

    std::string vertexSrc{R"(
    #version 330 core

    layout(location = 0) in vec3 a_Pos;
    layout(location = 1) in vec4 a_Color;

    out vec4 ourColor;

    void main()
    {
      gl_Position = vec4(a_Pos, 1.0);
      ourColor = a_Color;
    }
  )"};

    std::string fragSrc{R"(
    #version 330 core

    out vec4 FragColor;
    in vec4 ourColor;

    void main()
    {
    FragColor = ourColor;
    }
  )"};

    m_Shader = std::make_shared<Shader>(vertexSrc, fragSrc);
}

Application::Application(const std::string &name, u32 width, u32 height)
{
    TERR_ENGINE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    auto window{Window::Create({name, width, height})};
    m_Window = std::unique_ptr<Window>(window);
    m_Window->SET_EVENT_CB_LAMBDA(OnEvent);

    auto dearImGuiLayer{std::make_unique<DearImGuiLayer>()};
    m_DearImGuiLayer = dearImGuiLayer.get();
    PushOverlay(std::move(dearImGuiLayer));
}

Application::~Application()
{
    TERR_ENGINE_INFO("Exiting Application");
}

void Application::OnEvent(Event &e)
{
    EventDispatch dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    TERR_ENGINE_INFO(e.ToString());

    // Iterate through layerstack
    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnUpdate();
        if (e.m_Handled)
            break;
    }
}

bool Application::OnWindowClose(WindowCloseEvent &)
{
    m_Running = false;
    return true;
}

void Application::PushLayer(LayerStack::LayerPtr layer)
{
    layer->OnAttach();
    m_LayerStack.PushLayer(std::move(layer));
}

void Application::PushOverlay(LayerStack::LayerPtr overlay)
{
    overlay->OnAttach();
    m_LayerStack.PushOverlay(std::move(overlay));
}
} // namespace Terroir
