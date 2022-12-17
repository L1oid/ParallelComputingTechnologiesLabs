#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#include <Windows.h>

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
	int global_min_distance = INT_MAX;
	double start_time, end_time;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int n;
	if (rank == 0)
	{
		cout << "Enter number of cities: ";
		cin >> n;
		start_time = MPI_Wtime();
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

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

	int* min_way = new int[n];
	for (int i = 0; i < n; i++)
	{
		min_way[i] = 0;
	}

	int** way = new int* [n - 1];
	way[0] = new int[(n - 1) * n];
	for (int i = 1; i < n - 1; i++)
	{
		way[i] = way[i - 1] + n;
	}
	for (int i = 0; i < n - 1; i++)
	{
		way[i][0] = 0;
	}
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 1; j < n; j++)
		{
			if (j == 1)
			{
				way[i][j] = i + 1;
			}
			else if (way[i][1] == j)
			{
				way[i][j] = j - 1;
			}
			else if (j < i + 1)
			{
				way[i][j] = j - 1;
			}
			else if (j > i + 1)
			{
				way[i][j] = j;
			}
		}
	}

	for (int i = rank + 1; i < n - 1; i += size)
	{
		do
		{
			int len_distance = distance(way[i], distances, n);
			if (len_distance < min_distance)
			{
				min_distance = len_distance;
				for (int j = 0; j < n; j++)
				{
					min_way[j] = way[i][j];
				}
			}
		} while (permutations(way[i], 2, n));
	}

	MPI_Allreduce(&min_distance, &global_min_distance, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

	if (rank == 0)
	{
		cout << "Distance matrix:" << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << distances[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		if (min_distance != global_min_distance)
		{
			MPI_Recv(min_way, n, MPI_INT, MPI_ANY_SOURCE, 777, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		end_time = MPI_Wtime();
		cout << "Time: " << end_time - start_time << endl;
		cout << "Minimum way distance: " << min_distance << endl;
		cout << "Way: ";
		for (int i = 0; i < n; i++)
		{
			cout << min_way[i] << " ";
		}
		cout << endl;
	}
	else
	{
		if (min_distance == global_min_distance)
		{
			MPI_Send(min_way, n, MPI_INT, 0, 777, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
}