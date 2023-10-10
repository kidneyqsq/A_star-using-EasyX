#include"SearchArea.h"
#include"A_star.h"

int main()
{
	//��ͼ����
	const int x = 25;
	const int y = 25;
	const int width = 20;
	const int obs_num = 10;//�ϰ���ռ������

	//ָ�����յ�
	Vect2 init{1,22};//���
	Vect2 end{20,20};//�յ�
	std::array<Node, x*y> Node_Lib{};//�ڵ��
	Node init_Node(1, init, -1);
	Node end_Node(0, end, -1);
	init_Node.IsinCloselist = true;
	init_Node.IsinOpenlist = false;
	Node_Lib[1] = init_Node;
	Node_Lib[0] = end_Node;

	//ָ���ϰ���λ�� positions of obstacles
	std::array<Vect2, obs_num> obs{};//�ϰ���λ������,��ʼ��Ϊ0
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
		Node_Lib[i + 2].IsinOpenlist = false;//�ϰ������ȱ�����closelist
	}
	map<obs_num> m(x,y, width,obs,init,end) ;//����ͼ
	std::tie(init_Node.Cost_f, init_Node.Cost_g, init_Node.Cost_h) = Calcu_Cost(init_Node,end_Node,m.map_cell_width,end);
	init_Node.Cost_g = 0;
	init_Node.Cost_f = init_Node.Cost_g + init_Node.Cost_h;
	Node_Lib[1] = init_Node;
	
	//A*�㷨
	Node temp=init_Node;//������ǰ�ڵ���ӽڵ�
	while (1)
	{
		//�ӽڵ�ʶ��(Խ��)
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