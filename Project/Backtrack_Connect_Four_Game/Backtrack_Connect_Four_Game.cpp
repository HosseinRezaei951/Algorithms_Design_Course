//In the name of GOD
#include <conio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
using namespace std;

struct point
{
	int x, y;
	int right = 0, left = 0, up = 0, down = 0;
	int right_up = 0, left_up = 0, right_down = 0, left_down = 0;
};


class Game
{
	friend class PC;
private:
	int** Game_Map;
	int NumberOf_Columns = 0;
	int Max_Height = 0;
	int* Heights;

	vector<point> PL1_points; //Player
	vector<point> PL2_points; //PC 
	int Winner = 0;
	int Turn = 0;


public:
	//Initialization
	Game(int, int);
	void Start_Game();
	void Reset_Game();

	//Playing
	void Playing();

	//Ending
	void Add_Point(int, point &, vector<point> &);
	bool End_Game(int&);
	bool Up_winning(point &);
	bool RightUp_winning(point &);
	bool Right_winning(point &);
	bool RightDown_winning(point &);
	bool LeftDown_winning(point &);
	bool Left_winning(point &);
	bool LeftUp_winning(point &);

	//Handler Functions 
	bool Height_check(int);
	bool Column_check(int);
	void Show_Map();
	bool Map_isEmpty();
	void Sort_Points();
	void Change_Turn();
	void Update_Data();
	void Select(point &);


	//Logic
	void Cleaning_Drafts(int, vector<point> &);
	int Logical_Selection(point&, vector<point> &, vector<point> &);
	bool CheckingOut_TheBest_Choices(point &, vector<point> &, vector<point> &);

	
	bool Winning_Or_Disrupt_TheOpponents_Wins_By_Next_Choice(point &, vector<point> &);
	bool Filling_TheEmpty_Choice_For_Winning_Or_Disrupt_TheOpponents_Wins(point &, vector<point> &);
	bool Create_Or_Disrupt_TwoWay_Position(point &, vector<point> &);

	int Finding_Choice_WithThe_Least_Density(point &, vector<point> &);
	bool Select_TheNearest_Choice_To_LeastDensity(point &, vector<point> &, int);
	/* ################################################################### */



};

//Initialization
Game::Game(int _NumberOf_Columns, int _Max_Height)
{
	Max_Height = _Max_Height;
	NumberOf_Columns = _NumberOf_Columns;

	Game_Map = new int*[(_Max_Height + 1)];
	Heights = new int[_NumberOf_Columns + 1];

	for (int i = 0; i <= _Max_Height; i++)
	{
		Game_Map[i] = new int[(_NumberOf_Columns + 1)];
	}

	Reset_Game();
}


void Game::Start_Game()
{
	Reset_Game();
	int _turn;
	cout << "Plz enter the turn that you want to start the game[1: you  & 2: PC]: ";
	cin >> _turn;
	while (_turn != 1 && _turn != 2)
	{
		cout << "WRONG!! ... Plz enter the turn that you want to start the game[1: you  & 2: PC]: ";
		cin >> _turn;
	}
	Turn = _turn;
	Playing();
}

void Game::Reset_Game()
{
	int i, j;
	Game_Map[0][0] = 0;
	Heights[0] = 0;

	//Column number tags
	for (j = 1; j <= NumberOf_Columns; j++)
	{
		Game_Map[0][j] = j;
	}

	//Heights number tags
	for (i = 1; i <= Max_Height; i++)
	{
		Game_Map[i][0] = Max_Height - (i - 1);
	}

	//Cleaning the map
	for (i = 1; i <= Max_Height; i++)
	{
		for (j = 1; j <= NumberOf_Columns; j++)
		{
			Game_Map[i][j] = 0;
		}
	}

	//Cleaning the heights list
	for (int i = 1; i <= Max_Height; i++)
	{
		Heights[i] = 0;
	}

	//Cleaning the Players points
	if (PL1_points.empty() != true)
	{
		PL1_points.clear();
	}
	if (PL2_points.empty() != true)
	{
		PL2_points.clear();
	}

	//Finalization
	Winner = 0;
	Turn = 0;
}


//Playing
void Game::Playing()
{
	point _tmp_point;
	bool _end = false;
	int key;
	cout << endl;
	Show_Map();
	while (_end != true)
	{
		if (Turn == 1)
		{
			cout << "Player1 Turn: " << endl;
			Select(_tmp_point);
			Add_Point(Turn, _tmp_point, PL1_points);
			Sort_Points();
			Update_Data();
			Show_Map();
		}
		else
		{
			key = Logical_Selection(_tmp_point, PL2_points, PL1_points);
			while (key == 0)
			{
				cout << "!Wrong ... " << endl;
			}
			Add_Point(Turn, _tmp_point, PL2_points);
			Sort_Points();
			Update_Data();
			Show_Map();
		}

		_end = End_Game(Winner);
		Change_Turn();

	}

	cout << "\n ************ Player " << Winner << " win the game. ************" << endl;
}


//Logic
void Game::Cleaning_Drafts(int _num, vector<point> &_points)
{
	vector<point> _tmp_points;
	int i;
	for (i = 0; i < (_points.size() - _num); i++)
	{
		_tmp_points.push_back(_points[i]);
	}

	while (i <= _num)
	{
		Game_Map[_points[i].x][_points[i].y] = 0;
		Heights[_points[i].y]--;
		i++;
	}
	_points.clear();
	_points = _tmp_points;
}

int Game::Logical_Selection(point &_Point, vector<point> &_YourPoints, vector<point> &_OpponentsPoints)
{
	if (CheckingOut_TheBest_Choices(_Point, _YourPoints, _OpponentsPoints) == true)
	{
		return 1;
	}
	else
	{
		int key = Finding_Choice_WithThe_Least_Density(_Point, _YourPoints);
		if (Select_TheNearest_Choice_To_LeastDensity(_Point,_YourPoints,key) == true)
		{
			return 2;

		}
		else
		{
			return 0;
		}
	}
}

bool Game::CheckingOut_TheBest_Choices(point &_Point, vector<point> &_YourPoints, vector<point> &_OpponentsPoints)
{
	//Zero strategy: Fill_The_Center
	if (Game_Map[Max_Height][(int)(NumberOf_Columns / 2)] == 0)
	{
		_Point.x = Max_Height;
		_Point.y = (int)(NumberOf_Columns / 2);
		return true;
	}
	else if (Game_Map[Max_Height][((int)(NumberOf_Columns / 2)) + 1] == 0)
	{
		_Point.x = Max_Height;
		_Point.y = ((int)(NumberOf_Columns / 2)) + 1;
		return true;
	}

	//First strategy: Winning_TheGame_By_Next_Choice
	else if (_YourPoints.empty() == false && Winning_Or_Disrupt_TheOpponents_Wins_By_Next_Choice(_Point, _YourPoints) == true)
	{
		cout << " \t 1" << endl;
		return true;
	}
	//Second strategy: Disrupt_TheOpponents_Wins_In_The_Next_Move
	else if (_OpponentsPoints.empty() == false && Winning_Or_Disrupt_TheOpponents_Wins_By_Next_Choice(_Point, _OpponentsPoints) == true)
	{
		cout << " \t 2" << endl;
		return true;
	}
	//Third strategy: Filling_TheEmpty_Choice_For_Winning
	else if (_YourPoints.empty() == false && Filling_TheEmpty_Choice_For_Winning_Or_Disrupt_TheOpponents_Wins(_Point, _YourPoints) == true)
	{
		cout << " \t 3" << endl;
		Heights[_Point.y]++;
		return true;
	}
	//Fourth strategy: Filling_TheEmpty_Choice_For_Disrupt_TheOpponents_Wins
	else if (_OpponentsPoints.empty() == false && Filling_TheEmpty_Choice_For_Winning_Or_Disrupt_TheOpponents_Wins(_Point, _OpponentsPoints) == true)
	{
		cout << " \t 4" << endl;
		return true;
	}
	//Fifth strategy: Create_TwoWay_Position
	else if (_YourPoints.empty() == false && Create_Or_Disrupt_TwoWay_Position(_Point, _YourPoints) == true)
	{
		cout << " \t 5" << endl;
		return true;
	}
	//Sixth strategy: Disrupt_TwoWay_Position
	else if (_OpponentsPoints.empty() == false && Create_Or_Disrupt_TwoWay_Position(_Point, _OpponentsPoints) == true)
	{
		cout << " \t 6" << endl;
		return true;
	}

	return false;
}

bool Game::Winning_Or_Disrupt_TheOpponents_Wins_By_Next_Choice(point &_Point, vector<point> &_points)
{
	point _tmp_point;
	for (int i = 0; i < _points.size(); i++)
	{
		_tmp_point = _points[i];
		//Up winning
		if (_tmp_point.up == 2
			&& Height_check(_tmp_point.x - 3) == true
			&& Game_Map[_tmp_point.x - 3][_tmp_point.y] == 0)
		{
			_Point.x = _tmp_point.x - 3;
			_Point.y = _tmp_point.y;
			return true;
		}

		//Right-up winning
		else if (_tmp_point.right_up == 2
			&& Column_check(_tmp_point.y + 3) == true
			&& Height_check(_tmp_point.x - 3) == true
			&& Game_Map[_tmp_point.x - 3][_tmp_point.y + 3] == 0
			&& Heights[_tmp_point.y + 3] == Max_Height - (_tmp_point.x - 3))
		{
			_Point.x = _tmp_point.x - 3;
			_Point.y = _tmp_point.y + 3;
			return true;
		}

		//Right winning
		else if (_tmp_point.right == 2
			&& Column_check(_tmp_point.y + 3) == true
			&& Game_Map[_tmp_point.x][_tmp_point.y + 3] == 0
			&& Heights[_tmp_point.y + 3] == Max_Height - (_tmp_point.x))
		{
			_Point.x = _tmp_point.x;
			_Point.y = _tmp_point.y + 3;
			return true;
		}

		//Right-down winning
		else if (_tmp_point.right_down == 2
			&& Column_check(_tmp_point.y + 3) == true
			&& Height_check(_tmp_point.x + 3) == true
			&& Game_Map[_tmp_point.x + 3][_tmp_point.y - 3] == 0
			&& Heights[_tmp_point.y + 3] == Max_Height - (_tmp_point.x + 3))
		{
			_Point.x = _tmp_point.x + 3;
			_Point.y = _tmp_point.y + 3;
			return true;
		}

		//Left-down winning
		else if (_tmp_point.left_down == 2
			&& Column_check(_tmp_point.y - 3) == true
			&& Height_check(_tmp_point.x + 3) == true
			&& Game_Map[_tmp_point.x + 3][_tmp_point.y - 3] == 0
			&& Heights[_tmp_point.y - 3] == Max_Height - (_tmp_point.x + 3))
		{
			_Point.x = _tmp_point.x + 3;
			_Point.y = _tmp_point.y - 3;
			return true;
		}

		//Left winning
		else if (_tmp_point.left == 2
			&& Column_check(_tmp_point.y - 3) == true
			&& Game_Map[_tmp_point.x][_tmp_point.y - 3] == 0
			&& Heights[_tmp_point.y - 3] == Max_Height - (_tmp_point.x))
		{
			_Point.x = _tmp_point.x;
			_Point.y = _tmp_point.y - 3;
			return true;
		}

		//Left-up winning
		else if (_tmp_point.left_up == 2
			&& Column_check(_tmp_point.y - 3) == true
			&& Height_check(_tmp_point.x - 3) == true
			&& Game_Map[_tmp_point.x - 3][_tmp_point.y - 3] == 0
			&& Heights[_tmp_point.y - 3] == Max_Height - (_tmp_point.x - 3))
		{
			_Point.x = _tmp_point.x - 3;
			_Point.y = _tmp_point.y - 3;
			return true;
		}
	}
	return false;
}

bool Game::Filling_TheEmpty_Choice_For_Winning_Or_Disrupt_TheOpponents_Wins(point &_Point, vector<point> &_points)
{
	point _tmp_point;
	int _tmp_turn = Game_Map[_points[0].x][_points[0].y];
	for (int i = 0; i < _points.size(); i++)
	{
		_tmp_point = _points[i];
		//Right-up winning
		if (_tmp_point.right_up == 1
			&& Column_check(_tmp_point.y + 2) == true
			&& Column_check(_tmp_point.y + 3) == true
			&& Height_check(_tmp_point.x - 2) == true
			&& Height_check(_tmp_point.x - 3) == true
			&& Game_Map[_tmp_point.x - 3][_tmp_point.y + 3] == _tmp_turn
			&& Game_Map[_tmp_point.x - 2][_tmp_point.y + 2] == 0
			&& Heights[_tmp_point.y + 2] == Max_Height - (_tmp_point.x - 2))
		{
			_Point.x = _tmp_point.x - 2;
			_Point.y = _tmp_point.y + 2;
			return true;
		}

		//Right winning
		else if (_tmp_point.right == 1
			&& Column_check(_tmp_point.y + 2) == true
			&& Column_check(_tmp_point.y + 3) == true
			&& Game_Map[_tmp_point.x][_tmp_point.y + 3] == _tmp_turn
			&& Game_Map[_tmp_point.x][_tmp_point.y + 2] == 0
			&& Heights[_tmp_point.y + 2] == Max_Height - (_tmp_point.x))
		{
			_Point.x = _tmp_point.x;
			_Point.y = _tmp_point.y + 2;
			return true;
		}

		//Right-down winning
		else if (_tmp_point.right_down == 1
			&& Column_check(_tmp_point.y + 2) == true
			&& Column_check(_tmp_point.y + 3) == true
			&& Height_check(_tmp_point.x + 2) == true
			&& Height_check(_tmp_point.x + 3) == true
			&& Game_Map[_tmp_point.x + 3][_tmp_point.y + 3] == _tmp_turn
			&& Game_Map[_tmp_point.x + 2][_tmp_point.y + 2] == 0
			&& Heights[_tmp_point.y + 2] == Max_Height - (_tmp_point.x + 2))
		{
			_Point.x = _tmp_point.x + 2;
			_Point.y = _tmp_point.y + 2;
			return true;
		}

		//Left-down winning
		else if (_tmp_point.left_down == 1
			&& Column_check(_tmp_point.y - 2) == true
			&& Column_check(_tmp_point.y - 3) == true
			&& Height_check(_tmp_point.x + 2) == true
			&& Height_check(_tmp_point.x + 3) == true
			&& Game_Map[_tmp_point.x + 3][_tmp_point.y - 3] == _tmp_turn
			&& Game_Map[_tmp_point.x + 2][_tmp_point.y - 2] == 0
			&& Heights[_tmp_point.y - 2] == Max_Height - (_tmp_point.x + 2))
		{
			_Point.x = _tmp_point.x + 2;
			_Point.y = _tmp_point.y - 2;
			return true;
		}

		//Left winning
		else if (_tmp_point.left == 1
			&& Column_check(_tmp_point.y - 2) == true
			&& Column_check(_tmp_point.y - 3) == true
			&& Game_Map[_tmp_point.x][_tmp_point.y - 3] == _tmp_turn
			&& Game_Map[_tmp_point.x][_tmp_point.y - 2] == 0
			&& Heights[_tmp_point.y - 2] == Max_Height - (_tmp_point.x))
		{
			_Point.x = _tmp_point.x;
			_Point.y = _tmp_point.y - 2;
			return true;
		}

		//Left-up winning
		else if (_tmp_point.left_up == 1
			&& Column_check(_tmp_point.y - 2) == true
			&& Column_check(_tmp_point.y - 3) == true
			&& Height_check(_tmp_point.x - 2) == true
			&& Height_check(_tmp_point.x - 3) == true
			&& Game_Map[_tmp_point.x - 3][_tmp_point.y - 3] == _tmp_turn
			&& Game_Map[_tmp_point.x - 2][_tmp_point.y - 2] == 0
			&& Heights[_tmp_point.y - 2] == Max_Height - (_tmp_point.x - 2))
		{
			_Point.x = _tmp_point.x - 2;
			_Point.y = _tmp_point.y - 2;
			return true;
		}
	}
	return false;
}

bool Game::Create_Or_Disrupt_TwoWay_Position(point &_Point, vector<point> &_points)
{
	point _tmp_point;
	int _tmp_turn = Game_Map[_points[0].x][_points[0].y];
	for (int i = 0; i < _points.size(); i++)
	{
		_tmp_point = _points[i];
		//Right-up winning
		if (_tmp_point.right_up == 1
			&& Column_check(_tmp_point.y + 2) == true
			&& Column_check(_tmp_point.y + 3) == true
			&& Column_check(_tmp_point.y - 1) == true
			&& Height_check(_tmp_point.x - 2) == true
			&& Height_check(_tmp_point.x - 3) == true
			&& Height_check(_tmp_point.x + 1) == true
			&& Game_Map[_tmp_point.x - 2][_tmp_point.y + 2] == 0
			&& Game_Map[_tmp_point.x - 3][_tmp_point.y + 3] == 0
			&& Game_Map[_tmp_point.x + 1][_tmp_point.y - 1] == 0
			&& Heights[_tmp_point.y + 2] == Max_Height - (_tmp_point.x - 2)
			&& Heights[_tmp_point.y + 3] == Max_Height - (_tmp_point.x - 3)
			&& Heights[_tmp_point.y - 1] == Max_Height - (_tmp_point.x + 1)
			)
		{
			_Point.x = _tmp_point.x - 2;
			_Point.y = _tmp_point.y + 2;
			return true;
		}

		//Right winning
		else if (_tmp_point.right == 1
			&& Column_check(_tmp_point.y + 2) == true
			&& Column_check(_tmp_point.y + 3) == true
			&& Column_check(_tmp_point.y - 1) == true
			&& Game_Map[_tmp_point.x][_tmp_point.y + 2] == 0
			&& Game_Map[_tmp_point.x][_tmp_point.y + 3] == 0
			&& Game_Map[_tmp_point.x][_tmp_point.y - 1] == 0
			&& Heights[_tmp_point.y + 2] == Max_Height - (_tmp_point.x)
			&& Heights[_tmp_point.y + 3] == Max_Height - (_tmp_point.x)
			&& Heights[_tmp_point.y - 1] == Max_Height - (_tmp_point.x)
			)
		{
			_Point.x = _tmp_point.x;
			_Point.y = _tmp_point.y + 2;
			return true;
		}

		//Right-down winning
		else if (_tmp_point.right_down == 1
			&& Column_check(_tmp_point.y + 2) == true
			&& Column_check(_tmp_point.y + 3) == true
			&& Column_check(_tmp_point.y - 1) == true
			&& Height_check(_tmp_point.x + 2) == true
			&& Height_check(_tmp_point.x + 3) == true
			&& Height_check(_tmp_point.x - 1) == true
			&& Game_Map[_tmp_point.x + 2][_tmp_point.y + 2] == 0
			&& Game_Map[_tmp_point.x + 3][_tmp_point.y + 3] == 0
			&& Game_Map[_tmp_point.x - 1][_tmp_point.y - 1] == 0
			&& Heights[_tmp_point.y + 2] == Max_Height - (_tmp_point.x + 2)
			&& Heights[_tmp_point.y + 3] == Max_Height - (_tmp_point.x + 3)
			&& Heights[_tmp_point.y - 1] == Max_Height - (_tmp_point.x - 1)
			)
		{
			_Point.x = _tmp_point.x + 2;
			_Point.y = _tmp_point.y + 2;
			return true;
		}

		//Left-down winning
		else if (_tmp_point.left_down == 1
			&& Column_check(_tmp_point.y - 2) == true
			&& Column_check(_tmp_point.y - 3) == true
			&& Column_check(_tmp_point.y + 1) == true
			&& Height_check(_tmp_point.x + 2) == true
			&& Height_check(_tmp_point.x + 3) == true
			&& Height_check(_tmp_point.x - 1) == true
			&& Game_Map[_tmp_point.x + 2][_tmp_point.y - 2] == 0
			&& Game_Map[_tmp_point.x + 3][_tmp_point.y - 3] == 0
			&& Game_Map[_tmp_point.x - 1][_tmp_point.y + 1] == 0
			&& Heights[_tmp_point.y - 2] == Max_Height - (_tmp_point.x + 2)
			&& Heights[_tmp_point.y - 3] == Max_Height - (_tmp_point.x + 3)
			&& Heights[_tmp_point.y + 1] == Max_Height - (_tmp_point.x - 1)
			)
		{
			_Point.x = _tmp_point.x + 2;
			_Point.y = _tmp_point.y - 2;
			return true;
		}

		//Left winning
		else if (_tmp_point.left == 1
			&& Column_check(_tmp_point.y - 2) == true
			&& Column_check(_tmp_point.y - 3) == true
			&& Column_check(_tmp_point.y + 1) == true
			&& Game_Map[_tmp_point.x][_tmp_point.y - 2] == 0
			&& Game_Map[_tmp_point.x][_tmp_point.y - 3] == 0
			&& Game_Map[_tmp_point.x][_tmp_point.y + 1] == 0
			&& Heights[_tmp_point.y - 2] == Max_Height - (_tmp_point.x)
			&& Heights[_tmp_point.y - 3] == Max_Height - (_tmp_point.x)
			&& Heights[_tmp_point.y + 1] == Max_Height - (_tmp_point.x)
			)
		{
			_Point.x = _tmp_point.x;
			_Point.y = _tmp_point.y - 2;
			return true;
		}

		//Left-up winning
		else if (_tmp_point.left_up == 1
			&& Column_check(_tmp_point.y - 2) == true
			&& Column_check(_tmp_point.y - 3) == true
			&& Column_check(_tmp_point.y + 1) == true
			&& Height_check(_tmp_point.x - 2) == true
			&& Height_check(_tmp_point.x - 3) == true
			&& Height_check(_tmp_point.x + 1) == true
			&& Game_Map[_tmp_point.x - 2][_tmp_point.y - 2] == 0
			&& Game_Map[_tmp_point.x - 3][_tmp_point.y - 3] == 0
			&& Game_Map[_tmp_point.x + 1][_tmp_point.y + 1] == 0
			&& Heights[_tmp_point.y - 2] == Max_Height - (_tmp_point.x - 2)
			&& Heights[_tmp_point.y - 3] == Max_Height - (_tmp_point.x - 3)
			&& Heights[_tmp_point.y + 1] == Max_Height - (_tmp_point.x + 1)
			)
		{
			_Point.x = _tmp_point.x - 2;
			_Point.y = _tmp_point.y - 2;
			return true;
		}
	}
	return false;
}

int Game::Finding_Choice_WithThe_Least_Density(point &_Point, vector<point> &_points)
{
	point _tmp_point;
	int _tmp_turn = Game_Map[_points[0].x][_points[0].y];

	int* Density = new int[_points.size()];
	for (int i = 0; i < _points.size(); i++)
	{
		Density[i] = 0;
	}
	
	int x, y;
	for (int i = 0; i < _points.size(); i++)
	{
		_tmp_point = _points[i];
		//Up check
		if (Height_check(_tmp_point.x - 1) == true
			&& Game_Map[_tmp_point.x - 1][_tmp_point.y] == 0
			)
		{
			Density[i]++;
		}

		
		//Right-up check
		else if (Column_check(_tmp_point.y + 1) == true
			&& Height_check(_tmp_point.x - 1) == true
			&& Game_Map[_tmp_point.x - 1][_tmp_point.y + 1] == 0
			)
		{
			Density[i]++;
		}

		//Right check
		else if (Column_check(_tmp_point.y + 1) == true
			&& Game_Map[_tmp_point.x][_tmp_point.y + 1] == 0
			)
		{
			Density[i]++;
		}

		//Right-down check
		else if (Column_check(_tmp_point.y + 1) == true
			&& Height_check(_tmp_point.x + 1) == true
			&& Game_Map[_tmp_point.x + 1][_tmp_point.y + 1] == 0
			)
		{
			Density[i]++;
		}

		//Left-down check
		else if (Column_check(_tmp_point.y - 1) == true
			&& Height_check(_tmp_point.x + 1) == true
			&& Game_Map[_tmp_point.x + 1][_tmp_point.y - 1] == 0
			)
		{
			Density[i]++;
		}

		//Left check
		else if (Column_check(_tmp_point.y - 1) == true
			&& Game_Map[_tmp_point.x][_tmp_point.y - 1] == 0
			)
		{
			Density[i]++;
		}

		//Left-up check	
		else if (Column_check(_tmp_point.y - 1) == true
			&& Height_check(_tmp_point.x - 1) == true
			&& Game_Map[_tmp_point.x - 1][_tmp_point.y - 1] == 0
			)
		{
			Density[i]++;
		}


	}

	int key = 0;
	int k = 0;
	for (int i = 0; i < _points.size(); i++)
	{
		if (Density[i] >= key)
		{
			key = Density[i];
			k = i;
		}
	}
	return k;

}

bool Game::Select_TheNearest_Choice_To_LeastDensity(point &_Point, vector<point> &_points, int key)
{
	point _tmp_point;
	int i = key;
	_tmp_point = _points[i];
	//Up select
	if (Height_check(_tmp_point.x - 1) == true
		&& Game_Map[_tmp_point.x - 1][_tmp_point.y] == 0
		)
	{
		_Point.x = _tmp_point.x - 1;
		_Point.y = _tmp_point.y;
		return true;
	}
	
	//Right-up select
	if (Column_check(_tmp_point.y + 1) == true
		&& Height_check(_tmp_point.x - 1) == true
		&& Game_Map[_tmp_point.x - 1][_tmp_point.y + 1] == 0
		&& Heights[_tmp_point.y + 1] == Max_Height - (_tmp_point.x - 1)
		)
	{
		_Point.x = _tmp_point.x - 1;
		_Point.y = _tmp_point.y + 1;
		return true;
	}

	//Right select
	else if (Column_check(_tmp_point.y + 1) == true
		&& Game_Map[_tmp_point.x][_tmp_point.y + 1] == 0
		&& Heights[_tmp_point.y + 1] == Max_Height - (_tmp_point.x)
		)
	{
		_Point.x = _tmp_point.x;
		_Point.y = _tmp_point.y + 1;
		return true;
	}

	//Right-down select
	else if (Column_check(_tmp_point.y + 1) == true
		&& Height_check(_tmp_point.x + 1) == true
		&& Game_Map[_tmp_point.x + 1][_tmp_point.y + 1] == 0
		&& Heights[_tmp_point.y + 1] == Max_Height - (_tmp_point.x)
		)
	{
		_Point.x = _tmp_point.x + 1;
		_Point.y = _tmp_point.y + 1;
		return true;
	}

	//Left-down select
	else if (Column_check(_tmp_point.y - 1) == true
		&& Height_check(_tmp_point.x + 1) == true
		&& Game_Map[_tmp_point.x + 1][_tmp_point.y - 1] == 0
		&& Heights[_tmp_point.y - 1] == Max_Height - (_tmp_point.x + 1)
		)
	{
		_Point.x = _tmp_point.x + 1;
		_Point.y = _tmp_point.y - 1;
		return true;
	}

	//Left select
	else if (Column_check(_tmp_point.y - 1) == true
		&& Game_Map[_tmp_point.x][_tmp_point.y - 1] == 0
		&& Heights[_tmp_point.y - 1] == Max_Height - (_tmp_point.x)
		)
	{
		_Point.x = _tmp_point.x;
		_Point.y = _tmp_point.y - 1;
		return true;
	}

	//Left-up select
	else if (Column_check(_tmp_point.y - 1) == true
		&& Height_check(_tmp_point.x - 1) == true
		&& Game_Map[_tmp_point.x - 1][_tmp_point.y - 1] == 0
		&& Heights[_tmp_point.y - 1] == Max_Height - (_tmp_point.x - 1)
		)
	{
		_Point.x = _tmp_point.x - 1;
		_Point.y = _tmp_point.y - 1;
		return true;
	}

	return false;
}

//Ending
void Game::Add_Point(int _tmp_Turn, point &_Point, vector<point> &_points)
{
	Game_Map[_Point.x][_Point.y] = _tmp_Turn;
	_points.push_back(_Point);
	Heights[_Point.y]++;
}

bool Game::End_Game(int &_Winner)
{
	point _tmpPoint;
	int _tmpTurn;
	for (int i = 0; i < PL1_points.size(); i++)
	{
		_tmpPoint = PL1_points[i];
		_tmpTurn = Game_Map[PL1_points[0].x][PL1_points[0].y];
		if (Up_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (RightUp_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (Right_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (RightDown_winning(_tmpPoint) == true)
		{
			_Winner = Turn;
			return true;
		}
		else if (LeftDown_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (Left_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (LeftUp_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
	}

	for (int i = 0; i < PL2_points.size(); i++)
	{
		_tmpPoint = PL2_points[i];
		_tmpTurn = Game_Map[PL2_points[0].x][PL2_points[0].y];
		if (Up_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (RightUp_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (Right_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (RightDown_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (LeftDown_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (Left_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
		else if (LeftUp_winning(_tmpPoint) == true)
		{
			_Winner = _tmpTurn;
			return true;
		}
	}
	return false;
}

bool Game::Up_winning(point &_Point)
{
	if (_Point.up == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::RightUp_winning(point &_Point)
{
	if (_Point.right_up == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::Right_winning(point &_Point)
{
	if (_Point.right == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::RightDown_winning(point &_Point)
{
	if (_Point.right_down == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::LeftDown_winning(point &_Point)
{
	if (_Point.left_down == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::Left_winning(point &_Point)
{
	if (_Point.left == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::LeftUp_winning(point &_Point)
{
	if (_Point.left_up == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//Handler Functions 
bool Game::Height_check(int key)
{
	if (key > Max_Height || key < 1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Game::Column_check(int key)
{
	if (key > NumberOf_Columns || key < 1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Game::Show_Map()
{
	int i, j;
	cout << endl;
	for (i = 0; i <= Max_Height; i++)
	{
		for (j = 0; j <= NumberOf_Columns; j++)
		{
			cout << Game_Map[i][j] << " ";
		}
		cout << endl;
	}

	cout << "Heights: " << endl;
	for (i = 0; i <= NumberOf_Columns; i++)
	{
		cout << Heights[i] << " ";
	}
	cout << endl;
}

bool Game::Map_isEmpty()
{
	int i, j;
	for (i = 1; i <= NumberOf_Columns; i++)
	{
		if (Heights[i] != 0)
		{
			return false;
		}
	}
	for (i = 1; i <= Max_Height; i++)
	{
		for (j = 1; j <= NumberOf_Columns; j++)
		{
			if (Game_Map[i][j] != 0)
			{
				return false;
			}
		}
	}
	return true;
}

void Game::Sort_Points()
{
	point tmp;
	if (Turn == 1)
	{
		for (int i = 0; i < PL1_points.size(); i++)
		{
			for (int j = i + 1; j < PL1_points.size(); j++)
			{
				if (PL1_points[i].x <= PL1_points[j].x)
				{
					tmp = PL1_points[i];
					PL1_points[i] = PL1_points[j];
					PL1_points[j] = tmp;
				}
			}
		}

	}
	else
	{
		for (int i = 0; i < PL2_points.size(); i++)
		{
			for (int j = i + 1; j < PL2_points.size(); j++)
			{
				if (PL2_points[i].x <= PL2_points[j].x)
				{
					tmp = PL2_points[i];
					PL2_points[i] = PL2_points[j];
					PL2_points[j] = tmp;
				}
			}
		}
	}
}

void Game::Change_Turn()
{
	if (Turn == 1)
	{
		Turn = 2;
	}
	else
	{
		Turn = 1;
	}
}

void Game::Update_Data()
{
	point _Point;
	int x, y, i, j, k;
	if (Turn == 1)
	{
		for (k = 0; k < PL1_points.size(); k++)
		{
			_Point = PL1_points[k];
			x = _Point.x;
			y = _Point.y;

			PL1_points[k].up = 0;
			for (i = x - 1; 0 < i && Game_Map[i][y] == Turn; i--) // we have same point in up
			{
				PL1_points[k].up++;
			}

			i = x - 1;
			j = y + 1;
			PL1_points[k].right_up = 0;
			while (i > 0 && j <= NumberOf_Columns && Game_Map[i][j] == Turn) // we have same point in right_up
			{
				PL1_points[k].right_up++;
				i--, j++;
			}

			PL1_points[k].right = 0;
			for (j = y + 1; j <= NumberOf_Columns && Game_Map[x][j] == Turn; j++) // we have same point in right	
			{
				PL1_points[k].right++;
			}

			i = x + 1;
			j = y + 1;
			PL1_points[k].right_down = 0;
			while (i <= Max_Height && j <= NumberOf_Columns && Game_Map[i][j] == Turn) // we have same point in right_down
			{
				PL1_points[k].right_down++;
				i++, j++;
			}

			PL1_points[k].down = 0;
			for (i = x + 1; i <= Max_Height && Game_Map[i][y] == Turn; i++) // we have same point in down
			{
				PL1_points[k].down++;
			}

			i = x + 1;
			j = y - 1;
			PL1_points[k].left_down = 0;
			while (i <= Max_Height && j > 0 && Game_Map[i][j] == Turn) // we have same point in left_down
			{
				PL1_points[k].left_down++;
				i++, j--;
			}

			PL1_points[k].left = 0;
			for (j = y - 1; 0 < j && Game_Map[x][j] == Turn; j--) // we have same point in left
			{
				PL1_points[k].left++;
			}

			i = x - 1;
			j = y - 1;
			PL1_points[k].left_up = 0;
			while (i > 0 && j > 0 && Game_Map[i][j] == Turn) // we have same point in left_up
			{
				PL1_points[k].left_up++;
				i--, j--;
			}
		}
	}
	else
	{
		for (int k = 0; k < PL2_points.size(); k++)
		{
			_Point = PL2_points[k];
			x = _Point.x;
			y = _Point.y;

			PL2_points[k].up = 0;
			for (i = x - 1; 0 < i && Game_Map[i][y] == Turn; i--) // we have same point in up
			{
				PL2_points[k].up++;
			}

			i = x - 1;
			j = y + 1;
			PL2_points[k].right_up = 0;
			while (i > 0 && j <= NumberOf_Columns && Game_Map[i][j] == Turn) // we have same point in right_up
			{
				PL2_points[k].right_up++;
				i--, j++;
			}

			PL2_points[k].right = 0;
			for (j = y + 1; j <= NumberOf_Columns && Game_Map[x][j] == Turn; j++) // we have same point in right	
			{
				PL2_points[k].right++;
			}

			i = x + 1;
			j = y + 1;
			PL2_points[k].right_down = 0;
			while (i <= Max_Height && j <= NumberOf_Columns && Game_Map[i][j] == Turn) // we have same point in right_down
			{
				PL2_points[k].right_down++;
				i++, j++;
			}

			PL2_points[k].down = 0;
			for (i = x + 1; i <= Max_Height && Game_Map[i][y] == Turn; i++) // we have same point in down
			{
				PL2_points[k].down++;
			}

			i = x + 1;
			j = y - 1;
			PL2_points[k].left_down = 0;
			while (i <= Max_Height && j > 0 && Game_Map[i][j] == Turn) // we have same point in left_down
			{
				PL2_points[k].left_down++;
				i++, j--;
			}

			PL2_points[k].left = 0;
			for (j = y - 1; 0 < j && Game_Map[x][j] == Turn; j--) // we have same point in left
			{
				PL2_points[k].left++;
			}

			i = x - 1;
			j = y - 1;
			PL2_points[k].left_up = 0;
			while (i > 0 && j > 0 && Game_Map[i][j] == Turn) // we have same point in left_up
			{
				PL2_points[k].left_up++;
				i--, j--;
			}
		}
	}

}

void Game::Select(point &_Point)
{
	cout << "enter the number of column: ";
	cin >> _Point.y;
	while (_Point.y > NumberOf_Columns || _Point.y <= 0 || Heights[_Point.y] >= Max_Height)
	{
		cout << " *** Wrong !!! Select again. *** " << endl;
		cout << "enter the number of column: ";
		cin >> _Point.y;

	}
	_Point.x = Max_Height - (Heights[_Point.y]);
}



int main()
{
	int _NumberOf_Columns = 0;
	int _Max_Height = 0;
	bool quit = false;
	int input = 0;


	cout << " How many Columns do you want in the game: ";
	cin >> _NumberOf_Columns;
	cout << " And, how many heights do you want for every columns: ";
	cin >> _Max_Height;
	Game _game(_NumberOf_Columns, _Max_Height);
	_game.Start_Game();


	while (quit != true)
	{
		if (input == 1)
		{
			_game.Start_Game();
		}
		else if (input == 2)
		{
			quit = true;
			break;
		}
		else
		{
			cout << "\n If you want to play again enter 1 or 2 for \"Quit the Game\" : ";
			cin >> input;
			system("cls");
		}
	}
	return 0;
}