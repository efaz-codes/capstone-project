#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_NAME      50
#define MAX_COMPANY   50
#define MAX_MEDICINES 200
#define MAX_BILL_ITEMS 20

#define MEDICINE_FILE "medicines.txt"
#define SALES_FILE    "sales.txt"
#define LAST_BILL_FILE "last_bill.txt"

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "1234"

struct Staff {
    char id[20];
    char name[30];
};

/* Test staff accounts: ST001, ST002, ST003 */
struct Staff staffList[] = {
    {"ST001", "Asraful"},
    {"ST002", "Mahin"},
    {"ST003", "Karin"}
};
#define STAFF_COUNT 3

struct Staff loggedInStaff;

/*
   STRUCT DEFINITION
   */
struct Medicine {
    int    id;
    char   name[MAX_NAME];
    char   company[MAX_COMPANY];
    float  price;
    int    quantity;
    int    expiry_day;
    int    expiry_month;
    int    expiry_year;
};

struct BillItem {
    char  name[MAX_NAME];
    int   quantity;
    float price;
    float subtotal;
};

  // FUNCTION DECLARATIONS


/* Auth */
int  adminLogin();
int  staffLogin();

/* Medicine CRUD */
void addMedicine();
void viewMedicine();
void searchMedicine();
void updateMedicine();
void deleteMedicine();

/* Sales */
void sellMedicine();
void generateBill();
void generateLatestBill();
void staffSalesReport();

/* Reports */
void stockReport();
void expiredMedicineCheck();

/* File Helpers */
void loadMedicines(struct Medicine arr[], int *count);
void saveMedicines(struct Medicine arr[], int count);
int  getLastId();
int  getNextInvoiceNumber();

/* Utility */
void printLine();
void printHeader(char *title);
void pauseScreen();
void clearScreen();
void getCurrentDate(int *d, int *m, int *y);
int  isExpired(int ed, int em, int ey);

/* Menu */
void showRoleMenu();
void showAdminMenu();
void showStaffMenu();
void adminPanel();
void staffPanel();


/* ============================================================
   MAIN FUNCTION
   ============================================================ */
int main() {
    int choice;

    clearScreen();
    printf("\n");
    printf("  ############################################\n");
    printf("  #      MEDICAL STORE MANAGEMENT SYSTEM         #\n");
    printf("  #            Capstone Project              #\n");
    printf("  ############################################\n");
    printf("\n");

    do {
        showRoleMenu();
        printf("  Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                if (adminLogin()) adminPanel();
                else printf("\n  [!] Too many failed attempts. Returning to login menu.\n");
                break;
            case 2:
                if (staffLogin()) staffPanel();
                break;
            case 0:
                printf("\n  Thank you for using Medical Store MS, Goodbye!\n\n");
                break;
            default:
                printf("\n  [!] Invalid choice. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}


/* ============================================================
   ADMIN LOGIN
   ============================================================ */
int adminLogin() {
    char username[30];
    char password[30];
    int  attempts = 0;

    printHeader("ADMIN LOGIN");

    while (attempts < 3) {
        printf("  Username : ");
        scanf("%s", username);
        printf("  Password : ");
        scanf("%s", password);

        if (strcmp(username, ADMIN_USERNAME) == 0 &&
            strcmp(password, ADMIN_PASSWORD) == 0) {
            printf("\n  [+] Login successful! Welcome, Admin.\n");
            pauseScreen();
            return 1;
        } else {
            attempts++;
            printf("\n  [!] Wrong credentials. Attempts left: %d\n\n",
                   3 - attempts);
        }
    }
    return 0;
}


/* ============================================================
   STAFF LOGIN (ID only)
   ============================================================ */
int staffLogin() {
    char staffId[20];
    int i;

    printHeader("STAFF LOGIN");
    printf("  Staff ID : ");
    scanf("%19s", staffId);

    for (i = 0; i < STAFF_COUNT; i++) {
        if (strcmp(staffId, staffList[i].id) == 0) {
            loggedInStaff = staffList[i];
            printf("\n  [+] Login successful! Welcome, %s.\n", loggedInStaff.name);
            pauseScreen();
            return 1;
        }
    }

    printf("\n  [!] Invalid Staff ID. Please try again.\n");
    pauseScreen();
    return 0;
}


/* ============================================================
   SHOW MAIN MENU
   ============================================================ */
void showRoleMenu() {
    clearScreen();
    printHeader("LOGIN PAGE");
    printf("  [1] Admin Login\n");
    printf("  [2] Staff Login\n");
    printLine();
    printf("  [0] Exit\n");
    printLine();
    printf("\n  Test Staff IDs: ST001 (Asraful), ST002 (Mahin), ST003 (Karin)\n");
}

void showAdminMenu() {
    clearScreen();
    printHeader("ADMIN PANEL");
    printf("  [1] Add Medicine\n");
    printf("  [2] View All Medicines\n");
    printf("  [3] Search Medicine\n");
    printf("  [4] Update Medicine\n");
    printf("  [5] Delete Medicine\n");
    printf("  [6] Stock Report\n");
    printf("  [7] Expired Medicine Check\n");
    printf("  [8] Sales History\n");
    printf("  [9] Staff Sales Report\n");
    printLine();
    printf("  [0] Logout\n");
    printLine();
}

void showStaffMenu() {
    clearScreen();
    printHeader("STAFF PANEL");
    printf("  Logged in: %s (%s)\n\n", loggedInStaff.name, loggedInStaff.id);
    printf("  [1] Sell Medicine\n");
    printf("  [2] Generate Bill (Latest Sale)\n");
    printLine();
    printf("  [0] Logout\n");
    printLine();
}

void adminPanel() {
    int choice;
    do {
        showAdminMenu();
        printf("  Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addMedicine(); break;
            case 2: viewMedicine(); break;
            case 3: searchMedicine(); break;
            case 4: updateMedicine(); break;
            case 5: deleteMedicine(); break;
            case 6: stockReport(); break;
            case 7: expiredMedicineCheck(); break;
            case 8: generateBill(); break;
            case 9: staffSalesReport(); break;
            case 0: break;
            default: printf("\n  [!] Invalid choice.\n"); pauseScreen();
        }
    } while (choice != 0);
}

void staffPanel() {
    int choice;
    do {
        showStaffMenu();
        printf("  Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: sellMedicine(); break;
            case 2: generateLatestBill(); break;
            case 0: break;
            default: printf("\n  [!] Invalid choice.\n"); pauseScreen();
        }
    } while (choice != 0);
}


/* ============================================================
   ADD MEDICINE
   ============================================================ */
void addMedicine() {
    struct Medicine arr[MAX_MEDICINES];
    struct Medicine m;
    int count = 0;

    printHeader("ADD MEDICINE");

    loadMedicines(arr, &count);

    m.id = getLastId() + 1;

    printf("  Medicine Name   : ");
    scanf(" %[^\n]", m.name);

    printf("  Company Name    : ");
    scanf(" %[^\n]", m.company);

    printf("  Price (per unit): ");
    scanf("%f", &m.price);

    printf("  Quantity        : ");
    scanf("%d", &m.quantity);

    printf("  Expiry Date (DD MM YYYY): ");
    scanf("%d %d %d", &m.expiry_day, &m.expiry_month, &m.expiry_year);

    /* Add to array */
    arr[count] = m;
    count++;

    /* Save back to file */
    saveMedicines(arr, count);

    printf("\n  [+] Medicine added successfully! (ID: %d)\n", m.id);
    pauseScreen();
}


/* ============================================================
   VIEW ALL MEDICINES
   ============================================================ */
void viewMedicine() {
    struct Medicine arr[MAX_MEDICINES];
    int count = 0;
    int i;

    printHeader("ALL MEDICINES");

    loadMedicines(arr, &count);

    if (count == 0) {
        printf("  [!] No medicines found in database.\n");
        pauseScreen();
        return;
    }

    printf("  %-5s %-20s %-18s %-8s %-8s %-12s\n",
           "ID", "Name", "Company", "Price", "Qty", "Expiry");
    printLine();

    for (i = 0; i < count; i++) {
        printf("  %-5d %-20s %-18s %-8.2f %-8d %02d/%02d/%04d\n",
               arr[i].id,
               arr[i].name,
               arr[i].company,
               arr[i].price,
               arr[i].quantity,
               arr[i].expiry_day,
               arr[i].expiry_month,
               arr[i].expiry_year);
    }

    printf("\n  Total medicines: %d\n", count);
    pauseScreen();
}


/* ============================================================
   SEARCH MEDICINE
   ============================================================ */
void searchMedicine() {
    struct Medicine arr[MAX_MEDICINES];
    int    count = 0;
    int    found = 0;
    char   keyword[MAX_NAME];
    char   nameLower[MAX_NAME];
    char   keyLower[MAX_NAME];
    int    i, j;

    printHeader("SEARCH MEDICINE");

    loadMedicines(arr, &count);

    printf("  Enter medicine name to search: ");
    scanf(" %[^\n]", keyword);

    /* Convert keyword to lowercase */
    for (j = 0; keyword[j]; j++)
        keyLower[j] = (keyword[j] >= 'A' && keyword[j] <= 'Z')
                      ? keyword[j] + 32 : keyword[j];
    keyLower[j] = '\0';

    printf("\n");
    printf("  %-5s %-20s %-18s %-8s %-8s %-12s\n",
           "ID", "Name", "Company", "Price", "Qty", "Expiry");
    printLine();

    for (i = 0; i < count; i++) {
        /* Convert name to lowercase for comparison */
        for (j = 0; arr[i].name[j]; j++)
            nameLower[j] = (arr[i].name[j] >= 'A' && arr[i].name[j] <= 'Z')
                           ? arr[i].name[j] + 32 : arr[i].name[j];
        nameLower[j] = '\0';

        if (strstr(nameLower, keyLower) != NULL) {
            printf("  %-5d %-20s %-18s %-8.2f %-8d %02d/%02d/%04d\n",
                   arr[i].id,
                   arr[i].name,
                   arr[i].company,
                   arr[i].price,
                   arr[i].quantity,
                   arr[i].expiry_day,
                   arr[i].expiry_month,
                   arr[i].expiry_year);
            found++;
        }
    }

    if (found == 0)
        printf("  [!] No medicine found with name: %s\n", keyword);
    else
        printf("\n  Found %d result(s).\n", found);

    pauseScreen();
}


/* ============================================================
   UPDATE MEDICINE
   ============================================================ */
void updateMedicine() {
    struct Medicine arr[MAX_MEDICINES];
    int count = 0;
    int id, found = 0;
    int i;

    printHeader("UPDATE MEDICINE");

    loadMedicines(arr, &count);

    printf("  Enter Medicine ID to update: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++) {
        if (arr[i].id == id) {
            found = 1;
            printf("\n  Current Name    : %s\n", arr[i].name);
            printf("  Current Company : %s\n", arr[i].company);
            printf("  Current Price   : %.2f\n", arr[i].price);
            printf("  Current Quantity: %d\n", arr[i].quantity);
            printf("  Current Expiry  : %02d/%02d/%04d\n\n",
                   arr[i].expiry_day, arr[i].expiry_month, arr[i].expiry_year);

            printf("  New Name        : ");
            scanf(" %[^\n]", arr[i].name);

            printf("  New Company     : ");
            scanf(" %[^\n]", arr[i].company);

            printf("  New Price       : ");
            scanf("%f", &arr[i].price);

            printf("  New Quantity    : ");
            scanf("%d", &arr[i].quantity);

            printf("  New Expiry (DD MM YYYY): ");
            scanf("%d %d %d",
                  &arr[i].expiry_day,
                  &arr[i].expiry_month,
                  &arr[i].expiry_year);

            break;
        }
    }

    if (found) {
        saveMedicines(arr, count);
        printf("\n  [+] Medicine updated successfully!\n");
    } else {
        printf("\n  [!] Medicine with ID %d not found.\n", id);
    }

    pauseScreen();
}


/* ============================================================
   DELETE MEDICINE
   ============================================================ */
void deleteMedicine() {
    struct Medicine arr[MAX_MEDICINES];
    struct Medicine temp[MAX_MEDICINES];
    int count = 0;
    int newCount = 0;
    int id, found = 0;
    int i;
    char confirm;

    printHeader("DELETE MEDICINE");

    loadMedicines(arr, &count);

    printf("  Enter Medicine ID to delete: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++) {
        if (arr[i].id == id) {
            found = 1;
            printf("\n  Found: %s (%s)\n", arr[i].name, arr[i].company);
            printf("  Are you sure? (y/n): ");
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y') {
                /* Skip this record */
                continue;
            } else {
                printf("\n  [!] Delete cancelled.\n");
                pauseScreen();
                return;
            }
        }
        temp[newCount] = arr[i];
        newCount++;
    }

    if (found) {
        saveMedicines(temp, newCount);
        printf("\n  [+] Medicine deleted successfully!\n");
    } else {
        printf("\n  [!] Medicine with ID %d not found.\n", id);
    }

    pauseScreen();
}


/* ============================================================
   SELL MEDICINE
   ============================================================ */
void sellMedicine() {
    struct Medicine arr[MAX_MEDICINES];
    struct BillItem bill[MAX_BILL_ITEMS];
    int    count = 0;
    int    billCount = 0;
    int    i, found;
    char   moreSell;
    char   searchName[MAX_NAME];
    char   customerName[MAX_NAME];
    char   customerPhone[20];
    int    sellQty;
    int    invoiceNumber;
    float  total = 0.0;
    FILE   *fp;
    time_t t;
    struct tm *tm_info;
    char   timeStr[30];

    printHeader("SELL MEDICINE");

    loadMedicines(arr, &count);
    invoiceNumber = getNextInvoiceNumber();

    printf("  Customer Name  : ");
    scanf(" %49[^\n]", customerName);
    printf("  Customer Phone : ");
    scanf(" %19s", customerPhone);

    do {
        found = 0;
        printf("  Enter medicine name to sell: ");
        scanf(" %[^\n]", searchName);

        for (i = 0; i < count; i++) {
            if (strcasecmp(arr[i].name, searchName) == 0) {
                found = 1;

                if (arr[i].quantity == 0) {
                    printf("  [!] Out of stock!\n");
                    break;
                }

                printf("  Available Qty : %d | Price: %.2f\n",
                       arr[i].quantity, arr[i].price);
                printf("  Enter quantity to sell: ");
                scanf("%d", &sellQty);

                if (sellQty <= 0) {
                    printf("  [!] Quantity must be greater than zero.\n");
                    break;
                }

                if (sellQty > arr[i].quantity) {
                    printf("  [!] Not enough stock. Available: %d\n",
                           arr[i].quantity);
                    break;
                }

                /* Update stock */
                arr[i].quantity -= sellQty;

                /* Add to bill */
                strcpy(bill[billCount].name, arr[i].name);
                bill[billCount].quantity = sellQty;
                bill[billCount].price    = arr[i].price;
                bill[billCount].subtotal = arr[i].price * sellQty;
                total += bill[billCount].subtotal;
                billCount++;

                printf("  [+] Added to bill.\n");
                break;
            }
        }

        if (!found)
            printf("  [!] Medicine not found.\n");

        printf("\n  Sell another medicine? (y/n): ");
        scanf(" %c", &moreSell);

    } while ((moreSell == 'y' || moreSell == 'Y') &&
              billCount < MAX_BILL_ITEMS);

    if (billCount == 0) {
        printf("\n  [!] No medicine was sold, so no bill was created.\n");
        pauseScreen();
        return;
    }

    /* Save updated stock */
    saveMedicines(arr, count);

    /* Print bill on screen */
    time(&t);
    tm_info = localtime(&t);
    strftime(timeStr, 30, "%d/%m/%Y %H:%M:%S", tm_info);

    clearScreen();
    printLine();
    printf("  %*s\n", 40, "PHARMACY BILL");
    printf("  Invoice No: INV-%04d\n", invoiceNumber);
    printf("  Date: %s\n", timeStr);
    printf("  Customer: %s | Phone: %s\n", customerName, customerPhone);
    printf("  Sold by: %s (ID: %s)\n", loggedInStaff.name, loggedInStaff.id);
    printLine();
    printf("  %-20s %-6s %-8s %-10s\n",
           "Medicine", "Qty", "Price", "Subtotal");
    printLine();

    for (i = 0; i < billCount; i++) {
        printf("  %-20s %-6d %-8.2f %-10.2f\n",
               bill[i].name,
               bill[i].quantity,
               bill[i].price,
               bill[i].subtotal);
    }

    printLine();
    printf("  %*s%.2f\n", 38, "TOTAL = Tk. ", total);
    printLine();

    /* Save bill to sales history. Admin can see the selling staff here. */
    fp = fopen(SALES_FILE, "a");
    if (fp != NULL) {
        fprintf(fp, "=== BILL === Invoice: INV-%04d | Date: %s\n", invoiceNumber, timeStr);
        fprintf(fp, "Customer: %s | Phone: %s\n", customerName, customerPhone);
        fprintf(fp, "Sold by: %s (Staff ID: %s)\n", loggedInStaff.name, loggedInStaff.id);
        fprintf(fp, "%-20s %-6s %-8s %-10s\n",
                "Medicine", "Qty", "Price", "Subtotal");
        for (i = 0; i < billCount; i++) {
            fprintf(fp, "%-20s %-6d %-8.2f %-10.2f\n",
                    bill[i].name,
                    bill[i].quantity,
                    bill[i].price,
                    bill[i].subtotal);
        }
        fprintf(fp, "TOTAL: %.2f\n", total);
        fprintf(fp, "-----------------------------\n");
        fclose(fp);
        printf("\n  [+] Bill saved to %s\n", SALES_FILE);
    }

    /* Keep a printable copy for the Staff Panel's Generate Bill option. */
    fp = fopen(LAST_BILL_FILE, "w");
    if (fp != NULL) {
        fprintf(fp, "================ PHARMACY BILL ================\n");
        fprintf(fp, "Invoice No: INV-%04d\n", invoiceNumber);
        fprintf(fp, "Date: %s\n", timeStr);
        fprintf(fp, "Customer: %s | Phone: %s\n", customerName, customerPhone);
        fprintf(fp, "Sold by: %s (Staff ID: %s)\n", loggedInStaff.name, loggedInStaff.id);
        fprintf(fp, "-------------------------------------------------\n");
        fprintf(fp, "%-20s %-6s %-8s %-10s\n", "Medicine", "Qty", "Price", "Subtotal");
        for (i = 0; i < billCount; i++) {
            fprintf(fp, "%-20s %-6d %-8.2f %-10.2f\n",
                    bill[i].name, bill[i].quantity, bill[i].price, bill[i].subtotal);
        }
        fprintf(fp, "-------------------------------------------------\n");
        fprintf(fp, "TOTAL: Tk. %.2f\n", total);
        fclose(fp);
    }

    pauseScreen();
}


/* ============================================================
   STAFF: GENERATE / PRINT LATEST BILL
   ============================================================ */
void generateLatestBill() {
    FILE *fp;
    char line[200];

    printHeader("LATEST BILL");
    fp = fopen(LAST_BILL_FILE, "r");
    if (fp == NULL) {
        printf("  [!] No bill has been generated yet. Sell medicine first.\n");
        pauseScreen();
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("  %s", line);
    }
    fclose(fp);
    pauseScreen();
}


/* ============================================================
   GENERATE BILL (View Sales History)
   ============================================================ */
void generateBill() {
    FILE *fp;
    char  line[200];

    printHeader("SALES HISTORY / BILLS");

    fp = fopen(SALES_FILE, "r");
    if (fp == NULL) {
        printf("  [!] No sales record found yet.\n");
        pauseScreen();
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("  %s", line);
    }

    fclose(fp);
    pauseScreen();
}


/* ============================================================
   ADMIN: STAFF-WISE SALES REPORT
   ============================================================ */
void staffSalesReport() {
    FILE *fp;
    char line[200];
    char currentStaffId[20] = "";
    char currentStaffName[30] = "";
    int saleCount[STAFF_COUNT] = {0};
    float saleTotal[STAFF_COUNT] = {0};
    int i;

    printHeader("STAFF SALES REPORT");
    fp = fopen(SALES_FILE, "r");
    if (fp == NULL) {
        printf("  [!] No sales record found yet.\n");
        pauseScreen();
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        float total;
        if (sscanf(line, "Sold by: %29[^()] (Staff ID: %19[^)])",
                   currentStaffName, currentStaffId) == 2) {
            continue;
        }

        if (sscanf(line, "TOTAL: %f", &total) == 1) {
            for (i = 0; i < STAFF_COUNT; i++) {
                if (strcmp(currentStaffId, staffList[i].id) == 0) {
                    saleCount[i]++;
                    saleTotal[i] += total;
                    break;
                }
            }
            currentStaffId[0] = '\0';
        }
    }
    fclose(fp);

    printf("  %-10s %-15s %-12s %-15s\n",
           "Staff ID", "Name", "Sales Count", "Total Sales");
    printLine();
    for (i = 0; i < STAFF_COUNT; i++) {
        printf("  %-10s %-15s %-12d Tk. %-10.2f\n",
               staffList[i].id, staffList[i].name,
               saleCount[i], saleTotal[i]);
    }
    pauseScreen();
}


/* ============================================================
   STOCK REPORT
   ============================================================ */
void stockReport() {
    struct Medicine arr[MAX_MEDICINES];
    int count = 0;
    int i;
    int lowStock = 0;

    printHeader("STOCK REPORT");

    loadMedicines(arr, &count);

    if (count == 0) {
        printf("  [!] No medicines in database.\n");
        pauseScreen();
        return;
    }

    printf("  %-5s %-20s %-10s %-10s\n",
           "ID", "Name", "Quantity", "Status");
    printLine();

    for (i = 0; i < count; i++) {
        char *status;
        if (arr[i].quantity == 0)
            status = "OUT OF STOCK";
        else if (arr[i].quantity <= 10)
            status = "LOW STOCK";
        else
            status = "OK";

        if (arr[i].quantity <= 10) lowStock++;

        printf("  %-5d %-20s %-10d %-10s\n",
               arr[i].id,
               arr[i].name,
               arr[i].quantity,
               status);
    }

    printLine();
    printf("  Total Medicines  : %d\n", count);
    printf("  Low/Out of Stock : %d\n", lowStock);
    pauseScreen();
}


/* ============================================================
   EXPIRED MEDICINE CHECK
   ============================================================ */
void expiredMedicineCheck() {
    struct Medicine arr[MAX_MEDICINES];
    int count = 0;
    int i;
    int today_d, today_m, today_y;
    int expiredCount = 0;

    printHeader("EXPIRED MEDICINE CHECK");

    loadMedicines(arr, &count);
    getCurrentDate(&today_d, &today_m, &today_y);

    printf("  Today's Date: %02d/%02d/%04d\n\n", today_d, today_m, today_y);

    printf("  %-5s %-20s %-12s %-10s\n",
           "ID", "Name", "Expiry", "Status");
    printLine();

    for (i = 0; i < count; i++) {
        int expired = isExpired(arr[i].expiry_day,
                                arr[i].expiry_month,
                                arr[i].expiry_year);

        /* Check if expiring within 30 days */
        int soonExpire = 0;
        if (!expired) {
            /* Simple check: same month and year, day within 30 */
            if (arr[i].expiry_year == today_y &&
                arr[i].expiry_month == today_m &&
                arr[i].expiry_day - today_d <= 30)
                soonExpire = 1;
            else if (arr[i].expiry_year == today_y &&
                     arr[i].expiry_month == today_m + 1 &&
                     arr[i].expiry_day + (30 - today_d) <= 30)
                soonExpire = 1;
        }

        if (expired || soonExpire) {
            char *status = expired ? "EXPIRED" : "EXPIRING SOON";
            printf("  %-5d %-20s %02d/%02d/%04d  %-15s\n",
                   arr[i].id,
                   arr[i].name,
                   arr[i].expiry_day,
                   arr[i].expiry_month,
                   arr[i].expiry_year,
                   status);
            if (expired) expiredCount++;
        }
    }

    if (expiredCount == 0)
        printf("  [+] No expired medicines found.\n");
    else
        printf("\n  Total expired: %d\n", expiredCount);

    pauseScreen();
}


/* ============================================================
   FILE HELPER: LOAD MEDICINES FROM FILE
   ============================================================ */
void loadMedicines(struct Medicine arr[], int *count) {
    FILE *fp;
    struct Medicine m;
    *count = 0;

    fp = fopen(MEDICINE_FILE, "r");
    if (fp == NULL) return;

    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%f|%d|%d|%d|%d\n",
                  &m.id, m.name, m.company,
                  &m.price, &m.quantity,
                  &m.expiry_day, &m.expiry_month, &m.expiry_year) == 8) {
        arr[*count] = m;
        (*count)++;
        if (*count >= MAX_MEDICINES) break;
    }

    fclose(fp);
}


/* ============================================================
   FILE HELPER: SAVE MEDICINES TO FILE
   ============================================================ */
void saveMedicines(struct Medicine arr[], int count) {
    FILE *fp;
    int i;

    fp = fopen(MEDICINE_FILE, "w");
    if (fp == NULL) {
        printf("  [!] Error: Cannot open file to save.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%.2f|%d|%d|%d|%d\n",
                arr[i].id,
                arr[i].name,
                arr[i].company,
                arr[i].price,
                arr[i].quantity,
                arr[i].expiry_day,
                arr[i].expiry_month,
                arr[i].expiry_year);
    }

    fclose(fp);
}


/* ============================================================
   FILE HELPER: GET LAST ID FROM FILE
   ============================================================ */
int getLastId() {
    struct Medicine arr[MAX_MEDICINES];
    int count = 0;
    int lastId = 0;
    int i;

    loadMedicines(arr, &count);
    for (i = 0; i < count; i++) {
        if (arr[i].id > lastId)
            lastId = arr[i].id;
    }
    return lastId;
}


/* ============================================================
   FILE HELPER: GET NEXT INVOICE NUMBER FROM SALES HISTORY
   ============================================================ */
int getNextInvoiceNumber() {
    FILE *fp;
    char line[200];
    int lastInvoice = 1000;
    int invoice;

    fp = fopen(SALES_FILE, "r");
    if (fp == NULL) return lastInvoice + 1;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "=== BILL === Invoice: INV-%d", &invoice) == 1 &&
            invoice > lastInvoice) {
            lastInvoice = invoice;
        }
    }
    fclose(fp);
    return lastInvoice + 1;
}


/* ============================================================
   UTILITY: PRINT A HORIZONTAL LINE
   ============================================================ */
void printLine() {
    printf("  -------------------------------------------------------\n");
}


/* ============================================================
   UTILITY: PRINT SECTION HEADER
   ============================================================ */
void printHeader(char *title) {
    clearScreen();
    printLine();
    printf("  %s\n", title);
    printLine();
    printf("\n");
}


/* ============================================================
   UTILITY: PAUSE SCREEN
   ============================================================ */
void pauseScreen() {
    printf("\n  Press Enter to continue...");
    getchar();
    getchar();
}


/* ============================================================
   UTILITY: CLEAR SCREEN
   ============================================================ */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


/* ============================================================
   UTILITY: GET TODAY'S DATE
   ============================================================ */
void getCurrentDate(int *d, int *m, int *y) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    *d = tm_info->tm_mday;
    *m = tm_info->tm_mon + 1;
    *y = tm_info->tm_year + 1900;
}


/* ============================================================
   UTILITY: CHECK IF MEDICINE IS EXPIRED
   ============================================================ */
int isExpired(int ed, int em, int ey) {
    int td, tm, ty;
    getCurrentDate(&td, &tm, &ty);

    if (ey < ty) return 1;
    if (ey == ty && em < tm) return 1;
    if (ey == ty && em == tm && ed < td) return 1;
    return 0;
}
