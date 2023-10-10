#pragma once
#include"SearchArea.h"
#include<tuple>
#include<math.h>


template <size_t SIZE>//为了传入一定量的字符串使用了太多模板
void DrawRect(Vect2 &crd, map<SIZE>&m)//作正方形输出
{

	fillrectangle(m.map_startx + m.map_cell_width * crd[0], m.map_starty + m.map_cell_width * crd[1],
		m.map_startx + m.map_cell_width * (crd[0] + 1), m.map_starty + m.map_cell_width * (crd[1] + 1));

}
class Node
{
public:
	int Num;//编号 0起点，1终点，-1 不存在点的编号
	Vect2 Coord;//xy坐标
	int Father;
	Cld_List Child;//存编号
	int Cost_f;
	int Cost_g;
	int Cost_h;
	bool IsinOpenlist;
	bool IsinCloselist;
public:
	Node()
		:Num(-1) {}
	Node(int Num, Vect2& Crd, int father)
		:Num(Num), Coord(Crd), Father(father), IsinOpenlist(true),
		IsinCloselist(false), Cost_f(0), Cost_g(0), Cost_h(0)  
	{
		Cld_List::iterator arr_iter;
		for (arr_iter = Child.begin(); arr_iter != Child.end() ; arr_iter++)
			*arr_iter = -1;
	}
	
	//存Child的array已用到的位置返回
	int Used_ChildNum ()
	{
		int i;
		Cld_List::iterator iter;
		for (iter = Child.begin(),i=0; iter != Child.end() ;i++, iter++)
		{
			if (*iter != -1)
				continue;
			else
				break;
		}
		if (i == 8)
			return -1;
		else return i;
	}
	std::tuple< int,bool> IsinChild(int num)
	{
		int j;
		Cld_List::iterator arr_iter;
		for (arr_iter = Child.begin(),j=0; arr_iter != Child.end() ; j++,arr_iter++)
		{
			if (*arr_iter == num)
				return std::make_tuple(j, true);
		}
		return std::make_tuple(-1, false);
	}
	
};
template <size_t SIZE>
bool Boundary_Check(Vect2& temp,map<SIZE>& m)//检查节点是否在库
{
	if ((m.map_x) >=temp[0] && temp[0] >= 0&&temp[1] >= 0&&(m.map_y) >temp[1])
		return true;
	else
		return false;

}
template <size_t SIZE_Lib>
bool repeat_Check(Vect2& temp, std::array<Node, SIZE_Lib> &Node_Lib)
{
	for (auto iter = Node_Lib.begin(); iter != Node_Lib.end(); iter++)
	{
		if (iter->Coord==temp)
		return true;
	}

	return false;
}


std::tuple<int,int,int> Calcu_Cost(Node& node, Node& father, int width, Vect2& end)
{
	node.Cost_g = width*sqrt(pow((node.Coord[0] - father.Coord[0]),2)+ pow((node.Coord[1] - father.Coord[1]),2))+father.Cost_g;
	node.Cost_h = width*sqrt(pow((node.Coord[0] - end[0]),2) + pow((node.Coord[1] - end[1]),2));
	node.Cost_f = node.Cost_g + node.Cost_h;
	return std::make_tuple(node.Cost_f, node.Cost_g,node.Cost_h);
}
template<size_t SIZE>
Node Min_Cost_Child(std::array<Node, SIZE>& Node_Lib, Node & father)
{

	Node temp=Node_Lib[father.Child[0]];
	for (auto& i : father.Child)
	{
		temp.Father = father.Num;
		if(i!=-1)
		{ 
			
		if (temp.Cost_f>Node_Lib[i].Cost_f)
			temp=Node_Lib[i];
		}
		
	}
	return temp;
}
Vect2 Add(Vect2 a, Vect2 b)
{
	Vect2 c;
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	return c;
}

template<size_t SIZE_M, size_t SIZE_Lib>

std::tuple<Node, std::array<Node, SIZE_Lib>&> Search_Child(Node &father, std::array<Node, SIZE_Lib> &Node_Lib, map<SIZE_M> &m)
{
	std::array<Vect2, 8> Rel_Crd{};//八个可能的子节点位置
	Rel_Crd[0] = { -1,-1 };
	Rel_Crd[1] = { -1,0 };
	Rel_Crd[2] = { -1,1 };
	Rel_Crd[3] = { 0,-1 };
	Rel_Crd[4] = { 0,1 };
	Rel_Crd[5] = { 1,-1 };
	Rel_Crd[6] = { 1,0 };
	Rel_Crd[7] = { 1,1 };
	std::array<Vect2, 8>::iterator RC_iter;
	//size_t sz = static_cast <size_t>(m.Node_Num());
	//std::array<Node, sz>::iterator Lib_iter;
	
	int i,j;
	bool Is_Child = false , Is_Newnode = true;
	for (RC_iter = Rel_Crd.begin(), i = 0; RC_iter != Rel_Crd.end(); i++, RC_iter++)
	{
		Vect2& temp = *RC_iter;
		temp = Add(temp, father.Coord);
		Is_Newnode = true;
 		for (auto Lib_iter = Node_Lib.begin(); Lib_iter!= Node_Lib.end(); Lib_iter++)
		{
			//1.是否有已经搜索过的节点 是否被打入closelist和障碍识别
			if (Lib_iter->Num == -1)//排除未使用的lib节点
				break;
			else if (repeat_Check(temp, Node_Lib)&&Lib_iter->Coord==temp)//重复检测和节点坐标匹配
			{
				Is_Newnode = false;
				std::tie(j, Is_Child) = father.IsinChild(Lib_iter->Num);
				if (Lib_iter->IsinCloselist == false && Is_Child == true)
				{
					//Is_Newnode = false; 
					Lib_iter->Father = father.Num;
					std::tie(Lib_iter->Cost_f, Lib_iter->Cost_g, Lib_iter->Cost_h) = Calcu_Cost(*Lib_iter, father, m.map_cell_width, m.map_end);
					
				}
				else if (Lib_iter->IsinCloselist == false && Is_Child == false)
				{
					
					//Is_Newnode = false;
					Lib_iter->Father = father.Num;
					std::tie(Lib_iter->Cost_f, Lib_iter->Cost_g, Lib_iter->Cost_h) = Calcu_Cost(*Lib_iter, father, m.map_cell_width, m.map_end);
					j = father.Used_ChildNum();
					if (j != -1)
 						father.Child[j] = Lib_iter->Num;//记录编号到Node
					else
					{
						std::cout << "Child的坑位没了?搞啥呢！" << std::endl;
						__debugbreak;
					}
				}
				else
					continue;
			}

		}
			//2.若有新节点建立，需要进行边界检查，判断是否超出边界	
		if (Is_Newnode&&!m.Is_InObs(temp))//新节点是否需要创建命令和节点是否为障碍物
		{
			if(Boundary_Check(temp,m))//边界检查，是否新节点超出边界
			{ 
			j = 0;
			for (auto Lib_iter = Node_Lib.begin(); Lib_iter != Node_Lib.end(); Lib_iter++, j++) 
			{
				if (Lib_iter->Num == -1)
					break;
			}
			
			Node Child(j, temp, father.Num);
			std::tie(Child.Cost_f, Child.Cost_g, Child.Cost_h) = Calcu_Cost(Child, father, m.map_cell_width, m.map_end);
			Node_Lib[j] = Child;
			j = father.Used_ChildNum();
			if (j != -1)
				father.Child[j] = Child.Num;//记录编号到Node
			else
			{
				std::cout << "Child的坑位没了?搞啥呢！" << std::endl;
				__debugbreak;
			}
			}
		}
	}

	//3.f值计算最小者作为返回值之一,并且此节点为下一搜索父节点
	Node Next_Father = Min_Cost_Child(Node_Lib, father);
	
	Node_Lib[Next_Father.Num].IsinCloselist = true;//f值最小的全进Closelist
	Node_Lib[Next_Father.Num].IsinOpenlist = false;
	
	Next_Father=Node_Lib[Next_Father.Num];
	Node_Lib[father.Num] = father;
	setfillcolor(GREEN);
	DrawRect(Next_Father.Coord, m);

	return std::tie(Next_Father, Node_Lib);
}


