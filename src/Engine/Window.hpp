#pragma once

#include <string>
#include <functional>

struct GLFWwindow;

class Window
{
    protected:
        GLFWwindow* glfwContext = nullptr;
        std::string title = "";
        std::function<void()> drawCallback = nullptr;
        static inline Window* instance = nullptr;

        static void onGlfwWindowRefresh(GLFWwindow* window);

    public:
        static Window* create(std::function<void()> callback);
        static Window* getInstance();

        void run();

        void setTitle(const std::string& title);
        const std::string& getTitle();

        int getWindowWidth();
        int getWindowHeight();

        virtual void onDraw();
        virtual bool init();
};