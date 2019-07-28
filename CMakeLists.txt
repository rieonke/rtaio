cmake_minimum_required(VERSION 3.14)
project(rtaio)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(CMAKE_VERSION VERSION_LESS 3.0.0)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-std=c++14 COMPILER_SUPPORTS_CXX14)
    check_cxx_compiler_flag(-std=c++0x COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    endif()
else()
    SET(CMAKE_CXX_STANDARD 14)
    SET(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(modules/ddl_gen)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

option(STATIC "Set to ON to build xlnt as a static library instead of a shared library" OFF)
option(USER_SYSTEM_BOOST "User system boost library, default ON" OFF)
option(GUI "GUI" ON)
option(MAC_BUNDLE "Build macOS app bundle" ON)


set(PROJ_LIB ddl_gen)
set(PROJ_GUI ${PROJECT_NAME})
set(PROJ_VERSION 0.0.1)
set(PROJ_COPYRIGHT "By Rieon Ke")
set(PROJ_ID cn.rieon.ratio)
set(PROJ_INFO "Rubbish Toolkit All In One")

if (STATIC)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .a .dylib)
    SET(BUILD_SHARED_LIBS OFF)
    #SET(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")
    SET(Boost_USE_STATIC_LIBS ON)
endif ()


include_directories(include)

if (GUI)

    # resource
    set(GUI_RESOURCE
            resources/NotoSansCJKsc-Medium.otf
            resources/template.xlsx
            distribution/macos/AppIcon.icns
#            translation/zh/LC_MESSAGES/rtaio_main.mo
            )
    file(COPY ${CMAKE_SOURCE_DIR}/resources/NotoSansCJKsc-Medium.otf DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_SOURCE_DIR}/resources/template.xlsx DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_SOURCE_DIR}/translation/zh/LC_MESSAGES/rtaio_main.mo DESTINATION ${CMAKE_BINARY_DIR}/zh/LC_MESSAGES/)


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


    #resources

    if (MAC_BUNDLE)

        set(PROJ_MAC_ICON AppIcon.icns)

#        file(GLOB PROJ_GUI ${CMAKE_SOURCE_DIR}/translation/*)
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
#                MACOSX_PACKAGE_LOCATION Resources/
                RESOURCE "${GUI_RESOURCE}")


        file(COPY ${CMAKE_SOURCE_DIR}/translation/zh/LC_MESSAGES/rtaio_main.mo DESTINATION ${CMAKE_BINARY_DIR}/${PROJ_GUI}.app/Contents/Resources/zh/LC_MESSAGES/)

        # Note Mac specific extension .app
        set(APPS "${CMAKE_BINARY_DIR}/${PROJ_GUI}.app")

        # Directories to look for dependencies
        set(DIRS ${CMAKE_BINARY_DIR})

#        include(BundleUtilities)
        install(CODE "include(BundleUtilities)
        fixup_bundle(\"${APPS}\" \"\" \"${DIRS}\")")
#        FIXUP_BUNDLE(${APPS} ${PROJ_LIB} ${DIRS})


        set(CPACK_GENERATOR "DRAGNDROP")
        include(CPack)

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


#target_link_libraries(${PROJ_LIB} ${CONAN_LIBS})


if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif ()
