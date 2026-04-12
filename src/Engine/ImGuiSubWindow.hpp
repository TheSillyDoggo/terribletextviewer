#pragma once

#include "imgui.h"
#include <string>

class ImGuiSubWindow
{
    protected:
        std::string title = "";
        bool open = true;

        virtual void onDraw();

    public:
        static ImGuiSubWindow* create(std::string title);

        void setTitle(const std::string& title);
        const std::string& getTitle();

        const bool& isOpen();
        void setOpen(const bool& open);

        virtual void draw();
};