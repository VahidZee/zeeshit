cmake_minimum_required(VERSION 3.10)
project(cardReader)

set(CMAKE_CXX_STANDARD 11)


add_executable(cardReader main.cpp sourceFiles/preprocess.cpp sourceFiles/ocrprocess.cpp  sourceFiles/algorithms.cpp )
# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

if(CMAKE_VERSION VERSION_LESS "2.8.11")
    # Add OpenCV headers location to your include paths
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

# Declare the executable target built from your sources

# Link your application with OpenCV libraries
target_link_libraries(cardReader ${OpenCV_LIBS})


# Link your application with OpenCV libraries
target_link_libraries(cardReader ${OpenCV_LIBS})
target_link_libraries(cardReader tesseract)
target_link_libraries(cardReader ${Tesseract_LIBS})

find_package( PkgConfig REQUIRED)
find_library(TESSERACT REQUIRED)

pkg_search_module( TESSERACT REQUIRED tesseract )

pkg_search_module( LEPTONICA REQUIRED lept )

include_directories( ${TESSERACT_INCLUDE_DIRS} )

include_directories( ${LEPTONICA_INCLUDE_DIRS} )

link_directories( ${TESSERACT_LIBRARY_DIRS} )

link_directories( ${LEPTONICA_LIBRARY_DIRS} )


target_link_libraries( cardReader ${TESSERACT_LIBRARIES} )

target_link_libraries( cardReader ${LEPTONICA_LIBRARIES} )