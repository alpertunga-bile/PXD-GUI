#pragma once

#include "imgui/imgui.h"

#include <string>
#include <array>
#include <string_view>
#include <chrono>
#include <map>
#include <fstream>

namespace pxd
{
    class Calendar
    {
    public:
        struct Meeting
        {
            std::string name;

            void Serialize(std::ofstream& out) const
            {
                const size_t name_length = name.size();

                out.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
                out.write(name.data(), static_cast<std::streamsize>(name.size()));
            }

            static Meeting DeSerialize(std::ifstream& in)
            {
                Meeting meeting;
                size_t name_length = 0;

                in.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
                meeting.name.resize(name_length);

                in.read(meeting.name.data(), static_cast<std::streamsize>(name_length));
            
                return meeting;
            }

            constexpr bool operator==(const Meeting& rhs) const
            {
                return name == rhs.name;
            }
        };

    public:
        void init();
        void destroy();

        void render(std::string label);

    private:
        void draw_date_combo();
        void draw_calendar();
        void draw_add_meeting_window();
        void draw_meeting_list();

        void load_meetings_from_file(std::string_view filename);
        void save_meetings_from_file(std::string_view filename);
        void update_selected_date_variables();

    private:
        int selected_day = 1;
        int selected_month = 1;
        int selected_year = 2023;

        std::chrono::year_month_day selected_date;

        bool add_meeting_window_opened = false;

        float calendar_font_size = 2.0f;

        std::map<std::chrono::year_month_day, std::vector<Meeting>> meetings;

        std::array<std::string_view, 12> month_names = 
        {
            "January",
            "February",
            "March",
            "April",
            "May",
            "June",
            "July",
            "August",
            "September",
            "October",
            "November",
            "December"
        };

        unsigned int min_year = 2000;
        unsigned int max_year = 2038;

        int meeting_window_flags = ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoScrollbar;
        ImVec2 meeting_window_size = ImVec2(300.0f, 100.0f);
        ImVec2 meeting_window_button_size = ImVec2(120.0f, 0.0f);
    };
}