#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>


int main(int argc, char** argv)
{
  std::string filename;
  if (argc != 2)
  {
    std::cout << "Need a filename!" << std::endl;
    return -1;
  }
  else
  {
    filename = argv[1];
  }

  /// Read image
  cv::Mat img = cv::imread(filename, cv::IMREAD_UNCHANGED);

  if (img.type() == 0)
  {
    /// pgm image
    /// Invert image
    double max_value;
    cv::minMaxLoc(img, NULL, &max_value, NULL, NULL);

    std::cout << max_value << std::endl;

    cv::Mat img_inverted;
    cv::absdiff(img, max_value, img_inverted);

    /// Save inverted image
    std::size_t lastindex = filename.find_last_of(".");
    std::string rawname = filename.substr(0, lastindex);
    std::string extension = filename.substr(lastindex + 1);

    std::string new_filename = rawname + "_inverted." + extension;
    cv::imwrite(new_filename, img_inverted);

    cv::namedWindow("Lena", 1);
    cv::imshow("Lena", img);

    cv::namedWindow("Lena inverted", 1);
    cv::imshow("Lena inverted", img_inverted);

    cv::waitKey();

  }
  else if (img.type() == 16)
  {
    /// ppm image
    std::vector<cv::Mat> channels, channels_shifted;
    cv::split(img, channels); /// B G R

    channels_shifted.push_back(channels[1]);
    channels_shifted.push_back(channels[2]);
    channels_shifted.push_back(channels[0]);

    cv::Mat img_shifted(img.size(), img.type());
    merge(channels_shifted, img_shifted);

    cv::namedWindow("Lena shifted", 1);
    cv::imshow("Lena shifted", img_shifted);

    cv::Mat img_gray(img.size(), CV_64FC1);

    cv::addWeighted(channels[0], 1.0 / 3, channels[1], 1.0 / 3, 0, img_gray);
    cv::addWeighted(img_gray, 1, channels[2], 1.0 / 3, 0, img_gray);

    double maxVal;
    cv::minMaxLoc(img_gray, NULL, &maxVal, NULL, NULL);

    normalize(img_gray, img_gray, 0, 255, cv::NORM_MINMAX); // probably useless

    cv::namedWindow("Lena gray", 1);
    cv::imshow("Lena gray", img_gray);

    // Save images
    std::size_t lastindex = filename.find_last_of(".");
    std::string rawname = filename.substr(0, lastindex);
    std::string extension = filename.substr(lastindex + 1);

    std::string filename_shifted = rawname + "_shifted." + extension;
    cv::imwrite(filename_shifted, img_shifted);

    std::string filename_gray = rawname + "_gray." + extension;
    cv::imwrite(filename_gray, img_gray);

    cv::waitKey();
  }

  return 0;
}