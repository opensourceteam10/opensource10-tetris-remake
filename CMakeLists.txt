cmake_minimum_required(VERSION 3.16)
project(Pixeltetris)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 파일들
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# 인클루드 디렉토리
target_include_directories(${PROJECT_NAME} PRIVATE src)

# 플랫폼별 SDL2 설정
if(WIN32)
    # Windows 설정 (vcpkg 사용 권장)
    message(STATUS "Configuring for Windows")
    
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    
    target_link_libraries(${PROJECT_NAME} 
        SDL2::SDL2 
        SDL2::SDL2main
        $<TARGET_NAME_IF_EXISTS:SDL2_ttf::SDL2_ttf>
        $<TARGET_NAME_IF_EXISTS:SDL2_image::SDL2_image>
    )
    
    # Windows용 컴파일 정의
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
    )
    
    # Windows에서 assets 복사 (여러 위치)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/Debug/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/Release/assets
        COMMENT "Copying assets for Windows"
    )
    
elseif(APPLE)
    # macOS 설정
    message(STATUS "Configuring for macOS")
    
    # Homebrew 경로
    set(CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local" ${CMAKE_PREFIX_PATH})
    
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
    
    target_link_libraries(${PROJECT_NAME} 
        SDL2::SDL2 
        SDL2::SDL2main
        SDL2_ttf::SDL2_ttf
        SDL2_image::SDL2_image
    )
    
    # macOS 프레임워크 링크 (필요한 경우)
    target_link_libraries(${PROJECT_NAME}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
    
    # macOS에서 assets 복사
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
        COMMENT "Copying assets for macOS"
    )
    
else()
    # Linux 설정
    message(STATUS "Configuring for Linux")
    
    # pkg-config 사용
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    
    # 인클루드 디렉토리 추가
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
    )
    
    # 라이브러리 링크
    target_link_libraries(${PROJECT_NAME} 
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
    )
    
    # 컴파일 플래그 추가
    target_compile_options(${PROJECT_NAME} PRIVATE 
        ${SDL2_CFLAGS_OTHER}
        ${SDL2_TTF_CFLAGS_OTHER}
        ${SDL2_IMAGE_CFLAGS_OTHER}
    )
    
    # Linux에서 assets 복사
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
        COMMENT "Copying assets for Linux"
    )
    
endif()

# 공통 컴파일 옵션
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    message(STATUS "Building in Debug mode")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    message(STATUS "Building in Release mode")
endif()

# 컴파일러별 경고 설정
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# 필요한 assets 파일들 확인
set(REQUIRED_ASSETS
    "munro.ttf"
    "munro-small.ttf"
    "tetrominoSprites.png"
    "playfieldFrame.png"
)

# assets 존재 확인
foreach(asset ${REQUIRED_ASSETS})
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/assets/${asset}")
        message(WARNING "Required asset missing: assets/${asset}")
    endif()
endforeach()

# 디버그 정보 출력
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")