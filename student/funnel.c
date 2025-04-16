#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

// Binary merge: Merges two sorted runs: [start, mid) and [mid, end)
void binaryMerge(keytype* in, keytype* out, long start, long mid, long end) 
{
    long i = start, j = mid, k = start;
    while (i < mid && j < end) {
        if (compare(&in[i], &in[j]) <= 0)
            out[k++] = in[i++];
        else
            out[k++] = in[j++];
    }
    while (i < mid)
        out[k++] = in[i++];
    while (j < end)
        out[k++] = in[j++];
}

// Recursive binary merge function (binary funnel merge)
void binaryFunnelMerge(keytype* in, keytype* out, long N, long k, long subSize) 
{
    // Base case: If there is only one sorted run, copy if necessary and return.
    if (k <= 1) 
    {
        if (in != out)
            memcpy(out, in, N * sizeof(keytype));
        return;
    }

    long newK = (k + 1) / 2; // newK: number of runs after pairwise merging

    // For each pair of runs, merge them
    for (long i = 0; i < newK; i++) 
    {
        long start = i * 2 * subSize;
        long mid = start + subSize;
        long end = mid + subSize;
        if (mid > N) 
            mid = N;
        if (end > N)
            end = N;
        // Merge the pair [start, mid) and [mid, end) into out
        binaryMerge(in, out, start, mid, end);
        // If a run has no partner (k is odd), binaryMerge simply copies it
    }
    
    // Recursion with new subarrays, swapping in and out
    binaryFunnelMerge(out, in, N, newK, subSize * 2); // Pass new K and subSize
}

// Funnel sort main function
void funnelSort(long N, keytype* A) 
{
    // Base case: For small arrays, use quickSort.
    if (N <= 1000) 
    {
        quickSort(N, A);
        return;
    }
    
    // Determine the number of subarrays: N^(1/3)
    long k = pow(N, 1.0 / 3.0);
    if (k < 1)
        k = 1;

    // Compute subarray size (using ceiling division)
    long subSize = (N + k - 1) / k;
    
    // Sort each of the k subarrays individually using quickSort
    for (long i = 0; i < k; i++) 
    {
        long start = i * subSize;
        long length = (start + subSize < N) ? subSize : (N - start);
        quickSort(length, &A[start]);
    }
    
    // Allocate temp buffer for merging
    keytype* temp = (keytype*)malloc(N * sizeof(keytype));
    memcpy(temp, A, N * sizeof(keytype));
    
    // Recursively merge the k sorted runs using the binary funnel merge.
    binaryFunnelMerge(temp, A, N, k, subSize);
    
    free(temp);
}
