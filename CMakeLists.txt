cmake_minimum_required(VERSION 3.2)
project( main )

# Find the installed JSON library
find_package( PkgConfig REQUIRED )
pkg_check_modules( JSONCPP jsoncpp )
include_directories( ${JSONCPP_INCLUDE_DIRS} )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( main
	main.cpp

	border.cpp
	filter.cpp
	morph.cpp
	morphology.cpp
	label.cpp
	object.cpp
	util.cpp
)
target_link_libraries( main ${JSONCPP_LIBRARIES} ${OpenCV_LIBS} )