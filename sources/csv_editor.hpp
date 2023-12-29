#pragma once

#include "imgui/imgui.h"

#include <string>
#include <vector>
#include <string_view>
#include <map>
#include <fstream>

namespace pxd
{
    class CSVEditor
    {
    public:
        void init();
        void destroy();

        void render(std::string label);

    private:
        void save_popup();
        void load_popup();
        void save_csv(std::string_view filename);
        void load_csv(std::string_view filename);

        void draw_size_buttons();
        void draw_io_buttons();
        void draw_table();
        void draw_value_popup(const int row, const int col);

        template<typename T>
        void PlotCellValue(std::string_view formatter, const T value);
        void SetPopupLayout();

    private:
        int32_t max_num_rows = 30;
        int32_t max_num_cols = 8;
        int32_t num_cols = 0;
        int32_t num_rows = 0;
        std::vector<std::vector<float>> data;

        char filenamebuffer[256] = "test.csv";

        int popup_flags = ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoScrollbar;
        ImVec2 popup_size = ImVec2(300.0f, 100.0f);
        ImVec2 popupbutton_size = ImVec2(120.0f, 0.0f);
    };

template<typename T>
inline void CSVEditor::PlotCellValue(std::string_view formatter, const T value)
{
    ImGui::TableNextColumn();
    ImGui::Text(formatter.data(), value);
}

}