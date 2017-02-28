#include <iostream>  
#include <fstream>  
#include <vector>  
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<algorithm>
#include<string>
#include<ctime>
#include <list>  
#include<numeric>
#include<queue>
#include<set>
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
	Node(int a, double b) :u(a), w(b) {}
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
double distance_node(Mat &point1, Mat &point2)
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

void Dijstra(vector<list<Node>> &GraphAdj, int s)   //最短路径dijstra算法，s为源节点
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
	if (Path[k].pre == k) { cout << k; return; }
	Traverse(Path[k].pre);
	cout << " " << k;
}
void Print(int s, int n)             //输出最短路径，s为源节点，n为总的节点数
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

double cal_cost(Mat &pt, vector<list<Node>> &GraphAdj, int n, int m)
{
<<<<<<< HEAD
	Mat pt1, pt2, pt3;
=======
	Mat pt1, pt2,pt3;
>>>>>>> origin/master
	double distance_1 = 0.0;
	double distance_2 = 0.0;
	double Sum_distance = 0.0;
	for (auto iter = GraphAdj[m].begin(); iter != GraphAdj[m].end(); iter++)
	{
		if (iter->u != n)
		{
<<<<<<< HEAD

			pt.row(n).copyTo(pt1);
			pt.row(iter->u).copyTo(pt2);
			pt.row(m).copyTo(pt3);
			distance_1 = distance_node(pt1, pt3) + iter->w;  //n与m相连的距离+m与m的连接点相连的距离
=======
			
			pt.row(n).copyTo(pt1);
			pt.row(iter->u).copyTo(pt2);
			pt.row(m).copyTo(pt3);
			distance_1 = distance_node(pt1,pt3)+ iter->w;  //n与m相连的距离+m与m的连接点相连的距离
>>>>>>> origin/master
			distance_2 = distance_node(pt1, pt2);          //n与m的连接点直接连的距离
			Sum_distance += (distance_2 - distance_1)*(distance_2 - distance_1);
		}
	}
	return Sum_distance;
}
void Decrease_node(Mat &pt, vector<list<Node>> &GraphAdj, vector<Cost_Distance> &min_heap)
{
	Mat pt1, pt2;
	vector<int> index1;
	vector<int> index2;
	vector<list<Node>> result;
	index1.assign(GraphAdj.size(), 0); //要删除点的序号
	index2.assign(GraphAdj.size(), 0); //要保留点的序号
	double distance;
	double dis_cost;
	double dis_cost1;
<<<<<<< HEAD
	cout << min_heap.size() << endl;
	for (int i = 0;i<500; i++)
	{
		int t = min_heap[0].node_i;   //要保留的点的序号
		int k = min_heap[0].node_j;   //要删除的点的序号
=======
	for (int i = 0; i < 200; i++)
	{
		int t = min_heap[0].node_i;   //要保留的点
		int k = min_heap[0].node_j;   //要删除的点
		//if (index1[k] == 1)
		//{
		//	min_heap.erase(min_heap.begin());
		//	continue;
		//}
>>>>>>> origin/master
		index1[k] = 1;
		index2[t] = 1;
		index2[k] = 0;
		pt.row(t).copyTo(pt2);
		bool flag = true;
<<<<<<< HEAD
		/*for (auto iter = GraphAdj[t].begin(); iter != GraphAdj[t].end(); iter++)
=======
		for (auto iter = GraphAdj[t].begin(); iter != GraphAdj[t].end(); iter++)
>>>>>>> origin/master
		{
			if (iter->u == k)
			{
				iter = GraphAdj[t].erase(iter);
				break;
			}
<<<<<<< HEAD
		}*/
		for (auto it = min_heap.begin(); it != min_heap.end(); )
		{
			if (it->node_j == k || it->node_i == k)
=======
		}
		for (auto it = min_heap.begin(); it != min_heap.end(); )
		{
			if (it->node_j == k||it->node_i==k)
>>>>>>> origin/master
			{
				it = min_heap.erase(it);
				continue;
			}
<<<<<<< HEAD
			else
			{
				it++;
			}
		}
		/*if (min_heap.size() == 0)
		{
			cout << i << endl;
			break;
		}*/
		for (auto iter = GraphAdj[k].begin(); iter != GraphAdj[k].end(); )
		{
			pt.row(iter->u).copyTo(pt1);
			distance = distance_node(pt1, pt2);
			flag = true;
			for (auto iter1 = GraphAdj[t].begin(); iter1 != GraphAdj[t].end(); iter1++)   //防止重复插入
			{
				
				if (iter1->u == iter->u || iter->u == t)
				{
					flag = false;
					break;
				}
			}
			for (auto iter3 = GraphAdj[iter->u].begin(); iter3 != GraphAdj[iter->u].end(); iter3++)
			{
				if (iter3->u == k)
				{
					iter3 = GraphAdj[iter->u].erase(iter3);
					break;
=======
			it++;
		}
		for (auto iter = GraphAdj[k].begin(); iter != GraphAdj[k].end(); iter++)
		{
			pt.row(iter->u).copyTo(pt1);
			distance = distance_node(pt1, pt2);
			
			for (auto iter1 = GraphAdj[t].begin(); iter1 != GraphAdj[t].end(); iter1++)   //防止重复插入
			{
				flag = true;
				if (iter1->u == iter->u||iter->u==t)
				{
					flag = false;
					break;
>>>>>>> origin/master
				}
			}
			if (flag == true)
			{
				GraphAdj[t].push_back(Node(iter->u, distance));
				GraphAdj[iter->u].push_back(Node(t, distance));
<<<<<<< HEAD
				dis_cost = cal_cost(pt, GraphAdj, t, iter->u);
				dis_cost1 = cal_cost(pt, GraphAdj, iter->u, t);
				for (auto iter2 = min_heap.begin(); iter2 != min_heap.end(); iter2++)
				{
					if (min_heap.size() == 0)
					{
						iter2 = min_heap.insert(iter2, Cost_Distance(t, iter->u, dis_cost));
						break;
					}
=======
				dis_cost = cal_cost(pt, GraphAdj,t,iter->u);
				dis_cost1 = cal_cost(pt, GraphAdj, iter->u, t);
				for (auto iter2 = min_heap.begin(); iter2 != min_heap.end(); iter2++)
				{
					
>>>>>>> origin/master
					if (iter2->cost_distance <= dis_cost)
						continue;
					else
					{
						iter2 = min_heap.insert(iter2, Cost_Distance(t, iter->u, dis_cost));
						break;
					}
				}
				for (auto iter2 = min_heap.begin(); iter2 != min_heap.end(); iter2++)
				{
<<<<<<< HEAD
					if (min_heap.size() == 0)
					{
						iter2 = min_heap.insert(iter2, Cost_Distance(t, iter->u, dis_cost));
						break;
					}
=======

>>>>>>> origin/master
					if (iter2->cost_distance <= dis_cost1)
						continue;
					else
					{
						iter2 = min_heap.insert(iter2, Cost_Distance(iter->u, t, dis_cost1));
						break;
					}
				}
<<<<<<< HEAD
				iter = GraphAdj[k].erase(iter);
				continue;
=======
>>>>>>> origin/master
			}
			iter++;
		}
<<<<<<< HEAD
	/*	if (accumulate(index2.begin(), index2.end(), 0) == 92)
		{
			cout << i << endl;
			break;
		}*/
=======
>>>>>>> origin/master
	}
	cout << min_heap.size() << endl;	
	/*for (int i = 0; i < GraphAdj.size(); i++)
	{
		if (index1[i] == 1)
		{
			index2[i] = 0;
		}
<<<<<<< HEAD
	}*/
	cout << accumulate(index2.begin(), index2.end(), 0) << endl;
	/*for (int i = 0, j = 0; i < GraphAdj.size(); i++)
=======
	}
	for (int i = 0,j=0; i < GraphAdj.size(); i++)
>>>>>>> origin/master
	{
		if (index2[i] == 1)
		{
			cout << i << " : ";
			result.push_back(GraphAdj[i]);
			for (auto iter = result[j].begin(); iter != result[j].end(); iter++)
			{
				if (iter->u != NULL)
				{
					cout << iter->u << " ";
				}
			}
			cout << endl;
			j = j + 1;
<<<<<<< HEAD
		}
	}*/
	//cout << min_heap.size() << endl;
}

void Heapadjust(vector<Cost_Distance> &heap, int currentPos, int end)//建立最小堆
{
	int i = currentPos, j = 2 * i;
	Cost_Distance temp = heap[i];
	while (j <= end)
	{
		if (j < end&&heap[j].cost_distance > heap[j + 1].cost_distance)
			++j;
		if (temp.cost_distance <= heap[j].cost_distance)
			break;
		heap[i] = heap[j];
		i = j;
		j = j * 2;
	}
	heap[i] = temp;
}
void swap(vector<Cost_Distance> &heap, int i, int j)// 输出最小堆时使用的交换程序
{
	Cost_Distance temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;
}
void Create_minheap(Mat &pt, vector<list<Node>> &GraphAdj, vector<Cost_Distance> &heap_final)//建立最小堆
{
	Mat pt1, pt2;
	vector<Cost_Distance> heap;
	heap.push_back(Cost_Distance(0, 0, 0));
	for (int i = 0; i < GraphAdj.size(); i++)
	{

		double distance_1 = 0.0;    //i与j联通的点的间接距离
		double distance_2 = 0.0;    //i与j联通的点的直接距离
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
			heap.push_back(Cost_Distance(i, iter->u, Sum_distance));
		}
	}
	int k = heap.size() - 1;
	int currentPos = k / 2;
	while (currentPos > 0)
	{
		Heapadjust(heap, currentPos, k);
		--currentPos;
	}
	for (int i = k; i>0; i--)
	{
		swap(heap, 1, i);
		heap_final.push_back(heap[i]);
		Heapadjust(heap, 1, i - 1);
	}
	//for (int i = 0; i < 5; i++)
	//{
	//	cout << heap_final[i].node_i << " " << heap_final[i].node_j << " " << heap_final[i].cost_distance;
	//	cout << endl;
	//}
	//cout << heap_final.size()<<endl;
}
void Create_Min_Cost(Mat &pt, vector<list<Node>> &GraphAdj, min_Cost &min_heap)//建立优先级队列
{
	Mat pt1, pt2;
=======
		}
	}
	cout << min_heap.size() << endl;
}

void Heapadjust(vector<Cost_Distance> &heap, int currentPos,int end)//建立最小堆
{
	int i = currentPos, j = 2 * i;
	Cost_Distance temp = heap[i];
	while (j <= end)
	{
		if (j < end&&heap[j].cost_distance > heap[j+1].cost_distance)
			++j;
		if (temp.cost_distance <= heap[j].cost_distance)
			break;
        heap[i] = heap[j];
		i = j;
		j = j * 2;
	}
	heap[i] = temp;
}
void swap(vector<Cost_Distance> &heap, int i, int j)// 输出最小堆时使用的交换程序
{
	Cost_Distance temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;
}
void Create_minheap(Mat &pt, vector<list<Node>> &GraphAdj,vector<Cost_Distance> &heap_final)//建立最小堆
{
	Mat pt1, pt2;
	vector<Cost_Distance> heap;
	heap.push_back(Cost_Distance(0,0,0));
>>>>>>> origin/master
	for (int i = 0; i < GraphAdj.size(); i++)
	{

		double distance_1 = 0.0;    //i与j联通的点的间接距离
<<<<<<< HEAD
		double distance_2 = 0.0;     //i与j联通的点的直接距离
=======
		double distance_2 = 0.0;    //i与j联通的点的直接距离
>>>>>>> origin/master
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
<<<<<<< HEAD
			min_heap.push(Cost_Distance(i, iter->u, Sum_distance));
		}
=======
			heap.push_back(Cost_Distance(i, iter->u, Sum_distance));
		}
	}
	int k = heap.size()-1;
	int currentPos = k / 2;
	while (currentPos > 0)
	{
		Heapadjust(heap, currentPos, k);
		--currentPos;
	}
	for (int i = k; i>0; i--)
	{	
		swap(heap, 1, i);
		heap_final.push_back(heap[i]);
		Heapadjust(heap, 1, i - 1);
	}
	//for (int i = 0; i < 5; i++)
	//{
	//	cout << heap_final[i].node_i << " " << heap_final[i].node_j << " " << heap_final[i].cost_distance;
	//	cout << endl;
	//}
	//cout << heap_final.size()<<endl;
}
void Create_Min_Cost(Mat &pt, vector<list<Node>> &GraphAdj, min_Cost &min_heap)//建立优先级队列
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
>>>>>>> origin/master
	}
}
int main()
{

	string infileImageTrain("C:\\Users\\lab309\\Desktop\\MINIST\\train-images.idx3-ubyte");
	string infileLabTrain("C:\\Users\\lab309\\Desktop\\MINIST\\train-labels.idx1-ubyte");
	Mat digitsUcharTrain;
	Mat lableIntTrain;
	MNIST mmt;
	mmt.loadDigits(infileImageTrain, digitsUcharTrain);
	mmt.loadLabels(infileLabTrain, lableIntTrain);

	Mat digitsImageTrain;

	digitsUcharTrain.convertTo(digitsImageTrain, CV_64FC1);

	string infileImageTest("C:\\Users\\lab309\\Desktop\\MINIST\\t10k-images.idx3-ubyte");
	string infileLabTest("C:\\Users\\lab309\\Desktop\\MINIST\\t10k-labels.idx1-ubyte");

	Mat digitsUcharTest;
	Mat lableIntTest;
	mmt.loadDigits(infileImageTest, digitsUcharTest);
	mmt.loadLabels(infileLabTest, lableIntTest);
	Mat digitsImageTest;

	digitsUcharTest.convertTo(digitsImageTest, CV_64FC1);
	string file_name = "C:\\Users\\lab309\\Desktop\\MINIST\\pcaValue6000.xml";
	//PCA pca(digitsImageTrain, cv::Mat(), CV_PCA_DATA_AS_ROW, low_dimension);
	//savePCA(file_name.c_str(), pca);
	PCA pca;
	loadPCA(file_name.c_str(), pca);
	Mat point = pca.project(digitsImageTrain);
	//Mat pt1, pt2;
	//point.row(0).copyTo(pt1);
	//point.row(49).copyTo(pt2);
	//double res = distance_node(pt1, pt2);
	ifstream test("C:\\Users\\lab309\\Desktop\\MINIST\\knneartest600", ifstream::in);
	ofstream result("C:\\Users\\lab309\\Desktop\\MINIST\\knneartest600-cost", ofstream::out);
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
	/*for (int i = 0; i < 50; i++){
<<<<<<< HEAD
	for (auto iter = Adj[i].begin(); iter != Adj[i].end(); iter++)
	cout <<iter->u <<" ";
	cout << endl;
	}*/
	vector<Cost_Distance> heap_final;
	Create_minheap(point, Adj, heap_final);
	//	min_Cost min_heap;
	//	Create_Min_Cost(point, Adj,min_heap);
	Decrease_node(point, Adj, heap_final);
	//	cout << min_heap.size() << endl;
	/*for (int i = 0; i < min_heap.size(); i++)
	{
	result << min_heap.top().node_i << " " << min_heap.top().node_j << " " << min_heap.top().cost_distance;
	result << endl;
	min_heap.pop();
	}*/
	/*for (int i = 0; i < 5; i++)
=======
		for (auto iter = Adj[i].begin(); iter != Adj[i].end(); iter++)    
			cout <<iter->u <<" ";
		cout << endl;
	}*/
	vector<Cost_Distance> heap_final;
	Create_minheap(point, Adj,heap_final);
//	min_Cost min_heap;
//	Create_Min_Cost(point, Adj,min_heap);
	Decrease_node(point, Adj, heap_final);
//	cout << min_heap.size() << endl;
	/*for (int i = 0; i < min_heap.size(); i++)
>>>>>>> origin/master
	{
	cout << min_heap.top().node_i << " " << min_heap.top().node_j << " " << min_heap.top().cost_distance;
	cout << endl;
	min_heap.pop();
	}*/
	/*for (int i = 0; i < 5; i++)
	{
		cout << min_heap.top().node_i << " " << min_heap.top().node_j << " " << min_heap.top().cost_distance;
		cout << endl;
		min_heap.pop();
	}*/
	//Dijstra(Adj, 0);
	//Print(0, Adj.size());
	system("pause");
	return 0;
}					