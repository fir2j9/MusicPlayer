cmake_minimum_required(VERSION 3.1)
project(MusicPlayer)
set(CMAKE_CXX_STANDARD 11)

# 检测目标架构并设置 Qt5_DIR
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 位
    set(Qt5_DIR "D:/Qt/Qt5.14.2/5.14.2/msvc2017_64/lib/cmake/Qt5" CACHE PATH "Qt5 installation directory (64-bit)")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 位
    set(Qt5_DIR "D:/Qt/Qt5.14.2/5.14.2/msvc2017/lib/cmake/Qt5" CACHE PATH "Qt5 installation directory (32-bit)")
else()
    message(FATAL_ERROR "Unknown architecture: CMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P}")
endif()

# 设置 Qt 使用的模块 (根据你的需要调整)
set(QT_MODULES Core Gui Widgets)  #  如果需要网络功能，添加 Network

# 查找 Qt 5
find_package(Qt5 COMPONENTS ${QT_MODULES} REQUIRED)

# 打印 QT_USE_FILE 变量的值
# message(STATUS "QT_USE_FILE = ${QT_USE_FILE}")

# 包含 Qt 的宏
# include("${QT_USE_FILE}")

# 添加 Qt 的 include 目录 (不再需要手动添加)
# include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS}) # 5.14 不需要

# 添加 Qt 的宏 (如果需要，例如使用 signals 和 slots)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 包含头文件目录
include_directories(
    include
    src
    src/core
    src/core/player
    src/core/playlist
    src/core/metadata
    src/core/audio
    src/core/utils
    src/ui
)

# 查找源文件
# file(GLOB_RECURSE SOURCES
#     "src/*.cpp"
#     "src/core/*.cpp"
#     "src/core/player/*.cpp"
#     "src/core/playlist/*.cpp"
#     "src/core/metadata/*.cpp"
#     "src/core/audio/*.cpp"
#     "src/core/utils/*.cpp"
#     "src/ui/*.cpp"
# )

# 查找源文件和头文件
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"  # 添加头文件
    "src/core/*.cpp"
    "src/core/*.h"  # 添加头文件
    "src/core/player/*.cpp"
    "src/core/player/*.h"  # 添加头文件
    "src/core/playlist/*.cpp"
    "src/core/playlist/*.h"  # 添加头文件
    "src/core/metadata/*.cpp"
    "src/core/metadata/*.h"  # 添加头文件
    "src/core/audio/*.cpp"
    "src/core/audio/*.h"  # 添加头文件
    "src/core/utils/*.cpp"
    "src/core/utils/*.h"  # 添加头文件
    "src/ui/*.cpp"
    "src/ui/*.h"  # 添加头文件
)

# 创建可执行文件，并将找到的源文件作为源文件
add_executable(MusicPlayer ${SOURCES} src/main.cpp)

# 链接 Qt 库
# target_link_libraries(MusicPlayer ${QT_LIBRARIES})
target_link_libraries(MusicPlayer
    Qt5::Core
    Qt5::Gui   # 必须添加！(包含基础绘图功能)
    Qt5::Widgets
    Qt5::WinMain  # Windows应用的入口点
)

if(WIN32 AND MSVC)
    # 设置运行时库类型
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    
    # 添加自动部署脚本
    add_custom_command(TARGET MusicPlayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env "PATH=${Qt5_DIR}/../../../bin;$ENV{PATH}"
                "${Qt5_DIR}/../../../bin/windeployqt.exe"
                --no-svg 
                --no-angle 
                --no-opengl-sw
                "$<TARGET_FILE:MusicPlayer>"
        COMMENT "Deploying Qt runtime..."
    )
endif()

# 如果需要使用 Qt 的资源文件 (.qrc), 添加以下代码 (假设资源文件名为 resources.qrc)
# set(CMAKE_AUTORCC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}) # 可选：如果资源文件不在源目录，指定搜索路径
# add_executable(MusicPlayer ${SOURCES} resources.qrc) # 资源文件添加到源文件列表中
# target_link_libraries(MusicPlayer Qt5::Core Qt5::Widgets Qt5::Gui) # Qt5::Gui 模块也可能需要
