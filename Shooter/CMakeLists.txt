cmake_minimum_required(VERSION 3.10.0)
project(Shooter VERSION 0.1.0 LANGUAGES C CXX)
# Set the C++ standard
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# Set the warning level
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Set the output directory 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}) 

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Set the Target
set(TARGET ${PROJECT_NAME}_${CMAKE_SYSTEM_NAME})

set(SOURCES
    "src/main.cpp"
    "src/Game.cpp"
    "src/SceneMain.cpp"
    "src/SceneTitle.cpp"
    "src/object.cpp"
    "src/TextRender.cpp"
    "src/Scene.cpp"
    "src/SceneEnd.cpp"
)
if(WIN32)
    list(APPEND SOURCES "resources.rc")
endif()

# Add the executable
add_executable(${TARGET} 
    ${SOURCES}
        )

# Link the executable with sdl2
target_link_libraries(${TARGET} 
    ${SDL2_LIBRARIES} 
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)


if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
endif()