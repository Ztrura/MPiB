cmake_minimum_required(VERSION 3.5)

project(ACAM_mesh)

#设置不生成MinSizeRel;RelWithDebInfo
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

#path
SET(EIGEN_PATH "D:/VSworkplace/C++/Eigen_test/Eigen_test")

############################################################
# Create a library
############################################################

#Generate the static library from the library sources
add_library(ACAM_mesh STATIC 
    src/IOManger.cpp
	src/PolyMesh.cpp
	src/PolyMesh_Base.cpp
	src/PolyMeshIterators.cpp
)

#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) #设置库文件的输出目录

target_include_directories(ACAM_mesh
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)


############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable(ACAM_example 
    src/main.cpp
)

# link the new ACAM_example target with the ACAM_mesh target
target_link_libraries( ACAM_example
    PRIVATE 
        ACAM_mesh
)

############################################################
# Tutte
############################################################

file (GLOB TUT_FILE
    src/TUT/*.cpp
)

# Add an executable with the above sources
add_executable(${PROJECT_NAME}_TUT ${TUT_FILE})
target_include_directories(${PROJECT_NAME}_TUT
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${EIGEN_PATH}
)

target_link_libraries(${PROJECT_NAME}_TUT
    PRIVATE 
        ACAM_mesh
)

############################################################
# Lscm
############################################################

file (GLOB LSCM_FILE
    src/LSCM/*.cpp
)

# Add an executable with the above sources
add_executable(${PROJECT_NAME}_LSCM ${LSCM_FILE})
target_include_directories(${PROJECT_NAME}_LSCM
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${EIGEN_PATH}
)

target_link_libraries(${PROJECT_NAME}_LSCM
    PRIVATE 
        ACAM_mesh
)