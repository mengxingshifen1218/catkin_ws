#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include <iostream>
using namespace std;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cout << "\033[1;32m go! \033[0m" << endl;

  try
  {
    // 用于将ROS图像消息转化为Opencv支持的图像格式（采用BGR8编码方式）
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}
 
int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle node;

  cout << "\033[1;31m hw_opencv! \033[0m" << endl;

  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport transport(node);
  image_transport::Subscriber sub = transport.subscribe("OutImage", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");
}