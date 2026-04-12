#pragma once

#include <string>
#include <functional>

class Window
{
    protected:
        void* glfwContext = nullptr;
        std::string title = "";
        std::function<void()> drawCallback = nullptr;
        static inline Window* instance = nullptr;

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