cmake_minimum_required(VERSION 3.10)

# Set C++17 standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set debug type 
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

project(wolfenstein3D VERSION 1.0)

find_package(SDL2 2 REQUIRED)
find_package(GLM 1 REQUIRED)
find_package(GLEW 2 REQUIRED)
find_package(Freetype REQUIRED)

# Find SDL2_image
find_library(SDL2_IMAGE_LIB SDL2_image HINTS /opt/homebrew/lib)
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h HINTS /opt/homebrew/include/SDL2)


# Ensure SDL2_image were found
if (NOT SDL2_IMAGE_LIB OR NOT SDL2_IMAGE_INCLUDE_DIR)
    message(FATAL_ERROR "SDL2_image not found")
endif()

# Find SDL2_mixer
find_library(SDL2_MIXER_LIB SDL2_mixer HINTS /opt/homebrew/lib)
find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h HINTS /opt/homebrew/include/SDL2)

if (NOT SDL2_MIXER_LIB OR NOT SDL2_MIXER_INCLUDE_DIR)
    message(FATAL_ERROR "SDL2_mixer not found")
endif()

set(RESOURCE_PATH "${CMAKE_SOURCE_DIR}/res")
add_compile_definitions(RESOURCE_PATH="${RESOURCE_PATH}")

file(GLOB_RECURSE SRC_FILES Sources/*.cpp Sources/*.c)
add_executable(wolfenstein3D main.cpp ${SRC_FILES})

# Include directories
target_include_directories(wolfenstein3D PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/Header 
  ${SDL2_INCLUDE_DIR} 
  ${GLM_INCLUDE_DIR} 
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIRS}
  ${SDL2_MIXER_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
)
# Link libraries
target_link_libraries(wolfenstein3D PUBLIC 
  ${SDL2_LIBRARIES} 
  ${SDL2_IMAGE_LIB}
  ${SDL2_MIXER_LIB}
  ${FREETYPE_LIBRARIES}
  ${GLM_LIBRARIES} 
  ${GLEW_LIBRARIES} 
  "-framework OpenGL"
)
