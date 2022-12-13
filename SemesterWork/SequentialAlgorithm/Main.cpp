#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int distance(int* arr, int **distances, int n)
{
	int distance = distances[0][arr[0]] + distances[arr[n - 1]][0];
	for (int i = 1; i < n; i++)
	{
		distance += distances[arr[i - 1]][arr[i]];
	}
	return distance;
}

void swap(int* arr, int i, int j)
{
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

bool permutations(int* arr, int lf, int n)
{
	int rt = n - 1, i = rt - 1;
	while (i >= lf && arr[i] >= arr[i + 1])
	{
		i--;
	}
	if (i < lf)
	{
		return false;
	}
	int j = rt;
	while (arr[i] >= arr[j])
	{
		j--;
	}
	swap(arr, i, j);
	lf = i + 1;
	while (lf < rt)
	{
		swap(arr, lf++, rt--);
	}
	return true;
}

int main()
{
	int n;
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");


	cout << "Введите количество городов: ";
	cin >> n;
	cout << endl;
	int* way = new int[n];
	for (int i = 0; i < n; i++)
	{
		way[i] = i;
	}

	int min_distance = INT_MAX;
	int* min_way = new int[n];
	for (int i = 0; i < n; i++)
	{
		min_way[i] = 0;
	}

	int** distances = new int* [n];
	distances[0] = new int[n * n];
	for (int i = 1; i < n; i++)
	{
		distances[i] = distances[i - 1] + n;
	}


	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			distances[i][j] = 1 + rand() % (9 - 1 + 1);
			if (i == j)
			{
				distances[i][j] = 0;
			}
			else
			{
				distances[j][i] = distances[i][j];
			}
		}
	}


	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << distances[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	
	do
	{
		int len_distance = distance(way, distances, n);
		if (len_distance < min_distance)
		{
			min_distance = len_distance;
			for (int i = 0; i < n; i++)
			{
				min_way[i] = way[i];
			}
		}
	} while (permutations(way, 1, n));
	cout << "Расстояние минимального пути: " << min_distance << endl;
	cout << "Путь: ";
	for (int i = 0; i < n; i++)
	{
		cout << min_way[i] << " ";
	}
	cout << endl;
}