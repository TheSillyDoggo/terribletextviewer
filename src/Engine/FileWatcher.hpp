#pragma once

#include <filesystem>
#include <functional>
#include <iostream>
#include <fstream>
#include <thread>

class FileWatcher
{
    using Callback = std::function<void(const std::filesystem::path&)>;

    private:
        std::thread thread;

    protected:
        std::filesystem::path path;
        std::ifstream fileStream;
        Callback modifyCallback = nullptr;
        Callback createCallback = nullptr;
        Callback deleteCallback = nullptr;
        struct LinuxMembers
        {
            int fd;
            int wd;
        };
        LinuxMembers lm;

    public:
        FileWatcher(const std::filesystem::path& path);
        ~FileWatcher();

        void init();
        void destroy();
        void pollEvents();

        bool exists();

        std::string readAll();

        void setModifyCallback(Callback callback);
        void setCreateCallback(Callback callback);
        void setDeleteCallback(Callback callback);
};