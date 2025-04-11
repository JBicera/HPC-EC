#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

// Recursively perform k-way funnel merge
void kWayFunnelMerge(keytype* arr, long N, long k, long subSize) 
{
    // Base Case: Nothign left to merge
    if (k <= 1 || N <= subSize) 
        return;

    // New size as you merge two subarrays
    long newSubSize = subSize * 2;
    long newK = (k + 1) / 2;

    // Iterate through new number of subarrays
    for (long i = 0; i < newK; i++) 
    {
        // Compute start, mid, and end indices
        long start = i * newSubSize;
        long mid = start + subSize;
        long end = ((start + newSubSize) < N) ? (start + newSubSize) : N;

        // If the right half exists, merge
        if (mid < end) 
        {
            // Allocate temporary array to hold the result
            keytype* temp = (keytype*)malloc((end - start) * sizeof(keytype));
            long left = start;
            long right = mid;
            long idx = 0;

            // Merge elements from both subarrays and put in temp
            while (left < mid && right < end) 
            {
                if (compare(&arr[left], &arr[right]) <= 0)
                    temp[idx++] = arr[left++];
                else
                    temp[idx++] = arr[right++];
            }
            // Copy the leftover elements
            while (left < mid) temp[idx++] = arr[left++];
            while (right < end) temp[idx++] = arr[right++];

            // Copy the sorted temp result back into the original array
            memcpy(&arr[start], temp, (end - start) * sizeof(keytype));

            free(temp); 
        }
    }

    // Recursively merge the now larger subarrays 
    kWayFunnelMerge(arr, N, newK, newSubSize);
}

// Recursively sort subarrays then perform a k-way funnel merge
void funnelSort(long N, keytype* A) 
{
    // Small enough subarray, quickSort is better
    if (N <= 1000) 
    {
        quickSort(N, A);
        return;
    }
    
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
