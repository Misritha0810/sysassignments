# program Description 

   To read listing data from a CSV file, sort it based on different criteria (host name and price), and then write the sorted data into separate CSV files.

# Compilation Instructions
```bash
gcc -o lab6 lab6.c 
```

# running instructions 

```bash
    ./lab6
```

# My Files
## lab6.c

This program reads a csv data, and sort them by hostname and price data write them down to new csv files.

   1. Opened the listings.csv file and read data from it line by line using fgets.
   2. Data Parsed and stored in an array of structs, sort data by host name using qsort and written to a csv file(hostNameSorted.csv)
   3. Now, Sort the data by price using qsort and write it to priceSorted.csv.
   4. After writing the sorted data to the files then closes the file pointers to release system resources.


# Github link
https://github.com/Misritha0810/sysassignments.git