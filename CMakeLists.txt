cmake_minimum_required(VERSION 3.16)

project(SAPR VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS PrintSupport)
find_package(Qt6 REQUIRED COMPONENTS Pdf)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SAPR
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/scalablegraphicsview.h src/scalablegraphicsview.cpp
        src/sizetablemodel.h src/sizetablemodel.cpp
        src/app.h src/app.cpp
        src/settingsdialog.h src/settingsdialog.cpp
        src/preprocessor.h src/preprocessor.cpp
        src/nodemodel.h src/nodemodel.cpp
        src/modelmanager.h src/modelmanager.cpp

        src/bouncingtext.h src/bouncingtext.cpp
        src/processor.h src/processor.cpp
        src/scenedrawer.h src/scenedrawer.cpp
        src/postprocessor.h src/postprocessor.cpp
        src/numericdelegate.h src/numericdelegate.cpp
        src/positivenumericdelegate.h src/positivenumericdelegate.cpp
        src/positivedoubledelegate.h src/positivedoubledelegate.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SAPR APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SAPR SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SAPR
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SAPR PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SAPR PRIVATE Qt6::PrintSupport)
target_link_libraries(SAPR PRIVATE Qt6::Pdf)



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SAPR)
endif()
set_target_properties(SAPR PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SAPR
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SAPR)
endif()
