#include "sort_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>

// If keytype is unsigned long, define a maximal constant:
#define MAX_KEY ULONG_MAX

void kWayFunnelMerge(keytype* arr, long N, long k, long subSize) {
    keytype *output = (keytype *)malloc(N * sizeof(keytype));
    if (!output) {
        fprintf(stderr, "malloc failed in kWayFunnelMerge\n");
        exit(EXIT_FAILURE);
    }

    long *indices = (long *)calloc(k, sizeof(long));
    if (!indices) {
        fprintf(stderr, "calloc failed in kWayFunnelMerge\n");
        free(output);
        exit(EXIT_FAILURE);
    }

    long outputIndex = 0;

    while (outputIndex < N) {
        keytype minVal = MAX_KEY;  // Use MAX_KEY macro to initialize
        long minIndex = -1;

        // Loop over k subarrays and find the minimum element among the current indices.
        for (long i = 0; i < k; i++) {
            long idx = indices[i];
            long subStart = i * subSize;
            if (idx < subSize && (subStart + idx) < N) {
                // Compare pointers to the keys.
                if (compare(&arr[subStart + idx], &minVal) < 0) {
                    minVal = arr[subStart + idx];
                    minIndex = i;
                }
            }
        }

        if (minIndex != -1) {
            output[outputIndex++] = minVal;
            indices[minIndex]++;
        }
    }

    // Efficient copy back to arr (alternative: for loop if keytype is simple)
    memcpy(arr, output, N * sizeof(keytype));

    free(output);
    free(indices);
}

void funnelSort(long N, keytype* A) {
    // Base case: For small arrays, use quicksort directly.
    if (N <= 1000) {
        quickSort(N, A);
        return;
    }

    // Compute number of subarrays (k) as the cube root of N.
    long k = (long) pow(N, 1.0 / 3.0);
    if (k < 1) k = 1;

    // Compute subSize: ceiling division to cover all elements.
    long subSize = (N + k - 1) / k;

    // Step 1: Sort each subarray using quickSort.
    for (long i = 0; i < k; i++) {
        long start = i * subSize;
        long length = ((start + subSize) < N) ? subSize : (N - start);
        quickSort(length, &A[start]);
    }

    // Step 2: Merge all sorted subarrays using k-way merger.
    kWayFunnelMerge(A, N, k, subSize);
}
