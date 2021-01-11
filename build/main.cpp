#include <stdio.h>
#include <opencv2/opencv.hpp>

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
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    
    int nbr_lignes = image.rows;
    int nbr_colonnes = image.cols;
    
    std::cout<<"lignes : "<<nbr_lignes<<" colonnes : "<<nbr_colonnes<<std::endl;
    
    waitKey(0);

    return 0;
}
