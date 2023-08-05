cmake_minimum_required(VERSION 3.12)
add_compile_definitions(JUCE_MODAL_LOOPS_PERMITTED=1)
if (UNIX AND NOT APPLE)
    set(CMAKE_C_COMPILER "/usr/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
    add_compile_definitions(JUCE_USE_XRANDR=0)
    add_definitions(-DJUCE_ENABLE_WIN32_FILETYPE_ASSOCIATION=1)
endif()

project(OctaSplit VERSION 0.0.1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
add_compile_definitions(JUCE_DISPLAY_SPLASH_SCREEN=0)

file(GLOB_RECURSE ASSETS_FILES RELATIVE ${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/Assets/*")
foreach(ASSETS_FILE ${ASSETS_FILES})
    configure_file(${ASSETS_FILE} ${ASSETS_FILE} COPYONLY)
endforeach()

include(FetchContent)

FetchContent_Declare(
        portaudio
        GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
        GIT_TAG        master
)

FetchContent_Declare(
        libsndfile
        GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
        GIT_TAG        master
)

FetchContent_Declare(
        juce
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG master
)

FetchContent_MakeAvailable(portaudio libsndfile juce)

if (UNIX)
    juce_add_gui_app(OctaSplit
        PRODUCT_NAME "OctaSplit"
        COMPANY_NAME "Stéphane Bressani - bressani.dev"
        BUNDLE_ID "bressani.dev.octasplit"
        ICON_BIG ${CMAKE_SOURCE_DIR}/icon/MyIcon.icns
        # Other options...
    )
else ()
    juce_add_gui_app(OctaSplit
            PRODUCT_NAME "OctaSplit"
            COMPANY_NAME "Stéphane Bressani - bressani.dev"
            BUNDLE_ID "bressani.dev.octasplit"
            # Other options...
    )
endif()

target_sources(OctaSplit PRIVATE
    Source/Main.cpp Source/AudioFileProperties.cpp Source/AudioFileProperties.h
)

if (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.0)
    pkg_check_modules(CURL REQUIRED libcurl)

    include_directories(${GTK_INCLUDE_DIRS} ${WEBKIT2GTK_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
    link_directories(${GTK_LIBRARY_DIRS} ${WEBKIT2GTK_LIBRARY_DIRS} ${CURL_LIBRARY_DIRS})

    add_definitions(${GTK_CFLAGS_OTHER} ${WEBKIT2GTK_CFLAGS_OTHER} ${CURL_CFLAGS_OTHER})
    target_link_libraries(OctaSplit PRIVATE ${GTK_LIBRARIES} ${WEBKIT2GTK_LIBRARIES} ${CURL_LIBRARIES})
endif ()

if (UNIX)
    # Link the libraries to your target
    target_link_libraries(OctaSplit PRIVATE
        ${portaudio_BINARY_DIR}/libportaudio.a
        sndfile
        juce::juce_audio_utils
        # Other JUCE modules, third-party libraries...
    )
else ()
    # Link the libraries to your target
    target_link_libraries(OctaSplit PRIVATE
            "C:/Users/Steee/CLionProjects/2023/portaudio-19.7.0/build/Debug/portaudio_static_x64.lib"
            sndfile
            juce::juce_audio_utils
            # Other JUCE modules, third-party libraries...
    )
endif ()
add_custom_command(TARGET OctaSplit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets/ $<TARGET_FILE_DIR:OctaSplit>/Assets/)