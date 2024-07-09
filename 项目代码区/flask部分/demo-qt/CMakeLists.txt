cmake_minimum_required(VERSION 3.16)

# 项目名称
project(PyQtHelloWorld)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 库
find_package(Qt6 COMPONENTS Core Widgets Network REQUIRED)

# 将 Qt 的 MOC 文件生成规则添加到 CMake
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 源文件
set(SOURCES
    main.cpp
)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接 Qt6 库
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::Network)
