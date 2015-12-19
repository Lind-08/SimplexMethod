// SimplexMethod.cpp: ���������� ����� ����� ��� ����������� ����������.
//

//TODO: ���������� �������� ����������� �����������.

#include "stdafx.h"
#include <vector>
#include <limits>

using namespace std;

//���������� ���� vec_f ������������� vector<float> ��� ��������� ������
typedef vector<float> vec_f;


//��������� ��� �������� ���������� ����������
//������������(�������������) �������� ��������-�������
struct SimplexResult
{
	//������(������ ���������� �����), �������� � ���� �����������
	//�������� ��������� �������� �������
	vec_f CoordnatesValues; 
	//����������� �������� ������� �������
	float ResultValue;
	//���������� ����������, ������� ����� false, ���� ���������� ������ �������,
	//��� true, ���� ��� ������� ���������� ����� �������(��������)
	bool IsError;
};

//��������� ��� �������� ���������� � �������� ������
struct basisElement
{
	int number;	//����� ����������
	float value; //��������
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


//������� ���������� ������������ �������� ��������-�������
//coordinates - ������ ������������� ��� ���������� � ������� �������
//limitationsCoordinates - ������� ������������� � ������������ ��� ���������� ������������ ��������
//results - ������ �������� �����������
//type - ��� ������ (������� - true, �������� - false) 
SimplexResult SimplexMethod(vec_f coordinates,
							vector<vec_f > limitsCoordinates,
							vec_f results,
							bool type)
{
	SimplexResult res;
	res.IsError = false;
	res.ResultValue = 0;
	//������, ���������� � ���� �������� �������� �����
	//���������� ��� ������ �� ����� ���������
	vec_f basicPlan(coordinates.size(),0);

	//���������� � ������� ���� �������� �����������
	for (float x : results)
	{
		basicPlan.push_back(x);
	}

	//������, ���������� �������� ������������� ����������� ������ � ���������� �����������
	vector<vec_f > _limitsCoordinates;

	//���������� �������� �������
	//���������� ��������� ���������� � ���������� �� ��������
	/*
		��� �������
		4	2	3	|	1	0	0
		5	3	2	|	0	1	0
		5	2	1	|	0	0	1
		��������	|	���������
		����������	|	����������
	*/
	for (unsigned int i = 0; i < limitsCoordinates.size(); i++)
	{
		for (unsigned int j = 0; j < limitsCoordinates[i].size(); j++)
		{
			_limitsCoordinates[i].push_back(limitsCoordinates[i][j]);
		}
		//���������� ��������� ����������
		for (unsigned int j = 0; j < results.size(); j++)
		{
			if (i != j)
				_limitsCoordinates[i].push_back(0);
			else
				_limitsCoordinates[i].push_back(1);
		}
	}

	//������, �������� � ���� �������� � ������ �������� ����������
	vector<basisElement> basis = CalculateBasis(_limitsCoordinates, coordinates);

	//������, �������� � ���� �������� ��������� ������
	//�� ��� ������������ ���� ��������� � ����������� ���������� ��������
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
	}
}




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

