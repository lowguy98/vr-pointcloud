cmake_minimum_required(VERSION 3.17)

project(vr)


if(MSVC)
	set(CMAKE_DIRNAME "CMake" CACHE INTERNAL "Generic name of the folder with CMake modules")
	list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/${CMAKE_DIRNAME})
	
	set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")
	set(BINARY_DIR "${CMAKE_SOURCE_DIR}/external")
	set(GLFW_ROOT "${EXTERNAL_DIR}/glfw")
	
	set(CMAKE_PREFIX_PATH "${EXTERNAL_DIR}/glew")
	set(CMAKE_LIBRARY_PATH "${EXTERNAL_DIR}/glew/lib-win/msvc14/")
endif()


find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
add_subdirectory("${CMAKE_SOURCE_DIR}/external/openvr")

if(MSVC)
	find_package(GLFW REQUIRED)
else()
	find_package(glfw3 REQUIRED)
	set(GLFW_LIBRARIES glfw)
endif()


include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})
include_directories(SYSTEM ${GLFW_INCLUDE_DIRS})
link_directories(${CMAKE_SOURCE_DIR}/external/openvr/lib/win64)

add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shader")

FILE(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::GLEW ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} openvr_api)

if(MSVC)
	file(COPY ${EXTERNAL_DIR}/glew/lib-win/msvc14/glew32.dll DESTINATION ${CMAKE_BINARY_DIR}/)
	file(COPY ${EXTERNAL_DIR}/glew/lib-win/msvc14/glew32.dll DESTINATION ${CMAKE_BINARY_DIR}/Debug/)
	file(COPY ${EXTERNAL_DIR}/glew/lib-win/msvc14/glew32.dll DESTINATION ${CMAKE_BINARY_DIR}/Release/)
	file(COPY ${EXTERNAL_DIR}/openvr/bin/win64/openvr_api.dll DESTINATION ${CMAKE_BINARY_DIR}/)
endif()

