cmake_minimum_required(VERSION 3.15)

# ===== 重要：指定 vcpkg 安装目录 =====
# 这会让 vcpkg 使用项目根目录的 vcpkg_installed，而不是在 build 里重新安装
# set(VCPKG_INSTALLED_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed" CACHE PATH "vcpkg installed directory")
# message(STATUS "Using vcpkg installed directory: ${VCPKG_INSTALLED_DIR}")

project(WaterTown VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14) #因为我自己的g++版本不够，这里改成了14
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出构建信息
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "========================================")

# 查找依赖包
message(STATUS "Finding packages...")
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
message(STATUS "All packages found successfully!")

# 收集源文件
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

# 显示找到的源文件
message(STATUS "Source files found:")
foreach(SOURCE_FILE ${SOURCES})
    message(STATUS "  - ${SOURCE_FILE}")
endforeach()

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 设置包含目录（让 #include 能找到头文件）
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad::glad
    glm::glm
    imgui::imgui
)

# Windows + MinGW 特定设置
if(WIN32 AND MINGW)
    message(STATUS "Configuring for MinGW...")
    
    # 静态链接 MinGW 运行时库（避免依赖 DLL）
    target_link_libraries(${PROJECT_NAME} PRIVATE
        -static-libgcc
        -static-libstdc++
        -static
    )
    
    # 链接 Windows 必需的系统库
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
        gdi32
        winmm
        imm32
    )
    
    # 如果是 Release 模式，隐藏控制台窗口
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()
endif()

# 设置输出目录（让生成的 exe 在 build/ 目录下）
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
)

# 复制 assets 目录到构建目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets"
    COMMENT "Copying assets to build directory..."
)

# 显示最终配置信息
message(STATUS "========================================")
message(STATUS "Configuration completed!")
message(STATUS "Executable: ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.exe")
message(STATUS "Assets will be copied to: ${CMAKE_BINARY_DIR}/assets")
message(STATUS "vcpkg dependencies: ${VCPKG_INSTALLED_DIR}")
message(STATUS "========================================")