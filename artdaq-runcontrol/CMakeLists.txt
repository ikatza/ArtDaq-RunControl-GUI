cmake_minimum_required(VERSION 3.1.0)

project(artdaq-runcontrol)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

include_directories($ENV{XMLRPC_C_FQ_DIR}/include)
link_directories($ENV{XMLRPC_C_FQ_DIR}/lib)

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
    main.cpp
)

# Tell CMake to create the helloworld executable
add_executable(artdaqRunControl ${artdaq-runcontrol_srcs})
# Use the Widgets module from Qt 5
target_link_libraries(artdaqRunControl Qt5::Widgets xmlrpc_util++ xmlrpc_client++ xmlrpc++)


install(DIRECTORY ${TOP_CMAKE_BINARY_DIR}/bin DESTINATION  ${flavorqual_dir} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
install(DIRECTORY ${TOP_CMAKE_BINARY_DIR}/lib DESTINATION  ${flavorqual_dir} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
install(DIRECTORY ${TOP_CMAKE_BINARY_DIR}/resources DESTINATION  ${flavorqual_dir} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
