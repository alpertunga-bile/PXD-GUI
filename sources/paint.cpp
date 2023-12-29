#include "paint.hpp"

#include <fstream>

namespace pxd
{

void Paint::init()
{
    current_color = ImColor(255, 255, 255);
    point_draw_size = 2.0f;
}

void Paint::destroy()
{
}

void Paint::render(std::string label)
{
    ImGui::Begin(label.c_str());

    draw_menu();
    draw_canvas();

    ImGui::End();
}

void Paint::draw_menu()
{
    const bool ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const bool s_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
    const bool l_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L));

    if(ImGui::Button("Save") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Save Image");
    }

    ImGui::SameLine();

    if(ImGui::Button("Load") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Load Image");
    }

    ImGui::SameLine();

    if(ImGui::Button("Clear"))
    {
        clear_canvas();
    }

    draw_color_buttons();
    draw_size_settings();

    save_popup();
    load_popup();
}

void Paint::draw_canvas()
{
    canvas_pos = ImGui::GetCursorPos();
    float border_thickness = 1.5f;
    ImVec2 button_size = ImVec2(canvas_size.x + 2.0f * border_thickness, canvas_size.y + 2.0f * border_thickness);

    ImGui::InvisibleButton("##canvas", button_size);

    ImVec2 mouse_pos = ImGui::GetMousePos();
    bool is_mouse_hovering = ImGui::IsItemHovered();

    if(is_mouse_hovering && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        ImVec2 point = ImVec2(mouse_pos.x - canvas_pos.x - border_thickness, mouse_pos.y - canvas_pos.y - border_thickness);

        points.push_back(std::make_tuple(point, current_color, point_draw_size));
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    for(const auto& [point, color, size] : points)
    {
        ImVec2 pos = ImVec2(canvas_pos.x + border_thickness + point.x, canvas_pos.y + border_thickness + point.y);
        draw_list->AddCircleFilled(pos, size, color);
    }

    ImVec2 border_min = canvas_pos;
    ImVec2 border_max = ImVec2(canvas_pos.x + button_size.x - border_thickness, canvas_pos.y + button_size.y - border_thickness);

    draw_list->AddRect(border_min, border_max, IM_COL32(255, 255, 255, 255), 0.0f, ImDrawFlags_Closed, border_thickness);

}

void Paint::draw_color_buttons()
{
    bool is_red = current_color == ImColor(255, 0, 0);
    bool is_green = current_color == ImColor(0, 255, 0);
    bool is_blue = current_color == ImColor(0, 0, 255);
    bool is_white = current_color == ImColor(255, 255, 255);
    bool none_preset_color = !is_red && !is_green && !is_blue && !is_white;

    ImVec4 selected_color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);

    if(is_red)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
    }

    if(ImGui::Button("Red"))
    {
        current_color = ImColor(255, 0, 0);
    }

    if(is_red)
    {
        ImGui::PopStyleColor();
    }

    ImGui::SameLine();

    if(is_green)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
    }

    if(ImGui::Button("Green"))
    {
        current_color = ImColor(0, 255, 0);
    }

    if(is_green)
    {
        ImGui::PopStyleColor();
    }

    ImGui::SameLine();

    if(is_blue)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
    }

    if(ImGui::Button("Blue"))
    {
        current_color = ImColor(0, 0, 255);
    }

    if(is_blue)
    {
        ImGui::PopStyleColor();
    }

    ImGui::SameLine();

    if(is_white)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
    }

    if(ImGui::Button("White"))
    {
        current_color = ImColor(255, 255, 255);
    }

    if(is_white)
    {
        ImGui::PopStyleColor();
    }

    ImGui::SameLine();

    if(none_preset_color)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, selected_color);
    }

    if(ImGui::Button("Choose"))
    {
        ImGui::OpenPopup("Color Picker");
    }

    if(ImGui::BeginPopup("Color Picker"))
    {
        ImGui::ColorPicker3("##color_picker", reinterpret_cast<float*>(&current_color));
    
        ImGui::EndPopup();
    }

    if(none_preset_color)
    {
        ImGui::PopStyleColor();
    }
}

void Paint::draw_size_settings()
{
    ImGui::Text("Draw Size");
    ImGui::SameLine();

    ImGui::PushItemWidth(canvas_size.x - ImGui::GetCursorPosX());
    ImGui::SliderFloat("##draw_size", &point_draw_size, 1.0f, 10.0f);
    ImGui::PopItemWidth();
}

void Paint::save_popup()
{
    char savefilename_buffer[256] = "text.bin";
    const bool esc_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popup_size);
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetIO().DisplaySize.x / 2.0f - popup_size.x / 2.0f,
        ImGui::GetIO().DisplaySize.y / 2.0f - popup_size.y / 2.0f
    ));

    if(ImGui::BeginPopupModal("Save Image", nullptr, popup_flags))
    {
        ImGui::InputText("Filename", savefilename_buffer, sizeof(savefilename_buffer));

        if(ImGui::Button("Save", popupbutton_size))
        {
            save_image(filenamebuffer);

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

void Paint::load_popup()
{
    char loadfilename_buffer[256] = "text.bin";
    const bool esc_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popup_size);
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetIO().DisplaySize.x / 2.0f - popup_size.x / 2.0f,
        ImGui::GetIO().DisplaySize.y / 2.0f - popup_size.y / 2.0f
    ));

    if(ImGui::BeginPopupModal("Load Image", nullptr, popup_flags))
    {
        ImGui::InputText("Filename", loadfilename_buffer, sizeof(loadfilename_buffer));

        if(ImGui::Button("Load", popupbutton_size))
        {
            load_image(filenamebuffer);

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

void Paint::save_image(std::string_view filename)
{
    std::ofstream out_file(filename.data(), std::ios::binary);

    if(!out_file.is_open())
    {
        return;
    }

    size_t point_count = points.size();
    out_file.write(reinterpret_cast<const char*>(&point_count), sizeof(point_count));

    for(const auto &[point, color, size] : points)
    {
        out_file.write(reinterpret_cast<const char*>(&point), sizeof(point));
        out_file.write(reinterpret_cast<const char*>(&color), sizeof(color));
        out_file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    }

    out_file.close();
}

void Paint::load_image(std::string_view filename)
{
    std::ifstream in_file(filename.data(), std::ios::binary);

    if(!in_file.is_open())
    {
        return;
    }


    size_t point_count = 0;
    in_file.read(reinterpret_cast<char*>(&point_count), sizeof(point_count));

    for(size_t i = 0; i < point_count; i++)
    {
        ImVec2 point = ImVec2{};
        ImColor color = ImColor{};
        float size = float{};
        
        in_file.read(reinterpret_cast<char*>(&point), sizeof(point));
        in_file.read(reinterpret_cast<char*>(&color), sizeof(color));
        in_file.read(reinterpret_cast<char*>(&size), sizeof(size));

        points.push_back(std::make_tuple(point, color, size));
    }

    in_file.close();
}

void Paint::clear_canvas()
{
    points.clear();
}

    
}