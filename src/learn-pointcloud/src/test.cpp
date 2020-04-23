#include "ros/ros.h"    
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <sstream> //字符串转换
#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>

using namespace std;



int main(int argc, char **argv)
{

    ros::init(argc, argv, "sl_hw2");
    ros::NodeHandle node;

    cout << "\033[1;31m sl_hw2! \033[0m" << endl;

    return 0;
}
