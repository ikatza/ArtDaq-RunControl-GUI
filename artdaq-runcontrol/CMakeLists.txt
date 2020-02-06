cmake_minimum_required(VERSION 3.1.0)

project(artdaq-runcontrol)

add_definitions(-DVERSION="v1_00_00")
add_definitions(-DNAME="artdaq-runcontrol-gui")
add_definitions(-DSYSTEM="slf7-x86_64-e17-prof")

add_definitions(-DGIT_HASH="warawara")
add_definitions(-DGIT_BRANCH="master")

# # Not working attempt:
# add_definitions(-DVERSION="${CPACK_PACKAGE_VERSION}")
# add_definitions(-DNAME="${CPACK_PACKAGE_NAME}")
# add_definitions(-DSYSTEM="${CPACK_SYSTEM_NAME}")

# # Some other non working attempts
# add_definitions(-DGIT_CURRENT_SHA1="$(shell git -C \""$$_PRO_FILE_PWD_"\" describe)")
# add_definitions(-DGIT_CURRENT_SHA1="asdasdasdasf")
# add_definitions(-DGIT_HASH=`git rev-parse HEAD`)
# add_definitions(-DCOMPILE_TIME=`date -u +'%Y-%m-%d %H:%M:%S UTC'`)
# add_definitions(-DGIT_BRANCH="$(shell git rev-parse --abbrev-ref HEAD)")
# add_definitions(-DGIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags))

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

include_directories($ENV{XMLRPC_C_FQ_DIR}/include)
link_directories($ENV{XMLRPC_C_FQ_DIR}/lib)

# set(CMAKE_BUILD_TYPE Debug) # un-comment to build debug product

add_definitions(-DQT_NO_DEBUG_OUTPUT) # comment to have debug messages printed

# get context info about files, functions and lines where output is being printed 
add_definitions(-DQT_MESSAGELOGCONTEXT)

add_subdirectory(resources)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(artdaq-runcontrol_srcs
    mainwindow.ui
    mainwindow.cpp
    daqinterfacestate.cpp
    daqinterfacestate.ui
    xmlrpc_gui_comm.cpp
    db_dialog.cpp
    db_dialog.ui
    menuoptionsdialog.cpp
    menuoptionsdialog.ui
    messagehandler.cpp
    main.cpp
)

# Tell CMake to create the helloworld executable
add_executable(artdaqRunControl ${artdaq-runcontrol_srcs})
# Use the Widgets module from Qt 5
target_link_libraries(artdaqRunControl Qt5::Widgets xmlrpc_util++ xmlrpc_client++ xmlrpc++)


install(DIRECTORY ${TOP_CMAKE_BINARY_DIR}/bin DESTINATION  ${flavorqual_dir} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
install(DIRECTORY ${TOP_CMAKE_BINARY_DIR}/lib DESTINATION  ${flavorqual_dir} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
install(DIRECTORY ${TOP_CMAKE_BINARY_DIR}/resources DESTINATION  ${flavorqual_dir} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
install(DIRECTORY ${TOP_CMAKE_BINARY_DIR}/resources DESTINATION  ${flavorqual_dir} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
