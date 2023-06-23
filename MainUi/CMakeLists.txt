CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "ui")

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

set(VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(VERSION_PATCH ${PROJECT_VERSION_PATCH})

INCLUDE_DIRECTORIES(include Model/include)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/mainui/version.h"
)

FILE(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h")
FILE(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
FILE(GLOB_RECURSE FORMS CONFIGURE_DEPENDS "*.ui")
FILE(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS "*.qrc")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resource/logo/logo.rc")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    QT_ADD_EXECUTABLE(EasyTextEditor
            MANUAL_FINALIZATION
            ${HEADER_FILES}
            ${SRC_FILES}
            ${FORMS}
            ${RESOURCE_FILES}
            ${app_icon_resource_windows}
            )
else ()
    QT_ADD_EXECUTABLE(EasyTextEditor
            MANUAL_FINALIZATION
            ${HEADER_FILES}
            ${SRC_FILES}
            ${FORMS}
            ${RESOURCE_FILES}
            )
endif ()

TARGET_LINK_LIBRARIES(EasyTextEditor PUBLIC Model)
TARGET_LINK_LIBRARIES(EasyTextEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)

set_target_properties(EasyTextEditor PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS EasyTextEditor
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EasyTextEditor)
endif ()

