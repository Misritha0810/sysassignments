#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool isNumberPrime(int n) {
    if (n <= 1) {
        return false;
    }
    int loop_iterator = 2;
    do {
        if (n % loop_iterator == 0) {
            return false;
        }
        loop_iterator++;
    } while (loop_iterator <= sqrt(n));
    return true;
}

int identifyPrecedingPrime(int n) {
    int loop_iterator = n - 1;
    do {
        if (isNumberPrime(loop_iterator)) {
            return loop_iterator;
        }
        loop_iterator = loop_iterator - 1;
    } while (loop_iterator >= 2);
    return 0;
}

int calculateDigitTotal(int userValue) {
    int additionResult = 0;

    do {
        additionResult += userValue % 10;
        userValue /= 10;
    } while (userValue != 0);

    return additionResult;
}



void numberTransformer(int userValue) {
    switch (0) {
        case 0:
            if (userValue % 3 == 0 && userValue % 5 == 0) {
                printf("UAB CS 332&532\n");
                break;
            }
        case 1:
            if (isNumberPrime(userValue) && userValue != 3 && userValue != 5) {
                printf("Go Blazers\n");
                break;
            }
        case 2:
            if ((int)log2(userValue) == log2(userValue)) {
                int nearestPrime = identifyPrecedingPrime(userValue);
                int outputValue = userValue + nearestPrime;
                printf("%d\n", outputValue);
                break;
            }
        case 3:
            if (calculateDigitTotal(userValue) % 3 == 0) {
                printf("CS\n");
                break;
            }
        default:
            {
                int numberSquaredValue = pow(calculateDigitTotal(userValue), 2);
                printf("%d\n", numberSquaredValue);
            }
    }
}

bool UABNumber(int userValue) {
    int additionResult = 0;
    int loop_iterator = 1;
    do {
        if (userValue % loop_iterator == 0) {
            additionResult += loop_iterator;
        }
        loop_iterator++;
    } while (loop_iterator < userValue);

    return (additionResult == userValue);
}


int reverseNum(int userValue) {
    int reversed = 0;

    do {
        reversed = reversed * 10 + userValue % 10;
        userValue /= 10;
    } while (userValue != 0);

    return reversed;
}


int smallerThanIndex(int numbers[], int variableElementsSize) {
    int counter = 0;
    int loop_iterator = 0;

    if (variableElementsSize > 0) {
        do {
            if (numbers[loop_iterator] < loop_iterator) {
                counter++;
            }
            loop_iterator++;
        } while (loop_iterator < variableElementsSize);
    }

    return counter;
}


void arrayDetails(int variableElements[], int variableElementsSize) {
    if (variableElementsSize <= 0) {
        printf("Array is empty.\n");
        return;
    }

    int loop_iterator = 0;
    int lowestNumber = 2147483647, lowestIndex = 0, highestNumber = -2147483647, highestIndex = 0;
    float averageValue = 0;

    do {
        averageValue += variableElements[loop_iterator];

        if (variableElements[loop_iterator] < lowestNumber) {
            lowestNumber = variableElements[loop_iterator];
            lowestIndex = loop_iterator;
        }

        if (variableElements[loop_iterator] > highestNumber) {
            highestNumber = variableElements[loop_iterator];
            highestIndex = loop_iterator;
        }

        loop_iterator++;
    } while (loop_iterator < variableElementsSize);

    averageValue = averageValue / (float)variableElementsSize;

    printf("[%d, %d, %.2f, %d, %d]\n", lowestNumber, lowestIndex, averageValue, highestNumber, highestIndex);
}

int main() {
    int choice;
    while(1){
        printf("\n--------------------  Choose Options  --------------------\n");
        printf("1. numberTransformer\n");
        printf("2. UABNumber check\n");
        printf("3. reverseNum\n");
        printf("4. smallerThanIndex\n");
        printf("5. arrayDetails\n");
        printf("6. Exit\n");

        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int num;
                printf("\n--------------------  Task 1 Result  --------------------\n");
                printf("Please Enter a number for numberTransformer: ");
                scanf("%d", &num);
                numberTransformer(num);
                break;
                
            }

            case 2: {
                int checkUAB;
                printf("\n--------------------  Task 2 Result --------------------\n");
                printf("Please Enter a number for UABNumber check: ");
                scanf("%d", &checkUAB);
                printf("%s\n", UABNumber(checkUAB) ? "True" : "False");
                break;
            }

            case 3: {
                int reverseInput;
                printf("\n--------------------  Task 3 Result --------------------\n");
                printf("Please Enter a number for reverseNum: ");
                scanf("%d", &reverseInput);
                printf("%d\n", reverseNum(reverseInput));
                break;
            }

            case 4: {
                int variableElementsSize = 5;
                int *variableElements = (int *)malloc(variableElementsSize * sizeof(int));
                printf("\n--------------------  Task 4 Result --------------------\n");
                printf("Please Enter %d elements for the array for smaller than index counter :\n", variableElementsSize);

                for (int loop_iterator = 0; loop_iterator < variableElementsSize; loop_iterator++) {
                    printf("Input number %d: ", loop_iterator + 1);
                    scanf("%d", &variableElements[loop_iterator]);
                }

                printf("\n smaller than index counter : %d\n", smallerThanIndex(variableElements, variableElementsSize));
                free(variableElements);
                break;
            }

            case 5: {
                int dataSetSize = 6;
                int *dataSet = (int *)malloc(dataSetSize * sizeof(int));

                printf("\n--------------------  Task 5 Result --------------------\n");
                printf("Please Enter %d elements for the array for array details :\n", dataSetSize);

                for (int loop_iterator = 0; loop_iterator < dataSetSize; loop_iterator++) {
                    printf("Input number %d: ", loop_iterator + 1);
                    scanf("%d", &dataSet[loop_iterator]);
                }

                arrayDetails(dataSet, dataSetSize);
                printf("\n");
                free(dataSet);
                break;
            }
            case 6: {
                exit(0);
                break;
            }
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
