//
// Created by Rieon Ke on 2019-06-28.
//

#include <imgui.h>
#include <iostream>
#include <portable-file-dialogs.h>
#include <unistd.h>
#include "gui/gui_utils.h"
#include "gui_ddl_gen_window.h"
#include <boost/filesystem.hpp>
#include <gui/gui.h>
#include <TextEditor.h>
#include <modules/ddl_gen/include/ddl_gen/ddl_excel_parser.h>
#include <modules/ddl_gen/include/ddl_gen/ddl_table_sql_generator.h>

#define DDL_APP_NAME "DDLGEN"
#define DDL_APP_FULL_NAME  "DDL Generator"
#define DDL_APP_VERSION "0.1-20190628"

void static ShowDDLGenAboutWindow(bool *p_open) {
    if (!ImGui::Begin(_(DDL_APP_FULL_NAME), p_open, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::End();
        return;
    }
    // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
    ImGui::SetNextWindowPos(ImVec2(320, 40), ImGuiCond_FirstUseEver);
//    ImGui::SetNextWindowSize(ImVec2(150, 250), ImGuiCond_FirstUseEver);

    ImGui::Text(DDL_APP_FULL_NAME" %s", DDL_APP_VERSION);
    ImGui::Separator();
    ImGui::Text("By Rieon");
    ImGui::Text(DDL_APP_FULL_NAME"is licensed under the MIT License, see LICENSE for more information.");

    ImGui::End();
}

void dh::gui::gui_ddl_gen_window::show(bool *open) {


    static bool b_show_import_file = false;
    static bool b_parse_excel_error = false;
    static bool b_new_sql_copied = false;
    static bool b_export_template_file = false;
    static bool b_export_template_file_dialog = false;
    static bool b_show_about = false;
    static bool b_rendered = false;

    static std::string sql;
    static std::string export_path;
    static std::string parse_err_str;

    if (b_show_about) {
        ShowDDLGenAboutWindow(&b_show_about);
    }

    if (b_show_import_file) {

        const char *home = getenv("HOME");
        std::string path;

        pfd::open_file open(_("Choose Excel"), home, {"Xlsx Files)", "*",});

        auto p = open.result();

        if (p.empty()) {
            std::cerr << "Please choose an Excel file" << std::endl;
        } else {
            sql.erase();
            b_new_sql_copied = false;
            path = p[0];
            std::cout << path << std::endl;

            dh::ddl_excel_parser parser;
            dh::ddl_table_sql_generator generator;


            try {
                auto tables = parser.parse(path);
                for (auto t : tables) {
                    sql.append(generator.generate(t));
                }
            } catch (dh::ddl_exception &e) {
                b_parse_excel_error = true;
                parse_err_str = e.what();
                ImGui::OpenPopup(_("Import Error"));
            }


            b_rendered = false;
        }

        b_show_import_file = false;
    }
//
    if (b_parse_excel_error) {

        if (ImGui::BeginPopupModal(_("Import Error"), &b_parse_excel_error)) {

            ImGui::Text(_("Import Error"));
            ImGui::SameLine();
            ImGui::Text("%s", parse_err_str.c_str());

            if (ImGui::Button(_("Close"))) {
                b_parse_excel_error = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();

        }
    }


    if (b_export_template_file) {

        b_export_template_file = false;

        pfd::select_folder select_folder(_("Export Template"), getenv("HOME"));
        auto folder = select_folder.result();
        if (!folder.empty()) {

            boost::filesystem::path full_path(boost::filesystem::current_path());
            export_path = folder + "DDL_GEN_" + std::to_string(std::time(nullptr)) + ".xlsx";

            boost::filesystem::copy_file(full_path.string() + "/template.xlsx", export_path);

            ImGui::OpenPopup(_("Export Template Success"));
            b_export_template_file_dialog = true;

        }
    }

    if (ImGui::BeginPopupModal(_("Export Template Success"), &b_export_template_file_dialog)) {

        ImGui::Text(_("Export Success"));
        ImGui::Text(_("Location :"));
        ImGui::SameLine();
        ImGui::Text("%s", export_path.c_str());

        if (ImGui::Button(_("Close"))) {
            b_export_template_file_dialog = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();

    }

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;

    // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
    ImGui::SetNextWindowPos(ImVec2(280, 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(915, 650), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ImGui::Begin(_("Database > DDL Generator"), open, window_flags)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    //menu bar
    //Most "big" widgets share a common width settings by default.
    //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // Use 2/3 of the space for widgets and 1/3 for labels (default)
//    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
    // Use fixed width for labels (by passing a negative value), the rest goes to widgets. We choose a width proportional to our font size.

    // Menu Bar
    if (ImGui::BeginMenuBar()) {

        if (ImGui::BeginMenu(_("File"))) {
            ImGui::MenuItem(_("Import Excel"), NULL, &b_show_import_file);
            ImGui::MenuItem(_("Export Template"), NULL, &b_export_template_file);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(_("Help"))) {
            ImGui::MenuItem(_("About"), NULL, &b_show_about);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    {
        char text[sql.size() + 1];
        strcpy(text, sql.c_str());

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        flags = flags | ImGuiInputTextFlags_AutoSelectAll;

//        dh::gui::gui_utils::help_mark(
//                "Import");

        if (ImGui::Button(_("Copy"))) {
            ImGui::SetClipboardText(text);
            b_new_sql_copied = true;
        }

        if (b_new_sql_copied) {
            ImGui::SameLine();
            ImGui::Text(_("Copied"));
        }

        static bool b_sql_read_only = true;
        ImGui::SameLine();
        ImGui::Checkbox(_("Read Only"), &b_sql_read_only);

        static TextEditor editor;
        static auto lang = TextEditor::LanguageDefinition::SQL();

        if (!b_rendered) {
            editor.SetLanguageDefinition(lang);
            editor.SetText(sql);
            b_rendered = true;
        }

        editor.SetReadOnly(b_sql_read_only);

        editor.Render(_("TextEditor"));
    }


    ImGui::End();
}
