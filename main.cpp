#include <iostream>

#include "sources/imgui/imgui.h"
#include "sources/imgui/imgui_impl_glfw.h"
#include "sources/imgui/imgui_impl_opengl3.h"
#include "sources/GLFW/glfw3.h"

#include "sources/main_window.hpp"
#include "sources/clock.hpp"

int main()
{
    pxd::MainWindow main_window;
    main_window.init(1280, 720, "PXD - GUI");

    pxd::Clock clock;
    clock.init();

    while(!main_window.should_close())
    {
        main_window.new_frame();

        clock.render("Paint");

        main_window.render();
    }

    clock.destroy();

    main_window.destroy();

    return 0;
}