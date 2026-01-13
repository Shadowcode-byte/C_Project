#include <stdio.h>

int main() {
    int choice = 0, tickets, totalTickets = 0;
    float totalBill = 0, cost;

    
    while (choice != 4) {
        
        printf("\n--- Cinema Ticket Booking System ---\n");
        printf("1. Premium Seat - ₹200\n");
        printf("2. Standard Seat - ₹150\n");
        printf("3. Economy Seat - ₹100\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("How many Premium tickets? ");
            scanf("%d", &tickets);
            cost = tickets * 200;
            totalBill += cost;
            totalTickets += tickets;
            printf("Added %d Premium tickets, Cost = ₹%.2f\n", tickets, cost);
        }
        else if (choice == 2) {
            printf("How many Standard tickets? ");
            scanf("%d", &tickets);
            cost = tickets * 150;
            totalBill += cost;
            totalTickets += tickets;
            printf("Added %d Standard tickets, Cost = ₹%.2f\n", tickets, cost);
        }
        else if (choice == 3) {
            printf("How many Economy tickets? ");
            scanf("%d", &tickets);
            cost = tickets * 100;
            totalBill += cost;
            totalTickets += tickets;
            printf("Added %d Economy tickets, Cost = ₹%.2f\n", tickets, cost);
        }
        else if (choice == 4) {
            printf("\nBooking finished...\n");
        }
        else {
            printf("Invalid choice! Please select 1-4.\n");
        }
    }

    if (totalTickets >= 5) {
        printf("\nYou booked %d tickets. Eligible for 5%% discount!\n", totalTickets);
        totalBill = totalBill - (totalBill * 0.05);
    }

   
    printf("\nTotal Tickets: %d\n", totalTickets);
    printf("Final Bill: ₹%.2f\n", totalBill);

    return 0;
}