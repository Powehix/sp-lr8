#include<windows.h>
#include<iostream>
#include<time.h>

using namespace std;

const int array_size = 500;
const int elements_array = 10;

struct Structure
{
	int array_int[array_size];
	char array_char[array_size];
	float array_float[array_size];
};

int main()
{
	//5-� �������

	setlocale(LC_ALL, "Russian");

	// ����������� ������������� ��������� � ������� �������� ����������� ������
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	cout << "������������� ���������: " << si.dwAllocationGranularity << endl;
	cout << "������ �������� ����������� ������: " << si.dwPageSize << endl;

	//�������� ������� short � ���������� ��� ���������� �������
	srand(time(NULL));
	short array_short[elements_array];
	cout << "\n\nShort: { ";
	for (int i = 0; i < elements_array; i++)
	{
		array_short[i] = 1 + rand() % 9;
		cout << array_short[i] << " ";
	}
	cout << "}\n\n";

	//�������� ������� �������� � ��� ����������
	Structure array_structure[elements_array];
	for (int i = 0; i < elements_array; i++)
	{
		array_structure[i].array_int[array_size - 2] = i;
		array_structure[i].array_float[array_size - 2] = i;
		array_structure[i].array_char[array_size - 2] = char(65 + i);
	}

	//������ ����������� ����������� ������
	int bytes_array_short = sizeof(array_short) * elements_array;
	int bytes_array_structure = sizeof(array_structure) * elements_array;
	int total_bytes = bytes_array_short + bytes_array_structure;
	
	//���������� ������
	int pages = 0;
	short *a;
	a = (short*)VirtualAlloc(NULL, total_bytes, MEM_RESERVE, PAGE_READWRITE);
	if (!a)
	{
		cout << "������!" << endl;
		system("PAUSE");
		return GetLastError();
	}
	cout << "����� ����������� ������: " << a << endl;
	short *b = a;

	//������ ������� short
	for (int i = 0; i < elements_array; i++)
	{
		__try 
		{
			*b = array_short[i];
			b++;
		}

		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			cout << "����������!" << endl;
			cout << "���������: " << b << " ; ��������: " << i << " �� " << elements_array << endl;
			b = (short*)VirtualAlloc(b, si.dwPageSize, MEM_COMMIT, PAGE_READWRITE);
			cout << "����� ��������: " << b << endl;
			pages++;
			cout << "����������� ������ ���� ���������!" << endl;
			i--;
		}
	}
	cout << endl;

	//������ ������� array_structure
	Structure *c = (Structure*)b;
	for (int i = 0; i < elements_array; i++)
	{
		__try
		{
			*c = array_structure[i];
			c++;
			char* temp = (char*)c;
			temp += array_short[i];
			c = (Structure*)temp;
		}

		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			cout << "����������!" << endl;
			cout << "���������: " << c << " ; ��������: " << i << " �� " << elements_array << endl;
			Structure *d = (Structure*)VirtualAlloc(c + 1, sizeof(Structure), MEM_COMMIT, PAGE_READWRITE);
			cout << "����� ��������: " << c << endl;
			pages++;
			cout << "����������� ������ ���� ���������!" << endl;
			i--;
		}
	}
	cout << "\n����� ���������� �������: " << pages;

	//����� ������� short
	b = a;
	short array_short_input[elements_array];
	cout << "\n\nShort_input: { ";
	for (int i = 0; i < elements_array; i++)
	{
		array_short_input[i] = *b;
		cout << array_short_input[i] << " ";
		b++;
	}
	cout << "}\n\n";

	//����� ������� �������� (������������� ��������)
	c = (Structure*)b;
	Structure array_structure_input[10];
	cout << "Structure_input:";
	cout << "\n\nstructure\tint[498]\tfloat[498]\tchar[498]\n";
	cout << "___________________________________________________________\n";
	for (int i = 0; i < elements_array; i++)
	{
		Structure *array_structure_input = c;
		cout << i << ". \t\t";
		cout << array_structure_input->array_int[array_size - 2] << "\t\t";
		cout << array_structure_input->array_float[array_size - 2] << "\t\t";
		cout << array_structure_input->array_char[array_size - 2] << endl;
		cout << "___________________________________________________________\n";
		c++;
		char* temp = (char*)c;
		temp += array_short_input[i];
		c = (Structure*)temp;
	}

	//������������ ������
	if (!VirtualFree(a, 0, MEM_RELEASE))
	{
		cout << "������!" << endl;
		system("PAUSE");
		return GetLastError();
	}

	system("PAUSE");
	return 0;
}