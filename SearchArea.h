#pragma once
#include<graphics.h>
#include<iostream>
#include<array>
typedef std::array<int, 2> Vect2;
typedef std::array<int, 8> Cld_List;
const int redundant = 50; //留白边

template<size_t SIZE_obs>
class map
{
public:
	int map_x;//横排格子数
	int map_y;//纵排格子数
	int map_cell_width;
	std::array<Vect2,SIZE_obs> map_obs; //障碍物坐标
	Vect2 map_init;
	Vect2 map_end;
	int map_startx;//开始画格子的点，每个点在格子的左上角
	int map_starty;
	int map_lenx;
	int map_leny;
	

public:
	map() = default;
	map(int x, int y, int width, std::array<Vect2, SIZE_obs>& obs, 
		Vect2&init, Vect2&end)
		:map_x(x), map_y(y), map_cell_width(width), map_obs(obs), map_init(init), map_end(end)
	{
		map_lenx = map_x * map_cell_width;
		map_leny = map_y * map_cell_width;
		map_startx = redundant;
		map_starty = redundant;
		
		DrawMap();
		DrawObs();//画障碍物
		DrawIE();//画起终点
	}
	int Node_Num()
	{
		return map_x * map_y;
	}
	bool Is_InObs(Vect2& temp)
	{
		for (auto iter = map_obs.begin(); iter != map_obs.end(); iter++)
		{
			if (*iter==temp)
				return true;
		}
			return false;

	}
	
private:
	void DrawRect(Vect2 &crd)//作正方形输出
	{
		
		fillrectangle( map_startx + map_cell_width * crd[0], map_starty + map_cell_width * crd[1],
			map_startx + map_cell_width * (crd[0] + 1), map_starty + map_cell_width * (crd[1] + 1) );
		
	}
	void DrawMap()
	{
		std::cout << "创建了长宽为 " << map_lenx << " * " << map_leny
			<< ", 方格边长为：" << map_cell_width << " 的地图" << std::endl;

		
		initgraph(map_lenx + redundant * 2, map_leny + redundant * 2, SHOWCONSOLE);
		setbkcolor(WHITE);
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID,1);
		setfillcolor(YELLOW);
		cleardevice();
		//BeginBatchDraw();
		for (int i = 0; i < map_x+1; i++)
		{
			line(map_lenx + map_startx, map_starty + i* map_cell_width,
				 map_startx, map_starty + i * map_cell_width);
		}
		for (int j = 0; j < map_y+1; j++)
		{
			line(map_starty + j * map_cell_width, map_leny + map_starty,
				 map_startx + j * map_cell_width, map_starty);
		}

		//EndBatchDraw();
		
	}
	void DrawObs() 
	{
		setfillcolor(YELLOW);
		for (int i = 0; i < SIZE_obs; i++)
		{
			DrawRect(map_obs[i]);
		}
		
	}
	void DrawIE()
	{
		setfillcolor(BLUE);
		DrawRect(map_init);
		setfillcolor(RED);
		DrawRect(map_end);
	}

};
	




