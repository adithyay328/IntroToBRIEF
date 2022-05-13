cmake_minimum_required(VERSION 3.16)

project (IntroToBRIEF)

# Including our source code
add_subdirectory(src)

# Ensuring that opencv is available to app
FIND_PACKAGE( OpenCV REQUIRED )
TARGET_LINK_LIBRARIES (app ${OpenCV_LIBS})