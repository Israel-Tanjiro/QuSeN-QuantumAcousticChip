cmake_minimum_required(VERSION 3.5)
project(g4cmpQuSeN_QuantumAcoustic)

#----------------------------------------------------------------------------
# Find G4CMP package -- NOTE: Assumes user has run g4cmp_env.*sh script!
#
set(CMAKE_PREFIX_PATH $ENV{G4CMPINSTALL}/../..)
find_package(G4CMP)

#----------------------------------------------------------------------------
# Find Geant4 package
# NOTE: WITH_GEANT4_UIVIS and USE_GEANT4_STATIC_LIBS are defined here
#
include($ENV{G4CMPINSTALL}/FindGeant4.cmake)

#----------------------------------------------------------------------------
# Setup include directories and compile definitions
# NOTE: Need to include G4CMP directories before G4.
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${G4CMP_INCLUDE_DIRS})
include(${G4CMP_USE_FILE})
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# RPATH stuff
#
# Automatically add paths of linked libraries to RPATH *unless* they are in the
# build directory
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# Set the RPATH of the installed objects *unless* they are installing to a
# system directory, in which case they shouldn't need an explicitly set RPATH.
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
     "${CMAKE_INSTALL_PREFIX}/lib" is_SYSTEM_DIR)
if("${is_SYSTEM_DIR}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#----------------------------------------------------------------------------
# Build stuff
#
set(QuSeN_QuantumAcoustic_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticActionInitialization.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticConfigManager.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticConfigMessenger.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticDetectorConstruction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticPrimaryGeneratorAction.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticSensitivity.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticSteppingAction.cc
    #${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticQubitHousing.cc
    #${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticPad.cc
    #${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticTransmissionLine.cc
    #${CMAKE_CURRENT_SOURCE_DIR}/src/QuSeN_QuantumAcousticResonatorAssembly.cc
    )

if(USE_GEANT4_STATIC_LIBS)
    add_library(QuSeN_QuantumAcousticLib STATIC ${QuSeN_QuantumAcoustic_SOURCES})
else()
    add_library(QuSeN_QuantumAcousticLib SHARED ${QuSeN_QuantumAcoustic_SOURCES})
endif()
set_target_properties(QuSeN_QuantumAcousticLib PROPERTIES OUTPUT_NAME g4cmpQuSeN_QuantumAcoustic)

target_link_libraries(QuSeN_QuantumAcousticLib ${G4CMP_LIBRARIES} ${Geant4_LIBRARIES})

add_executable(g4cmpQuSeN_QuantumAcoustic g4cmpQuSeN_QuantumAcoustic.cc)
target_link_libraries(g4cmpQuSeN_QuantumAcoustic QuSeN_QuantumAcousticLib)

install(TARGETS QuSeN_QuantumAcousticLib DESTINATION lib)
install(TARGETS g4cmpQuSeN_QuantumAcoustic DESTINATION bin)
