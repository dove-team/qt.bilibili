cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS ui)

find_package(Qt5 COMPONENTS 
    Widgets 
    Gui 
    Core 
    REQUIRED
)

file(GLOB 
    SOURCES_FILES CONFIGURE_DEPENDS 
    *.cpp 
    cpp/*.cpp
)

file(GLOB 
    HEADER_FILES CONFIGURE_DEPENDS 
    *.h 
    h/*.h 
    ../Qt.BIlibili.Libs/*.h 
    ../Qt.BIlibili.Libs/*/*.h
)

file(GLOB 
    UI_FILES CONFIGURE_DEPENDS 
    ui/*.ui
)

file(GLOB 
    QRC_FILES CONFIGURE_DEPENDS 
    *.qrc
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
    ${QRC_FILES}
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)
