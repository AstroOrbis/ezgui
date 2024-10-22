#include <ezgui.hpp>

class Window1 : public Window {
public:
    float f = 0.0f;
    int counter = 0;

    Window1() : Window("Window1", true) {}

    void render() override {
        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Another Window", gui->getEnabledRef("Window2"));

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&gui->clear_color);

        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / (*GuiTools::io).Framerate, (*GuiTools::io).Framerate);
    }
};

class Window2 : public Window {
public:
    Window2() : Window("Window2", false) {}

    void render() override {
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            enabled = false;
    }
};

// apparently SDL does `#define main SDL_main` so we gotta undefine it
#undef main
int main() {
    auto window1 = std::make_shared<Window1>();
    auto window2 = std::make_shared<Window2>();
    auto gui = std::make_shared<GuiWrapper>();

    gui->addWindow(window1);
    gui->addWindow(window2);

    gui->setup();
    while (!gui->shouldClose()) {
        gui->frame();
    }
    gui->cleanup();
    return 0;
}
