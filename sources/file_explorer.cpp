#include "file_explorer.hpp"

#include <iostream>

namespace pxd
{
    void FileExplorer::init()
    {
        m_current_path = std::filesystem::current_path();
        m_selected_entry = m_current_path;
    }

    void FileExplorer::render(std::string label)
    {
        ImGui::Begin(label.c_str());

        draw_menu();
        ImGui::Separator();

        draw_content();
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 100.0f);
        ImGui::Separator();

        draw_actions();
        ImGui::Separator();

        draw_filter();

        ImGui::End();
    }

    void FileExplorer::draw_menu()
    {
        if (ImGui::Button("Go Up") && m_current_path.has_parent_path())
        {
            m_current_path = m_current_path.parent_path();
        }

        ImGui::SameLine();

        ImGui::Text("Current Directory : %s", m_current_path.string().c_str());
    }

    void FileExplorer::draw_content()
    {
        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_current_path))
        {
            const bool is_selected = entry.path() == m_selected_entry;
            const bool is_directory = entry.is_directory();
            const bool is_file = entry.is_regular_file();
            std::string entry_name = entry.path().filename().string();

            if (is_directory)
            {
                entry_name = "[D]" + entry_name;
            }
            else if (is_file)
            {
                entry_name = "[F]" + entry_name;
            }
            

            if (ImGui::Selectable(entry_name.c_str(), is_selected))
            {
                if (is_directory)
                {
                    m_current_path /= entry.path().filename();
                }

                m_selected_entry = entry.path();
            }
        }
    }

    void FileExplorer::draw_actions()
    {
        const bool is_file = std::filesystem::is_regular_file(m_selected_entry);

        if(std::filesystem::is_directory(m_selected_entry))
        {
            ImGui::Text("Selected directory: %s", m_selected_entry.string().c_str());
        }
        else if(is_file)
        {
            ImGui::Text("Selected file: %s", m_selected_entry.string().c_str());
        }
        else
        {
            ImGui::Text("NaN");
        }

        if(is_file && ImGui::Button("Open"))
        {
            open_file();
        }

        ImGui::SameLine();

        if(ImGui::Button("Rename"))
        {
            m_rename_popup_open = true;
            ImGui::OpenPopup("Rename File");
        }

        ImGui::SameLine();

        if(ImGui::Button("Delete"))
        {
            m_delete_popup_open = true;
            ImGui::OpenPopup("Delete File");
        }

        rename_file_popup();
        delete_file_popup();
    }

    void FileExplorer::open_file()
    {
        #ifdef _WIN32
            const auto command = "start \"\" \"" + m_selected_entry.string() + "\"";
        #elif __APPLE__
            const auto command = "open \"" + m_selected_entry.string() + "\"";
        #else
            const auto command = "xdg-open \"" + m_selected_entry.string() + "\"";
        #endif

        std::system(command.c_str());
    }

    void FileExplorer::rename_file_popup()
    {
        if(ImGui::BeginPopupModal("Rename File", &m_rename_popup_open))
        {
            static char buffer_name[512] = {"\0"};

            ImGui::Text("New name: ");
            ImGui::InputText("###new_name", buffer_name, sizeof(buffer_name));

            if(ImGui::Button("Rename"))
            {
                auto new_path = m_selected_entry.parent_path() / buffer_name;

                if(rename_file(m_selected_entry, new_path))
                {
                    m_rename_popup_open = false;
                    m_selected_entry = new_path;
                    std::memset(buffer_name, 0, sizeof(buffer_name));
                }
            }

            ImGui::SameLine();

            if(ImGui::Button("Close"))
            {
                m_rename_popup_open = false;
            }

            ImGui::EndPopup();
        }
    }

    bool FileExplorer::rename_file(std::filesystem::path & old_path, std::filesystem::path & new_path)
    {
        try
        {
            std::filesystem::rename(old_path, new_path);
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
        
    }

    void FileExplorer::delete_file_popup()
    {
        if(ImGui::BeginPopupModal("Delete File", &m_delete_popup_open))
        {
            ImGui::Text("Are you sure want to delete %s", m_selected_entry.filename().string().c_str());

            if(ImGui::Button("Yes"))
            {
                if(delete_file(m_selected_entry))
                {
                    m_selected_entry.clear();
                }

                m_delete_popup_open = false;
            }

            ImGui::SameLine();

            if(ImGui::Button("No"))
            {
                m_delete_popup_open = false;
            }

            ImGui::EndPopup();
        }
    }

    bool FileExplorer::delete_file(std::filesystem::path & path)
    {
        try
        {
            std::filesystem::remove(path);
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
        
    }

    void FileExplorer::draw_filter()
    {
        static char extension_filter[16] = {"\0"};

        ImGui::Text("Filter by extension");
        ImGui::SameLine();
        ImGui::InputText("###in filter", extension_filter, sizeof(extension_filter));

        unsigned int filter_counter = 0;

        for(const auto& entry : std::filesystem::directory_iterator(m_current_path))
        {
            if(!std::filesystem::is_regular_file(entry))
            {
                continue;
            }

            if(entry.path().extension().string() == extension_filter)
            {
                filter_counter++;
            }
        }

        ImGui::Text("Number of files: %u", filter_counter);
    }
}