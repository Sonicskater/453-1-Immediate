cmake_minimum_required(VERSION 3.0)
project(Tut02 VERSION 1.0 LANGUAGES C CXX)

#[[
        OpenGL
]]
find_package(OpenGL REQUIRED)

#[[
        GLFW
]]
set(GLFW_DIR external/glfw)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(${GLFW_DIR})

#[[
        glad
]]
set(GLAD_DIR
    external/glad
    )
set(GLAD_SOURCES
    external/glad/src/glad.c
    )
add_library(glad ${GLAD_SOURCES})
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)
#[[
        Executable
]]
add_executable(${PROJECT_NAME} "main.cpp")

target_compile_definitions(Tut02
    PRIVATE -DGLFW_INCLUDE_NONE
    )

if(MSVC)
    target_compile_definitions(Tut02
        PRIVATE -D_USE_MATH_DEFINES
        )
endif()

set_target_properties(Tut02 PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    MACOSX_BUNDLE TRUE
    MACOSX_FRAMEWORK_IDENTIFIER org.cmake.Tut02
    )

target_compile_definitions(Tut02
    PRIVATE GLFW_INCLUDE_NONE
    )

target_link_libraries(Tut02
    PRIVATE ${OPENGL_gl_LIBRARY}
    PRIVATE glfw
    PRIVATE ${GLFW_LIBRARIES}
    PRIVATE glad
    PRIVATE ${GLAD_LIBRARIES}
    PRIVATE ${CMAKE_DL_LIBS}
    )

target_include_directories(Tut02
    PRIVATE external
    PRIVATE ${GLFW_DIR}/include
    PRIVATE ${GLAD_DIR}/include
	${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    )


