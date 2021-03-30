cmake_minimum_required(VERSION 3.0)
project(Aster)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Aster/ThirdParty/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Aster/ThirdParty/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(Aster/ThirdParty/bullet)

add_subdirectory(Aster/ThirdParty/freetype)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Aster/Public/
                    Aster/ThirdParty/assimp/include/
                    Aster/ThirdParty/bullet/src/
                    Aster/ThirdParty/glad/include/
                    Aster/ThirdParty/glfw/include/
                    Aster/ThirdParty/glm/
                    Aster/ThirdParty/stb/
                    Aster/ThirdParty/freetype/include)

file(GLOB VENDORS_SOURCES Aster/ThirdParty/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Aster/Public/*.h)
file(GLOB PROJECT_SOURCES Aster/Private/*.cpp)
file(GLOB PROJECT_SHADERS Aster/Shaders/*.fs
                          Aster/Shaders/*.gs
                          Aster/Shaders/*.vs)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)
file(GLOB PROJECT_CONFIG Aster/Config/*.conf)
file(GLOB PROJECT_TEXTURES Aster/Textures/*.png)
file(GLOB PROJECT_FONTS Aster/Fonts/*.ttf)
file(GLOB PROJECT_LEVELS Aster/Levels/*.lvl)

source_group("Public" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Private" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("Config" FILES ${PROJECT_CONFIG})
source_group("Textures" FILES ${PROJECT_TEXTURES})
source_group("Fonts" FILES ${PROJECT_FONTS})
source_group("Levels" FILES ${PROJECT_LEVELS})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES} 
                               ${PROJECT_CONFIG} ${PROJECT_TEXTURES} ${PROJECT_FONTS} ${PROJECT_LEVELS})
target_link_libraries(${PROJECT_NAME} assimp glfw freetype
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Aster/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Aster/Config $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_CONFIG})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Aster/Textures $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_TEXTURES})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Aster/Fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_FONTS})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Aster/Levels $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_LEVELS})