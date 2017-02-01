// 2048(new).cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <Math.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <fstream>
#include <direct.h>

#define MaxUnits 16
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_QUIT 113
#define KEY_START 13
#define MAX_VALUE 64
#define MSG_QUIT "Goodbye. Thanks for playing"

using namespace std;

const string savename_cells = "save/cells.save", savename_other = "save/other_data.save";

vector<string> Explode(string data, char exploding)
{
	vector<string> result;
	string prom = "";
	for (char J : data)
	{
		if (J == exploding && prom != "")
		{
			result.push_back(prom);
			prom = "";
		}
		else
		{
			prom += J;
		}
	}
	if (prom != "")result.push_back(prom);
	return result;
}

bool DoesExist(string name)
{
	ifstream f(name.c_str());
	return f.good();
}

class Unit
{
public:
	Unit()
	{
		value = coorX = coorY = 0;
	}
	Unit(int x, int y, int val)
	{
		value = val;
		coorX = x;
		coorY = y;
	}
	Unit(Unit A, int val)
	{
		coorX = A.GetPosX();
		coorY = A.GetPosY();
		value = val;
	}
	//Access methods
	int GetPosY()
	{
		return coorY;
	}
	int GetPosX()
	{
		return coorX;
	}
	int GetValue()
	{
		return value;
	}
	bool isFree()
	{
		return value == 0 ? true : false;
	}
	void SetPosX(int x)
	{
		coorX = x;
	}
	void SetPosY(int y)
	{
		coorY = y;
	}
	void SetValue(int val)
	{
		value = val;
	}
private:
	//Position in OX, OY directions, and value of each Unit
	int coorX, coorY, value;
};

class Board
{
public:
	static void Clear()
	{
		Units.clear();
		Units.reserve(16);
		for (int x = 1; x <= 4; x++)
		{
			for (int y = 1; y <= 4; y++)
			{
				//Adding Units with coordinates (1..4,1..4) and values 0
				Units.push_back(Unit(x, y, 0));
			}
		}
	}
	static void Show()
	{
		string msg = "";
		for (int y = 4; y >= 1; y--)
		{
			for (int x = 1; x <= 4; x++)
			{
				msg = to_string(GetUnit(x, y).GetValue());
				for (int i = 0; i < 4 - msg.size(); i++) cout << "0";
				cout << msg << " || ";
				//Convert to 4 digit form
			}
			cout << endl;
		}
	}
	static void ChangePos(int x1, int y1, int x2, int y2)
	{
		//Change position of unit with coor (x1,y1) to (x2,y2)

	}
	static Unit GetUnit(int x, int y)
		{
			for (Unit U : Units)
			{
				if (U.GetPosX() == x && U.GetPosY() == y) return U;
			}
			return Unit();
		}
	static void SetUnit(int x, int y, Unit replace_unit)
		{
			for (int it = 0; it < Units.size();it++)
			{
				if (Units[it].GetPosX() == x && Units[it].GetPosY() == y) Units[it] = replace_unit;
			}
		}
	static vector<Unit> getUnits() { return Units; }
	static void SetUnits(vector<Unit> set)
	{
		Units = set;
	}
		private:
	static vector<Unit> Units;
	};

class Game
	{
	public:
		static void LetsGetItStarted()
		{
			LoadData();
			CheckFreeUnits();

		}
		static void createRndUnit()
		{
			int x, y;
			do
			{
				//Change x and y to random int from 1 to 4
				x = rand() % 4 + 1;
				y = rand() % 4 + 1;
				//It will continue, while (x,y) wouldn't be free
			} while (!Board::GetUnit(x, y).isFree());
			unitsBusy++;
			unitsFree--;
			Board::SetUnit(x, y, Unit(Board::GetUnit(x, y), ((rand() % 4 == 0) ? 4 : 2)));
		}
		static void SelectAction(int cmd)
		{
			switch (cmd)
			{
			case KEY_START:
				LetsGetItStarted();
				break;
			case KEY_QUIT:
				Quit();
				break;
			case KEY_RIGHT:
				MoveRight();
				break;
			case KEY_LEFT:
				MoveLeft();
				break;
			case KEY_UP:
				MoveUp();
				break;
			case KEY_DOWN:
				MoveDown();
				break;
			}
		}
		static void CheckProccess()
		{
			FindHighest();
			CheckFreeUnits();
			cout << "!!! Checked !!!" << endl << "FREE - " << unitsFree << "\nBusy -" << unitsBusy << endl;
			if (unitsFree == 0) Lose();
			if (highestValue == MAX_VALUE) Win();
		}
		static int GetScore()
		{
			return score;
		}
		static void IncreaseScore(int increment)
		{
			score += increment;
		}

	private:
		static void MoveRight()
		{
			int value = 0;
			for (int y = 1; y <= 4; y++)
			{
				for (int x = 1; x < 4; x++)
				{
					//Take value of current unit
					value = Board::GetUnit(x, y).GetValue();
					//Check righter unit
					if (Board::GetUnit(x + 1, y).isFree() || Board::GetUnit(x + 1, y).GetValue() == value)
					{
						//Set current cell free
						Board::SetUnit(x, y, Unit(x, y, 0));
						//Change righter cell
						Board::SetUnit(x + 1, y, Unit(x + 1, y, value + Board::GetUnit(x + 1, y).GetValue()));
						Game::IncreaseScore(value + Board::GetUnit(x + 1, y).GetValue());
					}
				}
			}
		}
		static void MoveLeft()
		{
			int value;
			for (int y = 1; y <= 4; y++)
			{
				for (int x = 4; x > 1; x--)
				{
					value = Board::GetUnit(x, y).GetValue();
					if (Board::GetUnit(x - 1, y).isFree() || Board::GetUnit(x - 1, y).GetValue() == value)
					{
						Board::SetUnit(x, y, Unit(x, y, 0));
						Board::SetUnit(x - 1, y, Unit(x - 1, y, value + Board::GetUnit(x - 1, y).GetValue()));
						//Increase score
						Game::IncreaseScore(value + Board::GetUnit(x - 1, y).GetValue());
					}
				}
			}
		}
		static void MoveUp()
		{
			int value = 0;
			for (int x = 1; x <= 4; x++)
			{
				for (int y = 1; y < 4; y++)
				{
					//Take value of cell
					value = Board::GetUnit(x, y).GetValue();
					//Check upper cell
					if (Board::GetUnit(x, y + 1).isFree() || Board::GetUnit(x, y + 1).GetValue() == value)
					{
						Board::SetUnit(x, y, Unit(x, y, 0));
						Board::SetUnit(x, y + 1, Unit(x, y + 1, value + Board::GetUnit(x, y + 1).GetValue()));
						Game::IncreaseScore(value + Board::GetUnit(x, y + 1).GetValue());
					}
				}
			}
		}
		static void MoveDown()
		{
			int value = 0;
			for (int x = 1; x <= 4; x++)
			{
				for (int y = 4; y > 1; y--)
				{
					//Take value of cell
					value = Board::GetUnit(x, y).GetValue();
					//Check upper cell
					if (Board::GetUnit(x, y - 1).isFree() || Board::GetUnit(x, y - 1).GetValue() == value)
					{
						Board::SetUnit(x, y, Unit(x, y, 0));
						Board::SetUnit(x, y - 1, Unit(x, y - 1, value + Board::GetUnit(x, y - 1).GetValue()));
						Game::IncreaseScore(value + Board::GetUnit(x, y - 1).GetValue());
					}
				}
			}
		}
		static void CheckFreeUnits()
		{
			unitsFree = 0;
			unitsBusy = 0;
			for (Unit U : Board::getUnits())
			{
				if (U.isFree()) unitsFree++;
				else unitsBusy++;
			}
		}
		static void FindHighest()
		{
			for (Unit U : Board::getUnits())
			{
				if (U.GetValue() > highestValue) highestValue = U.GetValue();
			}
		}
		static void Lose()
		{
			/*This method contains actions on losing */
			cout << "You lose and collected " << score << endl;
			Quit();
		}
		static void Win()
		{
			/*This method contains action what will be used on winning*/
			cout << "Congratulations! You won and collected " << score << endl;
			Quit();
		}
		static void Quit()
		{
			/* Just method for quiting from game*/
			SaveProgress();
			cout << MSG_QUIT << endl;
			system("pause");
			exit(0);
		}
		static void SaveProgress()
		{
			_mkdir("save/");
			ofstream file;
			file.open(savename_cells);
			vector<Unit> Saving = Board::getUnits();
			for (Unit U : Saving)
			{
				file << U.GetPosX() << " " << U.GetPosY() << " " << U.GetValue() << endl;
			}
			file.close();
			file.open(savename_other);
			int score = Game::GetScore();
			file << score << " " << unitsBusy << " "<< unitsFree << endl;
			file.close();
		}
		static void LoadData()
		{
			if (!DoesExist(savename_cells))
			{
				Board::Clear();
				score = 0;
			}
			ifstream file(savename_cells);
			string row;
			vector<string> prom;
			vector<Unit> result;
			int i = 1;
			while (getline(file, row))
			{
				prom = Explode(row, ' ');
				result.push_back(Unit(atoi(prom[0].c_str()), atoi(prom[1].c_str()), atoi(prom[2].c_str())));
				if (i++ > 9) break;
			}
			Board::SetUnits(result);
			file.close();
			file.open(savename_other.c_str());
			getline(file, row);
			score = atoi(row.c_str());
		}
		static int unitsFree, unitsBusy, highestValue, score;
	};

//Initialization of static data-members
int Game::unitsBusy, Game::unitsFree, Game::highestValue, Game::score;
vector<Unit> Board::Units;

int main()
{
	/*Game::LetsGetItStarted();
	int cmd;
	while (true)
	{
		Game::createRndUnit();
		Board::Show();
		Input:
		for (int x = 0; x != 2; x++)cmd = (int)_getch();
		if (cmd != KEY_QUIT && cmd != KEY_UP && cmd != KEY_DOWN && cmd != KEY_LEFT && cmd != KEY_RIGHT && cmd != KEY_START) goto Input;
		Game::SelectAction(cmd);
		Game::CheckProccess();
	}
	cout << endl << "Finita la comedia" << endl;*/
	Game::LetsGetItStarted();
	Board::Show();
	Game::createRndUnit();
	cout << endl << endl;
	Board::Show();
	Game::SelectAction(_getch());
	system("pause");
	return 0;
}