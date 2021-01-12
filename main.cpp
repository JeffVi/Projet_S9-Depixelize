#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "src/similarity_graph.h"
#include "src/voronoi_diagram.h"

using namespace cv;

int main()
{
    Mat image;
    image = imread("img/pixelart3.png");

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    imshow("Pixel art", image);
    
    int nbr_lignes = image.rows;
    int nbr_colonnes = image.cols;
    
    std::cout<<"lignes : "<<nbr_lignes<<" colonnes : "<<nbr_colonnes<<std::endl;
    
    waitKey(0);
    
    similarity_graph graphe(image);
    imshow("YUV", graphe.image_YUV);
    
    waitKey(0);
    
    graphe.init_graph();
    graphe.compare_YUV();
    graphe.resolu();
    
    imshow("nodes", graphe.draw_nodes(21));
    waitKey(0);

    return 0;
}
