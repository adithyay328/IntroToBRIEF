#pragma once

#include <opencv2/opencv.hpp>
#include <list>

// Binary descriptors are stored as bitsets
#include <bits/stdc++.h>

// Descriptor length needs to be known at compile time
const int DESC_LEN = 256;

// Patch size of descriptor. Represents the length of one side of the
// square patch. This / 2 should be the x and y distance
// between the center and the edges of the patch. This is important
// since we need to run this 
const int PATCH_SIZE = 70;

// This constant defines what intensity is
// returned if a point's coordinates are
// not in an image
const int DEFAULT_INTENSITY = 0;

// This header file describes the API for the BRIEF
// descriptor

// Image is expected to be BGR, all conversions happen internally
std::list<std::bitset<DESC_LEN>>* briefDescriptor(cv::Mat inputImg, std::list<cv::Point>* keyPoints);