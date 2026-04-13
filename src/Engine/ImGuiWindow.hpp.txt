#pragma once

#include "Window.hpp"
#include <functional>

class ImGuiWindow : public Window
{
    protected:
        std::function<void()> drawCallback = nullptr;

    public:
        static ImGuiWindow* create(std::function<void()> callback);

        void setCallback(std::function<void()> callback);

        virtual void onDraw();
        virtual bool init();
};