#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) array[i] = rand() % 100;
}

void printArray(int arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

// Function to merge two sorted arrays into a single sorted array
void merge(int arr[], int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Mergesort function to recursively divide and merge the array
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        // Create threads for each half
        pthread_t leftThread, rightThread;

        StartEndIndexes leftArgs = {left, middle};
        StartEndIndexes rightArgs = {middle + 1, right};

        pthread_create(&leftThread, NULL, mergeSort, (void*)&leftArgs);
        pthread_create(&rightThread, NULL, mergeSort, (void*)&rightArgs);

        // Wait for the threads to finish
        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);

        // Merge the sorted halves
        merge(arr, left, middle, right);
    }
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;

    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);

    // 2. Print the unsorted array.
    printf("Unsorted array: ");
    printArray(array);

    // 3. Create 2 threads for merge sort.
    pthread_t mergeSortThread;
    pthread_create(&mergeSortThread, NULL, mergeSort, (void*)&sei);

    // 4. Wait for mergesort to finish.
    pthread_join(mergeSortThread, NULL);

    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);

    return 0;
}