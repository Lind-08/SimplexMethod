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

//Функция нахождения оптимального значения симплекс-методом
//coordinates - массив коэффициентов при переменных в целевой функции
//limitationsCoordinates - матрица коэффициентов в ограничениях для вычисления оптимального значения
//results - массив значений ограничений
//type - тип задачи (минимум - true, максимум - false) 
SimplexResult SimplexMethod(vector<float> coordinates,
							vector<vector<float> > limitationsCoordinates,
							vector<float> results,
							bool type)
{
	//массив, хранящий в себе значения индексной строки
	//по ней определяются шаги алгоритма и возможность нахождения оптимума
	vector<float> indexRow;
	//массив, хранящий в себе номера переменных, входящих в базис
	vector<int> basis;
	//массив, содержащий в себе значения опорного плана
	vector<float> basicPlan;

}




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

