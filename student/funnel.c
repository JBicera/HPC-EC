#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

// Recursively perform k-way funnel merge
void kWayFunnelMerge(keytype* in, keytype* out, long N, long k, long subSize)
{
    // Base Case: Nothign left to merge, copy to out and return
    if (k <= 1 || N <= subSize) {
        if (in != out)
            memcpy(out, in, N * sizeof(keytype));
        return;
    }

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

        // If there is a right subarray to merge with
        if (mid < end) 
        {
            long left = start;
            long right = mid;
            long idx = start;

            // Standard two-way merge
            while (left < mid && right < end) 
            {
                if (compare(&in[left], &in[right]) <= 0)
                    out[idx++] = in[left++];
                else
                    out[idx++] = in[right++];
            }
            // Copy remaining
            while (left < mid)
                out[idx++] = in[left++];
            while (right < end)
                out[idx++] = in[right++];
        } 
        else
            // If there is no right subarray, just copy over the left subarray
            memcpy(&out[start], &in[start], (end - start) * sizeof(keytype));
    }

    // Recursively merge thew newK sorted subarrays
    kWayFunnelMerge(out,in, N, newK, newSubSize); // Have out as new input
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
    
    // Allocate a persistent temporary buffer for merging
    keytype* temp = (keytype*)malloc(N * sizeof(keytype));
    memcpy(temp, A, N * sizeof(keytype));


    // Call recursive based k-way merger
    kWayFunnelMerge(temp, A, N, k, subSize);

    free(temp);
}
