cmake_minimum_required(VERSION 3.22)

project(CYNTHIA VERSION 0.0.1)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#################################### JUCE ####################################
add_subdirectory(JUCE)     

juce_add_plugin(Cynthia
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # COMPANY_NAME ...                          # Specify the name of the plugin's author
    IS_SYNTH TRUE                               # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE                       # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE            # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Juce               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Dem0                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS VST3 Standalone                     # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "Cynthia")                     # The name of the final executable, which can differ from the target name

target_sources(Cynthia
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/Synth.cpp
        
    PUBLIC
        Source/Voice.h
        Source/WavetableOscillator.h
        Source/WaveformGenerator.h)

target_compile_definitions(Cynthia
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Cynthia
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_include_directories(Cynthia
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

#################################### Google Test ####################################

include(FetchContent)

# Declares GoogleTest as a dependency
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# Declare the test binary to build
add_executable(CynthiaTests
  Tests/TestWaveformGenerators.cpp
)

# Link binary with necessary targets
target_link_libraries(CynthiaTests
    PRIVATE
        juce::juce_audio_basics
        gtest_main
        Cynthia
)

# Tell CMake where to look for binary's include dependencies
target_include_directories(CynthiaTests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Enable CMake to discover the tests included in the binary
include(GoogleTest)
gtest_discover_tests(CynthiaTests)