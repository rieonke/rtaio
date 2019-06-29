cmake_minimum_required(VERSION 3.14)
project(rtaio)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

option(STATIC "Set to ON to build xlnt as a static library instead of a shared library" OFF)
option(USER_SYSTEM_BOOST "User system boost library, default ON" ON)
option(GUI "GUI" ON)
option(MAC_BUNDLE "Build macOS app bundle" OFF)

set(PROJ_LIB ${PROJECT_NAME}_lib)
set(PROJ_CLI ${PROJECT_NAME}_cli)
set(PROJ_GUI ${PROJECT_NAME})
set(PROJ_VERSION 0.0.1)
set(PROJ_COPYRIGHT "By Rieon Ke")
set(PROJ_ID cn.rieon.ratio)
set(PROJ_INFO "Rubbish Toolkit All In One")

if (STATIC)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    SET(BUILD_SHARED_LIBS OFF)
    SET(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")
    set(Boost_USE_STATIC_LIBS ON)
endif ()


include_directories(include)
include_directories(ddl)

add_library(${PROJ_LIB} ddl/ddl_table.cpp ddl/ddl_table.h ddl/ddl_table_column.cpp ddl/ddl_table_column.h ddl/ddl_table_sql_generator.cpp ddl/ddl_table_sql_generator.h ddl/ddl_excel_parser.cpp ddl/ddl_excel_parser.h gui/gui.h ddl/ddl_exception.cpp ddl/ddl_exception.h include/common.h)
add_executable(${PROJ_CLI} ddl/main.cpp)
target_link_libraries(${PROJ_CLI} ${PROJ_LIB})

if (GUI)

    # resource
    set(GUI_RESOURCE
            resources/NotoSansCJKsc-Medium.otf
            resources/template.xlsx
            distribution/macos/AppIcon.icns
            )
    file(COPY ${CMAKE_SOURCE_DIR}/resources/NotoSansCJKsc-Medium.otf DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_SOURCE_DIR}/resources/template.xlsx DESTINATION ${CMAKE_BINARY_DIR})


    # GLFW
    add_subdirectory(third_party/glfw)
    include_directories(third_party/glfw/include)
    include_directories(third_party/pfd)
    include_directories(third_party/imgui_code_editor)

    #imgui
    set(IMGUI_DIR third_party/imgui)
    set(IMGUI_CORE ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
    set(IMGUI_GLFW third_party/imgui/examples/imgui_impl_glfw.cpp third_party/imgui/examples/imgui_impl_opengl2.cpp)
    include_directories(${IMGUI_DIR} third_party/imgui/)
    include_directories(third_party/imgui/include)

    set(IMGUI_CODE_EDITOR third_party/imgui_code_editor/TextEditor.cpp third_party/imgui_code_editor/TextEditor.h)

    add_executable(${PROJ_GUI} ${GUI_RESOURCE} gui/gui_main.cpp ${IMGUI_CORE} ${IMGUI_CODE_EDITOR} ${IMGUI_GLFW} gui/gui_menu_window.cpp gui/gui_menu_window.h gui/ddl/gui_ddl_gen_window.cpp gui/ddl/gui_ddl_gen_window.h gui/gui_utils.cpp gui/gui_utils.h)
    target_link_libraries(${PROJ_GUI} ${PROJ_LIB})

    find_package(OpenGL REQUIRED)
    if (OPENGL_FOUND)
        include_directories(${OPENGL_INCLUDE_DIR})
        target_link_libraries(${PROJ_GUI} ${OPENGL_LIBRARIES})
    endif ()

    target_link_libraries(${PROJ_GUI} glfw ${GLFW_LIBRARIES})


    #resources

    if (MAC_BUNDLE)

        set(PROJ_MAC_ICON AppIcon.icns)

        set_target_properties(${PROJ_GUI} PROPERTIES
                MACOSX_BUNDLE TRUE
                MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/distribution/macos/Info.plist.in
                MACOSX_BUNDLE_BUNDLE_NAME ${PROJ_GUI}
                MACOSX_BUNDLE_BUNDLE_VERSION ${PROJ_VERSION}
                MACOSX_BUNDLE_COPYRIGHT ${PROJ_COPYRIGHT}
                MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJ_ID}
                MACOSX_BUNDLE_ICON_FILE ${PROJ_MAC_ICON}
                MACOSX_BUNDLE_INFO_STRING ${PROJ_INFO}
                MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJ_VERSION}
                MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJ_VERSION}
                RESOURCE "${GUI_RESOURCE}")

    endif ()

endif ()

if (USER_SYSTEM_BOOST)
    find_package(Boost REQUIRED COMPONENTS filesystem)
    if (Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        target_link_libraries(${PROJ_LIB} ${Boost_LIBRARIES})
        target_link_libraries(${PROJ_GUI} ${Boost_LIBRARIES})
    endif ()
endif ()


#add third party library
set(XLNT_DIR third_party/xlnt)
add_subdirectory(${XLNT_DIR})
include_directories(${XLNT_DIR}/include)
target_link_libraries(${PROJ_LIB} xlnt)


find_package(Intl REQUIRED)
if (Intl_FOUND)
    include_directories(${Intl_INCLUDE_DIRS})
    target_link_libraries(${PROJ_CLI} ${Intl_LIBRARIES})
    target_link_libraries(${PROJ_LIB} ${Intl_LIBRARIES})
    target_link_libraries(${PROJ_GUI} ${Intl_LIBRARIES})
endif ()

#find_package(PKGCONFIG REQUIRED)
#if (PKG_CONFIG_FOUND)
#    pkg_check_modules(GETTEXT REQUIRED gettext)
#endif ()
