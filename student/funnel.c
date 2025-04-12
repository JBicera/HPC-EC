#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>


// Use HeapNode to keep track of smallest value of all subarrays
typedef struct {
    keytype value;
    long subIdx;
    long pos;
} HeapNode;

// Swap Helper Function
void swap(HeapNode* x, HeapNode* y) {
    HeapNode temp = *x;
    *x = *y;
    *y = temp;
}

// Heapify Function
void minHeapify(HeapNode heap[], long heapSize, long i) {
    long smallest = i;
    long left = 2 * i + 1;
    long right = 2 * i + 2;
    
    if (left < heapSize && compare(&heap[left].value, &heap[smallest].value) < 0)
        smallest = left;
    if (right < heapSize && compare(&heap[right].value, &heap[smallest].value) < 0)
        smallest = right;
    
    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        minHeapify(heap, heapSize, smallest);
    }
}

// Build minHeap function
void buildMinHeap(HeapNode heap[], long heapSize) {
    for (long i = (heapSize - 2) / 2; i >= 0; i--)
        minHeapify(heap, heapSize, i);
}

// Merges sorted subarrays using a min-heap and recursion
void kWayFunnelMerge(keytype* in, keytype* out, long N, long k, long subSize) {
    // Base Case: Copy input to output and return
    if (k <= 1 || N <= subSize) 
    {
        if (in != out)
            memcpy(out, in, N * sizeof(keytype));
        return;
    }

    long m = 12; // How many sorted subarrays are merged in one step, can be changed

    long newSubSize = subSize * m;
    long newK = (k + m - 1) / m;

    // Build a new heap at each level of recursion for the current group of subarrays
    for (long i = 0; i < newK; i++) 
    {
        long groupStart = i * newSubSize;
        long groupEnd = (groupStart + newSubSize < N) ? groupStart + newSubSize : N;
        long groupK = ((i * m + m) <= k) ? m : (k - i * m);

        if (groupK > 0) 
        {
            HeapNode* heap = (HeapNode*)malloc(groupK * sizeof(HeapNode));
            long heapSize = 0;

            // Initialize heap to keep track of first values from each subarray
            for (long j = 0; j < groupK; j++) 
            {
                long subStart = groupStart + j * subSize;
                if (subStart < groupEnd) 
                {
                    heap[heapSize].value = in[subStart];
                    heap[heapSize].subIdx = j;
                    heap[heapSize].pos = 0;
                    heapSize++;
                }
            }

            buildMinHeap(heap, heapSize);

            long idx = groupStart;

            // Merging loop
            while (heapSize > 0) 
            {
                HeapNode minNode = heap[0]; // Get the smallest value
                out[idx++] = minNode.value; // Put in output array
                long newPos = minNode.pos + 1; // Next index in this subarray
                long subStart = groupStart + minNode.subIdx * subSize;

                // Check if subarray has more elements
                if ((subStart + newPos < groupEnd) && (newPos < subSize)) 
                {
                    // Update heap root with next value of same subarray
                    heap[0].value = in[subStart + newPos]; 
                    heap[0].pos = newPos; // Update position
                } 
                // If subarray is done, remove heap node by replacing with last elmeent of heap
                else 
                {
                    heap[0] = heap[heapSize - 1];
                    heapSize--;
                }

                // Re-heapify the min-heap after modifying it
                minHeapify(heap, heapSize, 0);
            }

            free(heap);
        }
    }
    // Call the recursive merge again with bigger subarrays and new k value
    kWayFunnelMerge(out, in, N, newK, newSubSize);
}

// Main function to sort an array using funnel sort
void funnelSort(long N, keytype* A) 
{
    // Quicksort if the input is that small
    if (N <= 1000) 
    {
        quickSort(N, A);
        return;
    }

    long k = pow(N, 1.0 / 3.0);
    if (k < 1) k = 1;
    long subSize = (N + k - 1) / k;

    // Individually sort N^(1/3) subarrays
    for (long i = 0; i < k; i++) 
    {
        long start = i * subSize;
        long length = (start + subSize < N) ? subSize : (N - start);
        quickSort(length, &A[start]);
    }

    // Use persistent memory buffer for merging
    keytype* temp = (keytype*)malloc(N * sizeof(keytype));
    memcpy(temp, A, N * sizeof(keytype));

    // Call recursive merge to merge the separate sorted subarrays
    kWayFunnelMerge(temp, A, N, k, subSize);

    free(temp);
}
