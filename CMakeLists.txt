# ======================================================================
#  icartdaq_core main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/artdaq-demo/ups/setup_for_development
#  buildtool
# ======================================================================



# use cmake 3.7 or later
cmake_minimum_required (VERSION 3.7)

project(artdaq-runcontrol-gui)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH 
        $ENV{CETBUILDTOOLS_DIR}/Modules
        ${CMAKE_MODULE_PATH})


include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
 WERROR
 NO_UNDEFINED
 EXTRA_FLAGS -pedantic
 )

SET ( SANITIZE_ADDRESS $ENV{SANITIZE_ADDRESS} )
IF ( SANITIZE_ADDRESS )
    MESSAGE (STATUS "Enable  -fsanitize=address -Wno-unused-command-line-argument -fno-omit-frame-pointer")
    cet_add_compiler_flags(-fsanitize=address -Wno-unused-command-line-argument -fno-omit-frame-pointer)
ENDIF()

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
#find_ups_product( TRACE v3_09_01 )
find_ups_product( cetbuildtools v5_08_01 )

set(TOP_CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}) 

include(CetTest)

# source
add_subdirectory(artdaq-runcontrol)
# add_subdirectory(test)

# ups - table and config files
add_subdirectory(ups)

# packaging utility
include(UseCPack)
