#include "FileWatcher.hpp"

FileWatcher::FileWatcher(const std::filesystem::path& path)
{
    this->path = std::move(path);
    fileStream.open(path.string());

    init();
}

std::string FileWatcher::readAll()
{
    return 0;
}

bool FileWatcher::exists()
{
    return std::filesystem::exists(path);
}

void FileWatcher::setModifyCallback(FileWatcher::Callback callback)
{
    this->modifyCallback = std::move(callback);
}
void FileWatcher::setCreateCallback(FileWatcher::Callback callback)
{
    this->createCallback = std::move(callback);
}
void FileWatcher::setDeleteCallback(FileWatcher::Callback callback)
{
    this->deleteCallback = std::move(callback);
}

FileWatcher::~FileWatcher()
{
    destroy();
    fileStream.close();
}

#ifdef PLATFORM_LINUX
#include <sys/inotify.h>

void FileWatcher::destroy()
{
    close(lm.fd);

    inotify_rm_watch(lm.fd, lm.wd);
}

void FileWatcher::init()
{
    lm.fd = inotify_init1(IN_NONBLOCK);
    if (lm.fd < 0) {
        perror("inotify_init");
        return;
    }

    lm.wd = inotify_add_watch(lm.fd, this->path.c_str(),
                               IN_MODIFY | IN_CREATE | IN_DELETE);

    if (lm.wd < 0) {
        perror("inotify_add_watch");
        close(lm.fd);
        return;
    }
}

void FileWatcher::pollEvents()
{
    constexpr size_t bufLen =
        1024 * (sizeof(inotify_event) + NAME_MAX + 1);

    std::vector<char> buffer(bufLen);

    int length = read(lm.fd, buffer.data(), buffer.size());

    if (length < 0)
        return;

    int i = 0;
    while (i < length) {
        auto* event = reinterpret_cast<inotify_event*>(&buffer[i]);

        std::filesystem::path eventPath = this->path;
        if (event->len > 0) {
            eventPath /= event->name;
        }

        if (event->mask & IN_MODIFY) {
            if (modifyCallback) modifyCallback(eventPath);
        }

        if (event->mask & IN_CREATE) {
            if (createCallback) createCallback(eventPath);
        }

        if (event->mask & IN_DELETE) {
            if (deleteCallback) deleteCallback(eventPath);
        }

        i += sizeof(inotify_event) + event->len;
    }
}
#endif

#ifdef PLATFORM_WINDOWS
void FileWatcher::init() {}
void FileWatcher::destroy() {}
void FileWatcher::pollEvents() {}
#endif