#pragma once

#include "imgui/imgui.h"

#include <string>

namespace pxd
{
    class TextEditor
    {
    public:
        void init();
        void destroy();

        void render(std::string label);

    private:
        void draw_menu();
        void draw_content();
        void draw_info();

        void save_popup();
        void load_popup();

        void save_file(std::string_view filename);
        void load_file(std::string_view filename);

        std::string get_file_extension(std::string_view filename);

    private:
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