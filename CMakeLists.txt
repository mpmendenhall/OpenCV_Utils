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

# sudo apt install libopencv-dev libraw-dev

find_package(OpenCV REQUIRED)
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
LIST(APPEND EXTLIBS ${OpenCV_LIBS})

LIST(APPEND SRCDIRS core)

find_library(LIBRAW raw)
if(LIBRAW)
    LIST(APPEND EXTLIBS ${LIBRAW})
    LIST(APPEND CXXOPTS "-DWITH_LIBRAW")
    LIST(APPEND SRCDIRS raw)
endif()

##############
# source files
##############

foreach(dir IN ITEMS ${SRCDIRS})
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
SET_SOURCE_FILES_PROPERTIES(core/CodeVersion.cc PROPERTIES COMPILE_DEFINITIONS "${CODEVERSION_COMPILE_DEFS}")

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

add_subdirectory(exec)

# run "cppcheck" static code analysis on project
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(cppcheck
        cppcheck --project=compile_commands.json --enable=all --quiet --suppress=missingIncludeSystem --suppress=useStlAlgorithm
            -i ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

