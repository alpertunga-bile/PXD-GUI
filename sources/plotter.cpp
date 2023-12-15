#include "plotter.hpp"

#include <array>
#include <string>

namespace pxd
{
    void Plotter::init()
    {
        ImPlot::CreateContext();
    }

    void Plotter::destroy()
    {
        ImPlot::DestroyContext();
    }

    void Plotter::render(std::string_view label)
    {
        ImGui::Begin(label.data());

        draw_selection();
        draw_plot();

        ImGui::End();
    }

    void Plotter::draw_selection()
    {
        for(const auto [func_name, curr_func] : m_function_map)
        {
            bool selected = m_selected_functions.contains(curr_func);

            if(ImGui::Checkbox(func_name.data(), &selected))
            {
                if(selected)
                {
                    m_selected_functions.insert(curr_func);
                }
                else
                {
                    m_selected_functions.erase(curr_func);
                }
            }
        }
    }

    void Plotter::draw_plot()
    {
        if(m_selected_functions.size() == 0 || (m_selected_functions.size() == 1 && m_selected_functions.contains(PXD_FUNC::NONE)))
        {
            ImPlot::BeginPlot("###plot", ImVec2(-1.0f, -1.0f), ImPlotFlags_NoTitle);

            ImPlot::EndPlot();

            return;
        }

        const int num_points = 10'000;
        double x_min = -100.0;
        double x_max = 100.0;
        double x_step = (std::abs(x_max) + std::abs(x_min)) / num_points;

        std::array<double, num_points> xs = std::array<double, num_points>{};
        std::array<double, num_points> ys = std::array<double, num_points>{};

        ImPlot::BeginPlot("###plot", ImVec2(-1.0f, -1.0f), ImPlotFlags_NoTitle);

        for(const PXD_FUNC& func : m_selected_functions)
        {
            double x = x_min;
            for(int i = 0; i < num_points; i++)
            {
                xs[i] = x;
                ys[i] = evaluate_function(func, x);

                x += x_step;
            }

            std::string plot_label = std::to_string(static_cast<int>(func));

            ImPlot::PlotLine(plot_label.c_str(), xs.data(), ys.data(), num_points);
        }

        ImPlot::EndPlot();
    }

    double Plotter::evaluate_function(const PXD_FUNC function, const double x)
    {
        switch(function)
        {
        case PXD_FUNC::SIN:
            return std::sin(x);
            break;
        case PXD_FUNC::COS:
            return std::cos(x);
            break;
        case PXD_FUNC::NONE:
        default:
            return 0.0;
        }
    }

}