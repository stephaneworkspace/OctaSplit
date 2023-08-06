cmake_minimum_required(VERSION 3.12)
add_compile_definitions(JUCE_MODAL_LOOPS_PERMITTED=1)

# Read each line in the .env file
file(STRINGS .env ENV_LINES)

# Process each line
foreach(LINE ${ENV_LINES})
    # Split the line into NAME and VALUE
    string(REPLACE "=" ";" LINE_LIST ${LINE})
    list(GET LINE_LIST 0 NAME)
    list(GET LINE_LIST 1 VALUE)

    # Process each name
    if("${NAME}" STREQUAL "APP_NAME")
        set(APP_NAME ${VALUE})
    elseif("${NAME}" STREQUAL "APP_VERSION")
        set(APP_VERSION ${VALUE})
    elseif("${NAME}" STREQUAL "APP_BUNDLE")
        set(APP_BUNDLE ${VALUE})
    endif()
endforeach()

message(STATUS "APP_NAME: ${APP_NAME}")
message(STATUS "APP_VERSION: ${APP_VERSION}")
message(STATUS "APP_BUNDLE: ${APP_BUNDLE}")

set(ENV_FILE_PATH "${CMAKE_SOURCE_DIR}/.env")

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
    if (APPLE)
        # Add these to your plist
        set(MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}")
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${APP_VERSION}")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING "${APP_VERSION}")
        set(MACOSX_BUNDLE_ICON_FILE MyIcon.icns)
        set(APP_ICON_MACOSX ${CMAKE_SOURCE_DIR}/icon/MyIcon.icns)
        set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        set(MY_ICON_NAME "MyIcon")
        set(MY_APP_NAME "${APP_NAME}")
        set(MY_APP_VERSION "${APP_VERSION}")
        set(MY_APP_BUNDLE "${APP_BUNDLE}")
        configure_file(${CMAKE_SOURCE_DIR}/Info.plist.in ${CMAKE_BINARY_DIR}/Info.plist)
        juce_add_gui_app(OctaSplit
                PRODUCT_NAME "OctaSplit"
                COMPANY_NAME "Stéphane Bressani - bressani.dev"
                BUNDLE_ID "bressani.dev.octasplit"
                ICON_BIG ${CMAKE_SOURCE_DIR}/icon/MyIcon.icns
                INFOPLIST_FILE ${CMAKE_BINARY_DIR}/Info.plist
                # Other options...
                )
        set_target_properties(OctaSplit PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/Info.plist)
    else ()
        juce_add_gui_app(OctaSplit
                PRODUCT_NAME "OctaSplit"
                COMPANY_NAME "Stéphane Bressani - bressani.dev"
                BUNDLE_ID "bressani.dev.octasplit"
                ICON_BIG ${CMAKE_SOURCE_DIR}/icon/MyIcon.icns
                # Other options...
                )
    endif ()
else ()
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/icon/ap.rc")
    juce_add_gui_app(OctaSplit
            PRODUCT_NAME "OctaSplit"
            COMPANY_NAME "Stéphane Bressani - bressani.dev"
            BUNDLE_ID "bressani.dev.octasplit"
            # Other options...
    )
endif()

if (APPLE)
    target_sources(OctaSplit PRIVATE
        .env
        Source/Misc.h
        Source/Misc.cpp
        Source/MainComponent.h
        Source/MainComponent.cpp
        Source/Main.cpp
        Source/AudioFileProperties.cpp
        Source/AudioFileProperties.h
        ${APP_ICON_MACOSX}
    )
else ()
    target_sources(OctaSplit PRIVATE
        .env
        Source/Misc.h
        Source/Misc.cpp
        Source/MainComponent.h
        Source/MainComponent.cpp
        Source/Main.cpp
        Source/AudioFileProperties.cpp
        Source/AudioFileProperties.h
        icon/app.rc
        #${APP_ICON_RESOURCE_WINDOWS}
    )
endif ()

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
    if (APPLE)
        # Link the libraries to your target
        target_link_libraries(OctaSplit PRIVATE
                #${portaudio_BINARY_DIR}/libportaudio.a
                #portaudio
                #portaudio::portaudio
                "/usr/local/lib/libportaudio.dylib"
                sndfile
                juce::juce_audio_utils
                # Other JUCE modules, third-party libraries...
        )
    else()
        find_library(PORTAUDIO_LIBRARY portaudio)
        find_path(PORTAUDIO_INCLUDE_DIR portaudio.h)
        # Link the libraries to your target
        target_link_libraries(OctaSplit PRIVATE
                ${PORTAUDIO_LIBRARY}
                sndfile
                juce::juce_audio_utils
                # Other JUCE modules, third-party libraries...
        )
    endif()
else ()
    # Link the libraries to your target
    target_link_libraries(OctaSplit PRIVATE
            "C:/Users/Stéphane Bressani/source/repos/portaudio/build/Debug/portaudio_static_x64.lib"
            sndfile
            juce::juce_audio_utils
            # Other JUCE modules, third-party libraries...
    )
endif ()
add_custom_command(TARGET OctaSplit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets/ $<TARGET_FILE_DIR:OctaSplit>/Assets/)
add_custom_command(TARGET OctaSplit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${ENV_FILE_PATH}
        $<TARGET_FILE_DIR:OctaSplit>)
