#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 999999
#define KERNEL 256//kernel 大小
int global[KERNEL];//盛放kernel的输入输出


void kernel(int A[], int start, int end)
{
	//冒泡排序
	int len = end - start + 1;
	for (int i = 1; i < len; i++)//比较len-1次
		for (int j = start; j <= end - i; j++)
			if (A[j] > A[j + 1])
			{
				int temp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = temp;
			}
}

int search(int nums[], int start, int end, int target) {
	int left = start;
	int right = end;
	while (left <= right)
	{
		int mid = (left + right) / 2;
		if (nums[mid] == target)
		{
			do
			{
				mid++;
			} while (mid <= end && nums[mid] == target);
			return mid - 1;
		}
		if (nums[mid] < target)
			left = mid + 1;
		if (nums[mid] > target)
			right = mid - 1;
	}
	return -1;
}

void kernel_merge(int A[], int B[], int start, int  mid, int end)
{

	int i, j, k;
	i = start;
	j = mid + 1;
	k = start;

	while (i <= mid && j <= end)
	{
		if ((i + KERNEL / 2 - 1) <= mid && (j + KERNEL / 2 - 1) <= end)
		{
			int g_index = 0;
			for (; g_index < KERNEL / 2; g_index++)
				global[g_index] = A[i+ g_index];
			for (g_index = 0; g_index < KERNEL/2; g_index++)
				global[g_index +KERNEL/2] = A[j + g_index];

			kernel(global, 0, KERNEL - 1);

			int middle_element = global[KERNEL / 2 - 1];
			int fetch_size = KERNEL / 2;
			while (global[fetch_size] == middle_element && fetch_size <KERNEL)
				fetch_size++;
			middle_element = global[fetch_size -1];

			int new_i = search(A, i, i + KERNEL/2  - 1, middle_element);
			int new_j;

			if (new_i == -1)
			{
				new_j = search(A, j, j + KERNEL/2  - 1, middle_element);
				new_i = fetch_size - (new_j - j + 1) + i - 1;
			}
			else
			{
				new_j = fetch_size - (new_i - i + 1) + j - 1;
			}
			new_i += 1;
			new_j += 1;

			i = new_i;
			j = new_j;

			for (g_index = 0; g_index < fetch_size; g_index++)
				B[k++] = global[g_index];
		}
		else
		{
			if (A[i] < A[j])
				B[k++] = A[i++];
			else
				B[k++] = A[j++];
		}
	}

	if (i <= mid)
		while (i <= mid)
			B[k++] = A[i++];
	else
		while (j <= end)
			B[k++] = A[j++];
	for (i = start; i <= end; i++)
		A[i] = B[i];
}

void kernel_mergesort(int A[], int B[], int start, int end)
{
	if (end - start + 1 > KERNEL)
	{
		int mid = (start + end) / 2;
		kernel_mergesort(A, B, start, mid);
		kernel_mergesort(A, B, mid + 1, end);
		kernel_merge(A, B, start, mid, end);//mid包含在前面的数组啊
		/*for (int ii = 0; ii < N; ii++)
			printf("%d ",A[ii]);
		printf("\n");*/
	}
	else
	{
		kernel(A, start, end);
		/*for (int ii = 0; ii < N; ii++)
			printf("%d ", A[ii]);
		printf("\n");*/
	}
}

void main()
{
	srand((unsigned)time(NULL));
	int* A = (int*)malloc(sizeof(int) * N);
	int* B = (int*)malloc(sizeof(int) * N);
	for (int i = 0; i < N; i++)
		A[i] = rand() % N;
	/*for (int i = 0; i < N; i++)
		printf("%d ", A[i]);
	printf("数据是\n");*/
	double time1 = (double)clock() / CLOCKS_PER_SEC;

	kernel_mergesort(A, B, 0, N - 1);

	double time2 = (double)clock() / CLOCKS_PER_SEC;
	printf("%lf\n", double(time2 - time1));

	/*for (int i = 0; i < N; i++)
		printf("%d ", A[i]);*/
	free(A);
	free(B);
}




