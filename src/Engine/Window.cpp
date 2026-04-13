#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include <IMEDispatcher.hpp>

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
    glfwSetCharCallback(glfwContext, onGlfwCharCallback);
    glfwSetKeyCallback(glfwContext, onGlfwKeyCallback);

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
        mainLoop();
    }
}

void Window::mainLoop()
{
    double t = glfwGetTime();
    delta = t - lastUpdate;
    lastUpdate = t;

    onDraw();
    glfwSwapBuffers(glfwContext);
}

double Window::getDeltaTime()
{
    return delta;
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

int Window::getWindowX()
{
    int ret = 0;
    glfwGetWindowPos(glfwContext, &ret, nullptr);
    return ret;
}

int Window::getWindowY()
{
    int ret = 0;
    glfwGetWindowPos(glfwContext, nullptr, &ret);
    return ret;
}

void Window::setWindowX(int x)
{
    glfwSetWindowPos(glfwContext, x, getWindowY());
}

void Window::setWindowY(int y)
{
    glfwSetWindowPos(glfwContext, getWindowX(), y);
}

void Window::onGlfwWindowRefresh(GLFWwindow* window)
{
    Window::getInstance()->mainLoop();
}

std::string to_utf8(unsigned int cp) {
    std::string out;
    if (cp <= 0x7f)              out.append(1, static_cast<char>(cp));
    else if (cp <= 0x7ff)        { out.append(1, static_cast<char>(0xc0 | (cp >> 6))); out.append(1, static_cast<char>(0x80 | (cp & 0x3f))); }
    else if (cp <= 0xffff)       { out.append(1, static_cast<char>(0xe0 | (cp >> 12))); out.append(1, static_cast<char>(0x80 | ((cp >> 6) & 0x3f))); out.append(1, static_cast<char>(0x80 | (cp & 0x3f))); }
    else if (cp <= 0x10ffff)     { out.append(1, static_cast<char>(0xf0 | (cp >> 18))); out.append(1, static_cast<char>(0x80 | ((cp >> 12) & 0x3f))); out.append(1, static_cast<char>(0x80 | ((cp >> 6) & 0x3f))); out.append(1, static_cast<char>(0x80 | (cp & 0x3f))); }
    return out;
}

void Window::onGlfwCharCallback(GLFWwindow* window, unsigned int codepoint)
{
    std::string str = to_utf8(codepoint);
    IMEDispatcher::get()->onTypeUTF8Char(str);
}

void Window::onGlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_RELEASE)
    {
        #ifdef PLATFORM_MACOS
        bool isPaste = (mods & GLFW_MOD_SUPER) && key == GLFW_KEY_V;
        #else
        bool isPaste = (mods & GLFW_MOD_CONTROL) && key == GLFW_KEY_V;
        #endif

        if (isPaste)
        {
            if (auto clip = glfwGetClipboardString(window))
                IMEDispatcher::get()->onPasteUTF8(std::string(clip));
        }

        if (key == GLFW_KEY_BACKSPACE)
            IMEDispatcher::get()->onDelete(false);

        if (key == GLFW_KEY_DELETE)
            IMEDispatcher::get()->onDelete(true);
    }
}