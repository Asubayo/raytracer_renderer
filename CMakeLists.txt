cmake_minimum_required(VERSION 2.8)

project( Raytracer )
set(EXE_NAME Raytracer)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")
message("CMAKE_MODULE_PATH : " ${CMAKE_MODULE_PATH})

message("PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}" )

macro(GroupSources curdir relativePath)

   file(GLOB children RELATIVE 
   ${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}
   ${PROJECT_SOURCE_DIR}/${relativePath}${curdir}/*) 
   
   foreach(child ${children})

		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}/${child})
			   GroupSources(${curdir}/${child} ${relativePath})
		else()
			string(REPLACE "/" "\\" groupname ${curdir})
			source_group(${groupname} FILES
			${PROJECT_SOURCE_DIR}/${relativePath}/${curdir}/${child})
		endif()
    endforeach()
	
endmacro()

# Sources.
file(
	GLOB_RECURSE
	SRC_FILES
	src/*
)

GroupSources(src "//")

# Exclude ZERO_CHECK project
SET(CMAKE_SUPPRESS_REGENERATION TRUE)

# Set C++11
set (CMAKE_CXX_STANDARD 11)

# FIND REQUIRED PACKAGES
find_package(wxWidgets REQUIRED)
message("wxWidgets_LIB_DIR : ${wxWidgets_LIB_DIR}" )
message("wxWidgets_ROOT_DIR : ${wxWidgets_ROOT_DIR}" )
link_directories(${wxWidgets_LIB_DIR})
include_directories(${wxWidgets_ROOT_DIR}/include)
include_directories(${wxWidgets_ROOT_DIR}/include/msvc)

#http://stackoverflow.com/questions/33400777/error-lnk2019-unresolved-external-symbol-main-referenced-in-function-int-cde
# force Unicode over Multi-byte
add_definitions(-DUNICODE -D_UNICODE)

#wxwidget seems to use some depracated functions : disable warnings on visual studio about it
add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

add_executable(${EXE_NAME} ${SRC_FILES})
#set(LIBS) 
#target_link_libraries(${EXE_NAME} ${LIBS})
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(${EXE_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Raytracer)
