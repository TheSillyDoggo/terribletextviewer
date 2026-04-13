#pragma once

#include <vector>
#include <string>

struct IMEDelegate
{
    public:
        bool isListening = false;

        IMEDelegate();
        ~IMEDelegate();

        virtual void onTypeUTF8Char(std::string text);
        virtual void onPasteUTF8(std::string text);
        virtual void onDelete(bool forwards);
};

class IMEDispatcher
{
    protected:
        std::vector<IMEDelegate*> delegates = {};

    public:
        static IMEDispatcher* get();

        void addDelegate(IMEDelegate* delegate);
        void removeDelegate(IMEDelegate* delegate);

        void onTypeUTF8Char(std::string text);
        void onPasteUTF8(std::string text);
        void onDelete(bool forwards);
};