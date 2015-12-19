// SimplexMethod.cpp: определяет точку входа для консольного приложения.
//

//TODO: Необходимо добавить недостающие комментарии.

#include "stdafx.h"
#include <vector>
#include <limits>

using namespace std;

//Объявление типа vec_f равнозначного vector<float> для упрощения записи
typedef vector<float> vec_f;


//Структура для хранения результата нахождения
//минимального(максимального) значения симплекс-методом
struct SimplexResult
{
	//Вектор(массив переменной длины), хранящий в себе оптимальные
	//значения координат исходной функции
	vec_f CoordnatesValues; 
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

vec_f GetCollumn(int number, vector<vec_f > limitsCoordinates)
{
	vec_f res;
	for (vec_f x : limitsCoordinates)
		res.push_back(x[number]);
	return res;
}

float CalculateIndex(vector<basisElement> basis, vec_f coordinates, float value)
{
	float res = 0;
	for (unsigned int i = 0; i < basis.size(); i++)
	{
		res += basis[i].value * coordinates[i];
	}
	return res -= value;
}

vec_f CalculateIndexes(vector<basisElement> basis, vec_f coordinates,
								vector<vec_f> limitsCoordinates)
{
	vec_f res;
	for (unsigned int i = 0; i < coordinates.size(); i++)
		res.push_back(CalculateIndex(basis, GetCollumn(i, limitsCoordinates), coordinates[i]));
	return res;
}

vector<basisElement> CalculateBasis(vector<vec_f> limitsCoordinates, vec_f coordinates)
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

int FindMax(vec_f values)
{
	int index = 0;
	for (unsigned int i = 0; i < values.size(); i++)
		if (values[index] > values[i])
			index = i;
	return index;
}

int FindMin(vec_f values)
{
	int index = 0;
	for (unsigned int i = 0; i < values.size(); i++)
	if (values[index] < values[i])
		index = i;
	return index;
}

int CheckIndexes(vec_f indexRow, vector<vec_f> limitsCoordinates)
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

vec_f CalculateTeta(vec_f collumn, vec_f result)
{
	vec_f res;
	for (unsigned int i = 0; i < collumn.size(); i++)
		res.push_back(result[i] / collumn[i]);
	return res;
}


//Функция нахождения оптимального значения симплекс-методом
//coordinates - массив коэффициентов при переменных в целевой функции
//limitationsCoordinates - матрица коэффициентов в ограничениях для вычисления оптимального значения
//results - массив значений ограничений
//type - тип задачи (минимум - true, максимум - false) 
SimplexResult SimplexMethod(vec_f coordinates,
							vector<vec_f > limitsCoordinates,
							vec_f results,
							bool type)
{
	SimplexResult res;
	res.IsError = false;
	res.ResultValue = 0;
	//массив, содержащий в себе значения опорного плана
	//заполнение его нулями по числу координат
	vec_f basicPlan(coordinates.size(),0);

	//Добавление в опорный план значений ограничений
	for (float x : results)
	{
		basicPlan.push_back(x);
	}

	//массив, содержащий значения коэффициентов ограничений вместе с балансными переменными
	vector<vec_f > _limitsCoordinates;

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
	vec_f indexRow = CalculateIndexes(basis, coordinates, limitsCoordinates);
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
		//TODO:  Закончить алгоритм.
	}
}




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

