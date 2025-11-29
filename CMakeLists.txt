cmake_minimum_required(VERSION 3.16)

# ========================================
# 项目名称配置（创建新项目时只需修改这里）
# ========================================
set(PROJECT_NAME "qt_tool_template")

project(${PROJECT_NAME} VERSION 0.0.0 LANGUAGES CXX)

# ========================================
# 安装程序配置（Inno Setup）- 新项目请修改这里
# ========================================
set(APP_PUBLISHER "BES")
set(APP_DESCRIPTION "Qt Tool Template Application")
set(APP_URL "https://github.com/yourusername/yourproject")
set(APP_GUID "{{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置 AUTOUIC 搜索路径（当 UI 文件和源文件不在同一目录时需要）
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC 编译器需要此选项才能正确报告 __cplusplus 值
if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

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
        src/main.cpp
        src/mainwindow.cpp
        src/logger.cpp
        include/mainwindow.h
        include/logger.h
        ui/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE #用于控制是否显示控制台
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
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
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --no-translations          # 不复制翻译文件
                --no-system-d3d-compiler   # 不复制 D3D 编译器
                --no-opengl-sw             # 不复制 OpenGL 软件渲染
                --no-svg                   # 不复制 SVG 支持
                --no-network               # 不复制网络插件
                --no-quick-import          # 不复制 QML 插件
                $<TARGET_FILE:${PROJECT_NAME}>
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
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/logs
    COMMENT "Creating application directory structure (config, data, logs)..."
)

# 复制资源文件夹和配置文件夹到输出目录（整个文件夹复制，添加新文件无需修改此处）
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
    COMMENT "Copying resources and config folders..."
)

# ========================================
# 安装程序构建（Inno Setup）
# ========================================
# 使用方法：
#   1. 编译 Release：cmake --build build --config Release
#   2. 构建安装程序：cmake --build build --target installer
# ========================================
if(WIN32)
    # 从注册表读取 Inno Setup 安装路径
    get_filename_component(INNO_SETUP_PATH
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Inno Setup 6_is1;InstallLocation]"
        ABSOLUTE CACHE
    )

    find_program(ISCC_EXECUTABLE ISCC
        HINTS
            "${INNO_SETUP_PATH}"
        PATHS
            "C:/Program Files (x86)/Inno Setup 6"
            "C:/Program Files/Inno Setup 6"
            "D:/software/innosetup/Inno Setup 6"
    )

    # 从模板生成 setup.iss（实现单一信息源）
    if(EXISTS "${CMAKE_SOURCE_DIR}/installer/setup.iss.in")
        configure_file(
            "${CMAKE_SOURCE_DIR}/installer/setup.iss.in"
            "${CMAKE_SOURCE_DIR}/installer/setup.iss"
            @ONLY
        )
        message(STATUS "Generated installer/setup.iss from template")
    endif()

    if(ISCC_EXECUTABLE)
        # 添加自定义目标：构建安装程序
        add_custom_target(installer
            COMMAND ${ISCC_EXECUTABLE} "${CMAKE_SOURCE_DIR}/installer/setup.iss"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/installer"
            COMMENT "Building installer with Inno Setup..."
            VERBATIM
        )

        # 确保在构建安装程序之前先构建主程序
        add_dependencies(installer ${PROJECT_NAME})

        message(STATUS "Inno Setup found: ${ISCC_EXECUTABLE}")
        message(STATUS "Use 'cmake --build build --target installer' to create installer")
    else()
        message(STATUS "Inno Setup not found. Installer target will not be available.")
        message(STATUS "Download from: https://jrsoftware.org/isinfo.php")
    endif()
endif()
