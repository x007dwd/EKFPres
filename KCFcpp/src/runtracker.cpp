#include <fstream>
#include <iostream>

#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <string>

#include "kcftracker.hpp"

#include <dirent.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

  std::cout << "ssssssssssssssss" << std::endl;
  if (argc > 5)
    return -1;

  bool HOG = true;
  bool FIXEDWINDOW = false;
  bool MULTISCALE = true;
  bool SILENT = false;
  bool LAB = false;

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "hog") == 0)
      HOG = true;
    if (strcmp(argv[i], "fixed_window") == 0)
      FIXEDWINDOW = true;
    if (strcmp(argv[i], "singlescale") == 0)
      MULTISCALE = false;
    if (strcmp(argv[i], "show") == 0)
      SILENT = false;
    if (strcmp(argv[i], "lab") == 0) {
      LAB = true;
      HOG = true;
    }
    if (strcmp(argv[i], "gray") == 0)
      HOG = false;
  }

  // Create KCFTracker object
  KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

  // Frame readed
  Mat frame;

  // Tracker results
  Rect result;

  // Path to list.txt
  ifstream listFile;
  std::string fileName =
      //"/home/bobin/Documents/code/git/EKFPres/data/datay/images.txt";
      "/home/bobin/Documents/code/git/EKFPres/data/dataset121/image.txt";
  listFile.open(fileName);

  // Read groundtruth for the 1st frame
  ifstream groundtruthFile;
  string groundtruth =
      //"/home/bobin/Documents/code/git/EKFPres/data/datay/out-rect.txt";
      "/home/bobin/Documents/code/git/EKFPres/data/dataset121/out-rect.txt";
  groundtruthFile.open(groundtruth);
  string firstLine;
  getline(groundtruthFile, firstLine);
  groundtruthFile.close();

  istringstream ss(firstLine);

  // Read groundtruth like a dumb

  // Using min and max of X and Y for groundtruth rectangle
  float xMin;
  float yMin;
  float width;
  float height;

  ss >> xMin;
  ss >> yMin;
  ss >> width;
  ss >> height;

  // Read Images
  ifstream listFramesFile;
  string listFrames =
      //"/home/bobin/Documents/code/git/EKFPres/data/datay/images.txt";
      "/home/bobin/Documents/code/git/EKFPres/data/dataset121/images.txt";
  listFramesFile.open(listFrames);
  string frameName;

  // Write Results
  ofstream resultsFile;
  string resultsPath =
      //"/home/bobin/Documents/code/git/EKFPres/data/datay/output.txt";
      "/home/bobin/Documents/code/git/EKFPres/data/dataset121/output.txt";
  resultsFile.open(resultsPath);

  // Frame counter
  int nFrames = 0;

  while (getline(listFramesFile, frameName)) {
    frameName = frameName;

    // Read each frame from the list
    frame = imread(frameName, CV_LOAD_IMAGE_COLOR);

    // First frame, give the groundtruth to the tracker
    if (nFrames == 0) {
      tracker.init(Rect(xMin, yMin, width, height), frame);
      rectangle(frame, Point(xMin, yMin), Point(xMin + width, yMin + height),
                Scalar(0, 255, 255), 1, 8);
      resultsFile << xMin << "," << yMin << "," << width << "," << height
                  << endl;
    }
    // Update
    else {
      result = tracker.update(frame);
      // cv::waitKey({0});
      rectangle(frame, Point(result.x, result.y),
                Point(result.x + result.width, result.y + result.height),
                Scalar(0, 255, 255), 1, 8);
      resultsFile << result.x << "," << result.y << "," << result.width << ","
                  << result.height << endl;
    }

    nFrames++;

    if (!SILENT) {
      imshow("Image", frame);
      waitKey(0);
    }
  }
  resultsFile.close();

  listFile.close();
}
