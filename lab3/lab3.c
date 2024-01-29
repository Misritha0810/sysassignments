#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Name : Misritha Maridi
    Blazer ID : mmaridi
    University : UAB
    Department : DS

    Note : Used previous lab02 code and seperated code to multiple functions.
*/

void displayArray(char* arr[], int size) {
    printf("[");
    for (int i = 0; i < size - 1; i++) {
        printf("%s, ", arr[i]);
    }
    printf("%s]\n", arr[size - 1]);
}

void insertionSort(char* arr[], int size) {
    char* temp;
    int currLoc;

    for (int i = 1; i < size; i++) {
        currLoc = i;

        // strcasecmp use this if you want case sensitive comparisons
        while (currLoc > 0 && strcmp(arr[currLoc - 1], arr[currLoc]) > 0) {
            temp = arr[currLoc];
            arr[currLoc] = arr[currLoc - 1];
            arr[currLoc - 1] = temp;
            currLoc--;
        }
    }
}

void readStrings(char* arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("Please enter string %d of the array: ", i + 1);
        arr[i] = (char*)malloc(256); 
        scanf("%s", arr[i]);
    }
}

int main() {
    int n;
    printf("Please enter the number of elements in the array: ");
    scanf("%d", &n);

    char* arr[n];

    readStrings(arr, n);

    printf("Given String array is: ");
    displayArray(arr, n);
    insertionSort(arr, n);

    printf("Sorted String array is: ");
    displayArray(arr, n);

    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }

    return 0;
}
