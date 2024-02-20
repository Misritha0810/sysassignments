#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LISTINGS 7000
#define MAX_LINE_LENGTH 102400

struct Accommodation {
    int id;
    int host_id;
    char host_name[256];
    char neighbourhood_group[256];
    char neighbourhood[256];
    double latitude;
    double longitude;
    char room_type[256];
    int price;
    int minimum_nights;
    int number_of_reviews;
    int calculated_host_listings_count;
    int availability_365;
};

struct Accommodation getDataFromCsv(char line[]) {
    struct Accommodation accom;
    char *token;
    int itr = 0;

    token = strtok(line, ",");
    while (token != NULL) {
        switch (itr) {
            case 0:
                accom.id = atoi(token);
                break;
            case 1:
                accom.host_id = atoi(token);
                break;
            case 2:
                strcpy(accom.host_name, token);
                break;
            case 3:
                strcpy(accom.neighbourhood_group, token);
                break;
            case 4:
                strcpy(accom.neighbourhood, token);
                break;
            case 5:
                accom.latitude = atof(token);
                break;
            case 6:
                accom.longitude = atof(token);
                break;
            case 7:
                strcpy(accom.room_type, token);
                break;
            case 8:
                accom.price = atoi(token);
                break;
            case 9:
                accom.minimum_nights = atoi(token);
                break;
            case 10:
                accom.number_of_reviews = atoi(token);
                break;
            case 11:
                accom.calculated_host_listings_count = atoi(token);
                break;
            case 12:
                accom.availability_365 = atoi(token);
                break;
        }
        itr++;
        token = strtok(NULL, ",");
    }

    return accom;
}

int compareByHostName(const void *a, const void *b) {
    const struct Accommodation *accom1 = (const struct Accommodation *)a;
    const struct Accommodation *accom2 = (const struct Accommodation *)b;
    return strcmp(accom1->host_name, accom2->host_name);
}

int compareByPrice(const void *a, const void *b) {
    const struct Accommodation *accom1 = (const struct Accommodation *)a;
    const struct Accommodation *accom2 = (const struct Accommodation *)b;
    return (accom1->price - accom2->price);
}

int main() {
    FILE *fPtr;
    char line[MAX_LINE_LENGTH];
    struct Accommodation listings[MAX_LISTINGS];
    int csvInfoLengthOfData = 0, itr;

    fPtr = fopen("listings.csv", "r");
    if (fPtr == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    while (fgets(line, MAX_LINE_LENGTH, fPtr) != NULL && csvInfoLengthOfData < MAX_LISTINGS) {
        listings[csvInfoLengthOfData] = getDataFromCsv(line);
        csvInfoLengthOfData++;
    }

    fclose(fPtr);

    qsort(listings, csvInfoLengthOfData, sizeof(struct Accommodation), compareByHostName);

    fPtr = fopen("hostNameSorted.csv", "w");
    if (fPtr == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    for (itr = 0; itr < csvInfoLengthOfData; itr++) {
        fprintf(fPtr, "%d,%d,%s,%s,%s,%f,%f,%s,%d,%d,%d,%d,%d\n", listings[itr].id,
                listings[itr].host_id, listings[itr].host_name, listings[itr].neighbourhood_group,
                listings[itr].neighbourhood, listings[itr].latitude, listings[itr].longitude,
                listings[itr].room_type, listings[itr].price, listings[itr].minimum_nights,
                listings[itr].number_of_reviews, listings[itr].calculated_host_listings_count,
                listings[itr].availability_365);
    }

    fclose(fPtr);

    qsort(listings, csvInfoLengthOfData, sizeof(struct Accommodation), compareByPrice);

    fPtr = fopen("priceSorted.csv", "w");
    if (fPtr == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    for (itr = 0; itr < csvInfoLengthOfData; itr++) {
        fprintf(fPtr, "%d,%d,%s,%s,%s,%f,%f,%s,%d,%d,%d,%d,%d\n", listings[itr].id,
                listings[itr].host_id, listings[itr].host_name, listings[itr].neighbourhood_group,
                listings[itr].neighbourhood, listings[itr].latitude, listings[itr].longitude,
                listings[itr].room_type, listings[itr].price, listings[itr].minimum_nights,
                listings[itr].number_of_reviews, listings[itr].calculated_host_listings_count,
                listings[itr].availability_365);
    }

    fclose(fPtr);

    return 0;
}
