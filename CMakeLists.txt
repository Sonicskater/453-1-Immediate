cmake_minimum_required(VERSION 3.0)
project(Assignment1 VERSION 1.0 LANGUAGES C CXX)

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
        GLM
]]
set(GLM_DIR external/glm/glm)
set(GLM_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLM example programs")
set(GLM_BUILD_TESTS OFF CACHE INTERNAL "Build the GLM test programs")
set(GLM_BUILD_DOCS OFF CACHE INTERNAL "Build the GLM documentation")
set(GLM_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(${GLM_DIR})
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
        Headers
]]
set(HEADERS
   include/vec3f.hpp
   include/triangle.hpp
   include/sierpinski.hpp
   include/shader_tools.hpp
   include/mat4f.hpp
   include/common_matrices.hpp
   include/cube.hpp
   )

#[[
        Sources
]]
set(SOURCES
    src/main.cpp
    src/vec3f.cpp
    src/triangle.cpp
    src/sierpinski.cpp
    src/shader_tools.cpp
    src/mat4f.cpp
    src/common_matrices.cpp
    src/cube.cpp
	src/main2.cpp
    )



#[[
        Executable
]]
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    PRIVATE external
    PRIVATE ${GLFW_DIR}/include
    PRIVATE ${GLAD_DIR}/include
    )

target_compile_definitions(${PROJECT_NAME}
    PRIVATE -DGLFW_INCLUDE_NONE
    )

# copied from learnopengl.com cmakelists for the purpose of putting shaders in external files.
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/windows/${PROJECT_NAME}")
elseif(UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/linux/${PROJECT_NAME}")
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/apple/${PROJECT_NAME}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin/apple/${PROJECT_NAME}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin/apple/${PROJECT_NAME}")
endif(WIN32)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE -D_USE_MATH_DEFINES
        )
endif()

file(GLOB SHADERS
                 "src/shaders/*.vs"
                 # "src/${CHAPTER}/${DEMO}/*.frag"
                 "src/shaders/*.fs"
                 "src/shaders/*.gs"
)
foreach(SHADER ${SHADERS})
    if(WIN32)
        # configure_file(${SHADER} "test")
        add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    elseif(UNIX AND NOT APPLE)
        file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/linux/${PROJECT_NAME})
    elseif(APPLE)
        # create symbolic link for *.vs *.fs *.gs
        get_filename_component(SHADERNAME ${SHADER} NAME)
        makeLink(${SHADER} ${CMAKE_CURRENT_BINARY_DIR}/bin/apple/${PROJECT_NAME}/${SHADERNAME} ${PROJECT_NAME})
    endif(WIN32)
endforeach(SHADER)

#end copied cmake code.

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    MACOSX_BUNDLE TRUE
    MACOSX_FRAMEWORK_IDENTIFIER org.cmake.${PROJECT_NAME}
    )

target_compile_definitions(${PROJECT_NAME}
    PRIVATE GLFW_INCLUDE_NONE
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${OPENGL_gl_LIBRARY}
    PRIVATE glfw
    PRIVATE ${GLFW_LIBRARIES}
    PRIVATE glad
    PRIVATE ${GLAD_LIBRARIES}
    PRIVATE ${CMAKE_DL_LIBS}
    )



