#include "FAST.hpp"

using namespace cv;

// Returns the pixel intensity of a black and white image at a given (col, row)
int getIntensity(Mat img, int col, int row) {
  // Access needs to be in (row, col) order since that's
  // how openCV allows their access. An annoying quirk
  return (int)img.at<uchar>(row, col);
}

// Given a BGR image, extracts FAST edges and draws a circle on
// each one of those points
std::list<std::array<int, 2>*> getEdgePoints(Mat image) {
  // Converting to gray scale
  Mat img_gray;
  cvtColor(image, img_gray, COLOR_BGR2GRAY, 1);

  // Blurring provides better edge detection, so apply it
  Mat img_blur;
  GaussianBlur(img_gray, img_blur, Size(3, 3), 0);

  // This list stores all corner cooridnates we've found
  std::list<std::array<int, 2>*> corners = std::list<std::array<int, 2>*>();
  
  // Iterating column first:
  for(int c = BRESRADIUS; c < img_blur.cols - BRESRADIUS; c++) {
      // Iterating row second
      for(int r = BRESRADIUS; r < img_blur.rows - BRESRADIUS; r++) {
        // A brehman with rad 3 has 16 point intensities we look at; we store them
        // here
        std::array<int, 16> intensities = std::array<int, 16>();

        // Getting all the intensities into an array by hardcoding
        intensities[0] = getIntensity(img_blur, c, r - 3);
        intensities[1] = getIntensity(img_blur, c + 1, r - 3);
        intensities[2] = getIntensity(img_blur, c + 2, r - 2);
        intensities[3] = getIntensity(img_blur, c + 3, r - 1);
        intensities[4] = getIntensity(img_blur, c + 3, r);
        intensities[5] = getIntensity(img_blur, c + 3, r + 1);
        intensities[6] = getIntensity(img_blur, c + 2, r + 2);
        intensities[7] = getIntensity(img_blur, c + 1, r + 3);
        intensities[8] = getIntensity(img_blur, c, r + 3);
        intensities[9] = getIntensity(img_blur, c - 1, r + 3);
        intensities[10] = getIntensity(img_blur, c - 2, r + 2);
        intensities[11] = getIntensity(img_blur, c - 3, r + 1);
        intensities[12] = getIntensity(img_blur, c - 3, r);
        intensities[13] = getIntensity(img_blur, c - 3, r - 1);
        intensities[14] = getIntensity(img_blur, c - 2, r - 2);
        intensities[15] = getIntensity(img_blur, c - 1, r - 3);

        // Now that we have all intensities, calculate how many are greater or lower than the center point
        int cIntensity = getIntensity(img_blur, c, r);

        // These counts count up how many contiguous intensity values
        // were higher or lower than the threshold.
        int lowCount = 0;
        int highCount = 0;

        for(int i = 0; i < intensities.size(); i++) {
          int current = intensities[i];
          
          // Updating counts based on current intensities
          if(current > cIntensity + THRESH) {
            lowCount = 0;
            highCount++;
          } else if(current < cIntensity - THRESH) {
            lowCount++;
            highCount = 0;
          } else {
            lowCount = 0;
            highCount = 0;
          }

          // Now that updates are done, if this satisfies a corner's requirements, add to corner list
          if(lowCount >= NUMBEROFINTENSITIES || highCount >= NUMBEROFINTENSITIES) {
            std::array<int, 2>* coords = new std::array<int, 2>{ c , r };

            // Pushing to corners list
            corners.push_back(coords);

            // Exiting out of the loop for this pixel
            break;
          }
        }
      }
  }

  return corners;
}