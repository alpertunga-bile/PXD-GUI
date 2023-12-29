#include "clock.hpp"
#include <chrono>

#include <cmath>

namespace pxd
{
void Clock::init()
{
}

void Clock::destroy()
{
}

void Clock::render(std::string label)
{
    ImGui::Begin(label.data());

    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    center = ImVec2(cursor_pos.x + circle_radius, cursor_pos.y + circle_radius);

    draw_circle(circle_radius);

    get_time();
    const auto [hour_theta, minute_theta, second_theta] = get_theta();

    draw_clock_hand(circle_radius * hours_hand_length, hour_theta, ImColor(1.0f, 0.0f, 0.0f));
    draw_clock_hand(circle_radius * min_hand_length, minute_theta, ImColor(0.0f, 1.0f, 0.0f));
    draw_clock_hand(circle_radius * seconds_hand_length, second_theta, ImColor(0.0f, 0.0f, 1.0f));

    draw_all_hour_strokes();
    draw_all_minute_strokes();

    draw_circle(inner_radius);
    draw_digital_clock();

    ImGui::End();
}

void Clock::draw_circle(const float radius)
{
    ImGui::GetWindowDrawList()->AddCircle(center, radius, ImGui::GetColorU32(ImGuiCol_Text), 100, 2.0f);
}

void Clock::draw_clock_hand(const float radius, const float theta, const ImColor color)
{
    float cos_value = std::cos(theta);
    float sin_value = std::sin(theta);
    ImVec2 endpoint = ImVec2(center.x - radius * cos_value, center.y - radius * sin_value);

    ImGui::GetWindowDrawList()->AddLine(center, endpoint, color, 3.0f);
}

void Clock::draw_all_hour_strokes()
{
    for(uint32_t hr = 0; hr < 12; hr++)
    {
        float theta = (hr * ((2.0f * PI) / 12.0f)) + offset;

        float cos_value = std::cos(theta);
        float sin_value = std::sin(theta);

        ImVec2 start_point = ImVec2(center.x + (circle_radius * hours_stroke_length) * cos_value, center.y + (circle_radius * hours_stroke_length) * sin_value);
        ImVec2 endpoint = ImVec2(center.x + circle_radius * cos_value, center.y + circle_radius * sin_value);
    
        ImGui::GetWindowDrawList()->AddLine(start_point, endpoint, ImGui::GetColorU32(ImGuiCol_Text), 3.0f);
    }
}

void Clock::draw_all_minute_strokes()
{
    for(uint32_t min = 0; min < 60; min++)
    {
        float theta = (min * ((2.0f * PI) / 60.0f)) + offset;

        float cos_value = std::cos(theta);
        float sin_value = std::sin(theta);

        ImVec2 start_point = ImVec2(center.x + (circle_radius * min_stroke_length) * cos_value, center.y + (circle_radius * min_stroke_length) * sin_value);
        ImVec2 endpoint = ImVec2(center.x + circle_radius * cos_value, center.y + circle_radius * sin_value);
    
        ImGui::GetWindowDrawList()->AddLine(start_point, endpoint, ImGui::GetColorU32(ImGuiCol_Text), 3.0f);
    }
}

void Clock::draw_digital_clock()
{
    ImGui::Text("%d:%d:%d", hours, minutues, seconds);
}

void Clock::get_time()
{
    auto curr_timestamp = std::chrono::system_clock::now();
    auto time_now = std::chrono::system_clock::to_time_t(curr_timestamp);
    auto time_struct = std::localtime(&time_now);

    seconds = time_struct->tm_sec;
    minutues = time_struct->tm_min;
    hours = time_struct->tm_hour;    
}

std::tuple<float,float,float> Clock::get_theta()
{
    float seconds_frac = static_cast<float>(seconds);
    float minutes_frac = static_cast<float>(minutues) + seconds_frac / 60.0f;
    float hour_frac = static_cast<float>(hours) + minutes_frac / 60.0f;

    float hour_theta = (hour_frac) * ((2.0f * PI) / 12.0f) + offset;
    float min_theta = (minutes_frac) * ((2.0f * PI) / 60.0f) + offset;
    float sec_theta = (seconds_frac) * ((2.0f * PI) / 60.0f) + offset;

    return std::make_tuple(hour_theta, min_theta, sec_theta);
}

}