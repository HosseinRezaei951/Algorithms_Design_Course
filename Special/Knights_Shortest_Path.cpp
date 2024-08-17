//In the name of GOD
#include <conio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
using namespace std;

struct Cell
{
	int x, y;
	
	Cell(int x_pos, int y_pos)
	{
		x = x_pos;
		y = y_pos;
	}
};

class KSP
{
private:
	int** Chess_map;
	vector<vector<Cell>> Knight_movse;

public:
	KSP();
	~KSP();

	bool KSP_Walk(Cell,int, int, int, int);
	bool Move_checker(int, int);
	bool Repetitive_Move(int, int, vector<Cell>&);
	void All_NextMoves(int, int, vector<Cell>&);
	void Add_Move(int, int, vector<Cell>&);


	// ###########################################################
	void Show_Moves();
};

void KSP::Show_Moves()
{
	cout << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << i + 1 << "): ";
		for (int j = 0; j < Knight_movse[i].size(); j++)
		{
			cout << "(" << Knight_movse[i].at(j).x << "," << Knight_movse[i].at(j).y << ") ";
		}
		cout << "\n\n";
	}
}

bool KSP::KSP_Walk(Cell Start,int x, int y,int X_end, int Y_end)
{
	/*if (Move_checker(x, y) == false)
	{
		return false;
	}*/
	cout /*<< endl */<< "(" << x << "," << y << ") ";
	/*Show_Moves();*/
	_getch();
	if (x == X_end && y == Y_end)
	{
		return true;
	}
	
	Cell _tmp_cell(x, y);
	if (Move_checker(x + 2, y + 1) == true && KSP_Walk(Start, x + 2 , y + 1, X_end, Y_end) == true && Repetitive_Move(x + 2, y + 1, Knight_movse[0]) == false)
	{
		_tmp_cell.x = x + 2;
		_tmp_cell.y = y + 1;
		Knight_movse[0].push_back(_tmp_cell);
	}
	
	if (Move_checker(x + 1, y - 2) == true && KSP_Walk(Start, x + 1, y - 2, X_end, Y_end) == true && Repetitive_Move(x + 1, y - 2, Knight_movse[1]) == false)
	{
		_tmp_cell.x = x + 1;
		_tmp_cell.y = y - 2;
		Knight_movse[1].push_back(_tmp_cell);
	}

	if (Move_checker(x - 2, y - 1) == true && KSP_Walk(Start, x - 2, y - 1, X_end, Y_end) == true && Repetitive_Move(x - 2, y - 1, Knight_movse[2]) == false)
	{
		_tmp_cell.x = x - 2;
		_tmp_cell.y = y - 1;
		Knight_movse[2].push_back(_tmp_cell);
	}

	if (Move_checker(x - 1, y + 2) == true && KSP_Walk(Start, x - 1, y + 2, X_end, Y_end) == true && Repetitive_Move(x - 1, y + 2, Knight_movse[3]) == false)
	{
		_tmp_cell.x = x - 1;
		_tmp_cell.y = y + 2;
		Knight_movse[3].push_back(_tmp_cell);
	}

	if (Move_checker(x + 1, y + 2) == true && KSP_Walk(Start, x + 1, y + 2, X_end, Y_end) == true && Repetitive_Move(x + 1, y + 2,Knight_movse[4]) == false)
	{
		_tmp_cell.x = x + 1;
		_tmp_cell.y = y + 2;
		Knight_movse[4].push_back(_tmp_cell);
	}

	if (Move_checker(x + 2, y - 1) == true && KSP_Walk(Start, x + 2, y - 1, X_end, Y_end) == true && Repetitive_Move(x + 2, y - 1, Knight_movse[5]) == false)
	{
		_tmp_cell.x = x + 2;
		_tmp_cell.y = y - 1;
		Knight_movse[5].push_back(_tmp_cell);
	}

	if (Move_checker(x - 1, y - 2) == true && KSP_Walk(Start, x - 1, y - 2, X_end, Y_end) == true && Repetitive_Move(x - 1, y - 2, Knight_movse[6]) == false)
	{
		_tmp_cell.x = x - 1;
		_tmp_cell.y = y - 2;
		Knight_movse[6].push_back(_tmp_cell);
	}

	if (Move_checker(x - 2, y + 1) == true && KSP_Walk(Start, x - 2, y + 1, X_end, Y_end) == true && Repetitive_Move(x - 2, y + 1,Knight_movse[7]) == false)
	{
		_tmp_cell.x = x - 2;
		_tmp_cell.y = y + 1;
		Knight_movse[7].push_back(_tmp_cell);
	}
	return false;
}

bool KSP::Move_checker(int x, int y)
{
	if (x > 8 || y > 8 || x < 1 || y < 1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool KSP::Repetitive_Move(int x, int y, vector<Cell> &_array)
{
	for (int i = 0; i < _array.size(); i++)
	{
		if (_array[i].x = x && _array[i].y == y)
		{
			return true;
		}
	}
	return false;
}

//void KSP::Add_Move(int x, int y, vector<Cell> &_array)
//{
//	Cell _tmp_cell;
//	if (Move_checker(x, y) == true && Repetitive_Move(x, y, _array) == false)
//	{
//		_tmp_cell.x = x;
//		_tmp_cell.y = y;
//		_array.push_back(_tmp_cell);
//		return;
//	}
//	else
//	{
//		return;
//	}
//}

//void KSP::All_NextMoves(int x, int y, vector<Cell> &_array)
//{
//	//1:
//	Add_Move(x + 2, y + 1, _array);
//
//	//2:
//	Add_Move(x + 1, y - 2, _array);
//	
//	//3: 
//	Add_Move(x - 2, y - 1, _array);
//	
//	//4:
//	Add_Move(x - 1, y + 2, _array);
//
//	//5:
//	Add_Move(x + 1, y + 2, _array);
//
//	//6:
//	Add_Move(x + 2, y - 1, _array);
//
//	//7:
//	Add_Move(x - 1, y - 2, _array);
//
//	//8:
//	Add_Move(x - 2, y + 1, _array);
//
//}

KSP::KSP()
{
	Chess_map = new int*[9];
	for (int i = 0; i < 9; i++)
	{
		Chess_map[i] = new int[9];
	}
	Knight_movse.resize(8);
}

KSP::~KSP()
{
}




int main()
{
	KSP Chess_Question;
	int x, y, X_end, Y_end;
	
	cout << " Plz enter the position of start cell:" << endl;
	cout << "X: ";
	cin >> x;
	cout << "Y: ";
	cin >> y;
	Cell Start(x,y);
	
	cout << " Now!, enter the position of the finish cell: " << endl;
	cout << "X: ";
	cin >> X_end;
	cout << "Y: ";
	cin >> Y_end;
	Chess_Question.KSP_Walk(Start, x, y, X_end, Y_end);
	Chess_Question.Show_Moves();


	_getch();
	return 0;
}