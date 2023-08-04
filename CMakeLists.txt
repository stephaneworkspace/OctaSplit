cmake_minimum_required(VERSION 3.12)

project(OctaSplit VERSION 0.0.1)

#add_subdirectory(JUCE)  # Assumes the JUCE library is in a subdirectory called JUCE

include(FetchContent)

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG master
)

FetchContent_MakeAvailable(juce)

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

target_link_libraries(OctaSplit PRIVATE
    juce::juce_audio_utils
    # Other JUCE modules, third-party libraries...
)

