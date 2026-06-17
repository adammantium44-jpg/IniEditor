cmake_minimum_required(VERSION 3.21)

project(IniEditor VERSION 1.0.0 LANGUAGES CXX)
set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")

# Output folder setup (tidak diubah)
if(DEFINED ENV{SKYRIM_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_FOLDER}/Data")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_FOLDER}/Data")
endif()

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
endif()

# SKSE plugin
find_package(CommonLibSSE CONFIG REQUIRED)
add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES
        plugin.cpp
        src/IniEditorUi.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNICODE
    _UNICODE
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

# --- Dear ImGui (from extern/imgui) ---
add_library(imgui STATIC
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imstb"
)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# --- SKSE Menu Framework ---
set(SKSEMFFRAMEWORK_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/SKSEMenuFramework/lib")

add_library(SKSEMenuFramework SHARED IMPORTED GLOBAL)

set_target_properties(SKSEMenuFramework PROPERTIES
    IMPORTED_LOCATION "${SKSEMFFRAMEWORK_LIB_DIR}/SKSEMenuFramework.dll"
    IMPORTED_IMPLIB   "${SKSEMFFRAMEWORK_LIB_DIR}/SKSEMenuFramework.lib"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${SKSEMFFRAMEWORK_LIB_DIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE SKSEMenuFramework)

# Copy built dll into Skyrim/mods folder if OUTPUT_FOLDER is set
if(DEFINED OUTPUT_FOLDER)
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()
endif()
