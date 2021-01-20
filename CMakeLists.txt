cmake_minimum_required(VERSION 2.8)

project( Depixelize )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} "/usr/include/irrlicht" )

link_directories( "/usr/lib/x86_64-linux-gnu/libIrrlicht.a" )

add_executable( Depix User_Interface.cpp )

# ajout de nouvelles librairies
# add_library([nom librairie] [chemin vers .cpp 1 ] [chemin vers .cpp 2 ] [chemin vers .cpp 3 ]... )
add_library( lDepix src/similarity_graph.cpp src/voronoi_diagram.cpp src/spline.cpp src/optimize.cpp ui/Projet_Depixelisation.cpp)

# lier les librairies au programme
# target_link_libraries( [nom programme] [nom librairie 1] [nom librairie 2] [nom librairie 3]... )
target_link_libraries( Depix ${OpenCV_LIBS} lDepix "/usr/lib/x86_64-linux-gnu/libIrrlicht.so" )
