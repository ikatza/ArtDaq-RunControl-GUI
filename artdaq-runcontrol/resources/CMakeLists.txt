file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/resources")
set(GUI_RESOURCES_DIRECTORY "${CMAKE_BINARY_DIR}/resources")

file(MAKE_DIRECTORY "${GUI_RESOURCES_DIRECTORY}/images")
set(GUI_IMAGES_DIRECTORY "${GUI_RESOURCES_DIRECTORY}/images")

add_subdirectory(images)

