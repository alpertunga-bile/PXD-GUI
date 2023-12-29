#include "text_editor.hpp"

#include <filesystem>
#include <fstream>

namespace pxd
{

void TextEditor::init()
{
    std::memset(text_buffer, 0, buffer_size);
}

void TextEditor::destroy()
{
}

void TextEditor::render(std::string label)
{
    ImGui::Begin(label.c_str());

    draw_menu();
    draw_content();
    draw_info();

    ImGui::End();
}

void TextEditor::draw_menu()
{
    const bool ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const bool s_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
    const bool l_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L));

    if(ImGui::Button("Save") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if(ImGui::Button("Load") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if(ImGui::Button("Clear"))
    {
        std::memset(text_buffer, 0, buffer_size);
    }

    save_popup();
    load_popup();
}

void TextEditor::draw_content()
{
    ImVec2 inputtext_size = ImVec2(1200.0f, 625.0f);
    ImVec2 linenumbers_size = ImVec2(30.0f, inputtext_size.y);
    int text_flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll;

    ImGui::BeginChild("LineNumbers", linenumbers_size);

    const auto line_count = std::count(text_buffer, text_buffer + buffer_size, '\n') + 1;

    for(int i = 1; i <= line_count; i++)
    {
        ImGui::Text("%d", i);
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::InputTextMultiline("###input_field", text_buffer, buffer_size, inputtext_size);
}

void TextEditor::draw_info()
{
    if(current_filename.size() == 0)
    {
        ImGui::Text("No file is opened");
        return;
    }

    std::string file_ext = get_file_extension(current_filename);

    ImGui::Text("Opened File: %s", current_filename.data());
}

void TextEditor::save_popup()
{
    char savefilename_buffer[256] = "text.txt";
    const bool esc_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popup_size);
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetIO().DisplaySize.x / 2.0f - popup_size.x / 2.0f,
        ImGui::GetIO().DisplaySize.y / 2.0f - popup_size.y / 2.0f
    ));

    if(ImGui::BeginPopupModal("Save File", nullptr, popup_flags))
    {
        ImGui::InputText("Filename", savefilename_buffer, sizeof(savefilename_buffer));

        if(ImGui::Button("Save", popupbutton_size))
        {
            save_file(savefilename_buffer);
            current_filename = savefilename_buffer;

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(ImGui::Button("Cancel", popupbutton_size) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup(); 
    }
}

void TextEditor::load_popup()
{
    char loadfilename_buffer[256] = "text.txt";
    const bool esc_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popup_size);
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetIO().DisplaySize.x / 2.0f - popup_size.x / 2.0f,
        ImGui::GetIO().DisplaySize.y / 2.0f - popup_size.y / 2.0f
    ));

    if(ImGui::BeginPopupModal("Load File", nullptr, popup_flags))
    {
        ImGui::InputText("Filename", loadfilename_buffer, sizeof(loadfilename_buffer));

        if(ImGui::Button("Save", popupbutton_size))
        {
            load_file(loadfilename_buffer);
            current_filename = loadfilename_buffer;

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if(ImGui::Button("Cancel", popupbutton_size) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void TextEditor::save_file(std::string_view filename)
{
    std::ofstream out_file(filename.data());

    if(!out_file.is_open())
    {
        return;
    }

    out_file << text_buffer;
    out_file.close();
}

void TextEditor::load_file(std::string_view filename)
{
    std::ifstream in_file(filename.data());

    if(!in_file.is_open())
    {
        return;
    }

    std::stringstream buffer = std::stringstream();
    buffer << in_file.rdbuf();

    std::memcpy(text_buffer, buffer.str().data(), buffer_size);

    in_file.close();
}

std::string TextEditor::get_file_extension(std::string_view filename)
{
    std::filesystem::path filepath(current_filename);

    return filepath.extension().string();
}


}