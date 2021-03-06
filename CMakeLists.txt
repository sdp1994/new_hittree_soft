cmake_minimum_required(VERSION 2.8)

### Initial definition of cmake variables
set(CMAKE_INSTALL_PREFIX $ENV{HOME} CACHE PATH "install prefix")
set(CMAKE_BUILD_TYPE Release CACHE STRING "build type")
set(CMAKE_CXX_FLAGS_DEBUG "-g -W -Wall" CACHE STRING "CXX_FLAGS for debug")
set(CMAKE_C_FLAGS_DEBUG "-g -W -Wall" CACHE STRING "C_FLAGS for debug")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -W -Wall" CACHE STRING "CXX_FLAGS for release")
set(CMAKE_C_FLAGS_RELEASE "-O3 -W -Wall" CACHE STRING "C_FLAGS for release")
set(CMAKE_MACOSX_RPATH 0)

### Definition of project
project(EventTree2HitTreeGen)
add_definitions("-std=c++11")

set(MY_LIBRARY EventTree2HitTreeGen)
set(USE_RUBY ON)

### External libraries
### BOOST ###
find_package(Boost 1.56.0 REQUIRED COMPONENTS system chrono thread)
set(BOOST_INC_DIR ${Boost_INCLUDE_DIRS})
set(BOOST_LIB_DIR ${Boost_LIBRARY_DIRS})
set(BOOST_LIB ${Boost_LIBRARIES})
message("-- BOOST_INC_DIR: ${BOOST_INC_DIR}")
message("-- BOOST_LIB_DIR: ${BOOST_LIB_DIR}")
message("-- BOOST_LIB: ${BOOST_LIB}")

### ANL ###
if(NOT DEFINED ANLNEXT_INSTALL)
  if(DEFINED ENV{ANLNEXT_INSTALL})
    set(ANLNEXT_INSTALL $ENV{ANLNEXT_INSTALL})
  else()
    set(ANLNEXT_INSTALL $ENV{HOME})
  endif()
endif(NOT DEFINED ANLNEXT_INSTALL)
set(ANLNEXT_INC_DIR ${ANLNEXT_INSTALL}/include/anl)
set(ANLNEXT_LIB_DIR ${ANLNEXT_INSTALL}/lib)
set(ANLNEXT_LIB ANLNext)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ANLNEXT_LIB_DIR}/anl)
message("-- ANLNEXT_INSTALL = ${ANLNEXT_INSTALL}")

### ROOT ###
set(ROOTSYS $ENV{ROOTSYS})
find_path(ROOT_INC_DIR
    NAMES TH1.h TTree.h TDirectory.h
  PATHS ${ROOTSYS}/include/root ${ROOTSYS}/include)
find_path(ROOT_LIB_DIR
  NAMES libHist.so libTree.so
  PATHS ${ROOTSYS}/lib/root ${ROOTSYS}/lib)
set(ROOT_LIB
  Core RIO Net Hist Graf Graf3d Gpad Tree
  Rint Postscript Matrix Physics MathCore Thread)
message("-- ROOTSYS = ${ROOTSYS}")
message("-- ROOT_INC_DIR = ${ROOT_INC_DIR}")
message("-- ROOT_LIB_DIR = ${ROOT_LIB_DIR}")

#set(ROOTSYS $ENV{ROOTSYS})
#set(ROOTDIR "")
#set(ROOT_INC_DIR ${ROOTSYS}/include${ROOTDIR})
#set(ROOT_LIB_DIR ${ROOTSYS}/lib${ROOTDIR})
#set(ROOT_LIB
#    Core RIO Net Hist Graf Graf3d Gpad Tree
#    Rint Postscript Matrix Physics MathCore Thread)
#add_definitions(-DUSE_ROOT)
#add_definitions(-DCS_BASIC2)
#message("-- ROOTSYS = ${ROOTSYS}")

# add_definitions(-DANL_USE_TVECTOR -DANL_USE_HEPVECTOR)

### other lib ###
set(EXT_LIB xerces-c)

include_directories(
  include
  ${ANLNEXT_INC_DIR}
  ${BOOST_INC_DIR}
  ${ROOT_INC_DIR}
  )

link_directories(
  ${ANLNEXT_LIB_DIR}
  ${BOOST_LIB_DIR}
  ${ROOT_LIB_DIR}
  )

set(ANL_MODULES
  src/EventDataBuffer.cc
  src/DataProcess_lv1.cc
  src/DataProcess_lv2_base.cc
  src/DataProcess_lv2_merge.cc
  src/DataProcess_lv3_base.cc
  src/DataProcess_lv3_reconstruct.cc
  src/ReadDetectorDatabase.cc
  src/ReadEventTree.cc
  src/ReadHitTree_lv1.cc
  src/ReadHitTree_lv2.cc
  src/SaveData.cc
  src/WriteHitTree_lv1.cc
  src/WriteHitTree_lv2.cc
  src/WriteHitTree_lv3.cc
  )

add_library(${MY_LIBRARY} SHARED
  ${ANL_MODULES})

target_link_libraries(${MY_LIBRARY}
  ${BOOST_LIB}
  ${ROOT_LIB}
  ${EXT_LIB}
  ANLNext
  )

install(TARGETS ${MY_LIBRARY} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

if(USE_RUBY)
  add_subdirectory(ruby)
endif(USE_RUBY)
