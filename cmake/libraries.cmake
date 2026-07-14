include(FetchContent)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)
find_package(LayerShellQt REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    LayerShellQt::Interface
)
message("Done!")
