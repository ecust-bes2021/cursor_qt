cmake_minimum_required(VERSION 3.16)

project(cursor_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========================================
# 设置输出目录 - 区分 Debug 和 Release
# ========================================
# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)
# 设置库文件输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)
# 设置归档文件输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)

# 确保多配置生成器（如 Visual Studio）也使用相同的输出目录
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin/${OUTPUTCONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin/${OUTPUTCONFIG})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin/${OUTPUTCONFIG})
endforeach()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cursor_qt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET cursor_qt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(cursor_qt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(cursor_qt
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(cursor_qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.cursor_qt)
endif()
set_target_properties(cursor_qt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS cursor_qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cursor_qt)
endif()

# ========================================
# 自动部署 Qt 依赖（Windows）
# ========================================
if(WIN32)
    # 获取 Qt 安装路径
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

    # 使用 windeployqt 自动复制 Qt DLL 到 exe 同级目录
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
        # 添加自定义命令，在构建后自动运行 windeployqt（精简版）
        add_custom_command(TARGET cursor_qt POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --no-translations          # 不复制翻译文件
                --no-system-d3d-compiler   # 不复制 D3D 编译器
                --no-opengl-sw             # 不复制 OpenGL 软件渲染
                --no-svg                   # 不复制 SVG 支持
                --no-network               # 不复制网络插件
                --no-quick-import          # 不复制 QML 插件
                $<TARGET_FILE:cursor_qt>
            COMMENT "Running windeployqt to copy Qt dependencies (minimal)..."
        )
    else()
        message(WARNING "windeployqt not found. Qt dependencies will not be automatically copied.")
    endif()
endif()

# ========================================
# 创建应用程序目录结构
# ========================================
# 在构建后自动创建必要的子目录
add_custom_command(TARGET cursor_qt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:cursor_qt>/config
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:cursor_qt>/data
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:cursor_qt>/logs
    COMMENT "Creating application directory structure (config, data, logs)..."
)

# 可选：复制默认配置文件模板（如果你创建了 config.example.json）
# add_custom_command(TARGET cursor_qt POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         ${CMAKE_SOURCE_DIR}/config.example.json
#         $<TARGET_FILE_DIR:cursor_qt>/config/config.example.json
#     COMMENT "Copying config template..."
# )
