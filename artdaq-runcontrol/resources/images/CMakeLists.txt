

file(COPY button_green.png DESTINATION "${GUI_IMAGES_DIRECTORY}")

set(state_machine_images 
	button_green.png
	button_light_green.png
	button_red.png
	StateDiagram_all_off.png
	StateDiagram_stopped.png
	StateDiagram_booted.png
	StateDiagram_booting.png
	StateDiagram_configuring.png
	StateDiagram_ready.png
	StateDiagram_running.png
	StateDiagram_starting_run.png
	StateDiagram_stopping_run.png
	StateDiagram_terminating.png
	plus.png
	minus.png
	)

foreach(item IN LISTS state_machine_images)
	file(COPY ${item} DESTINATION "${GUI_IMAGES_DIRECTORY}")
endforeach()

	
	
	
	
	
	
