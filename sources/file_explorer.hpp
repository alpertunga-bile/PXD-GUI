#pragma once

#include "imgui/imgui.h"

#include <filesystem>

namespace pxd
{
    class FileExplorer
    {
    public:
        void init();
        void render(std::string label);

    private:
        void draw_menu();
        void draw_content();
        void draw_actions();

        void open_file();
        void rename_file_popup();
        bool rename_file(std::filesystem::path& old_path, std::filesystem::path& new_path);
        void delete_file_popup();
        bool delete_file(std::filesystem::path& path);

        void draw_filter();
    
    private:
        std::filesystem::path m_current_path;
        std::filesystem::path m_selected_entry;

        bool m_rename_popup_open = false;
        bool m_delete_popup_open = false;
    };
}