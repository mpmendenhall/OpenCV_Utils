cmake_minimum_required(VERSION 3.2)
project(OpenCV_Utils)

###############
# project paths
###############

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
   message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

include(DefaultBuildType.cmake)

##############
# dependencies
##############

# sudo apt install libopencv-dev

find_package(OpenCV REQUIRED)
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
LIST(APPEND EXTLIBS ${OpenCV_LIBS})

##############
# source files
##############

foreach(dir IN ITEMS .)
    include_directories(${dir})
    file(GLOB DIRSRC ${dir}/*.c*)
    LIST(APPEND SOURCE_FILES ${DIRSRC})
endforeach(dir)

#################
# Compile options
#################

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CXXWarnings.cmake)

#############
# version tag
#############

INCLUDE(GitRepoTag.cmake)
SET_SOURCE_FILES_PROPERTIES(CodeVersion.cc PROPERTIES COMPILE_FLAGS ${CODEVERSION_COMPILE_FLAGS})
# force rebuild of CodeVersion
add_custom_command(OUTPUT codeversion COMMAND touch ${PROJECT_SOURCE_DIR}/CodeVersion.cc)
add_custom_target(update_codeversion DEPENDS codeversion)

###############
# Finalize compile flags
###############

# CXXFLAGS compile options, for c++ only.
message(STATUS "c++ compiler flags:")
FOREACH(CXXOPT ${CXXOPTS})
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:${CXXOPT}>)
    message(STATUS "\t${CXXOPT}")
ENDFOREACH()

message(STATUS "Link libraries:")
foreach(l ${EXTLIBS})
    message(STATUS "\t${l}")
endforeach(l)

################
# build targets
################

add_library(OCVU SHARED ${SOURCE_FILES})
target_link_libraries(OCVU ${EXTLIBS})
add_dependencies(OCVU update_codeversion)

add_subdirectory(exec)

