#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

// Heap node structure for the min-heap.
typedef struct 
{
    keytype value;   // Current element from the subarray
    long subIdx;     // Which subarray this element comes from
    long pos;        // Current index within that subarray
} HeapNode;

// Swap helper function
void swap(HeapNode *x, HeapNode *y) 
{
    HeapNode temp = *x;
    *x = *y;
    *y = temp;
}

// Heapfiy helper function
void minHeapify(HeapNode heap[], long heapSize, long i) {
    long smallest = i;
    long left = 2 * i + 1;
    long right = 2 * i + 2;
    
    if (left < heapSize && compare(&heap[left].value, &heap[smallest].value) < 0)
        smallest = left;
    if (right < heapSize && compare(&heap[right].value, &heap[smallest].value) < 0)
        smallest = right;
    
    if (smallest != i) 
    {
        swap(&heap[i], &heap[smallest]);
        minHeapify(heap, heapSize, smallest);
    }
}

// Build a min-heap from an array of HeapNode
void buildMinHeap(HeapNode heap[], long heapSize) 
{
    for (long i = (heapSize - 2) / 2; i >= 0; i--)
        minHeapify(heap, heapSize, i);
}

// Perform k-way merge using min-heap instead of for loop to scan for smallest
void kWayFunnelMerge(keytype* arr, long N, long k, long subSize) 
{

    keytype *output = (keytype *)malloc(N * sizeof(keytype)); // Output buffer for merged results
    
    HeapNode *heap = (HeapNode *)malloc(k * sizeof(HeapNode)); // Allocate an array of HeapNodes for the min-heap
    
    long heapSize = 0;

    // Initialize the heap with the first element from each subarray to keep track of all 
    for (long i = 0; i < k; i++) 
    {
        long subStart = i * subSize;
        if (subStart < N) 
        {
            heap[heapSize].value = arr[subStart];   
            heap[heapSize].subIdx = i;             
            heap[heapSize].pos = 0;                
            heapSize++;
        }
    }
    
    // Build the initial min-heap
    buildMinHeap(heap, heapSize);
    
    long outputIndex = 0;

    // Merge until all elements are moved into the output array.
    while (heapSize > 0) 
    {
        // Get smallest from root of minHeap
        HeapNode minNode = heap[0];
        output[outputIndex++] = minNode.value;
        
        // Advance the pointer within the subarray from which the smallest element came
        long newPos = minNode.pos + 1;
        long subStart = minNode.subIdx * subSize;

        // If there is another element within bounds of current subarray
        if (subStart + newPos < N && newPos < subSize) 
        {
            // Replace the root with the next element from the same subarray
            heap[0].value = arr[subStart + newPos];
            heap[0].pos = newPos;
        }
        // Reached end of current subarray 
        else 
        {
            // Remove root and replace with last element in heap
            heap[0] = heap[heapSize - 1];
            heapSize--;
        }
        
        // Restore the min-heap property after modifying
        minHeapify(heap, heapSize, 0);
    }
    
    // Copy the merged output back to the original array
    memcpy(arr, output, N * sizeof(keytype));
    
    free(output);
    free(heap);
}

// Recursively sort subarrays then perform a k-way funnel merge
void funnelSort(long N, keytype* A) 
{
    if (N <= 1) return;
    
    // Calculate number of subarrays, k = N^(1/3)
    long k = pow(N, 1.0 / 3.0);
    if (k < 1) 
        k = 1;
    
    // Calculate the size of each subarray 
    long subSize = (N + k - 1) / k;
    
    // Sort each of the k subarrays independently using quickSort
    for (long i = 0; i < k; i++) 
    {
        long start = i * subSize;
        long length = ((start + subSize) < N) ? subSize : (N - start);
        quickSort(length, &A[start]);
    }
    
    // Merge the sorted subarrays using the min-heap based k-way merger.
    kWayFunnelMerge(A, N, k, subSize);
}

