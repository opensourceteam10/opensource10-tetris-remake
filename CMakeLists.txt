cmake_minimum_required(VERSION 3.10)
project(Pixeltetris VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL 수동 경로 설정 (Apple Silicon 기준)
set(SDL2_INCLUDE_DIR "/opt/homebrew/include/SDL2")
set(SDL2_LIBRARY "/opt/homebrew/lib/libSDL2.dylib")
set(SDL2_TTF_INCLUDE_DIRS "/opt/homebrew/include/SDL2")
set(SDL2_TTF_LIBRARIES "/opt/homebrew/lib/libSDL2_ttf.dylib")
set(SDL2_IMAGE_INCLUDE_DIRS "/opt/homebrew/include/SDL2")
set(SDL2_IMAGE_LIBRARIES "/opt/homebrew/lib/libSDL2_image.dylib")

file(GLOB_RECURSE TARGET_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

if (WIN32)
	string(REPLACE "/include/SDL2" "/include/" SDL2_PARENT_DIR ${SDL2_INCLUDE_DIR})
	add_executable(Pixeltetris WIN32 ${TARGET_SOURCES})
else()
	add_executable(Pixeltetris ${TARGET_SOURCES})
endif()

target_include_directories(Pixeltetris PRIVATE
    /opt/homebrew/include
    /opt/homebrew/include/SDL2
)
target_link_libraries(Pixeltetris ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
