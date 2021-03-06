cmake_minimum_required (VERSION 3.18.2)
project (Arty)

###############
## ENV CHECK ##
###############

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
  message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
  message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

###############
## ARTY CORE ##
###############

file(GLOB_RECURSE ARTY_CORE_FILES
  include/arty/core/*
  include/arty/impl/*
  src/arty/core/*
  src/arty/impl/*
  )

add_library(arty_core ${ARTY_CORE_FILES})
target_compile_features(arty_core PUBLIC cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(arty_core PUBLIC -Werror -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)
target_include_directories(arty_core
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

##############
## EXTERNAL ##
##############

find_package(OpenGL)

if(OPENGL_FOUND)

# Compile external dependencies
add_subdirectory (external)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

endif(OPENGL_FOUND)

#############
## ARTY GL ##
#############

if(OPENGL_FOUND)

file(GLOB_RECURSE ARTY_GL_FILES include/arty/ext/opengl/* src/arty/ext/opengl/* include/arty/ext/glfw/* src/arty/ext/glfw/*)
add_library(arty_gl ${ARTY_GL_FILES})
target_link_libraries(arty_gl PUBLIC arty_core ${ALL_LIBS})
target_compile_features(arty_gl PUBLIC cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(arty_gl PUBLIC -Werror -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)
target_include_directories(arty_gl
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(arty_gl
  SYSTEM PUBLIC
  external/glfw-3.1.2/include/
  external/glew-1.13.0/include/
  external/bullet-2.81-rev2613/src/
)

endif(OPENGL_FOUND)

###############
## ARTY SFML ##
###############

if(OPENGL_FOUND)

add_subdirectory (external/SFML)
file(GLOB_RECURSE ARTY_SFML_FILES include/arty/ext/sfml/* src/arty/ext/sfml/*)
add_library(arty_sfml ${ARTY_SFML_FILES})
target_link_libraries(arty_sfml PUBLIC arty_core sfml-window sfml-system ${OPENGL_LIBRARY} GLEW_1130)
target_compile_features(arty_sfml PUBLIC cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(arty_sfml PUBLIC -Werror -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)
target_include_directories(arty_sfml
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(arty_sfml
  SYSTEM PUBLIC
  external/SFML/include/
  external/glew-1.13.0/include/
)

endif(OPENGL_FOUND)

###################
## ARTY FREETYPE ##
###################

if(OPENGL_FOUND)
file(GLOB_RECURSE ARTY_FREETYPE_FILES include/arty/ext/freetype-gl/* src/arty/ext/freetype-gl/*)
add_library(arty_sfml ${ARTY_SFML_FILES})
target_link_libraries(arty_sfml PUBLIC arty_core sfml-window sfml-system ${OPENGL_LIBRARY} GLEW_1130)
target_compile_features(arty_sfml PUBLIC cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(arty_sfml PUBLIC -Werror -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)
target_include_directories(arty_sfml
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(arty_sfml
  SYSTEM PUBLIC
  external/SFML/include/
  external/glew-1.13.0/include/
)

endif(OPENGL_FOUND)

##############
## ARTY EXE ##
##############

if(OPENGL_FOUND)

add_executable(playground playground/main.cpp playground/tile_systems.cpp playground/tile_systems.hpp)
target_link_libraries(playground arty_core arty_gl)

if(CMAKE_COMPILER_IS_GNUCXX)
add_custom_command(
   TARGET playground POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/playground${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)
endif(CMAKE_COMPILER_IS_GNUCXX)

add_subdirectory(benchmarks)

endif(OPENGL_FOUND)

add_subdirectory(yaide)

###########
## GTEST ##
###########

if(CMAKE_COMPILER_IS_GNUCXX)
enable_testing()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(external/googletest EXCLUDE_FROM_ALL)

add_subdirectory(test)
endif(CMAKE_COMPILER_IS_GNUCXX)


