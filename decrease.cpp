#include <iostream>  
#include <fstream>  
#include <vector>  
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<algorithm>
#include<string>
#include<ctime>
#include <list>  
#include<algorithm>
#include<queue>
#include<set>
#include <numeric>
#include<functional>
#include "minist.h"
using namespace std;
using namespace cv;
#define Maxsize 600


//邻接表中节点，每个节点与该节点对应的索引号指定一条边  
struct Node
{
	int u; //边终点节点号  
	double w; //边权值  
	Node(int a, double b) :u(a), w(b){}
};
struct Record
{
	int pre;
	double cost;
};
vector<Record> Path;
class Cost_Distance
{
  public:
	  int node_i;
	  int node_j;
	  double cost_distance;
	  Cost_Distance(int i, int j, double distance_)
	  {
		  node_i = i;
		  node_j = j;
		  cost_distance = distance_;
	  }
	  bool operator  < (const Cost_Distance &other) const
	  {
		  return this->cost_distance < other.cost_distance;
	  }
	  bool operator  > (const Cost_Distance &other) const
	  {
		  return this->cost_distance > other.cost_distance;
	  }
};
void savePCA(const string &file_name, cv::PCA pca_)
{
	FileStorage fs(file_name, FileStorage::WRITE);
	fs << "mean" << pca_.mean;
	fs << "e_vectors" << pca_.eigenvectors;
	fs << "e_values" << pca_.eigenvalues;
	fs.release();
}
typedef priority_queue<Cost_Distance, vector<Cost_Distance>, greater<Cost_Distance>> min_Cost;
void loadPCA(const string &file_name, PCA &pca_)
{
	FileStorage fs2(file_name, FileStorage::READ);
	fs2["mean"] >> pca_.mean;
	fs2["e_vectors"] >> pca_.eigenvectors;
	fs2["e_values"] >> pca_.eigenvalues;
	fs2.release();
}
double distance_node(Mat &point1,Mat &point2)
{
	assert(point1.cols == point2.cols);
	double distance = 0.0;
	for (int i = 0; i < point1.cols; i++)
	{
		double vec = point1.at<double>(0, i) - point2.at<double>(0, i);
		distance += vec*vec;
	}
	return distance;
}

void Dijstra(vector<list<Node>> &GraphAdj,int s)   //最短路径dijstra算法，s为源节点
{
	vector<bool> isUsed(GraphAdj.size(), false);
	list<int> Assi;
	Path.assign(GraphAdj.size(), Record());
	for (int i = 0; i < GraphAdj.size(); i++)
	{
		Path[i].pre = i;
		Path[i].cost = INT_MAX;
	}
	isUsed[s] = true;
	for (auto it = GraphAdj[s].begin(); it != GraphAdj[s].end(); it++)
	{
		Path[it->u].pre = s;
		Path[it->u].cost = it->w;
		Assi.push_back(it->u);
	}
	while (!Assi.empty())
	{
		list<int>::iterator It;
		int mincost = INT_MAX;
		for (auto it = Assi.begin(); it != Assi.end(); it++)
		{
			if (mincost > Path[*it].cost)
			{
				mincost = Path[*it].cost;
				It = it;
			}
		}
		int u = *It;
		Assi.erase(It);
		isUsed[u] = true;
		for (auto it = GraphAdj[u].begin(); it != GraphAdj[u].end(); it++)
		{
			if (isUsed[it->u]) continue;
			if (Path[it->u].cost == INT_MAX) Assi.push_back(it->u);
			if (Path[it->u].cost > mincost + it->w)
			{
				Path[it->u].cost = mincost + it->w;
				Path[it->u].pre = u;
			}
		}
	}
}
void Traverse(int k)
{
	if (Path[k].pre == k){ cout << k; return; }
	Traverse(Path[k].pre);
	cout << " " << k;
}
void Print(int s,int n)             //输出最短路径，s为源节点，n为总的节点数
{
	cout << "Result:\n";
	for (int i = 0; i < 10; i++)
	{
		if (i == s) continue;
		cout << "From " << s << " to " << i << ": ";
		if (Path[i].cost == INT_MAX)
		{ 
			cout << "No path\n\n"; continue;
		}
		Traverse(i);
		cout << endl;
		cout << "Minimal Cost: " << Path[i].cost << endl << endl;
	}
}   

//void Create_Min_Cost(Mat &pt, vector<list<Node>> &GraphAdj, min_Cost &min_heap)
//{
//	Mat pt1, pt2;
//	for (int i = 0; i <GraphAdj.size(); i++)
//	{
//		for (int j = 0; j < GraphAdj.size(); j++)
//		{
//			double Sum_distance=0.0;
//			double distance_1=0.0;    //i与j联通的点的间接距离
//			double distance_2 = 0.0;     //i与j联通的点的直接距离
//			for (auto iter = GraphAdj[i].begin(); iter != GraphAdj[i].end(); iter++)
//			{
//				if (iter->u == j)
//				{
//					for (auto iter1 = GraphAdj[iter->u].begin(); iter1 != GraphAdj[iter->u].end(); iter1++)
//					{
//						if (iter1->u != i)
//						{
//							distance_1 = iter->w + iter1->w;
//							pt.row(i).copyTo(pt1);
//							pt.row(iter1->u).copyTo(pt2);
//							distance_2 = distance_node(pt1, pt2);
//							Sum_distance += (distance_2 - distance_1)*(distance_2 - distance_1);
//						}
//					}
//					min_heap.push(Cost_Distance(i, iter->u, Sum_distance));
//				}
//			}
//		}
//	}
//}
void Create_Min_Cost(Mat &pt, vector<list<Node>> &GraphAdj, min_Cost &min_heap)
{
	Mat pt1, pt2;
	for (int i = 0; i < GraphAdj.size(); i++)
	{
			
			double distance_1 = 0.0;    //i与j联通的点的间接距离
			double distance_2 = 0.0;     //i与j联通的点的直接距离
			for (auto iter = GraphAdj[i].begin(); iter != GraphAdj[i].end(); iter++)
			{
				    double Sum_distance = 0.0;
					for (auto iter1 = GraphAdj[iter->u].begin(); iter1 != GraphAdj[iter->u].end(); iter1++)
					{
						if (iter1->u != i)
						{
							distance_1 = iter->w + iter1->w;
							pt.row(i).copyTo(pt1);
							pt.row(iter1->u).copyTo(pt2);
							distance_2 = distance_node(pt1, pt2);
							Sum_distance += (distance_2 - distance_1)*(distance_2 - distance_1);
						}
					}
					min_heap.push(Cost_Distance(i, iter->u, Sum_distance));
			}
	}
}
void Decrease_node(Mat &pt, vector<list<Node>> &GraphAdj, min_Cost &min_heap)
{
	Mat pt1, pt2;
	vector<int> index1;
	vector<int> index2;
	vector<list<Node>> result;
	index1.assign(GraphAdj.size(), 0);
	index2.assign(GraphAdj.size(), 0);
	double distance;
	for (int i = 0; i < 5; i++)
	{
		int t = min_heap.top().node_i;   //要保留的点
		int k = min_heap.top().node_j;   //要删除的点
		if (index1[k] == 1)
		{
			min_heap.pop();
			continue;
		}
		index1[k] = 1;
		index2[t] = 1;
		pt.row(t).copyTo(pt2);
		bool flag = true;
		for (auto iter = GraphAdj[k].begin(); iter != GraphAdj[k].end(); iter++)
		{
			pt.row(iter->u).copyTo(pt1);
			distance = distance_node(pt1, pt2);
			for (auto iter1 = GraphAdj[t].begin(); iter1 != GraphAdj[t].end(); iter1++)
			{
				if (iter1->u == iter->u)
				{
					flag == false;
				}
			}
			if (flag == true)
			{
				GraphAdj[t].push_back(Node(iter->u, distance));
				GraphAdj[iter->u].push_back(Node(t, distance));
			}
		}
		/*int sum = accumulate(index1.begin(), index2.end(), 0);
		if (sum >= 300)
		{
			break;
		}*/
		min_heap.pop();
	}
	for (int i = 0; i < GraphAdj.size(); i++)
	{
		if (index1[i] == 1)
		{
			index2[i] = 0;
		}
	}
	for (int i = 0; i < GraphAdj.size(); i++)
	{
		if (index2[i] == 1)
		{
			cout << i << endl;
			result.push_back(GraphAdj[i]);
		}
	}
	for (int i = 0; i < 5; i++) {
		for (auto iter = result[i].begin(); iter != result[i].end(); iter++)
		{
			if (iter ->u != NULL)
			{
				cout << iter->u << " ";
			}
		}
		cout << endl;
	}
}
int main()
{

	string infileImageTrain("D:\\code\\knn-decrease\\MINIST\\train-images.idx3-ubyte");
	string infileLabTrain("D:\\code\\knn-decrease\\MINIST\\train-labels.idx1-ubyte");
	Mat digitsUcharTrain;
	Mat lableIntTrain;
	MNIST mmt;
	mmt.loadDigits(infileImageTrain, digitsUcharTrain);
	mmt.loadLabels(infileLabTrain, lableIntTrain);

	Mat digitsImageTrain;

	digitsUcharTrain.convertTo(digitsImageTrain, CV_64FC1);

	string infileImageTest("D:\\code\\knn-decrease\\MINIST\\t10k-images.idx3-ubyte");
	string infileLabTest("D:\\code\\knn-decrease\\MINIST\\t10k-labels.idx1-ubyte");

	Mat digitsUcharTest;
	Mat lableIntTest;
	mmt.loadDigits(infileImageTest, digitsUcharTest);
	mmt.loadLabels(infileLabTest, lableIntTest);
	Mat digitsImageTest;

	digitsUcharTest.convertTo(digitsImageTest, CV_64FC1);
	string file_name = "D:\\code\\knn-decrease\\MINIST\\pcaValue6000.xml";
	//PCA pca(digitsImageTrain, cv::Mat(), CV_PCA_DATA_AS_ROW, low_dimension);
	//savePCA(file_name.c_str(), pca);
	PCA pca;
	loadPCA(file_name.c_str(), pca);
	Mat point = pca.project(digitsImageTrain);
	//Mat pt1, pt2;
	//point.row(0).copyTo(pt1);
	//point.row(49).copyTo(pt2);
	//double res = distance_node(pt1, pt2);
	ifstream test("D:\\code\\knn-decrease\\MINIST\\knneartest600", ifstream::in);
	ofstream result("D:\\code\\knn-decrease\\MINIST\\knneartest600-cost", ofstream::out);
	vector<int>  index;
	vector<vector<int>> knn;
	vector<vector<double>> knndistance;
	int t;
	double k;
	vector<list<Node>> Adj;
	Adj.assign(600, list<Node>());
	index.resize(600);
	knn.resize(600);
	knndistance.resize(600);
	for (int i = 0; i < 600; i++)
	{
		test >> index[i];
		knn[i].resize(4);
		knndistance[i].resize(4);
		test >> t;
		for (int j = 0; j < 3; j++)
		{
			test >> knn[i][j];
		}
		test >> k;
		for (int j = 0; j < 3; j++)
		{
			test >> knndistance[i][j];
		}
	}
	test.close();
	bool flag1 = true, flag2 = true;
	for (int i = 0; i < 600; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (auto iter = Adj[i].begin(); iter != Adj[i].end(); iter++)
			{
				if (iter->u == knn[i][j])
					flag2 = false;
			}
			if (flag2 == true)
			{
				Adj[i].push_back(Node(knn[i][j], knndistance[i][j]));
			}
			else
			{
				flag2 = true;
			}
			
			for (auto iter = Adj[knn[i][j]].begin(); iter != Adj[knn[i][j]].end(); iter++)
			{
				if (iter->u == i)
					flag1 = false;
			}
			if (flag1 == true)
			{
				Adj[knn[i][j]].push_back(Node(i, knndistance[i][j]));
			}
			else
			{
				flag1 = true;
			}
		}
	}
	
	min_Cost min_heap;
	Create_Min_Cost(point, Adj,min_heap);
	Decrease_node(point, Adj, min_heap);
	/*for (int i = 0; i < 5; i++) {
		for (auto iter = Adj[i].begin(); iter != Adj[i].end(); iter++)
			cout << iter->u << " ";
		cout << endl;
	}*/
	/*cout << min_heap.size() << endl;
	for (int i = 0; i < min_heap.size(); i++)
	{
		result << min_heap.top().node_i << " " << min_heap.top().node_j << " " << min_heap.top().cost_distance;
		result << endl;
		min_heap.pop();
	}*/
	//Dijstra(Adj, 0);
	//Print(0, Adj.size());
	system("pause");
	return 0;
}