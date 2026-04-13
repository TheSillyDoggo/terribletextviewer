#include "IMEDispatcher.hpp"
#include <algorithm>

IMEDispatcher* IMEDispatcher::get()
{
    static IMEDispatcher* instance = new IMEDispatcher();
    return instance;
}

void IMEDispatcher::onTypeUTF8Char(std::string text)
{
    for (auto delegate : delegates)
    {
        if (!delegate->isListening)
            continue;

        delegate->onTypeUTF8Char(text);
    }
}

void IMEDispatcher::onPasteUTF8(std::string text)
{
    for (auto delegate : delegates)
    {
        if (!delegate->isListening)
            continue;

        delegate->onPasteUTF8(text);
    }
}

void IMEDispatcher::onDelete(bool forwards)
{
    for (auto delegate : delegates)
    {
        if (!delegate->isListening)
            continue;

        delegate->onDelete(forwards);
    }
}

void IMEDispatcher::addDelegate(IMEDelegate* delegate)
{
    delegates.push_back(delegate);
}

void IMEDispatcher::removeDelegate(IMEDelegate* delegate)
{
    delegates.erase(std::find(delegates.begin(), delegates.end(), delegate));
}

IMEDelegate::IMEDelegate()
{
    IMEDispatcher::get()->addDelegate(this);
}

IMEDelegate::~IMEDelegate()
{
    IMEDispatcher::get()->removeDelegate(this);
}

void IMEDelegate::onPasteUTF8(std::string text) {}
void IMEDelegate::onTypeUTF8Char(std::string text) {}
void IMEDelegate::onDelete(bool forwards) {}