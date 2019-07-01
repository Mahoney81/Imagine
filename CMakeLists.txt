cmake_minimum_required(VERSION 3.9)
project(Imagine)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Concurrent CONFIG REQUIRED)

# Populate a CMake variable with the sources
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.h
        ${PROJECT_SOURCE_DIR}/*.ui
        ${PROJECT_SOURCE_DIR}/*.qrc
        ${PROJECT_SOURCE_DIR}/qdarkstyle/style.qrc)

set(MACOSX_BUNDLE_ICON_FILE app-icon.icns)
set(NixPlayer_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icons/app-icon.icns)
set_source_files_properties(${NixImage_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${NixImage_ICON} ${SOURCE_FILES})

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SCRIPTS_PATH}/Info.plist)
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Concurrent)
