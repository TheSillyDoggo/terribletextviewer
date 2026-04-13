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
        double lastUpdate = 0;
        double delta = 0;

        static void onGlfwWindowRefresh(GLFWwindow* window);
        static void onGlfwCharCallback(GLFWwindow* window, unsigned int codepoint);
        static void onGlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void mainLoop();

    public:
        static Window* create(std::function<void()> callback);
        static Window* getInstance();

        void run();

        void setTitle(const std::string& title);
        const std::string& getTitle();

        double getDeltaTime();

        int getWindowWidth();
        int getWindowHeight();

        int getWindowX();
        int getWindowY();

        void setWindowX(int x);
        void setWindowY(int y);

        virtual void onDraw();
        virtual bool init();
};