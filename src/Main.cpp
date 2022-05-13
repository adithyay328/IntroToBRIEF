#include "FAST.hpp"
#include <list>
#include <array>

int main(int argc, char** argv ) {
    // Loading image
    if ( argc != 2 ) {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    // Just showing the image
    cv::Mat image;
    image = cv::imread( argv[1], 1 );
    if ( !image.data ) {
        printf("No image data \n");
        return -1;
    }

    std::list<std::array<int, 2>*> res;
    res = getEdgePoints(image);

    // Showing original image
    cv::imshow("Original", image);
    cv::waitKey(0);

    // Printing array and drawing on original image
    for(auto it = res.begin(); it != res.end(); it++) {
        // Getting current array
        std::array<int, 2>* currPoint;
        currPoint = *it;
        std::array<int, 2> curr;
        curr = *currPoint;

        // Printing array first
        std::cout << curr[0] << ", " << curr[1] << std::endl;

        // Drawing circles on image
        // Drawing on image                                                                                                                                                                            
        cv::circle(image, cv::Point(curr[0], curr[1]), 1, cv::Scalar( 0, 0, 255 ), cv::LINE_8);
    }

    // Resizing image to some multiplier of their original size
    const double MULT = 1.5;
    cv::resize(image, image, cv::Size(), MULT, MULT);

    // Showing image
    cv::imshow("Points", image);
    cv::waitKey(0);

    return 0;
}
