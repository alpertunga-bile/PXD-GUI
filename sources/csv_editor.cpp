#include "csv_editor.hpp"
#include <sstream>
#include "imgui/implot.h"

namespace pxd
{
void CSVEditor::init()
{
}

void CSVEditor::destroy()
{
}

void CSVEditor::render(std::string label)
{
    ImGui::Begin(label.data());

    draw_size_buttons();
    ImGui::Separator();
    draw_io_buttons();
    ImGui::Separator();
    draw_table();

    ImGui::End();
}

void CSVEditor::save_popup()
{
    char savefilename_buffer[256] = "test.csv";

    SetPopupLayout();

    if(ImGui::BeginPopupModal("Save File", nullptr, popup_flags))
    {
        ImGui::InputText("Filename", savefilename_buffer, sizeof(savefilename_buffer));

        if(ImGui::Button("Save", popupbutton_size))
        {
            save_csv(filenamebuffer);

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(ImGui::Button("Cancel", popupbutton_size))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup(); 
    }
}

void CSVEditor::load_popup()
{
    char loadfilename_buffer[256] = "test.csv";

    SetPopupLayout();

    if(ImGui::BeginPopupModal("Load File", nullptr, popup_flags))
    {
        ImGui::InputText("Filename", loadfilename_buffer, sizeof(loadfilename_buffer));

        if(ImGui::Button("Load", popupbutton_size))
        {
            load_csv(filenamebuffer);

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(ImGui::Button("Cancel", popupbutton_size))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void CSVEditor::save_csv(std::string_view filename)
{
    std::ofstream out_file(filename.data());

    if(!out_file.is_open())
    {
        return;
    }

    for(int32_t row = 0; row < num_rows; row++)
    {
        for(int32_t col = 0; col < num_cols; col++)
        {
            out_file << data[row][col] << ",";
        }

        out_file << "\n";
    }

    out_file.close();
}

void CSVEditor::load_csv(std::string_view filename)
{
    std::ifstream in_file(filename.data());

    if(!in_file.is_open())
    {
        return;
    }

    data.clear();

    std::string line;
    num_rows = 0;

    while(std::getline(in_file, line))
    {
        std::istringstream ss(line);
        std::vector<float> row;
        std::string value;

        while(std::getline(ss, value, ','))
        {
            row.push_back(std::stof(value));
        }

        data.push_back(row);
        num_rows++;
    }

    num_cols = num_rows > 0 ? static_cast<int32_t>(data[0].size()) : 0;

    in_file.close();
}

void CSVEditor::draw_size_buttons()
{
    bool is_row_added = false;
    bool is_row_dropped = false;
    bool is_col_added = false;
    bool is_col_dropped = false;

    int32_t slider_rows = num_rows;
    int32_t slider_cols = num_cols;

    ImGui::Text("Num Rows: ");
    ImGui::SameLine();
    if(ImGui::SliderInt("##numRows", &slider_rows, 0, max_num_rows))
    {
        is_row_added = slider_rows > num_rows;
        is_row_dropped = !is_row_added; 

        num_rows = slider_rows;
    }

    ImGui::SameLine();

    if(ImGui::Button("Add Row") && num_rows < max_num_rows)
    {
        num_rows++;
        is_row_added = true;
        is_row_dropped = !is_row_added; 
    }

    ImGui::SameLine();

    if(ImGui::Button("Drop Row") && num_rows > 0)
    {
        num_rows--;
        is_row_dropped = true;
        is_row_added = !is_row_dropped; 
    }

    ImGui::Text("Num Cols: ");
    ImGui::SameLine();
    if(ImGui::SliderInt("##numCols", &slider_cols, 0, max_num_cols))
    {
        is_col_added = slider_cols > num_cols;
        is_col_dropped = !is_col_added; 

        num_cols = slider_cols;
    }

    ImGui::SameLine();

    if(ImGui::Button("Add Col") && num_cols < max_num_cols)
    {
        num_cols++;
        is_col_added = true;
        is_col_dropped = !is_col_added; 
    }

    ImGui::SameLine();

    if(ImGui::Button("Drop Col") && num_cols > 0)
    {
        num_cols--;
        is_col_dropped = true;
        is_col_added = !is_row_dropped; 
    }

    const int32_t data_row_num = static_cast<int32_t>(data.size());

    if(is_row_added)
    {
        for(int32_t row = data_row_num; row < num_rows; row++)
        {
            data.push_back(std::vector<float>(num_cols, 0.0f));
        }
    }
    else if(is_row_dropped)
    {
        for(int32_t row = data_row_num; row > num_rows; row--)
        {
            data.pop_back();
        }
    }
    else if(is_col_added)
    {
        for(int32_t row = 0; row < num_rows; row++)
        {
            const int32_t data_col_num = static_cast<int32_t>(data[row].size());

            for(int32_t col = data_col_num; data_col_num < num_cols; col++)
            {
                data[row].push_back(0.0f);
            }
        }
    }
    else if(is_col_dropped)
    {
        for(int32_t row = 0; row < num_rows; row++)
        {
            const int32_t data_col_num = static_cast<int32_t>(data[row].size());

            for(int32_t col = data_col_num; data_col_num > num_cols; col--)
            {
                data[row].pop_back();
            }
        }
    }
}
void CSVEditor::draw_io_buttons()
{
    if(ImGui::Button("Save"))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if(ImGui::Button("Load"))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if(ImGui::Button("Clear"))
    {
        data.clear();
        num_rows = 0;
        num_cols = 0;
    }

    save_popup();
    load_popup();
}

void CSVEditor::draw_table()
{
    int table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuter;

    int clicked_row = 0;
    int clicked_col = 0;

    if(num_cols == 0)
    {
        return;
    }

    ImGui::BeginTable("Table", num_cols, table_flags);

    for(int32_t col = 0; col < num_cols; col++)
    {
        const char col_name = 'A' + col;
        ImGui::TableSetupColumn(
            &col_name, 
            ImGuiTableColumnFlags_WidthFixed,
            1280.0f / static_cast<float>(num_cols)
        );
    }

    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
    for(std::int32_t col = 0; col < num_cols; col++)
    {
        PlotCellValue("%c", 'A' + col);
    }

    for(int32_t row = 0; row < num_rows; row++)
    {
        for(int32_t col = 0; col < num_cols; col++)
        {
            PlotCellValue("%f", data[row][col]);
            if(ImGui::IsItemClicked())
            {
                clicked_row = row;
                clicked_col = col;
                ImGui::OpenPopup("Change Value");
            }
            else if(ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Cell: %d, %d", row, col);
            }
        }

        ImGui::TableNextRow();
    }

    draw_value_popup(clicked_row, clicked_col);

    ImGui::EndTable();
}

void CSVEditor::draw_value_popup(const int row, const int col)
{
    char value_buffer[64] = "0";

    SetPopupLayout();

    if(ImGui::BeginPopupModal("Change Value", nullptr, popup_flags))
    {
        ImGui::InputText("value", value_buffer, sizeof(value_buffer));
        
        if(ImGui::Button("Change", popupbutton_size))
        {
            data[row][col] = std::stof(value_buffer);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(ImGui::Button("Cancel", popupbutton_size))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
void CSVEditor::SetPopupLayout()
{
    ImGui::SetNextWindowSize(popup_size);

    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetIO().DisplaySize.x / 2.0f - popup_size.x / 2.0f,
        ImGui::GetIO().DisplaySize.y / 2.0f - popup_size.y / 2.0f
    ));

}
}