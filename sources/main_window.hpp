#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

namespace pxd
{
    class MainWindow
    {
    public:
        void init(int width, int height, const char* title);
        void destroy();

        void new_frame();
        void render();

        bool should_close();

    private:
        GLFWwindow* m_window = nullptr;
        ImVec4 m_clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        ImGuiWindowFlags m_window_flags;
        ImGuiDockNodeFlags m_dockspace_flags;
        bool m_bDockOpen;
        int m_display_width, m_display_height;
    };
}