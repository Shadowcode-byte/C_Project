#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SEAT_FILE "seats.txt"
#define PASS_FILE "passengers.dat"

typedef struct {
    int pnr;
    char name[50];
    int age;
    char train_no[10];
    char status[10];
} Passenger;

// Function prototypes
int generatePNR();
void bookTicket();
void cancelTicket();
void showPassengers();
void prepareChart();
void updateSeats(const char *train_no, int change);
int getSeatCount(const char *train_no);

int main() {
    int choice;
    while (1) {
        printf("\nRailway Ticket Reservation System\n");
        printf("1. Book Ticket\n2. Cancel Ticket\n3. Show Passengers\n4. Prepare Chart\n5. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            exit(1);
        }
        switch (choice) {
            case 1: bookTicket(); break;
            case 2: cancelTicket(); break;
            case 3: showPassengers(); break;
            case 4: prepareChart(); break;
            case 5: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}

// Safely generate a unique PNR
int generatePNR() {
    srand((unsigned int)time(NULL));
    return rand() % 100000 + (int)(time(NULL) % 10000);
}

// Book a ticket and update seats file
void bookTicket() {
    Passenger p;
    printf("Enter passenger name: ");
    scanf("%s", p.name);
    printf("Enter age: ");
    scanf("%d", &p.age);
    printf("Enter train number: ");
    scanf("%s", p.train_no);

    int seats = getSeatCount(p.train_no);
    if (seats > 0) {
        strcpy(p.status, "Confirmed");
        updateSeats(p.train_no, -1);
    } else {
        strcpy(p.status, "Waiting");
    }

    p.pnr = generatePNR();
    FILE *fp = fopen(PASS_FILE, "ab");
    if (fp == NULL) {
        printf("Error: Could not open passenger file for writing.\n");
        return;
    }
    fwrite(&p, sizeof(Passenger), 1, fp);
    fclose(fp);
    printf("Ticket booked! PNR: %d, Status: %s\n", p.pnr, p.status);
}

// Cancel ticket and process refund, file-safe
void cancelTicket() {
    int pnr;
    printf("Enter PNR to cancel: ");
    scanf("%d", &pnr);

    FILE *fp = fopen(PASS_FILE, "rb");
    if (fp == NULL) {
        printf("Error: Could not open passenger file for reading.\n");
        return;
    }
    FILE *tmp = fopen("temp.dat", "wb");
    if (tmp == NULL) {
        printf("Error: Could not open temp file.\n");
        fclose(fp);
        return;
    }
    Passenger p;
    int found = 0, refund = 0;
    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (p.pnr == pnr) {
            found = 1;
            if (strcmp(p.status, "Confirmed") == 0)
                refund = 1000 - 100;
            else
                refund = 1000;
            updateSeats(p.train_no, 1);
        } else {
            fwrite(&p, sizeof(Passenger), 1, tmp);
        }
    }
    fclose(fp);
    fclose(tmp);
    remove(PASS_FILE);
    rename("temp.dat", PASS_FILE);
    if (found)
        printf("Ticket cancelled! Refund: %d\n", refund);
    else
        printf("PNR not found.\n");
}

// Show all passengers for input train number
void showPassengers() {
    char train_no[10];
    printf("Enter train number: ");
    scanf("%s", train_no);

    FILE *fp = fopen(PASS_FILE, "rb");
    if (fp == NULL) {
        printf("No passenger records yet.\n");
        return;
    }
    Passenger p;
    int passengers_found = 0;
    printf("Passenger List for train %s:\n", train_no);
    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (strcmp(p.train_no, train_no) == 0) {
            printf("PNR: %d, Name: %s, Status: %s\n", p.pnr, p.name, p.status);
            passengers_found = 1;
        }
    }
    if (!passengers_found)
        printf("No passengers found for this train.\n");
    fclose(fp);
}

// Show chart of confirmed, waiting tickets for a train
void prepareChart() {
    char train_no[10];
    printf("Enter train number: ");
    scanf("%s", train_no);

    FILE *fp = fopen(PASS_FILE, "rb");
    if (fp == NULL) {
        printf("No passenger records for chart.\n");
        return;
    }
    int confirmed = 0, rac = 0, waiting = 0;
    Passenger p;
    while (fread(&p, sizeof(Passenger), 1, fp) == 1) {
        if (strcmp(p.train_no, train_no) == 0) {
            if (strcmp(p.status, "Confirmed") == 0)
                confirmed++;
            else if (strcmp(p.status, "RAC") == 0)
                rac++;
            else
                waiting++;
        }
    }
    fclose(fp);
    printf("Chart for train %s:\nConfirmed: %d, RAC: %d, Waiting: %d\n", train_no, confirmed, rac, waiting);
}

// Update seat count in seat file, handle errors
void updateSeats(const char *train_no, int change) {
    FILE *fp = fopen(SEAT_FILE, "r+");
    if (fp == NULL) {
        fp = fopen(SEAT_FILE, "w");
        fprintf(fp, "%s %d\n", train_no, 100 + change);
        fclose(fp);
        return;
    }
    char lines[100][100], t_no[10];
    int seat, n = 0, found = 0;
    while (fgets(lines[n], 100, fp) && n < 100) {
        sscanf(lines[n], "%s %d", t_no, &seat);
        if (strcmp(train_no, t_no) == 0) {
            sprintf(lines[n], "%s %d\n", t_no, seat + change);
            found = 1;
        }
        n++;
    }
    if (!found) {
        sprintf(lines[n++], "%s %d\n", train_no, 100 + change);
    }
    freopen(SEAT_FILE, "w", fp);
    for (int i = 0; i < n; i++) {
        fputs(lines[i], fp);
    }
    fclose(fp);
}

// Get seat count for train number, with error handling
int getSeatCount(const char *train_no) {
    FILE *fp = fopen(SEAT_FILE, "r");
    if (fp == NULL) {
        return 100;
    }
    char t_no[10];
    int seat;
    while (fscanf(fp, "%s %d", t_no, &seat) != EOF) {
        if (strcmp(train_no, t_no) == 0) {
            fclose(fp);
            return seat;
        }
    }
    fclose(fp);
    return 100;
}
