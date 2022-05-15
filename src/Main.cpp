#include <opencv2/opencv.hpp>
#include "FAST.hpp"
#include "BRIEF.hpp"
#include <list>
#include <array>
#include <stdlib.h>

// In this program, we're going to load both images, then
// run FAST and BRIEF on both images. Then, we're going to
// try and match the feature points accross images and print
// the matches
int main(int argc, char** argv ) {
    // Loading image
    if ( argc != 3 ) {
        printf("usage: DisplayImage.out <Image_Path> <Image_Path>\n");
        return -1;
    }
    // Loading the first image
    cv::Mat firstImage;
    firstImage = cv::imread( argv[1], 1 );
    if ( !firstImage.data ) {
        printf("No firstImage data \n");
        return -1;
    }

    // Loading the second image
    cv::Mat secondImage;
    secondImage = cv::imread( argv[2], 1 );
    if ( !firstImage.data ) {
        printf("No secondImage data \n");
        return -1;
    }

    // Running FAST on both images
    std::list<cv::Point>* FASTFirstImage = FASTDetector(firstImage);
    std::list<cv::Point>* FASTSecondImage = FASTDetector(secondImage);

    // Creating clone to draw on
    cv::Mat fastDrawingClone = firstImage.clone();

    for(auto it = FASTFirstImage -> begin(); it != FASTFirstImage -> end(); it++) {
        cv::circle(fastDrawingClone, *it, 3, cv::Scalar(0, 255, 0));
    }

    cv::imshow("Points", fastDrawingClone);
    cv::waitKey(0);

    std::cout << FASTFirstImage -> size() << std::endl;

    // Getting BRIEF descriptors for both images
    std::list<std::bitset<DESC_LEN>>* firstImageBRIEF = briefDescriptor(firstImage, FASTFirstImage);
    std::list<std::bitset<DESC_LEN>>* secondImageBRIEF = briefDescriptor(secondImage, FASTSecondImage);

    for(int i = 0; i < FASTFirstImage -> size(); i++) {
        // Advancing FAST and BRIEF iterators to current index
        auto firstFastIt = FASTFirstImage -> begin();
        auto firstBriefIt = firstImageBRIEF -> begin();
        for(int j = 0; j < i; j++) {
            firstFastIt++;
            firstBriefIt++;
        }

        auto secFastIt = FASTSecondImage -> begin();
        auto secBriefIt = secondImageBRIEF -> begin();

        // Storing the best FAST point, and its associated hamming distance
        auto bestFastIt = FASTSecondImage -> begin();
        int bestHamming = INT_MAX;

        // For each BRIEF descriptor in the first image, search the second for the corresponding
        while(secFastIt != FASTSecondImage -> end()) {
            // Check if current hamming distance is less than the best hamming distance
            std::bitset<DESC_LEN> currSecondImgBrief = *secBriefIt;
            std::bitset<DESC_LEN> x = currSecondImgBrief ^ *firstBriefIt;
            int currHamming = x.count();

            // If this is the best match, update the current best result
            if(currHamming < bestHamming) {
                bestHamming = currHamming;
                bestFastIt = secFastIt;
            }

            // Advancing pointers
            secFastIt++;
            secBriefIt++;
        }

        // Creating a new copy of both images
        cv::Mat firstCopy = firstImage.clone();
        cv::Mat secondCopy = secondImage.clone();

        // Drawing circle onto both images
        cv::circle(firstCopy, *firstFastIt, 20, cv::Scalar(0, 255, 0));
        cv::circle(secondCopy, *bestFastIt, 90, cv::Scalar(0, 255, 0));

        // Reducing size of first image
        cv::resize(secondCopy, secondCopy, cv::Size(), 0.4, 0.4);

        // Showing both images
        cv::imshow("Second", secondCopy);
        cv::imshow("First", firstCopy);
        cv::waitKey(0);

        cv::destroyAllWindows();
    }

    return 0;
}