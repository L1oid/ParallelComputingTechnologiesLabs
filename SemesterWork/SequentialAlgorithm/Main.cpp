#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

int distance(int* arr, int** distances, int n)
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

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int rank;
	int size;
	int min_distance = INT_MAX;
	int count_permutations = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int n;
	cout << "Enter number of cities: ";
	cin >> n;

	double start_time = MPI_Wtime();

	int* way = new int[n];
	for (int i = 0; i < n; i++)
	{
		way[i] = i;
	}

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

	do
	{
		count_permutations++;
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

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << distances[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	double end_time = MPI_Wtime();
	cout << "Time: " << end_time - start_time << endl;
	cout << "Count of permutations: " << count_permutations << endl;
	cout << "Minimum way distance: " << min_distance << endl;
	cout << "Way: ";
	for (int i = 0; i < n; i++)
	{
		cout << min_way[i] << " ";
	}
	cout << endl;
	MPI_Finalize();
}