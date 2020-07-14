#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
int main(int argc, char** argv )
{
    const char* filename = argc >=2 ? argv[1] : "UL1-1_noter.png";

    // Load image
    Mat src = imread(samples::findFile(filename), IMREAD_COLOR);

    // Check if image is loaded fine
    if(src.empty())
    {
        printf("Error opening image\nProgram Arguements: [image_name -- default %s\n", filename);
        return EXIT_FAILURE;
    }

    // Move to gray scale
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);

    //medianBlur(gray, gray, 1);

    std::vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
                gray.rows/16, // change this value to detect circles with difference distances to each other
                100, 30, 1, 30 // change the last two parameters (min_radius & max_radius) to detect different sizes
                );

    // draw
    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        //circle center
        circle(src, center, 1, Scalar(0,100,100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle(src, center, radius, Scalar(255,0,255), 1, LINE_AA);
    }

    imshow("detected circles", src);
    waitKey();

    return EXIT_SUCCESS;
    
}