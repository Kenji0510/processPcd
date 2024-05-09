cmake_minimum_required(VERSION 3.10)
project(VoxelProcessOpen3D)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# CUDA support
find_package(CUDA REQUIRED)
enable_language(CUDA)

# Find Open3D with GPU support
# Set the path to the Open3D library with GPU support
set(Open3D_DIR "/usr/local/include/open3d_cuda/lib/cmake/Open3D")
find_package(Open3D REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

# Find Open3D, and you might need to set Open3D_DIR variable
# to the location of Open3DConfig.cmake file via -DOpen3D_DIR=...
find_package(Open3D REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

add_executable(mainOnGpu src/mainOnGpu.cpp)

# Link Open3D libraries
target_link_libraries(mainOnGpu
  ${Open3D_LIBRARIES}
  Boost::system
  Boost::filesystem
  CUDA::cudart # Link CUDA runtime library
)

# Include directories
target_include_directories(mainOnGpu PRIVATE
  ${Open3D_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
)

# Additional configurations for linking
target_compile_definitions(mainOnGpu PRIVATE ${Open3D_DEFINITIONS})
