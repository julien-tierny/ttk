cmake_minimum_required(VERSION 2.4)

project(TriangulationRequest)

set(TTK_BUILD_MODE "ParaView")

# path to the common code base
find_path(TTK_DIR ttk.cmake
  PATHS
    core/
    ../core/
    ../../core/
    ../../../core/
    sandbox/core/
    sandbox/../core/
    sandbox/../../core/
    sandbox/../../../core/
)
include(${TTK_DIR}/ttk.cmake)

# include the vtk wrapper for your TTK package 
ttk_add_vtkWrapper_package(ttkTriangulationRequest)

# Add the plugin
ttk_wrapup_paraview_plugin(
  TriangulationRequest
  1.0)
