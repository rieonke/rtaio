//
// Created by Rieon Ke on 2019-06-28.
//

#include "gui.h"
#include "gui_menu_window.h"
#include <imgui.h>
#include <math.h>
#include <cstdio>
#include <gui/ddl/gui_ddl_gen_window.h>


void dh::gui::gui_menu_window::show(bool *open) {

    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing ui context. failed!");

    static bool show_ddl_gen_window = false;

    if (show_ddl_gen_window) { dh::gui::gui_ddl_gen_window::show(&show_ddl_gen_window); }

    ImGuiWindowFlags window_flags = 0;
//    window_flags |= ImGuiWindowFlags_MenuBar;

    // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
    ImGui::SetNextWindowPos(ImVec2(20, 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(240, 650), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Tools", open, window_flags)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    // Most "big" widgets share a common width settings by default.
    //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // Use 2/3 of the space for widgets and 1/3 for labels (default)
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    // Use fixed width for labels (by passing a negative value), the rest goes to widgets. We choose a width proportional to our font size.

    ImGui::Text(APP_FULL_NAME" (%s)", APP_VERSION);
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Database")) {
        ImGui::Checkbox("DDL Generator", &show_ddl_gen_window);      // Edit bools storing our window open/close state
    }

    // End of ShowDemoWindow()
    ImGui::End();


}

