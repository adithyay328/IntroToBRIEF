#include <opencv2/opencv.hpp>
#include "BRIEF.hpp"
#include <array>
#include <stdlib.h>
#include <iostream>

// Sampling pairs store the column and row value of both points
struct SamplingPair {
    cv::Point p1, p2;
};

// This array pointer stores all sampling pairs for this descriptor
// It initializes to null, so if any client code notices it's null,
// it means it must be initialized
std::array<SamplingPair, DESC_LEN>* pairs;

// Returns the intensity of the pixel, or the default
// intensity if the coordinate is not in the image
int brief_getIntensity(cv::Mat image, int col, int row) {
    if ( (col >= 0 && col < image.cols) && (row >= 0 && row < image.rows) ) {
        return (int)image.at<uchar>(row, col);
    } else {
        return DEFAULT_INTENSITY;
    }
};

void initPairs() {
    // Initailizing RNG
    srand (time(0));

    // Only run the init operation if it's needed; should be idempotent
    if(pairs != nullptr) {
        return;
    }

    // Initializing array for sampling pairs
    pairs = new std::array<SamplingPair, DESC_LEN>();

    // Generating DESC_LEN sampling pairs using RNG
    for(int i = 0; i < DESC_LEN; i++) {
        // Stores an individual sampling pair
        SamplingPair samplePair;
        
        cv::Point pOne, pTwo;
        pOne.x = rand() % PATCH_SIZE - ( (rand() % PATCH_SIZE) / 2 );
        pOne.y = rand() % PATCH_SIZE -  ( (rand() % PATCH_SIZE) / 2 );

        pTwo.x = rand() % PATCH_SIZE - ( (rand() % PATCH_SIZE) / 2 );
        pTwo.y = rand() % PATCH_SIZE - ( (rand() % PATCH_SIZE) / 2 );

        samplePair.p1 = pOne;
        samplePair.p2 = pTwo;

        pairs -> at(i) = samplePair;
    }
};

// Runs BRIEF descriptor on an image, and returns all binary descriptors
// in the image.
// Input params are the image as a colored mat, and all keypoints as coordinate pairs
std::list<std::bitset<DESC_LEN>>* briefDescriptor(cv::Mat inputImg, std::list<cv::Point>* keyPoints) {
    // Ensuring that sampling pairs are initialized
    initPairs();

    // Converting to gray scale
    cv::Mat img_gray;
    cv::cvtColor(inputImg, img_gray, cv::COLOR_BGR2GRAY, 1);

    // Declaring the result list
    std::list<std::bitset<DESC_LEN>>* res;
    res = new std::list<std::bitset<DESC_LEN>>();

    // Iterating over every keypoint
    for(auto it = keyPoints -> begin(); it != keyPoints -> end(); it++) {
        cv::Point keyPoint;
        keyPoint = *it;
        
        // Extracting x and y values for keypoint
        int keyX = keyPoint.x;
        int keyY = keyPoint.y;

        // Bitset to store the descriptor
        std::bitset<DESC_LEN> desc;

        // Generating the descriptor bit-by-bit
        for(int i = 0; i < DESC_LEN; i++) {
            // Getting our sampling pair
            SamplingPair sPair = pairs->at(i);

            // Getting each cv point
            cv::Point p1 = sPair.p1;
            cv::Point p2 = sPair.p2;

            // Getting point intensities
            int intOne = brief_getIntensity(img_gray, keyX + p1.x, keyY + p1.y);
            int intTwo = brief_getIntensity(img_gray, keyX + p2.x, keyY + p2.y);
            
            // Setting the bit value to the test output
            desc[i] = (intOne < intTwo);
        }

        res -> push_back(desc);
    }

    return res;
};