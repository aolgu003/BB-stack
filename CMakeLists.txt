cmake_minimum_required(VERSION 3.0)
project(BB-Stack)
add_compile_options(-std=c++11 -fprofile-arcs)

add_subdirectory(lib)

if ( BUILD-BLACKLIB-TESTS )
  include(cmake/DownloadProject.cmake)
  download_project(PROJ     BlackLib
    GIT_REPOSITORY          https://github.com/aolgu003/BlackLib.git
    GIT_TAG                 master
    ${UPDATE_DISCONNECTED_IF_AVAILABLE}
    )

  add_subdirectory(${BlackLib_SOURCE_DIR}/v3_0 ${googletest_BINARY_DIR} )

  add_subdirectory( src )
endif()


if ( BUILD-UNIT-TESTS OR BUILD-AHRS-UT)
  include(cmake/CodeCoverage.cmake)
  setup_target_for_coverage(${PROJECT_NAME}_coverage tests mpu9250 coverage)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -pthread -g -O0 -fprofile-arcs -ftest-coverage")

  include( CTest )

  if (CMAKE_VERSION VERSION_LESS 3.2)
      set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
  else()
      set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
  endif()

  include(cmake/DownloadProject.cmake)
  download_project(PROJ                googletest
                   GIT_REPOSITORY      https://github.com/google/googletest.git
                   GIT_TAG             master
                   ${UPDATE_DISCONNECTED_IF_AVAILABLE}
  )

  # Prevent GoogleTest from overriding our compiler/linker options
  # when building with Visual Studio
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

  # When using CMake 2.8.11 or later, header path dependencies
  # are automatically added to the gtest and gmock targets.
  # For earlier CMake versions, we have to explicitly add the
  # required directories to the header search path ourselves.
  if (CMAKE_VERSION VERSION_LESS 2.8.11)
      include_directories("${gtest_SOURCE_DIR}/include"
                          "${gmock_SOURCE_DIR}/include")
  endif()

  enable_testing()
  include_directories(${GTEST_INCLUDE_DIRS})

  add_subdirectory( test )
endif()
