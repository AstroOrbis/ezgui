#pragma once

#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#undef main // SDL defines main as SDL_main, which is not what we want
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

namespace GuiTools {
	inline SDL_Window* window;
	inline char* glsl_version;
	inline ImGuiIO* io;
    inline SDL_GLContext gl_context;

	void decide_versions();
	int setup();
	int events();
	void newFrame();
	void endFrame(ImVec4 clear_color);
	void cleanup();
}

class GuiWrapper;

class Window {
public:
    bool enabled = false;
    std::string name;
    std::shared_ptr<GuiWrapper> gui;

    Window(const std::string& window_name, const bool enabled) : name(window_name), enabled(enabled) {}
    virtual ~Window() = default;
    virtual void render() = 0;
};

class GuiWrapper : public std::enable_shared_from_this<GuiWrapper> {
public:
    std::unordered_map<std::string, std::shared_ptr<Window>> windows;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool stop = false;

    void addWindow(std::shared_ptr<Window> window) {
        window->gui = shared_from_this();
        windows.insert({window->name, window});
    }

    void removeWindow(const std::string& name) {
        windows.erase(name);
    }

    bool* getEnabledRef(const std::string& name) {
        auto it = windows.find(name);
        if (it != windows.end()) {
            return &(it->second->enabled);
        }
        return nullptr;
    }

    void setup() {
        GuiTools::setup();
    }

    bool noWindowsOpen() {
        for (auto& window : windows) {
            if (window.second->enabled) return false;
        }
        return true;
    }

    bool shouldClose() {
        return stop || noWindowsOpen();
    }

    void frame() {
        if (GuiTools::events() != 0) {
            if (GuiTools::events() == -1) stop = true;
            return; // otherwise it's 1, meaning we waited and now we continue the loop
        }
        GuiTools::newFrame();
        for (auto& window : windows) {
            if (!window.second->enabled) continue;
            ImGui::Begin(window.first.c_str(), &window.second->enabled);
            window.second->render();
            ImGui::End();
        }
        GuiTools::endFrame(clear_color);
    }

    void cleanup() {
        GuiTools::cleanup();
    }
};
