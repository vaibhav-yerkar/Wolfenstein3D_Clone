cmake_minimum_required(VERSION 3.10)

# Set C++17 standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set debug type 
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Project Name
project(wolfenstein3D VERSION 1.0)

# Define Resouce Path
set(RESOURCE_PATH "${CMAKE_SOURCE_DIR}/res")
add_compile_definitions(RESOURCE_PATH="${RESOURCE_PATH}")

# Find Required dependencies
find_package(SDL2 REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

# Find SDL2_image
find_library(SDL2_IMAGE_LIB SDL2_image)
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h)

# Find SDL2_mixer
find_library(SDL2_MIXER_LIB SDL2_mixer)
find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h)

# Ensure SDL2_image and SDL2_mixer were found
if (NOT SDL2_IMAGE_LIB OR NOT SDL2_IMAGE_INCLUDE_DIR)
    message(FATAL_ERROR "SDL2_image not found")
endif()
if (NOT SDL2_MIXER_LIB OR NOT SDL2_MIXER_INCLUDE_DIR)
    message(FATAL_ERROR "SDL2_mixer not found")
endif()

# Add Source files
file(GLOB_RECURSE SRC_FILES Sources/*.cpp Sources/*.c)
add_executable(wolfenstein3D main.cpp ${SRC_FILES})

# mac-OS specific 
if (APPLE) 
	# Include directories
	target_include_directories(wolfenstein3D PRIVATE 
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

# Linux Specific 
elseif (UNIX)
  find_package(GLM REQUIRED)
  if (NOT GLM_FOUND)
    message(FATAL_ERROR "GLM not found! Please install GLM or provide a custom path.")
  endif()

  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_mixer REQUIRED)
  find_package(GLEW REQUIRED)
  find_package(Freetype REQUIRED)

  target_link_libraries(wolfenstein3D PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIB}
    ${SDL2_MIXER_LIB}
    ${FREETYPE_LIBRARIES}
    ${GLM_LIBRARIES}
    ${GLEW_LIBRARIES}
    OpenGL
  )

  target_include_directories(wolfenstein3D PUBLIC
    ${SDL2_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
  )

# Windows-specific
elseif(MSVC)
  # Add vcpkg toolchain file to find dependencies if needed
  if (EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
  endif()

  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_mixer REQUIRED)
  find_package(GLM REQUIRED)
  find_package(GLEW REQUIRED)
  find_package(Freetype REQUIRED)
	
  target_link_libraries(wolfenstein3D PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIB}
    ${SDL2_MIXER_LIB}
    ${FREETYPE_LIBRARIES}
    ${GLM_LIBRARIES}
    ${GLEW_LIBRARIES}
  )

  target_include_directories(wolfenstein3D PUBLIC
    ${SDL2_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
  )

endif()
