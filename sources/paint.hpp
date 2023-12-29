#pragma once

#include "imgui/imgui.h"

#include <string>
#include <vector>
#include <tuple>

namespace pxd
{
    class Paint
    {
    public:
        using PointData = std::tuple<ImVec2, ImColor, float>;

        void init();
        void destroy();

        void render(std::string label);

    private:
        void draw_menu();
        void draw_canvas();
        void draw_color_buttons();
        void draw_size_settings();

        void save_popup();
        void load_popup();

        void save_image(std::string_view filename);
        void load_image(std::string_view filename);
        void clear_canvas();

    private:
        std::uint32_t num_rows = 800;
        std::uint32_t num_cols = 600;
        std::uint32_t num_channels = 3;
        ImVec2 canvas_size = ImVec2(num_rows, num_cols);
        std::vector<PointData> points;
        ImVec2 canvas_pos;
        ImColor current_color;
        float point_draw_size;

        char filenamebuffer[256] = "test.bin";

        size_t buffer_size = 1024;
        char text_buffer[1024];
        std::string current_filename;
        int popup_flags = ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoScrollbar;
        ImVec2 popup_size = ImVec2(300.0f, 100.0f);
        ImVec2 popupbutton_size = ImVec2(120.0f, 0.0f);
    };
}