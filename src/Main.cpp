#include <Window.hpp>
#include "imgui.h"
#include "Include.hpp"
#include <Tint/Tint.hpp>
#include <iostream>
//#include <FileWatcher.hpp>
#include <format>

struct Vertex {
    float x, y;
    float u, v;
};

int main()
{
    Window* wnd = nullptr;

    tint::FontLibrary fl;

    // Load the font file
    auto font = tint::Font::loadFile(fl, "../assets/SawarabiGothic-Regular.ttf", tint::FontConfig {
        .sizePx = 16.f, // Font size in pixels
        .dpi = 72,      // Font DPI
        
        // Optional features (both are enabled by default, can be omitted here)
        .enableLigatures = true, 
        .enableKerning = true
    });
    if (!font) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    // Create a font atlas group with the loaded font
    tint::AtlasGroup atlasGroup(
        1024, 1024, // Atlas size
        tint::PixelFormat::RGBA8, // Pixel format
        tint::AtlasMode::Bitmap, 4.f // Atlas mode and SDF parameters
    );
    
    tint::Shaper shaper;

    int i = 0;

    /*FileWatcher watch("/home/aster/texteditor/src/Main.cpp");

    watch.setModifyCallback([&i, &watch](const std::filesystem::path& path)
    {
        i++;
    });*/

    double fps;
    double _updateInterval = 0.5f;
    double _timeLeft = _updateInterval;
    double _accum = 0;
    int _frames = 0;

    wnd = Window::create([&wnd, &shaper, &font, &atlasGroup, &watch, &i, &fps, &_updateInterval, &_timeLeft, &_accum, &_frames]
    {
        double dt = wnd->getDeltaTime();

        _timeLeft -= dt;
        _accum += 1 / dt;
        _frames++;

        if (_timeLeft <= 0)
        {
            fps = _accum / _frames;

            _timeLeft = _updateInterval;
            _accum = 0;
            _frames = 0;
        }

        watch.pollEvents();

        static unsigned int id = -1;

        auto shapedText = shaper.shapeMultiline(*font, std::format("FPS: {}", (int)fps));
        auto quads = atlasGroup.buildQuads(*font, shapedText);

        if (id == -1)
        {        
            glGenTextures(1, &id);
        }
        
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            atlasGroup.page(0).width(), atlasGroup.page(0).height(),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            atlasGroup.page(0).pixels().data()
        );
        

        auto width = wnd->getWindowWidth();
        auto height = wnd->getWindowHeight();

        glViewport(0, 0, width, height);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslated(0, height, 0);


        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, id);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_TRIANGLES);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor4f(0.35f, 0.35f, 0.35f, 1);

        for (auto& q : quads)
        {
            glTexCoord2f(q.u0, q.v0); glVertex2f(q.x0, -q.y0);
            glTexCoord2f(q.u1, q.v0); glVertex2f(q.x1, -q.y0);
            glTexCoord2f(q.u1, q.v1); glVertex2f(q.x1, -q.y1);

            glTexCoord2f(q.u0, q.v0); glVertex2f(q.x0, -q.y0);
            glTexCoord2f(q.u1, q.v1); glVertex2f(q.x1, -q.y1);
            glTexCoord2f(q.u0, q.v1); glVertex2f(q.x0, -q.y1);
        }

        glEnd();
    });

    wnd->setTitle("Text editor");
    wnd->run();
    return 0;
}