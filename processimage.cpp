#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <cmath>
cv::Mat image;
void imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
  ROS_INFO_STREAM("Received message");
  try
  {
    image = cv_bridge::toCvShare(msg, "bgr8")->image;
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    cv::Mat binaryImage;
    cv::inRange(grayImage, cv::Scalar(0, 0, 0), cv::Scalar(50, 50, 50), binaryImage);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (!contours.empty())
    {
      int largestContourIndex = 0;
      double largestContourArea = 0;
      for (int i = 0; i < contours.size(); i++)
      {
        double area = cv::contourArea(contours[i]);
        if (area > largestContourArea)
        {
          largestContourArea = area;
          largestContourIndex = i;
        }
      }
      cv::RotatedRect box = cv::minAreaRect(contours[largestContourIndex]);
      cv::Point2f rect[4];
      box.points(rect);
      cv::Point2f center = box.center;
      float angle = box.angle;
      cv::drawContours(image, contours, largestContourIndex, cv::Scalar(0, 255, 0), 2);
      for (int j = 0; j < 4; j++)
      {
        cv::line(image, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 0, 255), 2);
      }
      char width[20], height[20];
      ROS_INFO_STREAM("position:(" << center.x << "," << center.y << ")");
      ROS_INFO_STREAM("angle:" << angle);
    }
    else
      ROS_INFO_STREAM("failed to catch");

    cv::imshow("view", image);
  }
  catch (cv_bridge::Exception &e)
  {
    ROS_ERROR("Unable to convert '%s' to 'bgr8'.", msg->encoding.c_str());
  }
  cv::waitKey(1);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/camera/color/image_raw", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");
}