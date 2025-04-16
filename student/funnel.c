#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

// Binary merge: Merges two sorted runs: [start, mid) and [mid, end)
static inline void binaryMerge(keytype* in, keytype* out, long start, long mid, long end) 
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

static void iterativeBinaryFunnelMerge(keytype* in, keytype* out, long N, long k, long subSize) {
    keytype* src = in;
    keytype* dst = out;

    long numRuns = k;
    long runSize = subSize;

    while (numRuns > 1) {
        long newNumRuns = (numRuns + 1) / 2;

        for (long i = 0; i < newNumRuns; i++) {
            long start = i * 2 * runSize;
            long mid = start + runSize;
            long end = mid + runSize;

            if (mid > N) mid = N;
            if (end > N) end = N;

            binaryMerge(src, dst, start, mid, end);
        }

        // Swap buffers for next level
        keytype* temp = src;
        src = dst;
        dst = temp;

        numRuns = newNumRuns;
        runSize *= 2;
    }

    // If the final result is not in A, copy it back
    if (src != in) {
        memcpy(in, src, N * sizeof(keytype));
    }
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
    
    // Recursively merge the k sorted runs using the binary funnel merge.
    iterativeBinaryFunnelMerge(A, temp, N, k, subSize);

    
    free(temp);
}
