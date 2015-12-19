// SimplexMethod.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <vector>

using namespace std;


//��������� ��� �������� ���������� ����������
//������������(�������������) �������� ��������-�������
struct SimplexResult
{
	//������(������ ���������� �����), �������� � ���� �����������
	//�������� ��������� �������� �������
	vector<float> CoordnatesValues; 
	//����������� �������� ������� �������
	float ResultValue;
	//���������� ����������, ������� ����� false, ���� ���������� ������ �������,
	//��� true, ���� ��� ������� ���������� ����� �������(��������)
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

//������� ���������� ������������ �������� ��������-�������
//coordinates - ������ ������������� ��� ���������� � ������� �������
//limitationsCoordinates - ������� ������������� � ������������ ��� ���������� ������������ ��������
//results - ������ �������� �����������
//type - ��� ������ (������� - true, �������� - false) 
SimplexResult SimplexMethod(vector<float> coordinates,
							vector<vector<float> > limitsCoordinates,
							vector<float> results,
							bool type)
{
	//������, ���������� � ���� �������� �������� �����
	//���������� ��� ������ �� ����� ���������
	vector<float> basicPlan(coordinates.size(),0);
	//���������� � ������� ���� �������� �����������
	for (float x : results)
	{
		basicPlan.push_back(x);
	}
	//������, �������� � ���� �������� � ������ �������� ����������
	vector<basisElement> basis(results.size());
	//������, ���������� �������� ������������� ����������� ������ � ���������� �����������
	vector<vector<float> > _limitsCoordinates;
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
	for (int i = 0; i < limitsCoordinates.size(); i++)
	{
		for (int j = 0; j < limitsCoordinates[i].size(); j++)
		{
			_limitsCoordinates[i].push_back(limitsCoordinates[i][j]);
		}
		//���������� ��������� ����������
		for (int j = 0; j < results.size(); j++)
		{
			if (i != j)
				_limitsCoordinates[i].push_back(0);
			else
				_limitsCoordinates[i].push_back(1);
		}
	}
	//������, �������� � ���� �������� ��������� ������
	//�� ��� ������������ ���� ��������� � ����������� ���������� ��������
	vector<float> indexRow = CalculateIndexes(basis, coordinates, limitsCoordinates)

}




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

