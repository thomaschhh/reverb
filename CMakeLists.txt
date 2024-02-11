cmake_minimum_required(VERSION 3.15)
project(Reverb VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WW_POST_BUILD_INSTALL "SYSTEM")

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_directory_properties (PROPERTIES JUCE_COPY_PLUGIN_AFTER_BUILD ON)

    if (WW_POST_BUILD_INSTALL STREQUAL "SYSTEM")
        set (prefix "/Library/Audio/Plug-Ins")
    else()
        set (prefix "$ENV{HOME}/Library/Audio/Plug-Ins")
    endif()

    set(JUCE_VST3_COPY_DIR "${prefix}/VST3")
    set(JUCE_AU_COPY_DIR "${prefix}/Components")

endif()

add_subdirectory(JUCE)
add_subdirectory(Include)
add_subdirectory(Source)

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME wonderwood
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    PLUGIN_MANUFACTURER_CODE Wowo
    PLUGIN_CODE Wrev                                             
    FORMATS AU VST3                  
    PRODUCT_NAME "Reverb"
    VST3_COPY_DIR ${JUCE_VST3_COPY_DIR}
    AU_COPY_DIR ${JUCE_AU_COPY_DIR})


juce_generate_juce_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/Delay.cpp
        Include/PluginEditor.h      # add .h files for VSCs CMake Tools
        Include/PluginProcessor.h
        Include/Delay.h)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Include")

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
