#include "calendar.hpp"

namespace pxd
{
    
void Calendar::init()
{
    load_meetings_from_file("meetings.bin");
}

void Calendar::destroy()
{
    save_meetings_from_file("meetings.bin");
}

void Calendar::render(std::string label)
{
    ImGui::Begin(label.data());

    draw_date_combo();

    ImGui::Separator();

    draw_calendar();

    ImGui::Separator();

    draw_meeting_list();

    if(add_meeting_window_opened)
    {
        draw_add_meeting_window();
    }

    ImGui::End();
}


void Calendar::draw_date_combo()
{
    ImGui::Text("Select a date:");
    ImGui::PushItemWidth(50.0f);

    if(ImGui::BeginCombo("###day", std::to_string(selected_day).data()))
    {
        for(int32_t day_index = 1; day_index <= 31; day_index++)
        {
            std::chrono::year_month_day curr_date = std::chrono::year_month_day(std::chrono::year(selected_year), std::chrono::month(selected_month), std::chrono::day(day_index));

            if(!curr_date.ok())
            {
                break;
            }

            if(ImGui::Selectable(std::to_string(day_index).data(), day_index == selected_day))
            {
                selected_day = day_index;
                selected_date = curr_date;
            }
        }

        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushItemWidth(100.0f);

    if(ImGui::BeginCombo("###month", month_names[selected_month - 1].data() ))
    {
        for(int32_t month_index = 1; month_index <= 12; month_index++)
        {
            std::chrono::year_month_day curr_date = std::chrono::year_month_day(std::chrono::year(selected_year), std::chrono::month(month_index), std::chrono::day(selected_day));

            if(!curr_date.ok())
            {
                break;
            }

            if(ImGui::Selectable(month_names[month_index - 1].data(), month_index == selected_month))
            {
                selected_month = month_index;
                selected_date = curr_date;
            }
        }

        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushItemWidth(60.0f);

    if(ImGui::BeginCombo("###year", std::to_string(selected_year).data()))
    {
        for(int32_t year_index = min_year; year_index <= max_year; year_index++)
        {
            std::chrono::year_month_day curr_date = std::chrono::year_month_day(std::chrono::year(year_index), std::chrono::month(selected_month), std::chrono::day(selected_day));

            if(!curr_date.ok())
            {
                break;
            }

            if(ImGui::Selectable(std::to_string(year_index).data(), year_index == selected_year))
            {
                selected_year = year_index;
                selected_date = curr_date;
            }
        }

        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();

    if(ImGui::Button("Add Meeting"))
    {
        add_meeting_window_opened = true;
    }
}

void Calendar::draw_calendar()
{
    ImVec2 child_size = ImVec2(ImGui::GetContentRegionAvail().x, 400.0f);
    ImGui::BeginChild("###calendar", child_size, false);

    float orig_font_size = ImGui::GetFontSize();
    ImGui::SetWindowFontScale(calendar_font_size);

    auto curr_time = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    std::chrono::year_month_day todays_date = std::chrono::year_month_day(curr_time);

    unsigned int y = selected_year;
    for(int32_t m = 1; m <= 12; m++)
    {
        std::string_view m_prev(month_names[m - 1].substr(0, 3));

        ImGui::Text("%s", m_prev.data()); 
        ImGui::SameLine();

        for(int32_t d = 1; d <= 31; d++)
        {
            std::chrono::year_month_day curr_date = std::chrono::year_month_day(
                std::chrono::year(y),
                std::chrono::month(m),
                std::chrono::day(d)
            );

            if(!curr_date.ok())
            {
                break;
            }

            ImGui::SameLine();

            if(curr_date == todays_date)
            {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%d", d);
            }
            else if(curr_date == selected_date)
            {
                ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "%d", d);
            }
            else if(meetings.contains(curr_date))
            {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", d);
            }
            else
            {
                ImGui::Text("%d", d);
            }

            if(ImGui::IsItemClicked())
            {
                selected_date = curr_date;
                update_selected_date_variables();

            }
        }
    }

    ImGui::SetWindowFontScale(orig_font_size);

    ImGui::EndChild();
}

void Calendar::draw_add_meeting_window()
{
    char meeting_name_buffer[256] = "...";

    ImGui::SetNextWindowSize(meeting_window_size);
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetIO().DisplaySize.x / 2.0f - meeting_window_size.x / 2.0f,
        ImGui::GetIO().DisplaySize.y / 2.0f - meeting_window_size.y / 2.0f
    ));

    ImGui::Begin("###addMeeting", &add_meeting_window_opened, meeting_window_flags);

    ImGui::Text("Add meeting to %d.%s.%d", selected_day, month_names[selected_month - 1].data(), selected_year);

    ImGui::InputText("Meeting name", meeting_name_buffer, sizeof(meeting_name_buffer));

    if(ImGui::Button("Save"))
    {
        Meeting meeting;
        meeting.name = meeting_name_buffer;

        meetings[selected_date].push_back(meeting);
        std::memset(meeting_name_buffer, 0, sizeof(meeting_name_buffer));
        add_meeting_window_opened = false;
    }   

    ImGui::SameLine();

    if(ImGui::Button("Cancel"))
    {
        add_meeting_window_opened = false;
    } 

    ImGui::End();
}

void Calendar::draw_meeting_list()
{
    if(!meetings.contains(selected_date))
    {
        return;

    }

    if(meetings[selected_date].empty())
    {
        ImGui::Text("No meetings");
        return;
    }

    for(const auto& meeting : meetings[selected_date])
    {
        ImGui::BulletText("%s", meeting.name.data());

        if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            std::erase(meetings[selected_date], meeting);
            return;
        }
    }
}

void Calendar::load_meetings_from_file(std::string_view filename)
{
    std::ifstream in_file(filename.data(), std::ios::binary);

    if(!in_file.is_open())
    {
        return;
    }

    size_t meetings_count = 0;
    in_file.read(reinterpret_cast<char*>(&meetings_count), sizeof(meetings_count));

    for(size_t i = 0; i < meetings_count; i++)
    {
        auto date = std::chrono::year_month_day{};

        in_file.read(reinterpret_cast<char*>(&date), sizeof(date));

        size_t meetings_num{0};

        in_file.read(reinterpret_cast<char*>(&meetings_num), sizeof(meetings_num));

        for(std::size_t j = 0; j < meetings_num; j++)
        {
            auto meeting = Meeting::DeSerialize(in_file);
            meetings[date].push_back(meeting);
        }
    }

    in_file.close();
}

void Calendar::save_meetings_from_file(std::string_view filename)
{
    std::ofstream out_file(filename.data(), std::ios::binary);

    if(!out_file.is_open())
    {
        return;
    }

    size_t meeting_count = meetings.size();
    out_file.write(reinterpret_cast<const char*>(&meeting_count), sizeof(meeting_count));

    for(const auto &[date, meeting_vec] : meetings)
    {
        out_file.write(reinterpret_cast<const char*>(&date), sizeof(date));

        const size_t meeting_count = meeting_vec.size();

        out_file.write(reinterpret_cast<const char*>(&meeting_count), sizeof(meeting_count));

        for(const auto& meeting : meeting_vec)
        {
            meeting.Serialize(out_file);
        }
    }

    out_file.close();
}

void Calendar::update_selected_date_variables()
{
    selected_day = static_cast<int>(selected_date.day().operator unsigned int());
    selected_month = static_cast<int>(selected_date.month().operator unsigned int());
    selected_year = static_cast<int>(selected_date.year());
}
}