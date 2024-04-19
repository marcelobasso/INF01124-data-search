#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <cmath>

void swap(int arr[], int i, int f) {
    int aux;

    aux = arr[i];
    arr[i] = arr[f];
    arr[f] = aux;
}

int random_number(int min, int max) {
    srand(time(NULL));
    return (rand() % (max - min)) + min;
}

void insertion_sort(int arr[], int tam) {
    int i, j, key;

    for (j = 1; j < tam; j++) {
        key = arr[j];
        i = j - 1;
    
        while ((i >= 0) && (arr[i] > key)) {
            arr[i + 1] = arr[i];
            i--;
        }
    
        arr[i + 1] = key;
    }
}

int median3(int arr[], int min, int max) {
    int aux[3], med = floor((min - max) / 2);

    aux[0] = arr[min];
    aux[1] = arr[med];
    aux[2] = arr[max];
    insertion_sort(aux, 3);

    if (arr[min] == aux[1])
        return min;
    else if (arr[med] == aux[1])
        return med;
    else
        return max;
}