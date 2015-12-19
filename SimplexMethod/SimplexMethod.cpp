// SimplexMethod.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vector>
#include <limits>

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

//Структура для хранения информации о элементе базиса
struct basisElement
{
	int number;	//Номер координаты
	float value; //Значение
};

vector<float> GetCollumn(int number, vector<vector<float> > limitsCoordinates)
{
	vector<float> res;
	for (vector<float> x : limitsCoordinates)
		res.push_back(x[number]);
	return res;
}

float CalculateIndex(vector<basisElement> basis, vector<float> coordinates, float value)
{
	float res = 0;
	for (unsigned int i = 0; i < basis.size(); i++)
	{
		res += basis[i].value * coordinates[i];
	}
	return res -= value;
}

vector<float> CalculateIndexes(vector<basisElement> basis,
							   vector<float> coordinates,
							   vector<vector<float> > limitsCoordinates)
{
	vector<float> res;
	for (unsigned int i = 0; i < coordinates.size(); i++)
		res.push_back(CalculateIndex(basis, GetCollumn(i, limitsCoordinates), coordinates[i]));
	return res;
}

vector<basisElement> CalculateBasis(vector<vector<float> > limitsCoordinates, vector<float> coordinates)
{
	vector<basisElement> result;
	for (unsigned int j = 0; j < limitsCoordinates.size(); j++)
	{
		int number;
		float value;
		bool isBasis = true;
		for (unsigned int i = 0; i < limitsCoordinates[i].size(); i++)
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
		if (isBasis)
		{
			basisElement be;
			be.number = number;
			be.value = value;
			result.push_back(be);
		}
		else
			isBasis = true;

	}
	return result;
}

int FindMax(vector<float> values)
{
	int index = 0;
	for (unsigned int i = 0; i < values.size(); i++)
		if (values[index] > values[i])
			index = i;
	return index;
}

int FindMin(vector<float> values)
{
	int index = 0;
	for (unsigned int i = 0; i < values.size(); i++)
	if (values[index] < values[i])
		index = i;
	return index;
}

int CheckIndexes(vector<float> indexRow, vector<vector<float> > limitsCoordinates)
{
	int res = 0;
	for (unsigned int i = 0; i < indexRow.size(); i++)
	{
		if (indexRow[i] > 0)
		{
			res = -1;
			for (unsigned int j = 0; j < limitsCoordinates.size(); j++)
			{
				if (limitsCoordinates[j][i] > 0)
					res = 1;
			}
			break;
		}
	}
	return res;
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
	SimplexResult res;
	res.IsError = false;
	res.ResultValue = 0;
	//массив, содержащий в себе значения опорного плана
	//заполнение его нулями по числу координат
	vector<float> basicPlan(coordinates.size(),0);

	//Добавление в опорный план значений ограничений
	for (float x : results)
	{
		basicPlan.push_back(x);
	}

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
	for (unsigned int i = 0; i < limitsCoordinates.size(); i++)
	{
		for (unsigned int j = 0; j < limitsCoordinates[i].size(); j++)
		{
			_limitsCoordinates[i].push_back(limitsCoordinates[i][j]);
		}
		//Заполнение балансных переменных
		for (unsigned int j = 0; j < results.size(); j++)
		{
			if (i != j)
				_limitsCoordinates[i].push_back(0);
			else
				_limitsCoordinates[i].push_back(1);
		}
	}

	//массив, хранящий в себе значения и номера базисных переменных
	vector<basisElement> basis = CalculateBasis(_limitsCoordinates, coordinates);

	//массив, хранящий в себе значения индексной строки
	//по ней определяются шаги алгоритма и возможность нахождения оптимума
	vector<float> indexRow = CalculateIndexes(basis, coordinates, limitsCoordinates);
	int probe = CheckIndexes(indexRow, _limitsCoordinates);
	if (probe == -1)
	{
		res.IsError = true;
		return res;
	}
	else 
	if (probe == 0)
	{
		res.CoordnatesValues = basicPlan;
		for (unsigned int i = 0; i < coordinates.size(); i++)
		{
			res.ResultValue += coordinates[i] * basicPlan[i];
		}
		return res;
	}
	while (true)
	{
		int index = type ? FindMax(indexRow) : FindMin(indexRow);
		
	}
}




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

