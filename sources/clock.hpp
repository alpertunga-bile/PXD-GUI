#pragma once

#include "imgui/imgui.h"

#include <string>
#include <string_view>
#include <numbers>
#include <tuple>

namespace pxd
{
    class Clock
    {
    public:
        void init();
        void destroy();

        void render(std::string label);

    private:
        void draw_circle(const float radius);
        void draw_clock_hand(const float radius, const float theta, const ImColor color);
        void draw_all_hour_strokes();
        void draw_all_minute_strokes();
        void draw_digital_clock();

        void get_time();
        
        std::tuple<float, float, float> get_theta();

    private:
        ImVec2 center;

        int32_t seconds = 0;
        int32_t minutues = 0;
        int32_t hours = 0;

        float PI = std::numbers::pi_v<float>;
        float circle_radius = 250.0f;
        float offset = PI / 2.0f;
        float inner_radius = 5.0f;
        float hours_hand_length = 0.75f;
        float min_hand_length = 0.95f;
        float seconds_hand_length = 0.8f;
        float hours_stroke_length = 0.90f;
        float min_stroke_length = 0.95f;
    };
}