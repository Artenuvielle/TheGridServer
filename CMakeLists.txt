cmake_minimum_required(VERSION 2.8)


project(${User_ProjectName})


set(EXECUTABLE_OUTPUT_PATH ../exe/)
#set(LIBRARY_OUTPUT_PATH ../exe/)
set(SOURCE_DIR sources/)
set(HEADER_DIR includes/)

file(GLOB_RECURSE SRCS "${SOURCE_DIR}*.cpp")
file(GLOB_RECURSE INCS "${HEADER_DIR}*.h" "${HEADER_DIR}*.inl")

#link_directories(${CMAKE_CURRENT_LIST_DIR}/exe/${CMAKE_CONFIGURATION_TYPES}/)
include_directories(${HEADER_DIR})

add_executable (${PROJECT_NAME} ${SRCS} ${INCS})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "${PROJECT_NAME}")

add_subdirectory(enet)
target_include_directories(${PROJECT_NAME} PUBLIC "enet/include/")
target_link_libraries(${PROJECT_NAME} enet)
if(WIN32)
	target_link_libraries(${PROJECT_NAME} winmm.lib ws2_32.lib)
endif()

if(WIN32)
	file(WRITE start_server_${CMAKE_CONFIGURATION_TYPES}.bat ".\\exe\\${CMAKE_CONFIGURATION_TYPES}\\${PROJECT_NAME}.exe" )
else()
	file(WRITE start_server_${CMAKE_CONFIGURATION_TYPES} "./exe/${PROJECT_NAME}" )
	execute_process(COMMAND "chmod +x start_server_${CMAKE_CONFIGURATION_TYPES}")
endif()
