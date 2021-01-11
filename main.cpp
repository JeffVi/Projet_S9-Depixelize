#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "src/similarity_graph.h"

using namespace cv;

int main()
{
    Mat image;
    image = imread("img/pixelart.png");

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
    destroyAllWindows();
    
    similarity_graph graphe(image);
    imshow("YUV", graphe.image_YUV);
    
    waitKey(0);
    destroyAllWindows();
    
    graphe.init_graph();

    return 0;
}
