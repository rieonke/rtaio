// dear imgui: standalone example application for GLFW + OpenGL2, using legacy fixed pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_glfw_opengl2/ folder**
// See imgui_impl_glfw.cpp for details.

#include "gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "gui_menu_window.h"
#include <stdio.h>
#include <thread>
#include <iostream>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#define TARGET_FPS 60
#endif

#include <GLFW/glfw3.h>
#include <boost/filesystem.hpp>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    std::cout << "width: " << width << " height: " << height << std::endl;
    glViewport(0, 0, width, height);
}

int main(int, char **) {

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    boost::filesystem::path full_path(boost::filesystem::current_path());
    std::cout << full_path << std::endl;
//    boost::locale::generator gen;

    // Specify location of dictionaries
//    gen.add_messages_path(".");
//    gen.add_messages_domain("hello");
//
//    std::locale::global(gen(""));
//    std::cout.imbue(std::locale());


    //Setup intl
    setlocale(LC_ALL, "");
//
    bindtextdomain("rtaio_main", full_path.string().c_str());
    textdomain("rtaio_main");

//    GLFWwindow *tmp_window = glfwCreateWindow(1280, 720, APP_FULL_NAME, NULL, NULL);
//
//    GLFWmonitor *monitor = glfwGetWindowMonitor(tmp_window);
//    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
//    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
//    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
//    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
//    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
//    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, APP_FULL_NAME, NULL, NULL);

//    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);


    GLFWwindow *window = glfwCreateWindow(1280, 720, APP_FULL_NAME, NULL, NULL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    int width;
    int height;
    int display_width;
    int display_height;
    glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &display_width, &display_height);

    std::cout << _("width") << ":" << width << std::endl;
    std::cout << _("height") << ":" << height << std::endl;
    std::cout << _("display width") << ":" << display_width << std::endl;
    std::cout << _("display height") << ":" << display_height << std::endl;
    float pixel_ratio = (float) display_width / (float) width;


//    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//    io.Fonts->AddFontFromFileTTF(, 18.0f);


    boost::filesystem::path f_path(boost::filesystem::current_path());
    std::string filename = f_path.string() + "/NotoSansCJKsc-Medium.otf";
    std::cout << f_path << std::endl;
    io.Fonts->AddFontFromFileTTF(filename.c_str(), 20.0f * pixel_ratio, NULL, io.Fonts->GetGlyphRangesChineseFull());
    io.FontGlobalScale = 1.0 / pixel_ratio;

//    io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    static bool show_tools_window = true;
    static bool show_status_overlay = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        static bool show_app_metrics = false;
        static bool show_app_style_editor = false;
        static bool show_app_about = false;
        static bool exit_app = false;

        if (show_app_metrics) { ImGui::ShowMetricsWindow(&show_app_metrics); }
        if (show_status_overlay) { showRunStatus(&show_tools_window); }
        if (show_app_style_editor) {
            ImGui::Begin(_("Style Editor"), &show_app_style_editor);
            ImGui::ShowStyleEditor();
            ImGui::End();
        }

        if (exit_app) {
            glfwSetWindowShouldClose(window, 1);
        }

        if (show_app_about) { ShowAboutWindow(&show_app_about); }
        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu(_("File"))) {
                ImGui::MenuItem(_("Exit"), NULL, &exit_app);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(_("View"))) {
                ImGui::MenuItem(_("Tools"), NULL, &show_tools_window);
                ImGui::MenuItem(_("Status"), NULL, &show_status_overlay);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(_("Edit"))) {
                if (ImGui::MenuItem(_("Undo"), "CTRL+Z")) {}
                if (ImGui::MenuItem(_("Redo"), "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem(_("Cut"), "CTRL+X")) {}
                if (ImGui::MenuItem(_("Copy"), "CTRL+C")) {}
                if (ImGui::MenuItem(_("Paste"), "CTRL+V")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(_("Help"))) {
                ImGui::MenuItem(_("Metrics"), NULL, &show_app_metrics);
                ImGui::MenuItem(_("Style Editor"), NULL, &show_app_style_editor);
                ImGui::MenuItem(_("About Rtaio "), NULL, &show_app_about);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_tools_window) dh::gui::gui_menu_window::show(&show_tools_window);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
        // you may need to backup/reset/restore current shader using the commented lines below.
        //GLint last_program;
        //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        //glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);


//        long sl = ((lasttime + 1.0 / TARGET_FPS) - glfwGetTime()) * 1000;
//        if (sl > 0)
//            std::this_thread::sleep_for(std::chrono::milliseconds(sl));
//        lasttime += 1.0 / TARGET_FPS;
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

