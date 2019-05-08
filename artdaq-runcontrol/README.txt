
ArtDaq RunControl GUI package

To run the GUI:
# DAQInterface must be configured, all artdaq environment variables
# setuped and source_me sourced.
# source PATH_TO_/sbndaq/srcs/sbndaq/dab/DAQInterface/setup_daqinterface.sh
./ARTDAQ_console

build from source code process:
 qmake -o Makefile ARTDAQ_console.pro
 make

Qt version 5 is needed, on sbnd-daq33 pick it up like this:
 source /software/products/setup
 setup gcc v6_4_0
 qmake-qt5 -o Makefile ARTDAQ_console.pro
 make


