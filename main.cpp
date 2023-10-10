#include"SearchArea.h"
#include"A_star.h"

int main()
{
	//地图参数
	const int x = 25;
	const int y = 25;
	const int width = 20;
	const int obs_num = 10;//障碍物占格子数

	//指定起终点
	Vect2 init{1,22};//起点
	Vect2 end{20,20};//终点
	std::array<Node, x*y> Node_Lib{};//节点库
	Node init_Node(1, init, -1);
	Node end_Node(0, end, -1);
	init_Node.IsinCloselist = true;
	init_Node.IsinOpenlist = false;
	Node_Lib[1] = init_Node;
	Node_Lib[0] = end_Node;

	//指定障碍物位置 positions of obstacles
	std::array<Vect2, obs_num> obs{};//障碍物位置坐标,初始化为0
	for (int i = 0; i < obs_num; i++) 
	{
		/*if (i < obs_num / 2) {
			obs[i] = { 15+i,i };
			Node_Lib[i + 2] = Node(i + 2, obs[i], -1);
			
		}
		else */{
			obs[i] = { 10,i+15 };
			Node_Lib[i + 2] = Node(i + 2, obs[i], -1);
		}
		Node_Lib[i + 2].IsinCloselist = true;
		Node_Lib[i + 2].IsinOpenlist = false;//障碍物优先被打入closelist
	}
	map<obs_num> m(x,y, width,obs,init,end) ;//画地图
	std::tie(init_Node.Cost_f, init_Node.Cost_g, init_Node.Cost_h) = Calcu_Cost(init_Node,end_Node,m.map_cell_width,end);
	init_Node.Cost_g = 0;
	init_Node.Cost_f = init_Node.Cost_g + init_Node.Cost_h;
	Node_Lib[1] = init_Node;
	
	//A*算法
	Node temp=init_Node;//搜索当前节点的子节点
	while (1)
	{
		//子节点识别(越界)
		std::tie(temp,Node_Lib)=Search_Child(temp,Node_Lib,m);
		end_Node = Node_Lib[0];
		if (end_Node.Num==temp.Num)
		{ 
			while (!(temp.Num == init_Node.Num))
			{
				setfillcolor(BLUE);
				DrawRect(temp.Coord,m);
				temp = Node_Lib[temp.Father];
			}
			
			break;
		}
		
	}

	getchar();
	closegraph();
}