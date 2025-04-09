#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <limits.h>

void kWayFunnelMerge(keytype* arr, long N, long k, long subSize) {
    keytype *output = (keytype *)malloc(N * sizeof(keytype));
    long *indices = (long *)calloc(k, sizeof(long));

    long outputIndex = 0;

    while (outputIndex < N) {
        keytype minVal = LONG_MAX;
        long minIndex = -1;

        for (long i = 0; i < k; i++) {
            long idx = indices[i];
            long subStart = i * subSize;
            if (idx < subSize && subStart + idx < N) {
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

    for (long i = 0; i < N; i++) {
        arr[i] = output[i];
    }

    free(output);
    free(indices);
}

void funnelSort(long N, keytype* A) {
    if (N <= 1) return;

    long k = pow(N, 1.0 / 3.0);
    if (k < 1) k = 1;

    long subSize = (N + k - 1) / k;  // Ceiling division

    for (long i = 0; i < k; i++) {
        long start = i * subSize;
        long length = ((start + subSize) < N) ? subSize : (N - start);
        quickSort(length, &A[start]);
    }

    kWayFunnelMerge(A, N, k, subSize);
}
