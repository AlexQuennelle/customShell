cmake_minimum_required(VERSION 4.2.1)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message("Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

option(CONFIG_USE_ASAN "Whether to enable address sanitizer" ON)
option(CONFIG_USE_UBSAN "Whether to enable undefined behavior sanitizer" ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

add_compile_options("-freflection")

if(CONFIG_USE_UBSAN)
    add_compile_options("-fsanitize=undefined")
elseif(CONFIG_USE_ASAN)
    add_compile_options("-fsanitize=address")
endif()

# Change this to match the name of the current folder
project(
    customShell
    LANGUAGES CXX C
    VERSION 0.0
)

# Libraries here
# include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/libraries.cmake")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick WaylandClient)
find_package(LayerShellQt REQUIRED)
find_package(glaze REQUIRED)

set(glaze_ENABLE_REFLECTION26 ON)

qt_standard_project_setup()
qt_add_executable("${CMAKE_PROJECT_NAME}")

qt_add_qml_module(${CMAKE_PROJECT_NAME}
    URI Shell
    QML_FILES
        main.qml
    SOURCES
        src/main.cpp

        include/qmlTypes.hpp

        include/shellBackend.h
        src/shellBackend.cpp

        include/compositorBackend.h
        src/compositorBackend.cpp

        include/workspace/extWorkspaceManager.h
        src/workspace/extWorkspaceManager.cpp
)
# set(QML_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${QML_INCLUDES})
qt_generate_wayland_protocol_client_sources(${CMAKE_PROJECT_NAME}
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/protocols/ext-workspace-v1.xml
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::WaylandClient
    LayerShellQt::Interface
    glaze::glaze
)


if(CONFIG_USE_UBSAN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
elseif(CONFIG_USE_ASAN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

set(CMAKE_CXX_FLAGS
    "-Wall -Wextra -Wnon-virtual-dtor -Wold-style-cast -Wcast-align\
    -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion\
    -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wduplicated-cond\
    -Wduplicated-branches -Wlogical-op -Wuseless-cast -Wfatal-errors"
)
# file(GLOB_RECURSE
#     MY_SOURCES CONFIGURE_DEPENDS
#      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
# )
target_compile_definitions(
  ${CMAKE_PROJECT_NAME} PUBLIC $<$<CONFIG:Release>:PRODUCTION_BUILD>
                               NAME="${CMAKE_PROJECT_NAME}"
)

set_target_properties(
  "${CMAKE_PROJECT_NAME}"
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin"
             RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/\
${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION}"
)

set(RELEASE_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/\
${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION}/resources/"
)
set(DEBUG_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(
  ${CMAKE_PROJECT_NAME}
  PUBLIC
RESOURCES_PATH=$<IF:$<CONFIG:Release>,"./resources/","${DEBUG_RESOURCE_DIR}">
)

target_compile_features("${CMAKE_PROJECT_NAME}" PRIVATE cxx_std_26)

# target_include_directories(
#     "${CMAKE_PROJECT_NAME}" PRIVATE
#     ${CMAKE_BINARY_DIR}/shaders
# )
# target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})
target_sources("${CMAKE_PROJECT_NAME}"
    PRIVATE
    src/main.cpp

    include/statusBar.hpp
    src/statusBar.cpp

    include/shellBackend.h
    src/shellBackend.cpp

    include/shellController.h
    src/shellController.cpp

    include/compositorBackend.h
    src/compositorBackend.cpp

    include/compositors/niriBackend.h
    src/compositors/niriBackend.cpp
    include/compositors/waylandBackend.hpp
    src/compositors/waylandBackend.cpp

    include/workspace/niriWorkspaceManager.h
    src/workspace/niriWorkspaceManager.cpp
    include/workspace/extWorkspaceManager.h
    src/workspace/extWorkspaceManager.cpp
)

target_include_directories(
  "${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
target_include_directories(
  "${CMAKE_PROJECT_NAME}" PUBLIC "${PROJECT_BINARY_DIR}"
)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} # -E copy_directory
-DDEST_DIR=$<IF:$<CONFIG:Release>,"${RELEASE_RESOURCE_DIR}","${DEBUG_RESOURCE_DIR}">
    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/copyResources.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
