#include "ros/ros.h"    
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h> //头文件sensor_msgs/Image是ROS下的图像的类型，这个头文件中包含对图像进行编码的函数。

#include <image_transport/image_transport.h> //这个头文件中包含的是ImageTransport类，这个类提供ROS中图像的订阅和发布。
#include <opencv2/highgui/highgui.hpp> //调用opencv库
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h> //头文件cv_bridge中包含了CvBridge类，而CvBridge中的API可以将ROS下的sensor_msgs/Image消息类型转化成cv::Mat

#include <iostream>
#include <typeinfo>

using namespace std;

int main(int argc, char **argv)
{

    ros::init(argc, argv, "hw_opencv");
    ros::NodeHandle node;

    image_transport::ImageTransport transport(node);
    image_transport::Publisher image_pub = transport.advertise("/OutImage", 1);

    cout << "\033[1;31m hw_opencv! \033[0m" << endl;

    cv::Mat cv_image = cv::imread("/home/djq/me.jpg");

    if(cv_image.empty() )
    {
        ROS_ERROR("Read the picture failed!");
        return -1;
    }

    //将opencv格式的图像转化为ROS所支持的消息类型，从而发布到相应的话题上。
    sensor_msgs::ImagePtr im_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", cv_image).toImageMsg();
    // sensor_msgs::Image im=*im_msg;
    // printf("%s\n",typeid(im_msg).name());//类型
    // printf("%s\n",typeid(im).name());//类型

    ros::Rate loop_rate(1);

    //发布图片消息
    while (ros::ok()) 
    {
        image_pub.publish(im_msg);
        ROS_INFO("Converted Successfully!");
        
        ros::spinOnce();

        loop_rate.sleep();
    }

    return 0;
}
