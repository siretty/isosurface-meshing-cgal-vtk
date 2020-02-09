
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(SampleFunction)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkCommonDataModel
  vtkFiltersCore
  vtkFiltersModeling
  vtkImagingHybrid
  vtkInteractionStyle
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingOpenGL2 
  vtkIOGeometry
QUIET)

find_package(CGAL REQUIRED)
find_package(MPFR REQUIRED)
find_package(GMP REQUIRED)

if (NOT VTK_FOUND)
  message("Skipping SampleFunction: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()

message (STATUS "VTK_VERSION: ${VTK_VERSION}")

include(${VTK_USE_FILE})

add_executable(vtk-superquadric MACOSX_BUNDLE vtk-superquadric.cpp)
target_link_libraries(vtk-superquadric PRIVATE ${VTK_LIBRARIES})

add_executable(vtk-customsphere MACOSX_BUNDLE vtk-customsphere.cpp)
target_link_libraries(vtk-customsphere PRIVATE ${VTK_LIBRARIES})

add_executable(cgal-customsphere cgal-customsphere.cpp)
target_link_libraries(cgal-customsphere PRIVATE CGAL ${MPFR_LIBRARIES} ${GMP_LIBRARIES})

