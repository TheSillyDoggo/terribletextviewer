#include "ImGuiWindow.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

ImGuiWindow* ImGuiWindow::create(std::function<void()> callback)
{
    auto pRet = new ImGuiWindow();
    
    if (pRet->init())
    {
        pRet->setCallback(callback);
        return pRet;
    }

    delete pRet;
    return nullptr;
}

void ImGuiWindow::onDraw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (drawCallback)
        drawCallback();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(reinterpret_cast<GLFWwindow*>(glfwContext), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiWindow::init()
{
    if (!Window::init())
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(glfwContext), true);
    ImGui_ImplOpenGL3_Init("#version 150");

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = nullptr;

    io.Fonts->Build();

    return true;
}

void ImGuiWindow::setCallback(std::function<void()> callback)
{
    this->drawCallback = callback;
}