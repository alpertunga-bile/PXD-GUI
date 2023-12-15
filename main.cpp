#include <iostream>

#include "sources/imgui/imgui.h"
#include "sources/imgui/imgui_impl_glfw.h"
#include "sources/imgui/imgui_impl_opengl3.h"
#include "sources/GLFW/glfw3.h"

#include "sources/main_window.hpp"
#include "sources/file_explorer.hpp"
#include "sources/plotter.hpp"

int main()
{
    pxd::MainWindow main_window;
    main_window.init(1280, 720, "PXD - GUI");

    pxd::FileExplorer file_explorer;
    file_explorer.init();

    pxd::Plotter plotter;
    plotter.init();

    while(!main_window.should_close())
    {
        main_window.new_frame();

        plotter.render("Plot");

        main_window.render();
    }

    plotter.destroy();

    main_window.destroy();

    return 0;
}