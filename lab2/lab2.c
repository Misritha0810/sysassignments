#include <stdio.h>


/*
    Name : Misritha Maridi
    Blazer ID : mmaridi
    University : UAB
    Department : CS

    Note : Used same variable names and structure from java code and implemented inplace insertion sort.
*/

void displayArray(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size - 1; i++) {
        printf("%d, ", arr[i]);
    }
    printf("%d]\n", arr[size - 1]);
}

void insertionSort(int arr[], int size) {
    int temp, currLoc;
    for (int i = 1; i < size; i++) {
        currLoc = i;
        while (currLoc > 0 && arr[currLoc - 1] > arr[currLoc]) {
            // Swap elements
            temp = arr[currLoc];
            arr[currLoc] = arr[currLoc - 1];
            arr[currLoc - 1] = temp;
            currLoc--;
        }
    }
}

int main() {
    int n;
    // reading array size
    printf("Please enter the number of elements in the array: ");
    scanf("%d", &n);

    int arr[n];

    // reading all elements of the array
    for (int i = 0; i < n; i++) {
        printf("Please enter element %d of the array: ", i + 1);
        scanf("%d", &arr[i]);
    }

    // displaying given array
    printf("Given array is: ");
    displayArray(arr, n);

    // sorting using insertion sort
    insertionSort(arr, n);

    // final sorted array.
    printf("Sorted array is: ");
    displayArray(arr, n);

    return 0;
}
