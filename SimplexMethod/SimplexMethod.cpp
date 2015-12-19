// SimplexMethod.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vector>

using namespace std;


//Структура для хранения результата нахождения
//минимального(максимального) значения симплекс-методом
struct SimplexResult
{
	//Вектор(массив переменной длины), хранящий в себе оптимальные
	//значения координат исходной функции
	vector<float> CoordnatesValues; 
	//Оптимальное значение целевой функции
	float ResultValue;
	//Логическая переменная, которая равна false, если вычисление прошло успешно,
	//или true, если для функции невозможно найти минимум(максимум)
	bool IsError;
};

struct basisElement
{
	int number;
	float value;
};

float CalculateIndex(vector<basisElement> basis, vector<float> coordinates, float value)
{

}

vector<float> CalculateIndexes(vector<basisElement> basis,
							   vector<float> coordinates,
							   vector<vector<float> > limitsCoordinates)
{
	
}

vector<basisElement> CalculateBasis(vector<vector<float> > limitsCoordinates, vector<float> coordinates)
{
	for (int i = 0; i < limitsCoordinates.size(); i++)
	{
		int number;
		float value;
		bool isBasis = true;
		for (int j = 0; j < limitsCoordinates[i].size(); j++)
		{
			if (limitsCoordinates[i][j])
			{
				if (limitsCoordinates[i][j] != 0 && limitsCoordinates[i][j] != 1)
				{
					isBasis = false;
					break;
				}
				else 
				{
					number = j;
					if (j > coordinates.size())
						value = 0;
					else
						value = coordinates[j];
				}
			}
		}

	}
}

//Функция нахождения оптимального значения симплекс-методом
//coordinates - массив коэффициентов при переменных в целевой функции
//limitationsCoordinates - матрица коэффициентов в ограничениях для вычисления оптимального значения
//results - массив значений ограничений
//type - тип задачи (минимум - true, максимум - false) 
SimplexResult SimplexMethod(vector<float> coordinates,
							vector<vector<float> > limitsCoordinates,
							vector<float> results,
							bool type)
{
	//массив, содержащий в себе значения опорного плана
	//заполнение его нулями по числу координат
	vector<float> basicPlan(coordinates.size(),0);
	//Добавление в опорный план значений ограничений
	for (float x : results)
	{
		basicPlan.push_back(x);
	}
	//массив, хранящий в себе значения и номера базисных переменных
	vector<basisElement> basis(results.size());
	//массив, содержащий значения коэффициентов ограничений вместе с балансными переменными
	vector<vector<float> > _limitsCoordinates;
	//заполнение симплекс таблицы
	//добавление балансных переменных и присвоение им значений
	/*
		Вид матрицы
		4	2	3	|	1	0	0
		5	3	2	|	0	1	0
		5	2	1	|	0	0	1
		Основные	|	Балансные
		координаты	|	переменные
	*/
	for (int i = 0; i < limitsCoordinates.size(); i++)
	{
		for (int j = 0; j < limitsCoordinates[i].size(); j++)
		{
			_limitsCoordinates[i].push_back(limitsCoordinates[i][j]);
		}
		//Заполнение балансных переменных
		for (int j = 0; j < results.size(); j++)
		{
			if (i != j)
				_limitsCoordinates[i].push_back(0);
			else
				_limitsCoordinates[i].push_back(1);
		}
	}
	//массив, хранящий в себе значения индексной строки
	//по ней определяются шаги алгоритма и возможность нахождения оптимума
	vector<float> indexRow = CalculateIndexes(basis, coordinates, limitsCoordinates)

}




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

