cmake_minimum_required(VERSION 2.8)

project( Depixelize )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( Depix main.cpp )

# ajout de nouvelles librairies
# add_library([nom librairie] [chemin vers .cpp 1 ] [chemin vers .cpp 2 ] [chemin vers .cpp 3 ]... )
add_library( lDepix src/similarity_graph.cpp src/voronoi_diagram.cpp src/spline.cpp )

# lier les librairies au programme
# target_link_libraries( [nom programme] [nom librairie 1] [nom librairie 2] [nom librairie 3]... )
target_link_libraries( Depix ${OpenCV_LIBS} lDepix )
