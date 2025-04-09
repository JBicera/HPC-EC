#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

void kWayFunnelMerge(keytype* arr, long N, long k, long subSize) {
    keytype *output = (keytype *)malloc(N * sizeof(keytype)); // Output buffer for merged results
    long *indices = (long *)calloc(k, sizeof(long)); // Tracks current index in each subarray

    long outputIndex = 0;

    // Merge until all elements are moved into the output array
    while (outputIndex < N) 
    {
        keytype minVal = LONG_MAX; // Current min Value
        long minIndex = -1; // Track subarray of current min Value
        
        // Search all k subarrays for the current minimum element
        for (long i = 0; i < k; i++) 
        {
            long idx = indices[i];
            long subStart = i * subSize;

            // Don't go past subarray or array bounds
            if (idx < subSize && subStart + idx < N) 
            {
                if (compare(&arr[subStart + idx], &minVal) < 0) 
                {
                    minVal = arr[subStart + idx];
                    minIndex = i;
                }
            }
        }

        // Append the smallest element found to the output and move forward in its subarray
        if (minIndex != -1) 
        {
            output[outputIndex++] = minVal;
            indices[minIndex]++;
        }
    }

    // Copy merged output back to the original array
    memcpy(arr, output, N * sizeof(keytype));


    free(output);
    free(indices);
}

// Recursively sort subarrays then k-way funnel merge
void funnelSort(long N, keytype* A) 
{
    if (N <= 1) return; 

    long k = pow(N, 1.0 / 3.0);
    if (k < 1) k = 1;

    long subSize = (N + k - 1) / k;  // Ceiling division to get subarray size

    // Sort each k subarrays independently using quickSort
    for (long i = 0; i < k; i++) 
    {
        long start = i * subSize;
        long length = ((start + subSize) < N) ? subSize : (N - start);
        quickSort(length, &A[start]);
    }

    kWayFunnelMerge(A, N, k, subSize);
}
