#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>


int RATIO = 3, TL = 30, TH = 90;
std::string documentation =
 "*** SMOOTHING AND EDGE DETECTION FUNCTION ***\n"
 "Input arguments should be as follows:\n"
 " - filename of a GRAYSCALE image is the first argument\n"
 " - width of the noise filtering filter kernel is the second argument."
 " It must be odd and greater than 1.\n"
 " - filter type is the third argument ('gaussian' or 'median')\n"
 " - if filter type is gaussian, then the next argument is the standard"
 " deviation sigma\n"
 " - OPTIONAL: after the filter type and/or sigma, tl and th can be provided"
 " for the hysteresis thresholds";

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "NO ARGUMENTS GIVEN\n\n" << documentation << std::endl;
    return -1;
  }
  std::string filename = argv[1];
  /// Read in image
  cv::Mat img = cv::imread(filename, 1);
  if (!img.data)
  {
    std::cout << "Something wrong with the file/filename.\n\n" <<
     documentation << std::endl;

    return -1;
  }

  cv::cvtColor(img, img, CV_BGR2GRAY);

  if (argc < 3)
  {
    std::cout << "NO FILTER WIDTH GIVEN\n\n" << documentation << std::endl;

    return -1;
  }
  int filter_width = atoi(argv[2]);

  if (filter_width <= 1 || filter_width % 2 == 0)
  {
    std::cout << "Filter width must be odd and greater than 1.\n\n" <<
     documentation << std::endl;

    return -1;
  }

  if (argc < 4)
  {
    std::cout << "NO FILTER TYPE GIVEN\n\n" << documentation << std::endl;

    return -1;
  }
  std::string filter_type = argv[3];
  if (filter_type.compare("gaussian") && filter_type.compare("median"))
  {
    std::cout << "Incorrect filter type.\n\n" << documentation << std::endl;

    return -1;
  }

  int tl, th;
  float sigma;
  if (!filter_type.compare("median"))
  {
    if (argc < 5)
    {
      /// no thresholds given. default:
      tl = TL;
      th = TH;
    }
    else if (argc < 6)
    {
      /// only tl given
      tl = atoi(argv[4]);
      th = RATIO * tl;
    }
    else
    {
      tl = atoi(argv[4]);
      th = atoi(argv[5]);
    }
  }
  else
  {
    if (argc < 5)
    {
      std::cout << "NO SIGMA GIVEN\n\n" << documentation << std::endl;

      return -1;
    }
    sigma = atoi(argv[4]);
    if (sigma < 0)
    {
      std::cout << "Sigma has to be a positive number.\n\n" << documentation
       << std::endl;

      return -1;
    }

    if (argc < 6)
    {
      /// no thresholds given. default:
      tl = TL;
      th = TH;
    }
    else if (argc < 7)
    {
      /// only tl given
      tl = atoi(argv[5]);
      th = RATIO * tl;
    }
    else
    {
      tl = atoi(argv[5]);
      th = atoi(argv[6]);
    }
  }
  std::cout << "Input arguments OK! Proceed with smoothing and edge detection!"
   << std::endl;

  /// SMOOTHING
   cv::Mat img_smoothed(img.size(), img.type());
  if (!filter_type.compare("median"))
  {
    /// median smoothing
    cv::medianBlur(img, img_smoothed, filter_width);
  }
  else
  {
    /// gaussian smoothing
    cv::GaussianBlur(img, img_smoothed, cv::Size(filter_width, filter_width),
                     sigma);
  }


  /// EDGE DETECTION
  cv::Mat img_edges(img_smoothed.size(), img_smoothed.type());
  Canny(img_smoothed, img_edges, tl, th);

  cv::imshow("Original image", img);
  cv::imshow("Smoothed image", img_smoothed);
  cv::imshow("Image's edges", img_edges);

  cv::waitKey(0);


  return 0;
}