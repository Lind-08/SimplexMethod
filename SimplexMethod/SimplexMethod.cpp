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

//������� ���������� ������������ �������� ��������-�������
//coordinates - ������ ������������� ��� ���������� � ������� �������
//limitationsCoordinates - ������� ������������� � ������������ ��� ���������� ������������ ��������
//results - ������ �������� �����������
//type - ��� ������ (������� - true, �������� - false) 
SimplexResult SimplexMethod(vector<float> coordinates,
							vector<vector<float> > limitationsCoordinates,
							vector<float> results,
							bool type)
{
	//������, �������� � ���� �������� ��������� ������
	//�� ��� ������������ ���� ��������� � ����������� ���������� ��������
	vector<float> indexRow;
	//������, �������� � ���� ������ ����������, �������� � �����
	vector<int> basis;
	//������, ���������� � ���� �������� �������� �����
	vector<float> basicPlan;

}




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

