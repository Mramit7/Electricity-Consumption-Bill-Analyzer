
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "electricity_records.txt"

#define FIXED_CHARGE 100.0
#define DUTY_RATE 0.05

struct Consumer {
    int id;
    char name[50];
    char address[100];
    double previousReading;
    double currentReading;
    double previousUnits;
    double units;
    double energyCharge;
    double duty;
    double totalBill;
};

/* Calculate bill using progressive slabs */
double calculateEnergyCharge(double units) {
    double charge = 0.0;

    if (units > 500) {
        charge += (units - 500) * 7.50;
        units = 500;
    }

    if (units > 200) {
        charge += (units - 200) * 6.00;
        units = 200;
    }

    if (units > 100) {
        charge += (units - 100) * 4.50;
        units = 100;
    }

    charge += units * 3.00;

    return charge;
}

/* Read a complete line, including spaces */
void readLine(const char *message, char *buffer, int size) {
    int ch;

    printf("%s", message);

    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Clear remaining input */
            }
        }
    } else {
        buffer[0] = '\0';
    }
}

/* Safely read a non-negative number */
double readNumber(const char *message) {
    char line[100];
    char extra;
    double value;

    while (1) {
        readLine(message, line, sizeof(line));

        if (sscanf(line, " %lf %c", &value, &extra) == 1
            && value >= 0) {
            return value;
        }

        printf("Invalid input. Enter a non-negative number.\n");
    }
}

/* Display one bill */
void displayBill(const struct Consumer *c) {
    printf("\n========== ELECTRICITY BILL ==========\n");
    printf("Consumer ID       : %d\n", c->id);
    printf("Consumer Name     : %s\n", c->name);
    printf("Address           : %s\n", c->address);
    printf("--------------------------------------\n");
    printf("Previous Reading  : %.2f units\n",
           c->previousReading);
    printf("Current Reading   : %.2f units\n",
           c->currentReading);
    printf("Units Consumed    : %.2f units\n", c->units);
    printf("Energy Charge     : Rs. %.2f\n",
           c->energyCharge);
    printf("Fixed Charge      : Rs. %.2f\n", FIXED_CHARGE);
    printf("Electricity Duty  : Rs. %.2f\n", c->duty);
    printf("--------------------------------------\n");
    printf("TOTAL BILL        : Rs. %.2f\n", c->totalBill);
    printf("======================================\n");
}

/* Save a record in text-file format */
int saveRecord(const struct Consumer *c) {
    FILE *fp = fopen(FILE_NAME, "a");

    if (fp == NULL) {
        printf("Error: Unable to open the record file.\n");
        return 0;
    }

    fprintf(fp, "%d|%s|%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f\n",
            c->id, c->name, c->address,
            c->previousReading, c->currentReading,
            c->previousUnits, c->units,
            c->energyCharge, c->duty, c->totalBill);

    if (fclose(fp) != 0) {
        printf("Error: Could not finish saving the record.\n");
        return 0;
    }

    return 1;
}

/* Load a saved record */
int readRecord(FILE *fp, struct Consumer *c) {
    return fscanf(fp,
        "%d|%49[^|]|%99[^|]|%lf|%lf|%lf|%lf|%lf|%lf|%lf\n",
        &c->id, c->name, c->address,
        &c->previousReading, &c->currentReading,
        &c->previousUnits, &c->units,
        &c->energyCharge, &c->duty, &c->totalBill) == 10;
}

/* Generate a unique consumer ID */
int generateID(void) {
    FILE *fp = fopen(FILE_NAME, "r");
    struct Consumer c;
    int maxID = 1000;

    if (fp == NULL) {
        return maxID + 1;
    }

    while (readRecord(fp, &c)) {
        if (c.id > maxID) {
            maxID = c.id;
        }
    }

    fclose(fp);
    return maxID + 1;
}

/* Find the latest saved record for a consumer */
int findPreviousRecord(int id, struct Consumer *result) {
    FILE *fp = fopen(FILE_NAME, "r");
    struct Consumer c;
    int found = 0;

    if (fp == NULL) {
        return 0;
    }

    while (readRecord(fp, &c)) {
        if (c.id == id) {
            *result = c;
            found = 1;
        }
    }

    fclose(fp);
    return found;
}

/* Add a new bill */
void addConsumer(void) {
    struct Consumer c, old;
    char line[100];
    char extra;
    int hasPrevious = 0;

    printf("\n--- ADD ELECTRICITY BILL ---\n");

    printf("1. New consumer\n");
    printf("2. Existing consumer\n");
    printf("Select option: ");

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return;
    }

    if (sscanf(line, " %d %c", &c.id, &extra) != 1
        || (c.id != 1 && c.id != 2)) {
        printf("Invalid option.\n");
        return;
    }

    if (c.id == 1) {
        c.id = generateID();
        c.previousReading = 0;
        c.previousUnits = 0;

        readLine("Enter consumer name: ",
                 c.name, sizeof(c.name));
        readLine("Enter address: ",
                 c.address, sizeof(c.address));

        if (strlen(c.name) == 0 || strlen(c.address) == 0) {
            printf("Name and address cannot be empty.\n");
            return;
        }
    } else {
        double requestedID = readNumber("Enter consumer ID: ");

        if (requestedID < 1001 || requestedID > 2147483647.0
            || requestedID != (int)requestedID) {
            printf("Invalid consumer ID.\n");
            return;
        }

        if (!findPreviousRecord((int)requestedID, &old)) {
            printf("Consumer not found. Check the ID.\n");
            return;
        }

        c.id = old.id;
        strcpy(c.name, old.name);
        strcpy(c.address, old.address);
        c.previousReading = old.currentReading;
        c.previousUnits = old.units;
        hasPrevious = 1;
    }

    c.currentReading = readNumber("Enter current meter reading: ");

    if (c.currentReading < c.previousReading) {
        printf("Error: Current reading cannot be less than "
               "previous reading.\n");
        return;
    }

    c.units = c.currentReading - c.previousReading;
    c.energyCharge = calculateEnergyCharge(c.units);
    c.duty = c.energyCharge * DUTY_RATE;
    c.totalBill = c.energyCharge + FIXED_CHARGE + c.duty;

    displayBill(&c);

    if (saveRecord(&c)) {
        printf("Bill record saved successfully.\n");
    }

    if (hasPrevious) {
        printf("Previous bill consumption: %.2f units\n",
               c.previousUnits);
        printf("Current bill consumption : %.2f units\n",
               c.units);

        if (c.units > c.previousUnits) {
            printf("Alert: Electricity consumption increased.\n");
        } else if (c.units < c.previousUnits) {
            printf("Good: Electricity consumption decreased.\n");
        } else {
            printf("Consumption is unchanged.\n");
        }
    }
}

/* View all saved records */
void viewRecords(void) {
    FILE *fp = fopen(FILE_NAME, "r");
    struct Consumer c;
    int found = 0;

    if (fp == NULL) {
        printf("\nNo saved records found.\n");
        return;
    }

    printf("\n--------------- SAVED RECORDS ---------------\n");
    printf("%-6s %-20s %-10s %-12s %-12s\n",
           "ID", "Name", "Units", "Bill (Rs.)", "Reading");

    while (readRecord(fp, &c)) {
        printf("%-6d %-20.20s %-10.2f %-12.2f %-12.2f\n",
               c.id, c.name, c.units, c.totalBill,
               c.currentReading);
        found = 1;
    }

    if (!found) {
        printf("No valid records available.\n");
    }

    fclose(fp);
}

/* Analyze all saved bills */
void analyzeConsumption(void) {
    FILE *fp = fopen(FILE_NAME, "r");
    struct Consumer c;
    double totalUnits = 0, totalAmount = 0;
    double highestUnits = -1;
    int count = 0, highestID = 0;

    if (fp == NULL) {
        printf("\nNo records available for analysis.\n");
        return;
    }

    while (readRecord(fp, &c)) {
        totalUnits += c.units;
        totalAmount += c.totalBill;
        count++;

        if (c.units > highestUnits) {
            highestUnits = c.units;
            highestID = c.id;
        }
    }

    fclose(fp);

    if (count == 0) {
        printf("No valid records available.\n");
        return;
    }

    printf("\n======= CONSUMPTION ANALYSIS =======\n");
    printf("Total bills recorded : %d\n", count);
    printf("Total units consumed : %.2f\n", totalUnits);
    printf("Total billed amount  : Rs. %.2f\n", totalAmount);
    printf("Average units/bill   : %.2f\n",
           totalUnits / count);
    printf("Average bill amount  : Rs. %.2f\n",
           totalAmount / count);
    printf("Highest consumption  : %.2f units (ID %d)\n",
           highestUnits, highestID);

    printf("\nConsumption category of highest bill:\n");

    if (highestUnits <= 100) {
        printf("Low consumption\n");
    } else if (highestUnits <= 300) {
        printf("Moderate consumption\n");
    } else {
        printf("High consumption\n");
    }

    printf("====================================\n");
}

/* Main menu */
int main(void) {
    char line[100];
    char extra;
    int choice;

    do {
        printf("\n====================================\n");
        printf(" ELECTRICITY CONSUMPTION AND BILL ANALYZER\n");
        printf("====================================\n");
        printf("1. Add Consumer / Calculate Bill\n");
        printf("2. View Saved Records\n");
        printf("3. Analyze Consumption\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        if (sscanf(line, " %d %c", &choice, &extra) != 1) {
            choice = 0;
        }

        switch (choice) {
            case 1:
                addConsumer();
                break;

            case 2:
                viewRecords();
                break;

            case 3:
                analyzeConsumption();
                break;

            case 4:
                printf("Thank you for using the analyzer.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 4);

    return 0;
}
