#pragma once

#include <opencv2/opencv.hpp>
#include <list>
#include <array>

// This header file describes the API for the FAST detector

// Constants for FAST

// Defining our bresenham circle as having
// radius 3. This is just the way this
// detector is configured. Note, this implementation
// of pixel selection only works for radius 3; hasn't
// been generalized just yet
const int BRESRADIUS = 3;

// The number of points that need to be greater or lower.
// This is arbitrary, we can learn it later
const int NUMBEROFINTENSITIES = 12;

// This is the threshold value. Arbitrarily selected
const int THRESH = 40;

// This function takes in a raw, BGR image, and returns a list
// of [col, row] pairs that correspond to all the features it found
std::list<std::array<int, 2>*> getEdgePoints(cv::Mat image);