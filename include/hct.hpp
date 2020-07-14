#ifndef HCT_HPP
#define HCT_HPP
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <fstream>
#include <cmath>

using namespace cv;
int hct(const char* filename)
{
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

SimpleBlobDetector::Params setup_params()
{
    SimpleBlobDetector::Params params;
	// Change thresholds
    params.minDistBetweenBlobs = 0;

    params.thresholdStep = 5;
	params.minThreshold = 10;
	params.maxThreshold = 220;
	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 100;
   
    params.maxArea = 500000;

	// Filter by Circularity
	params.filterByCircularity = false;
    params.minCircularity = 0.5;

	// Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.5;
	// Filter by Inertia
	//params.filterByInertia = true;
	params.minInertiaRatio = 0.01;

    return params;
}

bool compare_keyPoints(const KeyPoint& lhs, const KeyPoint& rhs)
{
    return lhs.size > rhs.size;
}
inline float area(float diameter)
{
    float radius = diameter/2;
    return M_PI*radius*radius;
}


int blob(const char* base_path, int img_count)
{
    Ptr<SimpleBlobDetector> detector_ptr = SimpleBlobDetector::create(setup_params());

    for (size_t i = 0; i < img_count; i++)
    {
        Mat img_with_keypoints;
        Mat img;
        Mat grey_tresh;
        int base_length = strlen(base_path);
        char* file_path = new char[base_length+5];
        sprintf(file_path, "%s%ld.png",base_path,i);
        printf("%s\n",file_path);
        img = imread(file_path, IMREAD_GRAYSCALE);

        adaptiveThreshold(img, grey_tresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 3);


        // Set up the detector with default parameters


        // detect blobs
        std::vector<KeyPoint> keypoints;
        detector_ptr->detect(grey_tresh, keypoints);

        // Draw detected blobs
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob

        drawKeypoints(img, keypoints, img_with_keypoints, Scalar(0,255,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        char* write_path = new char[base_length+12];
        
        sprintf(write_path, "%s%ld-result.png",base_path,i);
    
        imwrite(write_path, img_with_keypoints);

        std::fstream file_stream;
        char* txt_write_path = new char[base_length+12];
        sprintf(txt_write_path, "%s%ld-result.txt", base_path,i);
        int image_size = img_with_keypoints.size().area();

        file_stream.open(txt_write_path, std::fstream::in |  std::fstream::app);
        file_stream << "Image size: " << image_size << std::endl;
        file_stream << "Blobs found: " << keypoints.size()<< std::endl;

        std::sort(keypoints.begin(), keypoints.end(), compare_keyPoints);
        
        int total_area = 0;
        for (size_t i = 0; i < keypoints.size(); i++)
        {
            total_area += area(keypoints[i].size);
        }
        
        file_stream << "Total area: " << total_area << "("<< ((float)total_area/(float)image_size)*100<< "%)"<< std::endl;
        file_stream << "Area for each blob:"<< std::endl;
        float _size = 0;
        for (size_t i = 0; i < keypoints.size(); i++)
        {
            _size =  area(keypoints[i].size);
            file_stream << _size << "("<< (_size/(float)image_size)*100<< "%)"<< std::endl;
        }

        file_stream.close();
        delete[] file_path;
        delete[] write_path;
        delete[] txt_write_path;
    }
    return EXIT_SUCCESS;
}






#endif //HCT_HPP