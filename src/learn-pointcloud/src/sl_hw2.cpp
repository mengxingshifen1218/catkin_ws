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
#include <math.h>


using namespace std;

bool cmp_0(vector<double>&a,vector<double>&b)
{
	return a[0]<b[0];
}

bool cmp_1(vector<double>&a,vector<double>&b)
{
	return a[1]<b[1];
}

bool cmp_2(vector<double>&a,vector<double>&b)
{
	return a[2]<b[2];
}


vector<vector<double>> test_data(int db_size)
{
    vector<vector<double>> vec1;
    for(int i=0;i<db_size;i++ )
    {
        int k = 2;
        vector<double> vec2;
        vec2.push_back(k-i+2);
        vec2.push_back(2*i-k);
        vec2.push_back(k-i+1);
        if(k%2==0)
        {
            k = (-1) * k + 2;
        }
        vec1.push_back(vec2);
    }

    for(int j=0;j<vec1.size();j++)
    {
        cout << j+1 <<':'<< vec1[j][0] << ',' << vec1[j][1]<< ',' << vec1[j][2] << endl;
    }

    cout << "total points:" << vec1.size() << endl;

   return vec1; 
}

vector<vector<double>> test_data_2()
{
    vector<vector<double>> vec1;
	vector<double> vec2;

	vec2.push_back(1);
	vec2.push_back(2);
	vec2.push_back(4);
 	vec1.push_back(vec2);
	vec2.clear();

	vec2.push_back(2);
	vec2.push_back(4);
	vec2.push_back(6);
 	vec1.push_back(vec2);
	vec2.clear();

	vec2.push_back(6);
	vec2.push_back(3);
	vec2.push_back(1);
 	vec1.push_back(vec2);
	vec2.clear();
   
    vec2.push_back(0);
	vec2.push_back(2);
	vec2.push_back(1);
 	vec1.push_back(vec2);
	vec2.clear();

	vec2.push_back(9);
	vec2.push_back(4);
	vec2.push_back(3);
 	vec1.push_back(vec2);
	vec2.clear();

	vec2.push_back(6);
	vec2.push_back(3);
	vec2.push_back(1);
 	vec1.push_back(vec2);
	vec2.clear();

	vec2.push_back(7);
	vec2.push_back(8);
	vec2.push_back(4);
 	vec1.push_back(vec2);
	vec2.clear();

	vec2.push_back(5);
	vec2.push_back(6);
	vec2.push_back(9);
 	vec1.push_back(vec2);
	vec2.clear();

    for(int j=0;j<vec1.size();j++)
    {
        cout << "原始值" <<':'<< vec1[j][0] << ',' << vec1[j][1]<< ',' << vec1[j][2] << endl;
    }

   return vec1; 
}

vector<vector<double>> read_data()
{
    ifstream file;
    file.open("/home/djq/catkin_ws/src/learn-pointcloud/src/data/000000.txt");
    
    vector<vector<double>> pointcloud;
    string line;


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
    // show data
    // for(int j=0;j<pointcloud.size();j++)
    // {
    //     cout << j+1 <<':'<< pointcloud[j][0] << ',' << pointcloud[j][1]<< ',' << pointcloud[j][2] << endl;
    // }
    return pointcloud;
}


class DistIndex
{
private:
	int distance;
	int index;

public:
	DistIndex(int dis, int ind) 
	{
		distance = dis;
		index = ind;
	}

    void set_distance(int dis)
    {
        distance = dis;
    }

    void set_index(int ind)
    {
        index = ind;
    }

    int get_distance()
    {
        return distance;
    }

    int get_index()
    {
        return index;
    }

};

class KNNResultSet
{
private:
	int capacity;
	int count;
	int worst_dist;
	vector<DistIndex> dist_index_list;
	int comparison_counter = 0;

public:
	// 构造函数
	KNNResultSet(int capa)
	{
		cout << "KNNResultSet is being created, capacity = " << capa << endl;
		capacity = capa;
		count = 0;
		worst_dist = INT_MAX;
		comparison_counter = 0;

		for (int i = 0; i < capacity; i++)
		{
			dist_index_list.push_back(DistIndex(worst_dist,0));
		}

        cout << "dist_index_list size = " << dist_index_list.size() << endl;

        // cout << dist_index_list[0].get_distance() << ',' << dist_index_list[0].get_index() << endl;

	}

    void set_worst_dist(int dis)
    {
        worst_dist = dis;
    }

	int size()
	{
		return count;
	}

	bool full()
	{
		return count == capacity;
	}

	int get_worstDist()
	{
		return worst_dist;
	}

	void get_capacity()
	{
		cout << "capacity is :" << capacity << endl;
	}

	void get_count()
	{
		cout << "count is :" << count << endl;
	}

	void get_dist_index_list(KNNResultSet* set)
	{
        for (int i = 0; i < set->dist_index_list.size(); i++)
		{

            cout << "dist is:" << set->dist_index_list[i].get_distance()  << ","
                 << "index is :"  << set->dist_index_list[i].get_index() << endl;
		}
	}

	void get_comparison_counter()
	{
		cout << "comparison_counter is :" << comparison_counter << endl;
	}

	void add_point(KNNResultSet* set, int dis, int ind)
	{

		set->comparison_counter += 1;

		if (dis > set->worst_dist) 
		{
			return;
		}
			
		if (set->count < set->capacity)
		{
			set->count += 1;
		}

		int i = set->count - 1;

        while (i > 0)
        {
            if(set->dist_index_list[i-1].get_distance() > dis)
            {
                set->dist_index_list[i] = set->dist_index_list[i-1];
                i -=1;
            }
            else
            {
                break;
            }
            
        }

        set->dist_index_list[i].set_distance(dis);
        set->dist_index_list[i].set_index(ind);
        set->set_worst_dist(set->dist_index_list[set->capacity-1].get_distance());
	}

    void show(KNNResultSet* set)
    {

        cout << "\033[31m index  -  dist \033[0m" << endl;

        for(int i=0; i < set->dist_index_list.size(); i++)
        {
            cout << set->dist_index_list[i].get_index() << "  -  "
                 << set->dist_index_list[i].get_distance() << endl;
        }

        cout << "In total " << set->comparison_counter << " comparison operations" << endl;
    }

};


class Node
{
private:

	int axis;
	double value;
	Node* left;
	Node* right;
	vector<int> point_indices;
    bool _is_ok;

public:
	// 判断是否是叶子节点
	bool is_leaf(Node* node)
	{
		if (node->value == 0)
			return true;
		else
			return false;
	}

    // //构造函数
    // Node(int ax)
    // {
    //     axis = ax;
    //     cout << "Object is being created" << endl;
    // }

    Node(void)
    {
        axis = -1;
        value = -1;
        left = NULL;
        right = NULL;
        _is_ok = false;

    }

    Node(int ax, int val, Node* le, Node* ri, vector<int> &point_ind) 
	{
		axis = ax;
        value = val;
        left = le;
        right = ri;
        point_indices = point_ind;
	}

    bool get_is_ok()
    {
        return _is_ok;
    }

    void set_is_ok()
    {
        _is_ok = true;
    }

    int get_axis()
    {
        return axis;
    }

    double get_value()
    {
        return value;
    }

    Node* get_left()
    {
        return left;
    }

    Node* get_right()
    {
        return right;
    }

    int get_point_indices_len()
    {
        return point_indices.size();
    }

    void set_axis(int axi)
    {
        axis =  axi;
    }

    void set_value(double val)
    {
        value = val;
    }

    void set_left(Node* le)
    {
         left = le;
    }

    void set_right(Node* ri)
    {
        right = ri;
    }

    void set_point_indices(vector<int> indices)
    {
        for(int i =0;i<indices.size();i++)
        {
            point_indices[i] = indices[i];
            cout << "point_indices[" << i << "]" << point_indices[i] << endl;
        }
    }

};


vector<int> sort_key_by_vale(vector<int> &point_indices, 
							 vector<vector<double>> &db,
							 int axis)

{
	vector<vector<double>> result;
	vector<double> temp;

	for(int i = 0;i<point_indices.size();i++)
	{

		temp.push_back(db[point_indices[i]][0]);
		temp.push_back(db[point_indices[i]][1]);
		temp.push_back(db[point_indices[i]][2]);
		temp.push_back(point_indices[i]); //保存排序前的下标
		result.push_back(temp);
		temp.clear();
	}

	for(int j = 0;j<point_indices.size();j++)
	{
		 cout << "排序前的数据,加了index信息: " << result[j][0] << ',' << result[j][1]<< ',' << result[j][2] << ',' << result[j][3] << endl;
	}


	//排序
	if(axis == 0)
	{
		sort(result.begin(),result.end(),cmp_0);
	}
	else if (axis == 1)
	{
		sort(result.begin(),result.end(),cmp_1);
	}
	else 
	{
		sort(result.begin(),result.end(),cmp_2);
	}

	
    vector<int> result_index;
	for(int i = 0;i<point_indices.size();i++)
	{
		result_index.push_back(result[i][3]);
		cout << "排序index：" << result[i][3] << endl;
	}

	for(int j = 0;j<point_indices.size();j++)
	{
		 cout << "排序之后的数据：" << result[j][0] << ',' << result[j][1]<< ',' << result[j][2] << endl;
	}

	return result_index;

}

// 选择切的维度，用轮换的方式找
// dim = 3
int axis_round_robin(int axis, int dim)
{
	if (axis == dim - 1)
		return 0;
	else
		return axis + 1;
}


// void kdtree_construction(vector<vector<double>> points, int leaf_size) 
// {
//     int N = points.size();
//     int dim = points[0].size();
//     int axis = 0;


//     cout << "N:" << N << endl;
//     cout << "dim:" << dim << endl;

//     vector<int> point_indices(N); ////初始化了N个默认值为0的元素

//     for(int i=0;i<point_indices.size();i++)
//     {
//         point_indices[i] = i;
//     }

//     Node root(-1,-1,NULL,NULL);


//     root = kdtree_recursive_build(root,points,point_indices,axis,leaf_size); //新建一个节点

//     // return root;
// }


//point_indices 存放的是所有的叶子的index
// Node  kdtree_recursive_build(Node& root, vector<vector<double>>& db, vector<int>& point_indices, int& axis, int& leaf_size)
// {
//     if(root.get_axis()==-1 && root.get_value()==-1)
//     {
//         Node root2(axis,0,NULL,NULL,point_indices);
//         cout << "-------------" << root2.get_value() << endl;
//     }
        

    
// }
    

//     // 当大于设定的叶子节点时才执行
//     if( (*point_indices).size() > (*leaf_size))
//     {
//         // 以某一个方向(维度)，将点排序
//         vector<int> point_indices_sorted;
//         point_indices_sorted = sort_key_by_vale(*point_indices,*db,*axis);

//         cout << point_indices_sorted.size() << endl;
//     }

//     return root2;
            
    
        

// //         int middle_left_idx = ceil(point_indices_sorted.size() / 2) - 1;
// //         cout << "middle_left_idx: " << middle_left_idx << endl;

// //         int middle_left_point_idx = point_indices_sorted[middle_left_idx];
// //         cout << "middle_left_point_idx :" << middle_left_point_idx << endl;

// //         double middle_left_point_value = points[middle_left_point_idx][axis];
// //         cout << "middle_left_point_value :" << middle_left_point_value << endl;



// //         int middle_right_idx = middle_left_idx + 1;
// //         cout << "middle_right_idx: " << middle_right_idx << endl;

// //         int middle_right_point_idx = point_indices_sorted[middle_right_idx];
// //         cout << "middle_right_point_idx :" << middle_right_point_idx << endl;

// //         double middle_right_point_value = points[middle_right_point_idx][axis];
// //         cout << "middle_right_point_value :" << middle_right_point_value << endl;

// //         double root_value = (middle_left_point_value + middle_right_point_value) * 0.5;
// // 	    cout << "root_value :" << root_value << endl;


// //         root->
// //         root.left = kdtree_recursive_build(root.left,
// //                                            db,
// //                                            point_indices_sorted[0:middle_right_idx],
// //                                            axis_round_robin(axis, dim=db.shape[1]),
// //                                            leaf_size)
 
// //     }


//point_indices 存放的是所有的叶子的index
// Node  kdtree_build(Node& root, vector<vector<double>>& db, vector<int>& point_indices, int& axis, int& leaf_size)
// {
//     // cout << "root.get_axis(): " << root.get_axis() << endl;

//     if(root.get_axis() == -1)
//     {
//         root.set_axis(axis);
//         root.set_point_indices(point_indices);
//     }

//     if(point_indices.size() > leaf_size)
//     {
//         vector<int> point_indices_sorted;
//         point_indices_sorted = sort_key_by_vale(point_indices,db,axis);

//         cout << "point_indices_sorted.size() :" << point_indices_sorted.size() << endl;

//         int middle_left_idx = ceil(point_indices_sorted.size() / 2) - 1;
//         cout << "middle_left_idx: " << middle_left_idx << endl;

//         int middle_left_point_idx = point_indices_sorted[middle_left_idx];
//         cout << "middle_left_point_idx :" << middle_left_point_idx << endl;

//         double middle_left_point_value = db[middle_left_point_idx][axis];
//         cout << "middle_left_point_value :" << middle_left_point_value << endl;



//         int middle_right_idx = middle_left_idx + 1;
//         cout << "middle_right_idx: " << middle_right_idx << endl;

//         int middle_right_point_idx = point_indices_sorted[middle_right_idx];
//         cout << "middle_right_point_idx :" << middle_right_point_idx << endl;

//         double middle_right_point_value = db[middle_right_point_idx][axis];
//         cout << "middle_right_point_value :" << middle_right_point_value << endl;

//         root.set_value((middle_left_point_value + middle_right_point_value) * 0.5);

// 	    cout << "root.value :" << root.get_value() << endl;


//         // return root;

//         // root.left = kdtree_recursive_build(root.left,
//         //                                    db,
//         //                                    point_indices_sorted[0:middle_right_idx],
//         //                                    axis_round_robin(axis, dim=db.shape[1]),
//         //                                    leaf_size)
//         // root.right = kdtree_recursive_build(root.right,
//         //                                    db,
//         //                                    point_indices_sorted[middle_right_idx:],
//         //                                    axis_round_robin(axis, dim=db.shape[1]),
//         //                                    leaf_size)

//     }

    
// }

// vector<vector<double>>& db, vector<int>& point_indices, 

// Node  kdtree_build(vector<vector<double>>* points,vector<int>* point_indices,int* axis,int* leaf_size)
// {
//     Node root2(*axis,-1,NULL,NULL,*point_indices);

//     if((*point_indices).size() > *leaf_size)
//     {
//         vector<int> point_indices_sorted;
//         point_indices_sorted = sort_key_by_vale(*point_indices,*points,*axis);

//         cout << "point_indices_sorted.size() :" << point_indices_sorted.size() << endl;

//         int middle_left_idx = ceil(point_indices_sorted.size() / 2) - 1;
//         cout << "middle_left_idx: " << middle_left_idx << endl;

//         int middle_left_point_idx = point_indices_sorted[middle_left_idx];
//         cout << "middle_left_point_idx :" << middle_left_point_idx << endl;

//         double middle_left_point_value = (*points)[middle_left_point_idx][*axis];
//         cout << "middle_left_point_value :" << middle_left_point_value << endl;



//         int middle_right_idx = middle_left_idx + 1;
//         cout << "middle_right_idx: " << middle_right_idx << endl;

//         int middle_right_point_idx = point_indices_sorted[middle_right_idx];
//         cout << "middle_right_point_idx :" << middle_right_point_idx << endl;

//         double middle_right_point_value = (*points)[middle_right_point_idx][*axis];
//         cout << "middle_right_point_value :" << middle_right_point_value << endl;

//         root2.set_value((middle_left_point_value + middle_right_point_value) * 0.5);

// 	    cout << "root.value :" << root2.get_value() << endl;


//         vector<int> left_point_indices_sorted(point_indices_sorted.begin(), point_indices_sorted.begin()+middle_left_idx+1);
//         vector<int> right_point_indices_sorted(point_indices_sorted.begin()+middle_right_idx, point_indices_sorted.end());

//         for(int j = 0;j<left_point_indices_sorted.size();j++)
//         {
//             cout << "左边的的数据：" << left_point_indices_sorted[j] << endl;
//         }

//         for(int j = 0;j<right_point_indices_sorted.size();j++)
//         {
//             cout << "右边的的数据：" << right_point_indices_sorted[j] << endl;
//         }

//         *axis = axis_round_robin(*axis, (*points)[0].size());
//         cout << "axis: " << *axis << endl;



//         vector<vector<double>>* points2 = points;
//         int* axis2 = axis;
//         int* leaf_size2 = leaf_size;

//         cout << "address: " << &kdtree_build(points2, &left_point_indices_sorted, axis2, leaf_size2) << endl;

//         // root2.set_left(&kdtree_build(&points2, &left_point_indices_sorted, &axis2, &leaf_size2));
        

       


//         // root2.set_left(&(kdtree_build(points,left_point_indices_sorted,axis,leaf_size)));
//     }

//     return root2;
// }




// Node kdtree_build(Node& root, vector<vector<double>>& points,vector<int>& point_indices,int& axis,int& leaf_size)
// {
    
//     if(root.get_is_ok() == false)
//     {
//         root.set_axis(axis);
//         root.set_point_indices(point_indices);
//     }

//     return root;

    // if(point_indices.size() > leaf_size)
    // {
    //     vector<int> point_indices_sorted;
    //     point_indices_sorted = sort_key_by_vale(point_indices,points,axis);

    //     cout << "point_indices_sorted.size() :" << point_indices_sorted.size() << endl;

    //     int middle_left_idx = ceil(point_indices_sorted.size() / 2) - 1;
    //     cout << "middle_left_idx: " << middle_left_idx << endl;

    //     int middle_left_point_idx = point_indices_sorted[middle_left_idx];
    //     cout << "middle_left_point_idx :" << middle_left_point_idx << endl;

    //     double middle_left_point_value = points[middle_left_point_idx][axis];
    //     cout << "middle_left_point_value :" << middle_left_point_value << endl;



    //     int middle_right_idx = middle_left_idx + 1;
    //     cout << "middle_right_idx: " << middle_right_idx << endl;

    //     int middle_right_point_idx = point_indices_sorted[middle_right_idx];
    //     cout << "middle_right_point_idx :" << middle_right_point_idx << endl;

    //     double middle_right_point_value = points[middle_right_point_idx][axis];
    //     cout << "middle_right_point_value :" << middle_right_point_value << endl;

    //     root.set_value((middle_left_point_value + middle_right_point_value) * 0.5);

	//     cout << "root.value :" << root.get_value() << endl;


    //     vector<int> left_point_indices_sorted(point_indices_sorted.begin(), point_indices_sorted.begin()+middle_left_idx+1);
    //     vector<int> right_point_indices_sorted(point_indices_sorted.begin()+middle_right_idx, point_indices_sorted.end());

    //     // axis = axis_round_robin(axis, points[0].size());
    //     // cout << "axis: " << axis << endl;


    //     // cout << "axis: " << &(kdtree_build(points,left_point_indices_sorted,axis,leaf_size)) << endl;

       


    //     // root2.set_left(&(kdtree_build(points,left_point_indices_sorted,axis,leaf_size)));
    // }

// }


int main(int argc, char **argv)
{

	cout << "\033[1;31m sl_hw2! \033[0m" << endl;

    //参数设置
    int db_size = 8;
    int leaf_size = 1;
    int dim = 3;
    int k = 1;

    //变量定义
    vector<vector<double>> points;


    //数据获取
	// points = read_data();
    // points = test_data(db_size);
    points = test_data_2();



	KNNResultSet set(3);
	// set.get_capacity();
	// set.get_worst_dist();

    cout << set.full() << endl;
    cout << set.get_worstDist() << endl;
    cout << "size: " << set.size() << endl;

    set.add_point(&set,100,1);
    cout << "size: " << set.size() << endl;
    cout << "get_worstDist():" << set.get_worstDist() << endl;
    set.add_point(&set,1000,2);
    cout << "size: " << set.size() << endl;
    cout << "get_worstDist():" << set.get_worstDist() << endl;
    set.add_point(&set,10000,3);
    cout << "size: " << set.size() << endl;
    cout << "get_worstDist():" << set.get_worstDist() << endl;

    set.add_point(&set,99,4);
    cout << "size: " << set.size() << endl;
    cout << "get_worstDist():" << set.get_worstDist() << endl;

    set.add_point(&set,9,5);
    cout << "size: " << set.size() << endl;
    cout << "get_worstDist():" << set.get_worstDist() << endl;
    
    //set.get_dist_index_list(&set);

    set.show(&set);

    // kdtree_construction(points,leaf_size);

    // Node root(-1);

    // if (root.get_axis() == -1)
    // {
    //     // root = Node(axis, None, None, None, point_indices)
    //     cout << "-------------" << endl;
    // }


    int N = points.size();
    // int dim = points[0].size();
    int axis = 0;

    cout << "N:" << N << endl;
    cout << "dim:" << dim << endl;

    vector<int> point_indices(N); ////初始化了N个默认值为0的元素

    for(int i=0;i<point_indices.size();i++)
    {
        point_indices[i] = i;
    }


    // Node root;

    // root = kdtree_build(root, points, point_indices, axis, leaf_size);

    // cout << "root.get_axis():" << root.get_axis() << endl;
    // cout << "root.get_point_indices_len():" << root.get_point_indices_len() << endl;

	return 0;

}

// 1.stod转化精度不足的问题
// 2.类的变量怎么判断是私有的还是公有的？


// class Line
// {
// public:
// 	void setLength(double len);
// 	double getLength(void);
// 	Line(double len);  // 这是构造函数

// private:
// 	double length;
// };

// // 成员函数定义，包括构造函数
// Line::Line(double len)
// {
// 	cout << "Object is being created, length = " << len << endl;
// 	length = len;
// }

// void Line::setLength(double len)
// {
// 	length = len;
// }

// double Line::getLength(void)
// {
// 	return length;
// }