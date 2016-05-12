message(STATUS "example: ${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB SPACESHIP_SRC  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE SPACESHIP_FRAMEWORK_SRC  "${CMAKE_CURRENT_SOURCE_DIR}/framework/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/framework/*.h")
file(GLOB_RECURSE SPACESHIP_GENERATED_SRC  "${CMAKE_CURRENT_SOURCE_DIR}/behaviac_generated/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/behaviac_generated/*.h")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_SOURCE_DIR}/inc")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/framework")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include")

# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SDL2/x86)
# set (SDL2_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/x86")

if (BUILD_USE_64BITS)
	set (SDL2_DLL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/x64)
else()
	set (SDL2_DLL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/x86)
endif()

find_library(SDL2_LIBS 
	NAMES SDL2 
	PATHS ${SDL2_DLL_PATH}
	)
mark_as_advanced(SDL2_LIBS)

find_library(SDL2_LIBS_MAIN 
	NAMES SDL2main 
	PATHS ${SDL2_DLL_PATH}
	)
mark_as_advanced(SDL2_LIBS_MAIN)

find_library(SDL2_LIBS_IMAGE 
	NAMES SDL2_image 
	PATHS ${SDL2_DLL_PATH}
	)
mark_as_advanced(SDL2_LIBS_IMAGE)

find_library(SDL2_LIBS_TTF
	NAMES SDL2_ttf 
	PATHS ${SDL2_DLL_PATH}
	)
mark_as_advanced(SDL2_LIBS_TTF)

# add the executable
add_executable(spaceship ${SPACESHIP_SRC} ${SPACESHIP_FRAMEWORK_SRC} ${SPACESHIP_GENERATED_SRC})

set (LEGACY_LIBS "")

if (MSVC)
	if(NOT (MSVC_VERSION LESS 1900))
 		set (LEGACY_LIBS "legacy_stdio_definitions")
	endif()
endif()

# target_link_libraries (spaceship  libbehaviac SDL2main SDL2 SDL2_image SDL2_ttf)
target_link_libraries (spaceship  libbehaviac ${CMAKE_THREAD_LIBS_INIT} ${LEGACY_LIBS} ${SDL2_LIBS} ${SDL2_LIBS_MAIN} ${SDL2_LIBS_IMAGE} ${SDL2_LIBS_TTF})

set_target_properties(spaceship PROPERTIES PREFIX "")
# set_target_properties(spaceship PROPERTIES OUTPUT_NAME_DEBUG libbehaviac${${CMAKE_DEBUG_POSTFIX}}
# set_target_properties(spaceship PROPERTIES OUTPUT_NAME_RELEASE libbehaviac${${CMAKE_RELEASE_POSTFIX}}

set_target_properties(spaceship PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(spaceship PROPERTIES RELEASE_POSTFIX ${CMAKE_RELEASE_POSTFIX})

# Gather list of all dlls
file(GLOB SDL2_DLLS ${SDL2_DLL_PATH}/*.dll)

foreach(SDL2_DLL ${SDL2_DLLS})
	add_custom_command(TARGET spaceship POST_BUILD     
	    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_DLL}  ${BINARY_PATH})
endforeach()

