#include "ros/ros.h"    
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <sstream> //字符串转换
#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <limits.h>

using namespace std;

void read_data()
{
    ifstream file;
    file.open("/home/djq/catkin_ws/src/learn-pointcloud/src/data/000000.txt");
    
    vector<vector<double>> pointcloud;
    string line;

    if(file) 
    {
        while (getline(file, line)) //按行读取到line中
        {
            istringstream input(line); //create string input object
            string temp;
            vector<double> Waypoints;

            while (getline(input, temp, ','))  
            {
                Waypoints.push_back(stod(temp)); //str to double 
            }
            
            pointcloud.push_back(Waypoints);
            Waypoints.clear();
        }

        file.close();
        cout << "total points:" << pointcloud.size() << endl;
    }
    else 
    {
        cout<<"no such file"<<endl;;
    }

    // show data
    // for(int j=0;j<pointcloud.size();j++)
    // {
    //     cout << j+1 <<':'<< pointcloud[j][0] << ',' << pointcloud[j][1]<< ',' << pointcloud[j][2] << endl;
    // }
}


class DistIndex
{
    private:
        int distance;
        int index;
};

class KNNResultSet
{
    private:
    int capacity;
    int count = 0;
    int worst_dist = INT_MAX;
    vector<vector<int>> dist_index_list;
    int comparison_counter = 0;

    public:
    // 构造函数
    KNNResultSet(int capa)
    {
        cout << "KNNResultSet is being created, capacity = " << capa << endl;
        capacity = capa;
    }

    void get_capacity()
    {
        cout << "capacity is :" << capacity << endl;
    }

    void get_count()
    {
        cout << "count is :" << count << endl;
    }

    void get_worst_dist()
    {
        cout << "worst_dist is :" << worst_dist << endl;
    }

    void get_dist_index_list()
    {
        for(int i=0;i< dist_index_list.size();i++)
        {
            cout << "dist is :" << dist_index_list[i][0] << "index is :" << dist_index_list[i][1] << endl;
        }
    }

    void get_comparison_counter()
    {
        cout << "comparison_counter is :" << comparison_counter << endl;
    }
};


class Node
{
    private:

        int axis;
        int value;
        Node* left;
        Node* right;
        vector<int> point_indices;

    public:
        // 判断是否是叶子节点
        bool is_leaf(Node* node)
        {
            if (node->value == NULL)
                return true;
            else
                return false;
        }
};


void sort_key_by_vale(int key, int value)
{

}


// 选择切的维度，用轮换的方式找
int axis_round_robin(int axis, int dim)
{
    if (axis == dim-1)
        return 0;
    else
        return axis + 1;
}


class Line
{
   public:
      void setLength( double len );
      double getLength( void );
      Line(double len);  // 这是构造函数
 
   private:
      double length;
};
 
// 成员函数定义，包括构造函数
Line::Line( double len)
{
    cout << "Object is being created, length = " << len << endl;
    length = len;
}
 
void Line::setLength( double len )
{
    length = len;
}
 
double Line::getLength( void )
{
    return length;
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "sl_hw2");
    ros::NodeHandle node;

    cout << "\033[1;31m sl_hw2! \033[0m" << endl;

    read_data();

    KNNResultSet set(3);
    set.get_capacity();
    set.get_worst_dist();

    return 0;

}

// 1.stod转化精度不足的问题
// 2.类的变量怎么判断是私有的还是公有的？