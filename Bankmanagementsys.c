#include <stdio.h>
#include <string.h>

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();
void view_transaction_history();
void save_transaction(int acc_no, const char* type, float amount, float balance_after);
void delete_account();
void calculate_interest();
void transfer_money();
void view_all_accounts();    
void view_account_details();  
void mini_statement();        

const char* ACCOUNT_FILE = "account.dat";
const char* TRANSACTION_FILE = "transactions.dat";

typedef struct {
    char name[50];
    int acc_no;
    float balance;
} Account;

typedef struct {
    int acc_no;
    char type[20];  
    float amount;
    float balance_after;
} Transaction;

int main() {
    while (1) {
        int choice;
        printf("\n*** Bank Management System ***\n");
        printf("\n1. Create Account");
        printf("\n2. Deposit Money");
        printf("\n3. Withdraw Money");
        printf("\n4. Check Balance");
        printf("\n5. View Transaction History");
        printf("\n6. Delete Account");
        printf("\n7. Calculate Interest on Savings");
        printf("\n8. Transfer Money to Another Account");
        printf("\n9. Admin Panel (View All Accounts)");
        printf("\n10. View Account Details");
        printf("\n11. Mini Statement (Last 5 Transactions)");
        printf("\n12. Exit");  
        printf("\nEnter your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_account(); break;
            case 2: deposit_money(); break;
            case 3: withdraw_money(); break;
            case 4: check_balance(); break;
            case 5: view_transaction_history(); break;
            case 6: delete_account(); break;
            case 7: calculate_interest(); break;
            case 8: transfer_money(); break;
            case 9: view_all_accounts(); break;
            case 10: view_account_details(); break;
            case 11: mini_statement(); break;
            case 12:
                printf("\nClosing the Bank, Thanks For Your Visit....\n");
                return 0;
            default:
                printf("\nInvalid Choice! Try again....\n");
                break;
        }
    }
}

void create_account() {
    Account acc;
    FILE *file = fopen(ACCOUNT_FILE, "ab+");
    if (file == NULL) {
        printf("\nUnable to open file!\n");
        return;
    }
    char c;
    do { c = getchar(); } while (getchar() != '\n' && c != EOF);

    printf("\nEnter Your Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';

    printf("Enter Your Account Number: ");
    scanf("%d", &acc.acc_no);
    acc.balance = 0;

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);
    printf("\nAccount Created Successfully!\n");
}

void deposit_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) { printf("\nUnable to open file!\n"); return; }
    
    int acc_no;
    float money;
    printf("\nEnter Account Number: "); scanf("%d", &acc_no);
    printf("Enter Amount to Deposit: "); scanf("%f", &money);

    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            acc.balance += money;
            fseek(file, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);
            fclose(file);
            save_transaction(acc_no, "Deposit", money, acc.balance);
            printf("\nRs.%.2f Deposited. New Balance: Rs.%.2f\n", money, acc.balance);
            return;
        }
    }
    fclose(file);
    printf("\nAccount Number %d not found.\n", acc_no);
}

void withdraw_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) { printf("\nUnable to open file!\n"); return; }

    int acc_no;
    float money;
    printf("\nEnter Account Number: "); scanf("%d", &acc_no);
    printf("Enter Amount to Withdraw: "); scanf("%f", &money);

    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            if (acc.balance >= money) {
                acc.balance -= money;
                fseek(file, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                fclose(file);
                save_transaction(acc_no, "Withdraw", money, acc.balance);
                printf("\nRs.%.2f Withdrawn. New Balance: Rs.%.2f\n", money, acc.balance);
            } else {
                printf("\nInsufficient Balance!\n");
            }
            return;
        }
    }
    fclose(file);
    printf("\nAccount Number %d not found.\n", acc_no);
}

void check_balance() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) { printf("\nUnable to open file!\n"); return; }

    int acc_no;
    printf("\nEnter Account Number: "); scanf("%d", &acc_no);

    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            printf("\nCurrent Balance: Rs.%.2f\n", acc.balance);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("\nAccount Number %d not found.\n", acc_no);
}

void view_transaction_history() {
    FILE *file = fopen(TRANSACTION_FILE, "rb");
    if (file == NULL) { printf("\nNo transaction history found!\n"); return; }

    int acc_no;
    printf("\nEnter Account Number to View Transactions: ");
    scanf("%d", &acc_no);

    Transaction t;
    int found = 0;
    printf("\n--- Transaction History for Account %d ---\n", acc_no);
    printf("Type\t\tAmount\t\tBalance After\n");

    while (fread(&t, sizeof(t), 1, file)) {
        if (t.acc_no == acc_no) {
            printf("%s\t\t%.2f\t\t%.2f\n", t.type, t.amount, t.balance_after);
            found = 1;
        }
    }
    fclose(file);
    if (!found) printf("\nNo transactions found for Account %d.\n", acc_no);
}

void save_transaction(int acc_no, const char* type, float amount, float balance_after) {
    FILE *file = fopen(TRANSACTION_FILE, "ab+");
    if (file == NULL) return;

    Transaction t;
    strcpy(t.type, type);
    t.acc_no = acc_no;
    t.amount = amount;
    t.balance_after = balance_after;

    fwrite(&t, sizeof(t), 1, file);
    fclose(file);
}

void delete_account() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) { printf("\nUnable to open file!\n"); return; }

    FILE *temp = fopen("temp.dat", "wb");
    if (temp == NULL) { fclose(file); printf("\nUnable to create temp file!\n"); return; }

    int acc_no, found = 0;
    printf("\nEnter Account Number to Delete: "); scanf("%d", &acc_no);

    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            found = 1;
            printf("\nAccount %d Deleted Successfully.\n", acc_no);
        } else {
            fwrite(&acc, sizeof(acc), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);

    if (found) { remove(ACCOUNT_FILE); rename("temp.dat", ACCOUNT_FILE); }
    else { printf("\nAccount %d not found.\n", acc_no); remove("temp.dat"); }
}

void calculate_interest() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) { printf("\nUnable to open file!\n"); return; }

    int acc_no;
    float rate = 5.0, time;
    printf("\nEnter Account Number: "); scanf("%d", &acc_no);
    printf("Enter Time Period (Years): "); scanf("%f", &time);

    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            float interest = (acc.balance * rate * time) / 100;
            printf("\n--- Interest Calculation ---\n");
            printf("Account Holder: %s\nCurrent Balance: Rs.%.2f\n", acc.name, acc.balance);
            printf("Interest for %.2f years @ %.2f%% = Rs.%.2f\n", time, rate, interest);
            printf("Total Amount after Interest: Rs.%.2f\n", acc.balance + interest);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("\nAccount %d not found.\n", acc_no);
}

void transfer_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) { printf("\nUnable to open file!\n"); return; }

    int sender_acc, receiver_acc;
    float amount;
    printf("\nEnter Your (Sender) Account Number: "); scanf("%d", &sender_acc);
    printf("Enter Receiver's Account Number: "); scanf("%d", &receiver_acc);
    printf("Enter Amount to Transfer: "); scanf("%f", &amount);

    if (sender_acc == receiver_acc) {
        printf("\nCannot transfer to the same account!\n");
        fclose(file);
        return;
    }

    Account acc;
    int sender_found = 0, receiver_found = 0;
    float sender_balance = 0;

    FILE *temp = fopen("temp.dat", "wb");
    if (temp == NULL) { fclose(file); printf("\nUnable to create temp file!\n"); return; }

    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == sender_acc) {
            sender_found = 1;
            if (acc.balance >= amount) {
                acc.balance -= amount;
                sender_balance = acc.balance;
            } else {
                printf("\nInsufficient Balance!\n");
                fclose(file); fclose(temp); remove("temp.dat");
                return;
            }
            fwrite(&acc, sizeof(acc), 1, temp);
        } else if (acc.acc_no == receiver_acc) {
            receiver_found = 1;
            acc.balance += amount;
            fwrite(&acc, sizeof(acc), 1, temp);
        } else {
            fwrite(&acc, sizeof(acc), 1, temp);
        }
    }
    fclose(file); fclose(temp);

    if (!sender_found) { printf("\nSender Account not found.\n"); remove("temp.dat"); return; }
    if (!receiver_found) { printf("\nReceiver Account not found.\n"); remove("temp.dat"); return; }

    remove(ACCOUNT_FILE); rename("temp.dat", ACCOUNT_FILE);
    printf("\nRs.%.2f transferred from %d to %d.\n", amount, sender_acc, receiver_acc);

    save_transaction(sender_acc, "TransferOut", amount, sender_balance);
    save_transaction(receiver_acc, "TransferIn", amount, sender_balance + amount);
}

void view_all_accounts() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) {
        printf("\nNo accounts found.\n");
        return;
    }

    Account acc;
    int count = 0;

    printf("\n--- All Accounts ---\n");
    printf("%-15s %-15s %-15s\n", "Name", "Account No", "Balance");

    while (fread(&acc, sizeof(acc), 1, file)) {
        printf("%-15s %-15d Rs.%-12.2f\n", acc.name, acc.acc_no, acc.balance);
        count++;
    }

    if (count == 0)
        printf("No accounts to display.\n");

    fclose(file);
}

void view_account_details() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) {
        printf("\nUnable to open account file.\n");
        return;
    }

    int acc_no, found = 0;
    printf("\nEnter Account Number to View Details: ");
    scanf("%d", &acc_no);

    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            printf("\n--- Account Details ---\n");
            printf("Name: %s\n", acc.name);
            printf("Account Number: %d\n", acc.acc_no);
            printf("Balance: Rs.%.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("\nAccount Number %d not found.\n", acc_no);
    }
}

void mini_statement() {
    FILE *file = fopen(TRANSACTION_FILE, "rb");
    if (file == NULL) {
        printf("\nNo transaction history available.\n");
        return;
    }

    int acc_no;
    printf("\nEnter Account Number for Mini Statement: ");
    scanf("%d", &acc_no);

    Transaction list[1000];
    int count = 0;

    Transaction t;
    while (fread(&t, sizeof(t), 1, file)) {
        if (t.acc_no == acc_no) {
            list[count++] = t;
        }
    }

    fclose(file);

    if (count == 0) {
        printf("\nNo transactions found for Account %d.\n", acc_no);
        return;
    }

    printf("\n--- Last %d Transaction(s) for Account %d ---\n", (count >= 5 ? 5 : count), acc_no);
    printf("Type\t\tAmount\t\tBalance After\n");

    for (int i = (count - 5 < 0 ? 0 : count - 5); i < count; i++) {
        printf("%s\t\t%.2f\t\t%.2f\n", list[i].type, list[i].amount, list[i].balance_after);
    }
}
