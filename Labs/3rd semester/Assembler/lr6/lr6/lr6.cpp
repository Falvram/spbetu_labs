#include "pch.h"
#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
using namespace std;

#define NUMBER  16000		//максимальное длина числа
#define BORD  24		//максимальное количество интервалов
void get_arr(int & amount, int* & array, int & Xmin, int & Xmax, int & Border, int* & LeftBorder);
void generation(int* & array, int amount, int min, int max);
void res_func1(int Xmin, int* array, int amount);
void res_func2(int Xmax, int* LeftBorder, int* array, int amount);
ofstream fout("res.txt");

extern "C"
{
	void func1(int array[], int amount, int counter[], int Xmin);
	void func2(int With1Range[], int LeftBorder[], int InterDif[], int Border, int Xmin);
}

int main(void) {
	setlocale(LC_ALL, "rus");

	int amount = 0; //кол-во псевдослучайных чисел
	int	Xmin = 0, Xmax = 0, MainRange = 0;
	int	Border = 0; //кол - во границ
	int *array = NULL; 		//массив чисел
	int *LeftBorder = NULL;		 //массив левых границ
	int *RightBorder = NULL; //массив правых границ
	int *counter = NULL;	//для интервалов единичной длины
	int* InterDif = NULL; 	//для интервалов заданной длины

	/*Получение псевдослучайных чисел*/
	get_arr(amount, array, Xmin, Xmax, Border, LeftBorder);

	/*Создание массива правых границ, исходя из
	массива левых границ и граничного значения Xmax*/
	RightBorder = new int[Border];
	for (int i = 0; i < Border - 1; i++)
		RightBorder[i] = LeftBorder[i + 1] - 1;

	RightBorder[Border - 1] = Xmax;

	MainRange = Xmax - Xmin + 1;
	counter = new int[MainRange] {0};
	InterDif = new int[Border] {0};
	func1(array, amount, counter, Xmin); //распределение по ед. интервалам 
	func2(counter, RightBorder, InterDif, Border, Xmin); //распределение по опр. интервалам 
	res_func1(Xmin, counter, MainRange); 	//вывод результата первой процедуры
	res_func2(Xmax, LeftBorder, InterDif, Border); 	//вывод результата второй процедуры

	system("pause");
	return 0;
}

void get_arr(int & amount, int *&ArrNumber, int &Xmin, int &Xmax, int &Border, int *&LeftBorder)
{
	do {
		cout << "Введите количество случайных чисел, 0 < N <= " << NUMBER << ": ";
		cin >> amount;
		if (amount <= 0 || amount > NUMBER)
			cout << "\nОшибка диапазона!\n\n";
	} while (amount <= 0 || amount > NUMBER);
	ArrNumber = new int[amount];
	do {
		cout << "\nВведите диапазон случайных чисел: \n" << "	от: ";  cin >> Xmin;
		cout << "	до :";  cin >> Xmax;
		if (Xmax <= Xmin)
			cout << "\nНеверное задание границ! Повторите попытку.\n\n";
	} while (Xmax <= Xmin);
	generation(ArrNumber, amount, Xmin, Xmax);

	do {
		cout << "\nВведите количество интервалов разбиения заданного диапазона ( 0 < N <= " << BORD << "): ";
		cin >> Border; cout << endl;
		if (Border <= 0 || Border > BORD)
			cout << "\nОшибка: количество интервалов не входит в указанный диапазон!Повторите попытку.\n";
	} while (Border <= 0 || Border > BORD);

	LeftBorder = new int[Border];
	cout << "\nВвод интервалов по возрастанию (1-ый интервал равен левой границе)\n";
	LeftBorder[0] = Xmin;	//левая граница
	cout << "Граница 1: " << Xmin << "\n";
	int tmp = 0;
	for (int i = 1; i < Border; i++)
	{
		do {
			cout << "Граница " << i + 1 << ": ";
			cin >> tmp;
			if (tmp <= LeftBorder[i - 1] || tmp >= Xmax)
			{
				cout << "\n\nВыход за пределы диапазона!\n\n";
			}
			else
			{
				LeftBorder[i] = tmp;
				break;
			}
		} while (true);
	}
}

void generation(int* & array, int len, int min, int max)
{
	random_device rd; // класс, который описывает результаты, равномерно распределенные в замкнутом диапазоне [0, 2^32).
	mt19937 gen(rd());
	uniform_int_distribution<> distr(min, max); //формирует равномерное распределение целых чисел в заданном интервале
	for (int i = 0; i < len; i++) {
		array[i] = distr(gen);
	}
}

void res_func1(int Xmin, int* counter, int amount)
{
	cout << "\n***Распределение случайных чисел по интервалам единичной длины***\n"; fout << "\n***Распределение случайных чисел по интервалам единичной длины***\n";
	cout << "Число\t|Кл-во\n"; fout << "№\tЧисло\tКл-во\n";


	for (int i = 0; i < amount; i++) {
		cout << Xmin + i << "\t|" << counter[i] << '\n'; fout << i + 1 << '\t' << Xmin + i << "\t" << counter[i] << '\n';
	}
}


void res_func2(int Xmax, int* LeftBorder, int* InterDif, int amount)
{

	cout << "\n***Распределение случайных чисел по заданным интервалам***\n"; fout << "\n***Распределение случайных чисел по заданным интервалам***\n";
	cout << "№\t|[левая;правая гр]\t|Количество\t\n"; fout << "№\t|[левая;правая гр]\t|Количество\t\n";
	for (int i = 0; i < amount; i++)
	{
		cout << i << "\t" << LeftBorder[i] << "\t\t"; fout << i << "\t" << LeftBorder[i] << "\t\t";
		if (i == amount - 1)
		{
			cout << Xmax; fout << Xmax;
		}
		else
		{
			cout << LeftBorder[i + 1] - 1; fout << LeftBorder[i + 1] - 1;
		}

		cout << "\t\t" << InterDif[i] << '\n'; fout << "\t\t" << InterDif[i] << '\n';

	}
}
