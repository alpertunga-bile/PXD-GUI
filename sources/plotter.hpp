#include "imgui/implot.h"

#include <string_view>
#include <set>
#include <unordered_map>

namespace pxd
{
    enum class PXD_FUNC
    {
        NONE,
        SIN,
        COS,
    };

    class Plotter
    {
    public:
        void init();
        void destroy();

        void render(std::string_view label);

    private:
        void draw_selection();
        void draw_plot();
        double evaluate_function(const PXD_FUNC function, const double x);

    private:
        std::unordered_map<std::string_view, PXD_FUNC> m_function_map = 
        {
            {"unk", PXD_FUNC::NONE},
            {"sin(x)", PXD_FUNC::SIN},
            {"cos(x)", PXD_FUNC::COS}
        };

        std::set<PXD_FUNC> m_selected_functions;
    };
}