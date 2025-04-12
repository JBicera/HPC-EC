#include "sort_utils.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

typedef struct {
    keytype value;
    long subIdx;  // Which subarray (within a group) the value comes from.
    long pos;     // Current index within that subarray.
} HeapNode;

void swap(HeapNode* x, HeapNode* y) {
    HeapNode temp = *x;
    *x = *y;
    *y = temp;
}

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

void buildMinHeap(HeapNode heap[], long heapSize) {
    for (long i = (heapSize - 2) / 2; i >= 0; i--)
        minHeapify(heap, heapSize, i);
}

// Recursive multiway (m-way) funnel merge using double buffering.
// 'in' has k sorted subarrays (each of size 'subSize' or less) and 'out' is the merge output buffer.
void kWayFunnelMerge(keytype* in, keytype* out, long N, long k, long subSize) {
    // Base case: nothing to merge.
    if (k <= 1 || N <= subSize) {
        if (in != out)
            memcpy(out, in, N * sizeof(keytype));
        return;
    }

    long m = 16;                      // How many sorted subarrays to merge concurrently.
    long newSubSize = subSize * m;      // New subarray size after merging m subarrays.
    long newK = (k + m - 1) / m;        // New number of subarrays (ceiling division).

    // Preallocate a heap buffer (max size = m) for reuse across groups.
    HeapNode* heapBuffer = (HeapNode*)malloc(m * sizeof(HeapNode));
    if (!heapBuffer) {
        fprintf(stderr, "Memory allocation error for heapBuffer\n");
        exit(EXIT_FAILURE);
    }
    // Preallocate a temporary buffer for group merges (max group size = newSubSize).
    keytype* tempBuffer = (keytype*)malloc(newSubSize * sizeof(keytype));
    if (!tempBuffer) {
        fprintf(stderr, "Memory allocation error for tempBuffer\n");
        free(heapBuffer);
        exit(EXIT_FAILURE);
    }

    // Process each group of up to m sorted subarrays.
    for (long i = 0; i < newK; i++) {
        long groupStart = i * newSubSize;
        long groupEnd = (groupStart + newSubSize < N) ? groupStart + newSubSize : N;
        long groupK = ((i * m + m) <= k) ? m : (k - i * m);

        if (groupK > 0) {
            long currentHeapSize = 0;
            // Initialize the heap from each subarray in the current group.
            for (long j = 0; j < groupK; j++) {
                long subStart = groupStart + j * subSize;
                if (subStart < groupEnd) {
                    heapBuffer[currentHeapSize].value = in[subStart];
                    heapBuffer[currentHeapSize].subIdx = j;
                    heapBuffer[currentHeapSize].pos = 0;
                    currentHeapSize++;
                }
            }

            buildMinHeap(heapBuffer, currentHeapSize);

            // Use tempBuffer as workspace for merging this group.
            long idx = groupStart;
            while (currentHeapSize > 0) {
                HeapNode minNode = heapBuffer[0];
                tempBuffer[idx - groupStart] = minNode.value;
                idx++;
                long newPos = minNode.pos + 1;
                long subStart = groupStart + minNode.subIdx * subSize;

                if ((subStart + newPos < groupEnd) && (newPos < subSize)) {
                    heapBuffer[0].value = in[subStart + newPos];
                    heapBuffer[0].pos = newPos;
                } else {
                    heapBuffer[0] = heapBuffer[currentHeapSize - 1];
                    currentHeapSize--;
                }
                minHeapify(heapBuffer, currentHeapSize, 0);
            }
            // Copy merged group result from tempBuffer to out.
            memcpy(&out[groupStart], tempBuffer, (groupEnd - groupStart) * sizeof(keytype));
        }
    }
    // Clean up preallocated temporary buffers.
    free(heapBuffer);
    free(tempBuffer);

    // Recurse: merge the newK sorted subarrays.
    kWayFunnelMerge(out, in, N, newK, newSubSize);
}

// Top-level funnel sort function.
void funnelSort(long N, keytype* A) {
    // For small arrays, use quickSort directly.
    if (N <= 1000) {
        quickSort(N, A);
        return;
    }

    long k = pow(N, 1.0 / 3.0);
    if (k < 1) k = 1;
    long subSize = (N + k - 1) / k;

    // Sort each of the k subarrays independently using quickSort.
    for (long i = 0; i < k; i++) {
        long start = i * subSize;
        long length = (start + subSize < N) ? subSize : (N - start);
        quickSort(length, &A[start]);
    }

    // Allocate a persistent temporary buffer for merging.
    keytype* temp = (keytype*)malloc(N * sizeof(keytype));
    if (!temp) {
        fprintf(stderr, "Memory allocation error in funnelSort\n");
        exit(EXIT_FAILURE);
    }
    memcpy(temp, A, N * sizeof(keytype));

    // Merge the sorted subarrays recursively.
    kWayFunnelMerge(temp, A, N, k, subSize);

    free(temp);
}
