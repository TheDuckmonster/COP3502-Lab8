#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;


//Heap function (Max Heap)
//Adjusts a node (index) upward so that it maintains heap rules
void percolateUp(int heap[], int n, int index)
{
	//converts heap indices to array indices
	int node = index - 1;
	int parent = index/2 - 1;

	while(parent != -1 && heap[node] > heap[parent])
	{
		//swap
		int temp = heap[parent];
		heap[parent] = heap[node];
		heap[parent] = temp;

		//set up next node index
		index = index/2;
		node = index - 1;
		parent = index/2 - 1;

	}
}

//Heap function (Max Heap)
//Adjusts a node (index) downward so that it maintains heap rules
void percolateDown(int heap[], int n, int index)
{
	//converts heap indices to array indices
	int node = index - 1;
	int childL = index*2 - 1;
	int childR = index*2;
	int temp;

	while (childL < n) //continues as long as at least one child exists
	{
		
		if (childR < n) //if both children exist and at least one is bigger than node, swap with largest child
		{
			if (heap[childL] >= heap[childR] && heap[childL] > heap[node]) //Swap with left child
			{
				temp = heap[childL];
				heap[childL] = heap[node];
				heap[node] = temp;

				node = childL;
				childL = (node + 1)*2 - 1;
				childR = (node + 1)*2;
			}
			else if (heap[childR] > heap[childL] && heap[childR] > heap[node]) //Swap with right child
			{
				temp = heap[childR];
				heap[childR] = heap[node];
				heap[node] = temp;

				node = childR;
				childL = (node + 1)*2 - 1;
				childR = (node + 1)*2;
			}
			else //break if neither child is larger
				break;
		}
		else //if only the left child exists, swap if larger than node
		{
			if (heap[childL] > heap[node]) //Swap with left child then break
			{
				temp = heap[childL];
				heap[childL] = heap[node];
				heap[node] = temp;

				break;
			}
			else //break if child is not larger than node
				break;
		}

	}
}

//Heap function (Max Heap)
//Turns array into a heap
void heapify(int arr[], int n)
{

	for (int index = n; index > 0; index--)
	{
		percolateDown(arr, n, index);
	}
	
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	heapify(arr, n);

	for (int i = n - 1; i > 0; i--)
	{
		//Modified deleteMax algorithm
		//swaps head and tail of the heap, then percolates the head value down (ignoring the swapped value)
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		percolateDown(arr, i, 1);
	}
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int pData[], int l, int m, int r)
{
	//initialize temp array and allocate memory
	int* tempArray = (int*) malloc((r-l+1)*sizeof(int));
	extraMemoryAllocated += (r - l + 1)*sizeof(int);

	//Merge sorted lists into temp array
	int indexL = l;
	int indexR = m+1;
	for (int i = 0; i <= r-l; i++)
	{
		if (indexL > m) //indicates that the left array is empty
		{
			tempArray[i] = pData[indexR];
			indexR++;
		}
		else if (indexR > r) //indicates that the right array is empty
		{
			tempArray[i] = pData[indexL];
			indexL++;
		}
		else
		{
			if (pData[indexL] < pData[indexR]) //insert left array value if smaller than right array value
			{
				tempArray[i] = pData[indexL];
				indexL++;
			}
			else //otherwise insert right array value
			{
				tempArray[i] = pData[indexR];
				indexR++;
			}
		}
	}

	//replace values in original array with temp array values
	for (int i = l; i <= r; i++)
	{
		pData[i] = tempArray[i-l];
	}

	free(tempArray);
}


void mergeSort(int pData[], int l, int r)
{
	if (l < r) //if l >= r, then the list is already sorted
	{
		int m = (l + r)/2;
		mergeSort(pData, l, m); //left side
		mergeSort(pData,m+1,r); //right side
		merge(pData, l, m, r); //merge sorted sides
	}
	
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}