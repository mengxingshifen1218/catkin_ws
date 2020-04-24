
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

#include <vector>
#include <iomanip>
#include <stdio.h>
#include <sstream> //字符串转换
#include <fstream>
#include <string>
#include <typeinfo>
#include <limits.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <math.h>

#define KDtreeSize 1000

#define UL unsigned long

using namespace std;


struct coordinate
{
    double x = 0;
    double y = 0;
    UL index = 0;
};

struct TreeNode
{
    struct coordinate dom_elt;
    UL split = 0;
    struct TreeNode* left = nullptr;
    struct TreeNode* right = nullptr;
};

bool cmp1(const coordinate& a, const coordinate& b){
    return a.x < b.x;
}

bool cmp2(const coordinate& a, const coordinate& b){
    return a.y < b.y;
}

bool equal(const coordinate& a, const coordinate& b){
    return (a.x == b.x && a.y == b.y);
}

void ChooseSplit(coordinate exm_set[], UL size, UL &split, coordinate &SplitChoice){
    /*compute the variance on every dimension. Set split as the dismension that have the biggest
     variance. Then choose the instance which is the median on this split dimension.*/
    /*compute variance on the x,y dimension. DX=EX^2-(EX)^2*/
    double tmp1, tmp2;
    tmp1 = tmp2 = 0;
    for (int i = 0; i < size; ++i)
    {
        tmp1 += 1.0 / (double)size * exm_set[i].x * exm_set[i].x;
        tmp2 += 1.0 / (double)size * exm_set[i].x;
    }
    double v1 = tmp1 - tmp2 * tmp2;  //compute variance on the x dimension

    tmp1 = tmp2 = 0;
    for (int i = 0; i < size; ++i)
    {
        tmp1 += 1.0 / (double)size * exm_set[i].y * exm_set[i].y;
        tmp2 += 1.0 / (double)size * exm_set[i].y;
    }
    double v2 = tmp1 - tmp2 * tmp2;  //compute variance on the y dimension

    split = v1 > v2 ? 0:1; //set the split dimension

    if (split == 0)
    {
        sort(exm_set,exm_set + size, cmp1);
    }
    else{
        sort(exm_set,exm_set + size, cmp2);
    }

    //set the split point value
    SplitChoice.x = exm_set[size / 2].x;
    SplitChoice.y = exm_set[size / 2].y;

}

TreeNode* build_kdtree(coordinate exm_set[], UL size, TreeNode* T){
    //call function ChooseSplit to choose the split dimension and split point
    if (size == 0){
        return nullptr;
    }
    else{
        UL split;
        coordinate dom_elt;
        ChooseSplit(exm_set, size, split, dom_elt);
        coordinate exm_set_right [KDtreeSize];
        coordinate exm_set_left [KDtreeSize];
        UL size_left ,size_right;
        size_left = size_right = 0;

        if (split == 0)
        {
            for (UL i = 0; i < size; ++i)
            {

                if (!equal(exm_set[i],dom_elt) && exm_set[i].x <= dom_elt.x)
                {
                    exm_set_left[size_left].x = exm_set[i].x;
                    exm_set_left[size_left].y = exm_set[i].y;
                    size_left++;
                }
                else if (!equal(exm_set[i],dom_elt) && exm_set[i].x > dom_elt.x)
                {
                    exm_set_right[size_right].x = exm_set[i].x;
                    exm_set_right[size_right].y = exm_set[i].y;
                    size_right++;
                }
            }
        }
        else{
            for (UL i = 0; i < size; ++i)
            {

                if (!equal(exm_set[i],dom_elt) && exm_set[i].y <= dom_elt.y)
                {
                    exm_set_left[size_left].x = exm_set[i].x;
                    exm_set_left[size_left].y = exm_set[i].y;
                    size_left++;
                }
                else if (!equal(exm_set[i],dom_elt) && exm_set[i].y > dom_elt.y)
                {
                    exm_set_right[size_right].x = exm_set[i].x;
                    exm_set_right[size_right].y = exm_set[i].y;
                    size_right++;
                }
            }
        }
        T = new TreeNode;
        T->dom_elt.x = dom_elt.x;
        T->dom_elt.y = dom_elt.y;
        T->split = split;
        T->left = build_kdtree(exm_set_left, size_left, T->left);
        T->right = build_kdtree(exm_set_right, size_right, T->right);
        return T;

    }
}


double Distance(coordinate a, coordinate b){
    double tmp = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    return sqrt(tmp);
}


void searchNearest(TreeNode * Kd, coordinate target, coordinate &nearestpoint, double & distance){

    //1. 如果Kd是空的，则设dist为无穷大返回

    //2. 向下搜索直到叶子结点

    stack<TreeNode*> search_path;
    TreeNode* pSearch = Kd;
    coordinate nearest;
    double dist;

    while(pSearch != nullptr)
    {
        //pSearch加入到search_path中;
        search_path.push(pSearch);

        if (pSearch->split == 0)
        {
            if(target.x <= pSearch->dom_elt.x) /* 如果小于就进入左子树 */
            {
                pSearch = pSearch->left;
            }
            else
            {
                pSearch = pSearch->right;
            }
        }
        else{
            if(target.y <= pSearch->dom_elt.y) /* 如果小于就进入左子树 */
            {
                pSearch = pSearch->left;
            }
            else
            {
                pSearch = pSearch->right;
            }
        }
    }
    //取出search_path最后一个赋给nearest
    nearest.x = search_path.top()->dom_elt.x;
    nearest.y = search_path.top()->dom_elt.y;
    search_path.pop();


    dist = Distance(nearest, target);
    //3. 回溯搜索路径

    TreeNode* pBack;

    while(search_path.empty())
    {
        //取出search_path最后一个结点赋给pBack
        pBack = search_path.top();
        search_path.pop();

        if(pBack->left == nullptr && pBack->right == nullptr) /* 如果pBack为叶子结点 */

        {

            if( Distance(nearest, target) > Distance(pBack->dom_elt, target) )
            {
                nearest = pBack->dom_elt;
                dist = Distance(pBack->dom_elt, target);
            }

        }

        else

        {

            UL s = pBack->split;
            if (s == 0)
            {
                if( fabs(pBack->dom_elt.x - target.x) < dist) /* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
                {
                    if( Distance(nearest, target) > Distance(pBack->dom_elt, target) )
                    {
                        nearest = pBack->dom_elt;
                        dist = Distance(pBack->dom_elt, target);
                    }
                    if(target.x <= pBack->dom_elt.x) /* 如果target位于pBack的左子空间，那么就要跳到右子空间去搜索 */
                        pSearch = pBack->right;
                    else
                        pSearch = pBack->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索 */
                    if(pSearch != nullptr)
                        //pSearch加入到search_path中
                        search_path.push(pSearch);
                }
            }
            else {
                if( fabs(pBack->dom_elt.y - target.y) < dist) /* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
                {
                    if( Distance(nearest, target) > Distance(pBack->dom_elt, target) )
                    {
                        nearest = pBack->dom_elt;
                        dist = Distance(pBack->dom_elt, target);
                    }
                    if(target.y <= pBack->dom_elt.y) /* 如果target位于pBack的左子空间，那么就要跳到右子空间去搜索 */
                        pSearch = pBack->right;
                    else
                        pSearch = pBack->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索 */
                    if(pSearch != nullptr)
                        // pSearch加入到search_path中
                        search_path.push(pSearch);
                }
            }

        }
    }

    nearestpoint.x = nearest.x;
    nearestpoint.y = nearest.y;
    distance = dist;

}


void test_kdtree(){
    coordinate exm_set[6];
    exm_set[0].x = 2;    exm_set[0].y = 3;
    exm_set[1].x = 5;    exm_set[1].y = 4;
    exm_set[2].x = 9;    exm_set[2].y = 6;
    exm_set[3].x = 4;    exm_set[3].y = 7;
    exm_set[4].x = 8;    exm_set[4].y = 1;
    exm_set[5].x = 7;    exm_set[5].y = 2;

    struct TreeNode * root = nullptr;
    root = build_kdtree(exm_set, 6, root);

    coordinate nearestpoint;
    double distance;
    coordinate target;
    target.x = 2.1;
    target.y = 3.2;

    searchNearest(root, target, nearestpoint, distance);
    cout<<"The nearest distance is "<<distance<<",and the nearest point is "<<nearestpoint.x<<","<<nearestpoint.y<<endl;
}


int main(){
    test_kdtree();
    return 0;
}






















































// #include <iostream>
// // #include <pcl/common/common_headers.h>
// // #include <pcl/io/pcd_io.h>
// // #include <pcl/visualization/pcl_visualizer.h>
// // #include <pcl/visualization/cloud_viewer.h>
// // #include <pcl/console/parse.h>
// #include <vector>
// #include <iomanip>
// #include <stdio.h>
// #include <sstream> //字符串转换
// #include <fstream>
// #include <string>
// #include <vector>
// #include <typeinfo>
// #include <limits.h>
// #include <algorithm>
// #include <bits/stdc++.h>
// #include <math.h>
 
// using namespace std;

// // void QuickSort(int *arr,int low,int high)
// // {
// // 	int i=low,j=high;
// // 	int temp=arr[low];
// // 	while(i<j)
// // 	{
// // 		for(;i<j&&arr[j]>=temp;j--);
// // 		if(i<j)
// // 			arr[i++]=arr[j];
// // 		for(;i<j&&arr[i]<=temp;i++);
// // 		if(i<j)
// // 			arr[j--]=arr[i];
// // 	}
// // 	arr[i]=temp;
// // 	if(i-1>low)
// // 	    QuickSort(arr,low,i-1);
// // 	if(j+1<high)
// // 		QuickSort(arr,j+1,high);
// // }



// class Node
// {
// 	private:
// 		int _distance;

// 	public:
// 		Node(int distance)
// 		{
// 			_distance = distance;
// 		}

// 		int get_distance()
// 		{
// 			return _distance;
// 		}
// };

// // bool comp(const vector<Node> &a, const vector<Node> &b)
// // {
// //     return a.get_distance() > b.get_distance();
// // }



// // bool cmp(vector<int>&a,vector<int>&b){

// // 	cout << 'a[0]' << a[0] << endl;
// // 	cout << 'b[0]' << b[0] << endl;
// //     // if(a[0]!=b[0]) return a[0]<b[0];
// //     // else return a[1]>b[1];

// // 	return a[1]<b[1];

// // }


// bool cmp_0(vector<double>&a,vector<double>&b)
// {
// 	return a[0]<b[0];
// }

// bool cmp_1(vector<double>&a,vector<double>&b)
// {
// 	return a[1]<b[1];
// }

// bool cmp_2(vector<double>&a,vector<double>&b)
// {
// 	return a[2]<b[2];
// }


// vector<int> sort_key_by_vale(vector<int> &point_indices, 
// 							 vector<vector<double>> &db,
// 							 int axis)

// {
// 	vector<vector<double>> result;
// 	vector<double> temp;

// 	for(int i = 0;i<point_indices.size();i++)
// 	{

// 		temp.push_back(db[point_indices[i]][0]);
// 		temp.push_back(db[point_indices[i]][1]);
// 		temp.push_back(db[point_indices[i]][2]);
// 		temp.push_back(point_indices[i]); //保存排序前的下标
// 		result.push_back(temp);
// 		temp.clear();
// 		cout << "i: " << i << endl;
// 	}

// 	for(int j = 0;j<point_indices.size();j++)
// 	{
// 		 cout << "排序前加了index信息" <<':'<< result[j][0] << ',' << result[j][1]<< ',' << result[j][2] << ',' << result[j][3] << endl;
// 	}


// 	//排序
// 	if(axis == 0)
// 	{
// 		sort(result.begin(),result.end(),cmp_0);
// 	}
// 	else if (axis == 1)
// 	{
// 		sort(result.begin(),result.end(),cmp_1);
// 	}
// 	else 
// 	{
// 		sort(result.begin(),result.end(),cmp_2);
// 	}

	
//     vector<int> result_index;
// 	for(int i = 0;i<point_indices.size();i++)
// 	{
// 		result_index.push_back(result[i][3]);
// 		cout << "result_index" << result[i][3] << endl;
// 	}

// 	for(int j = 0;j<point_indices.size();j++)
// 	{
// 		 cout << "排序之后的值" <<':'<< result[j][0] << ',' << result[j][1]<< ',' << result[j][2] << endl;
// 	}



// 	// vector<vector<double>> after_sort;
// 	// vector<double> after_sort_temp;
// 	// for(int i = 0;i<point_indices.size();i++)
// 	// {
// 	// 	after_sort_temp.push_back(result[result_index[i]][0]);
// 	// 	after_sort_temp.push_back(result[result_index[i]][1]);
// 	// 	after_sort_temp.push_back(result[result_index[i]][2]);
// 	// 	after_sort.push_back(after_sort_temp);
// 	// 	after_sort_temp.clear();
// 	// }

// 	// for(int j = 0;j<point_indices.size();j++)
// 	// {
// 	// 	 cout << "排序之后的值" <<':'<< after_sort[j][0] << ',' << after_sort[j][1]<< ',' << after_sort[j][2] << endl;
// 	// }

// 	return result_index;

// }

// vector<vector<double>> test_data(int db_size)
// {
//     vector<vector<double>> vec1;
//     for(int i=0;i<db_size;i++ )
//     {
//         int k = 2;
//         vector<double> vec2;
//         vec2.push_back(k-i+2);
//         vec2.push_back(2*i-k);
//         vec2.push_back(k-i+1);
//         if(k%2==0)
//         {
//             k = (-1) * k + 2;
//         }
//         vec1.push_back(vec2);
//     }

//     for(int j=0;j<vec1.size();j++)
//     {
//         cout << "原始值" <<':'<< vec1[j][0] << ',' << vec1[j][1]<< ',' << vec1[j][2] << endl;
//     }

//    return vec1; 
// }



// vector<vector<double>> test_data_2()
// {
//     vector<vector<double>> vec1;
// 	vector<double> vec2;

// 	vec2.push_back(1);
// 	vec2.push_back(2);
// 	vec2.push_back(4);
//  	vec1.push_back(vec2);
// 	vec2.clear();

// 	vec2.push_back(2);
// 	vec2.push_back(4);
// 	vec2.push_back(6);
//  	vec1.push_back(vec2);
// 	vec2.clear();

// 	vec2.push_back(6);
// 	vec2.push_back(3);
// 	vec2.push_back(1);
//  	vec1.push_back(vec2);
// 	vec2.clear();
   
//     vec2.push_back(0);
// 	vec2.push_back(2);
// 	vec2.push_back(1);
//  	vec1.push_back(vec2);
// 	vec2.clear();

// 	vec2.push_back(9);
// 	vec2.push_back(4);
// 	vec2.push_back(3);
//  	vec1.push_back(vec2);
// 	vec2.clear();

// 	vec2.push_back(6);
// 	vec2.push_back(3);
// 	vec2.push_back(1);
//  	vec1.push_back(vec2);
// 	vec2.clear();

// 	vec2.push_back(7);
// 	vec2.push_back(8);
// 	vec2.push_back(4);
//  	vec1.push_back(vec2);
// 	vec2.clear();

// 	vec2.push_back(5);
// 	vec2.push_back(6);
// 	vec2.push_back(9);
//  	vec1.push_back(vec2);
// 	vec2.clear();

//     for(int j=0;j<vec1.size();j++)
//     {
//         cout << "原始值" <<':'<< vec1[j][0] << ',' << vec1[j][1]<< ',' << vec1[j][2] << endl;
//     }

//    return vec1; 
// }


// int main(int argc, char **argv) 
// {
//     std::cout << "Test PCL !!!" << std::endl;
//     cout << "\033[1;31m PCL! \033[0m" << endl;

// 	int N = 8;
// 	int M = 8;
// 	int axis = 0;

// 	vector<vector<double>> points;
// 	points = test_data_2();

// 	vector<int> point_indices(M); ////初始化了N个默认值为0的元素

//     for(int i=0;i<point_indices.size();i++)
//     {
//         point_indices[i] = i;
// 		cout << "point_indices" <<':'<< point_indices[i] << endl;
//     }

// 	vector<int> point_indices_sorted;
// 	point_indices_sorted = sort_key_by_vale(point_indices,points,axis);

	

// 	int middle_left_idx = ceil(point_indices_sorted.size() / 2) - 1;
// 	cout << "middle_left_idx: " << middle_left_idx << endl;

// 	int middle_left_point_idx = point_indices_sorted[middle_left_idx];
// 	cout << "middle_left_point_idx :" << middle_left_point_idx << endl;

//     double middle_left_point_value = points[middle_left_point_idx][axis];
// 	cout << "middle_left_point_value :" << middle_left_point_value << endl;



// 	int middle_right_idx = middle_left_idx + 1;
// 	cout << "middle_right_idx: " << middle_right_idx << endl;

// 	int middle_right_point_idx = point_indices_sorted[middle_right_idx];
// 	cout << "middle_right_point_idx :" << middle_right_point_idx << endl;

//     double middle_right_point_value = points[middle_right_point_idx][axis];
// 	cout << "middle_right_point_value :" << middle_right_point_value << endl;

// 	double root_value = (middle_left_point_value + middle_right_point_value) * 0.5;
// 	cout << "root_value :" << root_value << endl;
	
//     return 0;
// }


// // int a[]={23,31,49,31,6,19};
// 	// QuickSort(a,0,5);
// 	// for(int i=0;i<6;i++)
// 	//     cout<<a[i]<<" ";
// 	// cout<<endl;	


// //  pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
// //     uint8_t r(255), g(15), b(15);
// //     for (float z(-1.0); z <= 1.0; z += 0.05)
// //     {
// //       for (float angle(0.0); angle <= 360.0; angle += 5.0)
// //       {
// // 	pcl::PointXYZRGB point;
// // 	point.x = 0.5 * cosf (pcl::deg2rad(angle));
// // 	point.y = sinf (pcl::deg2rad(angle));
// // 	point.z = z;
// // 	uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
// // 		static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(b));
// // 	point.rgb = *reinterpret_cast<float*>(&rgb);
// // 	point_cloud_ptr->points.push_back (point);
// //       }
// //       if (z < 0.0)
// //       {
// // 	r -= 12;
// // 	g += 12;
// //       }
// //       else
// //       {
// // 	g -= 12;
// // 	b += 12;
// //       }
// //     }
    
// //     point_cloud_ptr->width = (int) point_cloud_ptr->points.size ();
// //     point_cloud_ptr->height = 1;
    
// //     pcl::visualization::CloudViewer viewer ("test");
// //     viewer.showCloud(point_cloud_ptr);
// //     while (!viewer.wasStopped()){ };