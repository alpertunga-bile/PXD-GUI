#include "file_diff.hpp"

#include <fstream>

namespace pxd
{


void FileDiff::init()
{
    filepath_left = "test_left.txt";
    filepath_right = "test_right.txt";

    filecontent_left = load_file_content(filepath_left);
    filecontent_right = load_file_content(filepath_right);

    create_diff();
}

void FileDiff::destroy()
{
}

void FileDiff::render(std::string label)
{
    ImGui::Begin(label.data());

    draw_selection();
    draw_diff_view();
    draw_stats();

    ImGui::End();
}

void FileDiff::draw_selection()
{
    ImGui::InputText("Left", (char*)filepath_left.c_str(), filepath_left.capacity() + 1);
    ImGui::SameLine();

    if(ImGui::Button("Save###Left"))
    {
        save_file_content(filepath_left, filecontent_left);
    }

    ImGui::InputText("Right", (char*)filepath_right.c_str(), filepath_right.capacity() + 1);
    ImGui::SameLine();

    if(ImGui::Button("Save###Right"))
    {
        save_file_content(filepath_right, filecontent_right);
    }

    if(ImGui::Button("Compare"))
    {
        filecontent_left = load_file_content(filepath_left);
        filecontent_right = load_file_content(filepath_right);

        create_diff();
    }
}

void FileDiff::draw_diff_view()
{
    ImVec2 parent_size = ImVec2(
        ImGui::GetContentRegionAvail().x, 500.0f
    );

    ImVec2 child_size = ImVec2(
        parent_size.x / 2.0f - 40.0f, parent_size.y
    );

    ImVec2 swap_size = ImVec2(40.0f, child_size.y);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::BeginChild("Parent", parent_size, true);

    if(ImGui::BeginChild("Diff_Left", child_size, false))
    {
        for(std::size_t i = 0; i < filecontent_left.size(); i++)
        {
            if(!diff_result_left[i].empty())
            {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", filecontent_left[i].c_str());
            }
            else
            {
                ImGui::Text("%s", filecontent_left[i].c_str());
            }
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    float line_height = ImGui::GetTextLineHeightWithSpacing();
    ImVec2 button_size = ImVec2(15.0f, line_height);

    if(ImGui::BeginChild("Swap"), swap_size, true)
    {
        for(std::size_t i = 0; i < diff_result_left.size(); i++)
        {
            std::string left_label = "<##" + std::to_string(i);
            std::string right_label = ">##" + std::to_string(i);

            if(!diff_result_left[i].empty() && !diff_result_right[i].empty())
            {
                if(ImGui::Button(left_label.data(), button_size))
                {
                    if(filecontent_left.size() > i && filecontent_right.size() > i)
                    {
                        filecontent_left[i] = filecontent_right[i];
                    }
                    else if(filecontent_right.size() > i)
                    {
                        filecontent_left.push_back(filecontent_right[i]);
                    }

                    create_diff();
                }

                ImGui::SameLine();

                if(ImGui::Button(right_label.data(), button_size))
                {
                    if(filecontent_right.size() > i && filecontent_left.size() > i)
                    {
                        filecontent_right[i] = filecontent_left[i];
                    }
                    else if(filecontent_left.size() > i)
                    {
                        filecontent_right.push_back(filecontent_left[i]);
                    }

                    create_diff();
                }
            }
            else
            {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + line_height);
            }
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    if(ImGui::BeginChild("Diff_Right", child_size, false))
    {
        for(std::size_t i = 0; i < filecontent_right.size(); i++)
        {
            if(!diff_result_right[i].empty())
            {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", filecontent_right[i].c_str());
            }
            else
            {
                ImGui::Text("%s", filecontent_right[i].c_str());
            }
        }
    }
    ImGui::EndChild();

    ImGui::EndChild(); // Parent

    ImGui::PopStyleVar();
}

void FileDiff::draw_stats()
{
    size_t diff_lines_count = 0;

    for(const auto& line : diff_result_left)
    {
        if(!line.empty())
        {
            diff_lines_count++;
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 20.0f);
    ImGui::Text("Diff lines count: %u", diff_lines_count);
}

std::vector<std::string> FileDiff::load_file_content(std::string_view filepath)
{
    std::vector<std::string> content;

    std::ifstream in_file(filepath.data());

    if(!in_file.is_open())
    {
        return content;
    }

    std::string line;

    while(std::getline(in_file, line))
    {
        content.push_back(line);
    }

    in_file.close();

    return content;
}

void FileDiff::save_file_content(std::string_view filepath, std::vector<std::string>& filecontent)
{
    std::ofstream out_file(filepath.data());

    if(!out_file.is_open())
    {
        return;
    }

    for(const auto& line : filecontent)
    {
        out_file << (line + "\n");
    }

    out_file.close();
}

void FileDiff::create_diff()
{
    diff_result_left.clear();
    diff_result_right.clear();

    std::size_t max_num_lines = std::max(filecontent_left.size(), filecontent_right.size());

    for(std::size_t i = 0; i < max_num_lines; i++)
    {
        std::string line_left = filecontent_left.size() > i ? filecontent_left[i] : "EMPTY";
        std::string line_right = filecontent_right.size() > i ? filecontent_right[i] : "EMPTY";

        if(line_left != line_right)
        {
            diff_result_left.push_back(line_left);
            diff_result_right.push_back(line_right);
        }
        else
        {
            diff_result_left.push_back("");
            diff_result_right.push_back("");
        }
    }
}


}