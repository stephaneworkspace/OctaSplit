cmake_minimum_required(VERSION 3.12)

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

juce_add_gui_app(OctaSplit
    PRODUCT_NAME "OctaSplit"
    COMPANY_NAME "Stéphane Bressani - bressani.dev"
    BUNDLE_ID "bressani.dev.octasplit"
    # Other options...
)


target_sources(OctaSplit PRIVATE
    Source/Main.cpp
    # Other source files...
    )

# Link the libraries to your target
target_link_libraries(OctaSplit PRIVATE
        ${portaudio_BINARY_DIR}/libportaudio.a
        sndfile
        )

target_link_libraries(OctaSplit PRIVATE
    juce::juce_audio_utils
    # Other JUCE modules, third-party libraries...
)

add_custom_command(TARGET OctaSplit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets/ $<TARGET_FILE_DIR:OctaSplit>/Assets/)