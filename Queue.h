#ifndef QUEUE_H
#define QUEUE_H
#include <vtkType.h>
#include <queue>
#include <map>
#include <vector>
#include <time.h>

struct MyData
{
	int PointId;
	double Distance;
	MyData(){
		PointId = -1;
		Distance = VTK_DOUBLE_MAX;
	}

	MyData(int pId, double dist){
		PointId = pId;
		Distance = dist;
	}

	//bool operator < (const MyData& mydata)const
	//{
	//	return Distance < mydata.Distance ? false : true;
	//}
};

//class cmp
//{
//public:
//	bool operator()( const MyData & n1, const MyData & n2) const
//	{
//		return n1 < n2;
//	}
// };

struct Queue
{
	std::queue<int> queue;
	std::map<int, bool> IsExist;
	//std::priority_queue<int, std::vector<MyData>,cmp> queue2;
	Queue(){};
	void push(int tmp)
	{
		if(!IsExist[tmp])
		{
			bool flag = true;
			IsExist[tmp] = flag;
			queue.push(tmp);
		}
	}

	int pop()
	{
		int id = queue.front();
		queue.pop();
		bool flag = false;
		IsExist[id] = flag;
		return id;
	}

	bool empty()
	{
		return queue.empty();
	}


};

#endif