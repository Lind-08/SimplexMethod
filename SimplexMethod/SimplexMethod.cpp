// SimplexMethod.cpp: определяет точку входа для консольного приложения.
//

//TODO: Необходимо добавить недостающие комментарии.

#include "stdafx.h"
#include <vector>
#include <limits>
#include <iostream>

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
	int position; //Позиция в столбце
	float value; //Значение
};

//Функция получения столбца матрицы
vec_f GetCollumn(int number, vector<vec_f > limitsCoordinates)
{
	vec_f res;
	//Range-based for
	//Цикл перебора элементов коллекции. Поочереди выбирает каждый элемент.
	//Добавлен в с++11
	for (vec_f x : limitsCoordinates)
		res.push_back(x[number]);
	return res;
}

//Функция вычисления значения индекса
//Это скалярное произведение двух матриц минус значение координаты
float CalculateIndex(vector<basisElement> basis, vec_f coordinates, float value)
{
	float res = 0;
	for (unsigned int i = 0; i < basis.size(); i++)
	{
		//Получение элемента базиса, соответствующее необходимой позиции
		basisElement z;
		for (basisElement x: basis)
			if (x.position == i)
				z = x;
		res += z.value * coordinates[i];
	}
	return res -= value;
}

//Функция проверки вхождения номера координаты в базис
bool InBasis(vector<basisElement> basis, int index)
{
	for (basisElement x : basis)
	{
		if (x.number == index)
			return true;
	}
	return false;
}

//Функция вычисления значения  индексной строки
vec_f CalculateIndexes(vector<basisElement> basis, vec_f coordinates,
								vector<vec_f> limitsCoordinates)
{
	vec_f res;
	for (unsigned int i = 0; i < limitsCoordinates[0].size(); i++)
		//Если координата не входит в базис,
		//то вычисляем значение индекса, иначе оно равно нулю
		if (!InBasis(basis,i))
			res.push_back(CalculateIndex(basis, GetCollumn(i, limitsCoordinates), coordinates[i]));
		else res.push_back(0);
	return res;
}


//Функция вычисления базиса
//Проверяется вся симплекс матрица и в те координаты, в столбцах которых есть только одна единица,
//заносятся в базис
vector<basisElement> CalculateBasis(vector<vec_f> limitsCoordinates, vec_f coordinates)
{
	vector<basisElement> result;
	for (unsigned int j = 0; j < limitsCoordinates[0].size(); j++)
	{
		//Вспомогательные переменные номера координаты, позиции и значения
		int number;
		int position;
		float value;
		//Указывает, является ли данный столбец базисной координатой
		bool isBasis = true;
		for (unsigned int i = 0; i < limitsCoordinates.size(); i++)
		{
			//Если это не ноль, то проверяем дальше
			if (limitsCoordinates[i][j])
			{
				//Если имеется любое значение, кроме единицы, то это точно не базисная координата
				if (limitsCoordinates[i][j] != 1)
				{
					isBasis = false;
					break;
				}
				else 
				{
					//иначе сохраняем номер, значение и позицию
					number = j;
					position = i;
					value = coordinates[j];
				}
			}
		}
		//если координата базисная
		if (isBasis)
		{
			//то добавим ее в базис
			basisElement be;
			be.number = number;
			be.value = value;
			be.position = position;
			result.push_back(be);
		}
		else
			isBasis = true;

	}
	return result;
}

//Следующие функции возвращают индекс искомого элемента

//Функция нахождения максимума
int FindMax(vec_f values)
{
	int index = 0;
	for (unsigned int i = 0; i < values.size(); i++)
		if (values[i] > values[index])
			index = i;
	return index;
}

//Функция нахождения минимума
int FindMin(vec_f values)
{
	int index = 0;
	for (unsigned int i = 0; i < values.size(); i++)
		//так как, если занчение ограничения равно 0, то данная строка не учитывается, 
		//то необходимо добавить проверку на это
	if (values[i] < values[index] && values[i] != 0)
		index = i;
	return index;
}

//Функция проверки индексной строки
//Возможны три случая [минимум(максимум)]: 
//1) все оценки в индексной строке неположительны(положительны) - значит полученный план оптимален;
//2) среди оценок есть хотя бы одна положительная(отрицательная), и в столбце над ней есть хотя бы один
//положительный(отрицательный) коэффициент - план неоптимален, возможно его улучшение;
//3) среди оценок есть хотя бы одна положительная(отрицательная), и в столбце над ней нет ни одного положительного(отрицательного)
//коэффициента - целевая функция не ограничена сверху(снизу), оптимального плана не существует.

int CheckIndexes(vec_f indexRow, vector<vec_f> limitsCoordinates, bool type)
{
	int res = 0;
	for (unsigned int i = 0; i < indexRow.size(); i++)
	{
		if (type)
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
		else
		{
			if (indexRow[i] < 0)
			{
				res = -1;
				for (unsigned int j = 0; j < limitsCoordinates.size(); j++)
				{
					if (limitsCoordinates[j][i] < 0)
						res = 1;
				}
				break;
			}
		}
	}
	return res;
}

//Вычисление отношения значения ограничения деленного на элемент 
//столбца, соотв. максимальному(минимальному) значению индексной строки
vec_f CalculateTeta(vec_f collumn, vec_f result)
{
	vec_f res;
	for (unsigned int i = 0; i < collumn.size(); i++)
		res.push_back(result[i] / collumn[i]);
	return res;
}


//Функция нахождения базового плана
vec_f CalculateBasicPlan(vector<vec_f > limitsCoordinates, vector<basisElement> basis, vec_f results)
{
	//Заполняем его нулями
	vec_f res(limitsCoordinates[0].size(),0);
	for (basisElement x : basis)
	{
		//На место базисных переменных ставим значения соответствующх по позиции ограничений
		res[x.number] = results[x.position];
	}
	return res;
}


//Функция вычисления значения целевой функции
float CalculateFunction(vec_f coordinates, vec_f basicPlan)
{
	float res = 0;
	for (unsigned int i = 0; i < coordinates.size(); i++)
	{
		res += coordinates[i] * basicPlan[i];
	}
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

	vec_f _coordinates(coordinates);
	for (unsigned i = 0; i < limitsCoordinates.size(); i++)
		_coordinates.push_back(0);

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
		_limitsCoordinates.push_back(vec_f());
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
	vec_f indexRow = CalculateIndexes(basis, _coordinates, _limitsCoordinates);
	//Проверяем индексную строку
	int probe = CheckIndexes(indexRow, _limitsCoordinates,type);
	//Если -1, то решение найти невозможно
	if (probe == -1)
	{
		res.IsError = true;
		return res;
	}
	else 
		//если ноль, то план уже оптимален
	if (probe == 0)
	{
		res.CoordnatesValues = basicPlan;
		res.ResultValue = CalculateFunction(_coordinates, basicPlan);
		return res;
	}
	while (true)
	{
		//Находим необходимый элемент индексной строки
		//Используется тернарный оператор
		//он действует так 
		//если type = true, то выполняется FindMax, иначе FindMin 
		int index = type ? FindMax(indexRow) : FindMin(indexRow);
		//Получаем соответствующий столбец
		vec_f indexCollumn = GetCollumn(index, _limitsCoordinates);
		//Определяем ведущую строчку по необходимому отношению
		int indexTeta = type ? FindMin(CalculateTeta(indexCollumn, results)) : FindMax(CalculateTeta(indexCollumn, results));
		float indexValue = _limitsCoordinates[indexTeta][index];
		//Делим ведущую строчку на элемент ведущего столбца
		for (unsigned int i = 0; i < _limitsCoordinates[index].size(); i++)
		{
			_limitsCoordinates[indexTeta][i] /= indexValue;
		}
		//и делим значение ограничения
		results[indexTeta] /= indexValue;
		
		//Сложение строк с ведущей строкой
		for (unsigned int i = 0; i < _limitsCoordinates.size(); i++)
		{
			if (i != indexTeta)
			{
				for (unsigned int j = 0; j < _limitsCoordinates[i].size(); j++)
				{
					_limitsCoordinates[i][j] += _limitsCoordinates[indexTeta][j] * (-indexCollumn[i]);
				}
				results[i] += results[indexTeta] * (-indexCollumn[i]);
			}
		}

		//Действия аналогичны тем, что мы делали ранее
		basis = CalculateBasis(_limitsCoordinates, _coordinates);

		basicPlan = CalculateBasicPlan(_limitsCoordinates, basis, results);

		indexRow = CalculateIndexes(basis, _coordinates, _limitsCoordinates);
		probe = CheckIndexes(indexRow, _limitsCoordinates,type);
		if (probe == -1)
		{
			res.IsError = true;
			return res;
		}
		else
		if (probe == 0)
		{
			res.CoordnatesValues = basicPlan;
			res.ResultValue = CalculateFunction(coordinates, basicPlan);
			return res;
		}
	}
}




int _tmain(int argc, _TCHAR* argv[])
{
	//Создаю массивы координат функции, ограничений и их значений
	vec_f coord({ 10, -7, -5 });
	vector<vec_f> _limits;
	vec_f test0({ 6, 15, 6 });
	vec_f test1({ 14, 42, 16 });
	vec_f test2({ 2, 8, 2 });
	_limits.push_back(test0);
	_limits.push_back(test1);
	_limits.push_back(test2);
	vec_f res({ 9, 21, 4 });
	//Вычисляю результат
	SimplexResult result = SimplexMethod(coord, _limits, res, true);
	cout << "Min value = " << result.ResultValue << endl;
	cout << "Coordinates:" << endl;
	for (float x : result.CoordnatesValues)
	{
		cout << x << " ";
	}
	cout << endl;
	system("pause");
	return 0;
}

