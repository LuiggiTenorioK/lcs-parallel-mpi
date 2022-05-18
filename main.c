#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#define TAG 21

void load_input(char *filename, char **s1ptr, char **s2ptr, int rank)
{

	int l1, l2;
	char *s1, *s2;
	FILE *file;

	if (rank == 0)
	{
		file = fopen(filename, "r");
		fscanf(file, "%d\n%d\n", &l1, &l2);
	}

	MPI_Bcast(&l1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&l2, 1, MPI_INT, 0, MPI_COMM_WORLD);

	s1 = malloc(l1 + 1);
	s2 = malloc(l2 + 1);

	if (rank == 0)
	{
		for (int i = 0; i < l1; i++)
			s1[i] = fgetc(file);
		s1[l1] = '\0';
		if (fgetc(file) != '\n')
		{
			printf("error in input file format");
			exit(1);
		}
		for (int i = 0; i < l2; i++)
			s2[i] = fgetc(file);
		s2[l2] = '\0';
	}

	MPI_Bcast(&s1[0], l1, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s2[0], l2, MPI_CHAR, 0, MPI_COMM_WORLD);

	*s1ptr = s1;
	*s2ptr = s2;
}

int main(int argc, char **argv)
{
	int rank, nproc;
	double start_time, finish_time, duration;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char *s1, *s2;
	load_input(argv[1], &s1, &s2, rank);
	int l1 = strlen(s1);
	int l2 = strlen(s2);

	if (rank == 0){
		printf("input %s %s\n", s1, s2);
	}

	MPI_Status *status;

	int recvfrom = (nproc + rank - 1) % nproc;
	int sendto = (rank + 1) % nproc;

	start_time = MPI_Wtime();

	for (int j = rank; j < l2; j += nproc)
	{
		int curRow = 0;
		int *prevCol;
		int *curCol;
		prevCol = malloc(l1 * sizeof(int));
		curCol = malloc(l1 * sizeof(int));

		for (int i = 0; i < l1; i++)
		{
			if (j > 0)
			{
				// printf("task %i: Try to receive from task %i\n", rank, recvfrom);
				MPI_Recv(&prevCol[i], 1, MPI_INT, recvfrom, TAG, MPI_COMM_WORLD, status);
				// printf("task %i: I have received %i from task %i\n", rank, prevCol[i], recvfrom);
			}

			if (s1[i] == s2[j])
			{
				curCol[i] = 1;
				if (j > 0 && i > 0)
					curCol[i] += prevCol[i - 1];
			}
			else
			{
				curCol[i] = curRow;
				if (j > 0 && prevCol[i] > curRow)
					curCol[i] = prevCol[i];
			}

			if (j < l2 - 1)
			{
				MPI_Send(&curCol[i], 1, MPI_INT, sendto, TAG, MPI_COMM_WORLD);
				// printf("task %i: I'm sending %i to task %i\n", rank, curCol[i], sendto);
			}
			else if (i==l1-1)
			{
				finish_time = MPI_Wtime();
				duration = finish_time - start_time;
				printf("task %i: Final column item %i is: %i\n", rank, i, curCol[i]);
				printf("Completed in %f ms \n", duration*1000);
			}

			curRow = curCol[i];
		}
	}

	MPI_Finalize();
	return 0;
}
