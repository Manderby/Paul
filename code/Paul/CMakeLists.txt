cmake_minimum_required(VERSION 3.13)
project(Paul)

set(TARGET_NAME Paul)

add_executable(${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  add_definitions(-DNA_USE_WINDOWS_COMMON_CONTROLS_6=1)
  
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)
  # unicode. Theoretically, needs _MBCS to be undefined, but cmake does that.
  add_definitions(-D_UNICODE)

  # working directory
  set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $(OutDir))

  set(OUTPUT_DIRECTORY $(OutDir))

else()

  set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$CONFIGURATION/)

endif()



# ######### Sources ################

set(sourceFiles
  src/PLApplication.c
  src/PLApplication.h
  src/PLFunction.c
  src/PLFunction.h
  src/PLParam.c
  src/PLParam.h
  src/PLParamController.c
  src/PLParamController.h
  src/PLParamEditorController.c
  src/PLParamEditorController.h
  src/main.c
  src/main.h
  src/PLUser.c
  src/PLWindowController.c
  src/PLWindowController.h
)

source_group("src" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})



# ######### Linking to NALib ################

# Define GUI and OpenGL to be in use and hide the CMake GUI options.
add_definitions(-DNA_COMPILE_GUI=1)
add_definitions(-DNA_COMPILE_OPENGL=1)
set(COMPILE_NALIB_GUI 1 CACHE INTERNAL "GUI required")
set(COMPILE_NALIB_OPENGL 1 CACHE INTERNAL "OpenGL required")

set(NALIB_DIR "../../lib/NALib/code/NALib")
target_include_directories(${TARGET_NAME} PRIVATE ${NALIB_DIR}/src)
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} PRIVATE NALib)



# ######### Linking to other libraries ################

find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE ${OPENGL_gl_LIBRARY})

if (MSVC)
  target_link_libraries(${TARGET_NAME} PRIVATE ComCtl32.lib)
else()
  FIND_LIBRARY(CARBON_FRAMEWORK Carbon)
  FIND_LIBRARY(COCOA_FRAMEWORK Cocoa)
  FIND_LIBRARY(UTYPE_FRAMEWORK UniformTypeIdentifiers)
  target_link_libraries(${TARGET_NAME} PRIVATE ${CARBON_FRAMEWORK})
  target_link_libraries(${TARGET_NAME} PRIVATE ${COCOA_FRAMEWORK})
  target_link_libraries(${TARGET_NAME} PRIVATE ${UTYPE_FRAMEWORK})
  
endif()
