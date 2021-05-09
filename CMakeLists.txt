cmake_minimum_required(VERSION 3.0)
project(Aster)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(ThirdParty/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(ThirdParty/assimp)

add_subdirectory(ThirdParty/freetype)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++14")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Source/
                    ThirdParty/assimp/include/
                    ThirdParty/glad/include/
                    ThirdParty/glfw/include/
                    ThirdParty/glm/
                    ThirdParty/stb/
                    ThirdParty/freetype/include/
                    ThirdParty/json/single_include/)

file(GLOB VENDORS_SOURCES ThirdParty/glad/src/glad.c)
file(GLOB PROJECT_SOURCES Source/*.h
                          Source/*.cpp
                          Source/Entity/*.h
                          Source/Entity/*.cpp)
file(GLOB PROJECT_SHADERS Shaders/*.fs
                          Shaders/*.gs
                          Shaders/*.vs)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)
file(GLOB PROJECT_CONFIG Config/*.conf)
file(GLOB PROJECT_TEXTURES 	Textures/*.png 
							Textures/*.anim 
							Textures/*.hit)
file(GLOB PROJECT_FONTS Fonts/*.ttf)
file(GLOB PROJECT_LEVELS 	Levels/*.lvl
							Levels/*.json)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${PROJECT_SOURCES})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("Config" FILES ${PROJECT_CONFIG})
source_group("Textures" FILES ${PROJECT_TEXTURES})
source_group("Fonts" FILES ${PROJECT_FONTS})
source_group("Levels" FILES ${PROJECT_LEVELS})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES} 
                               ${PROJECT_CONFIG} ${PROJECT_TEXTURES} ${PROJECT_FONTS} ${PROJECT_LEVELS})

target_link_libraries(${PROJECT_NAME} assimp glfw freetype ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Config $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_CONFIG})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Textures $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_TEXTURES})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_FONTS})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Levels $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_LEVELS})
