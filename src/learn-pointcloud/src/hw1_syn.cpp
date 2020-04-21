#include "ros/ros.h"    
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/sync_policies/exact_time.h>


#include <iostream>

using namespace std;
using namespace sensor_msgs;
using namespace message_filters;

ros::Publisher PointCloudInfo_pub;
ros::Publisher ImageInfo_pub;

PointCloud2 syn_pointcloud;
Image syn_iamge;

void Syncallback(const PointCloud2ConstPtr& ori_pointcloud,const ImageConstPtr& ori_image)
{
    cout << "\033[1;32m Syn! \033[0m" << endl;
    syn_pointcloud = *ori_pointcloud;
    syn_iamge = *ori_image;
    cout << "syn pointcloud' timestamp : " << syn_pointcloud.header.stamp << endl;
    cout << "syn image's timestamp : " << syn_iamge.header.stamp << endl;
    PointCloudInfo_pub.publish(syn_pointcloud);
    ImageInfo_pub.publish(syn_iamge);
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "hw1");
    ros::NodeHandle node;

    cout << "\033[1;31m hw1! \033[0m" << endl;

    // 建立需要订阅的消息对应的订阅器
    message_filters::Subscriber<PointCloud2> PointCloudInfo_sub(node, "/rslidar_points", 1);
    message_filters::Subscriber<Image> ImageInfo_sub(node, "/zed/zed_node/left/image_rect_color", 1);
    
    typedef sync_policies::ApproximateTime<PointCloud2, Image> MySyncPolicy; 
    
    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), PointCloudInfo_sub, ImageInfo_sub); //queue size=10
    sync.registerCallback(boost::bind(&Syncallback, _1, _2));


    PointCloudInfo_pub = node.advertise<PointCloud2>("/djq_pc", 10);
    ImageInfo_pub = node.advertise<Image>("/djq_image", 10);

    ros::spin();
    return 0;

}

