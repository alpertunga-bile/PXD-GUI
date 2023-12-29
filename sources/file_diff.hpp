#pragma once

#include "imgui/imgui.h"

#include <string>
#include <vector>

namespace pxd
{
    class FileDiff
    {
    public:
        void init();
        void destroy();

        void render(std::string label);

    private:
        void draw_selection();
        void draw_diff_view();
        void draw_stats();

        std::vector<std::string> load_file_content(std::string_view filepath);
        void save_file_content(std::string_view filepath, std::vector<std::string>& filecontent);
        void create_diff();

    private:
        std::string filepath_left;
        std::string filepath_right;

        std::vector<std::string> filecontent_left;
        std::vector<std::string> filecontent_right;
    
        std::vector<std::string> diff_result_left;
        std::vector<std::string> diff_result_right;
    };
}