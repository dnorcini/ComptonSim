#----------------------------------------------------------------------------
# Compile command, from inside build directory:
# cmake ../ComptonSim
# make -j8 

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(ComptonSim)

add_compile_options(-std=c++11)

#----------------------------------------------------------------------------
#For Mac OS X Use @rpath on OS X
if(APPLE)
  if(POLICY CMP0042)
      CMAKE_POLICY(SET CMP0042 NEW)
   endif()
endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build ComptonSim with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
   include(${Geant4_USE_FILE})
   Message("Geant4_USE_FILE = ${Geant4_USE_FILE}")

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#-------------------------------
# ROOT dependencies
#-------------------------------

######## locate ROOT install, headers, includes
include(FindROOT.cmake)
include_directories(${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(ComptonSim ComptonSim.cc ${sources} ${headers})
target_link_libraries(ComptonSim ${Geant4_LIBRARIES})

##############
# Output paths
##############

#set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

#################
# Copy Macros
#################

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/mac")
file(COPY "mac" DESTINATION "${CMAKE_BINARY_DIR}")

#################
# Move Macro
#################
configure_file(
      ${PROJECT_SOURCE_DIR}/mac/vis.mac
      ${PROJECT_BINARY_DIR}/vis.mac
      COPYONLY
      )

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ComptonSim DESTINATION bin)

#################
# Build settings
#################

#set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3 -Wextra")
