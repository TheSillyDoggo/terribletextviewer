#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>

Window* Window::create(std::function<void()> callback)
{
    auto pRet = new Window();
    
    if (pRet->init())
    {
        pRet->drawCallback = std::move(callback);
        return pRet;
    }

    delete pRet;
    return nullptr;
}

Window* Window::getInstance()
{
    return instance;
}

void Window::onDraw()
{
    if (drawCallback)
        drawCallback();
}

bool Window::init()
{
    glfwSetErrorCallback([](int error_code, const char* description){
        std::cerr << std::format("GLFW Error: {}: {}\n", error_code, description);
    });

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_FOCUSED, 1);
    glfwWindowHint(0x2000C, 1);

    glfwContext = glfwCreateWindow(640, 480, "", nullptr, nullptr);

    if (!glfwContext)
    {
        return false;
    }

    glfwSetWindowRefreshCallback(glfwContext, onGlfwWindowRefresh);

    glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(glfwContext));
    glfwSwapInterval(1);

    instance = this;
    return true;
}

void Window::run()
{
    bool isRunning = true;

    auto window = reinterpret_cast<GLFWwindow*>(glfwContext);
    while (!glfwWindowShouldClose(window) && isRunning)
    {
        glfwPollEvents();
        onDraw();
        glfwSwapBuffers(window);
    }
}

void Window::setTitle(const std::string& title)
{
    this->title = title;

    glfwSetWindowTitle(reinterpret_cast<GLFWwindow*>(glfwContext), title.c_str());
}

const std::string& Window::getTitle()
{
    return title;
}

int Window::getWindowWidth()
{
    int ret = 0;
    glfwGetWindowSize(reinterpret_cast<GLFWwindow*>(glfwContext), &ret, nullptr);
    return ret;
}

int Window::getWindowHeight()
{
    int ret = 0;
    glfwGetWindowSize(reinterpret_cast<GLFWwindow*>(glfwContext), nullptr, &ret);
    return ret;
}

void Window::onGlfwWindowRefresh(GLFWwindow* window)
{
    Window::getInstance()->onDraw();
    glfwSwapBuffers(window);
}