#include "ImGuiSubWindow.hpp"

ImGuiSubWindow* ImGuiSubWindow::create(std::string title)
{
    auto pRet = new ImGuiSubWindow();
    pRet->setTitle(title);
    return pRet;
}

void ImGuiSubWindow::draw()
{
    ImGui::Begin(title.c_str(), & open);

    onDraw();

    ImGui::End();
}

void ImGuiSubWindow::onDraw()
{
    // overriden callback
}

void ImGuiSubWindow::setTitle(const std::string& title)
{
    this->title = title;
}

const std::string& ImGuiSubWindow::getTitle()
{
    return title;
}

const bool& ImGuiSubWindow::isOpen()
{
    return open;
}

void ImGuiSubWindow::setOpen(const bool& open)
{
    this->open = open;
}