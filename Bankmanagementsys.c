#include<stdio.h>
#include<string.h>

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();

const char* ACCOUNT_FILE="account.dat";
typedef struct
{
    char name[50];
    int acc_no;
    float balance;
}Account;

int main(){
    while(1){
        int choice;
        printf("\n*** Bank Management System***\n");
        printf("\n1. Create Account");
        printf("\n2. Deposit Money");
        printf("\n3. Withdraw Money");
        printf("\n4. Check Balance");
        printf("\n5. Exit");
        printf("\nEnter your Choice : ");
        scanf("%d",&choice);

        switch (choice){
            case 1:
            create_account();
            break;
            case 2:
            deposit_money();
            break;
            case 3:
            withdraw_money();
            break;
            case 4:
            check_balance();
            break;
            case 5:
            printf("\nClosing the Bank,Thanks For Your Visit....\n");
            break;
            return 0;
            default:
            printf("\nInvalid Choice!,Try again....\n");
            break;

        }

        
    }
}

void create_account(){
     Account acc;
    FILE *file = fopen(ACCOUNT_FILE,"ab+"); //a->append mode (means at the end) b->binery file
    if(file==NULL){
        printf("\n Unable to open file !!");
        return;
    }
    char c;
    do{
        c=getchar();
    }
   while(getchar() != '\n' && c != EOF);

    printf("\n Enter Your Name:");
    fgets(acc.name,sizeof(acc.name),stdin);
    int ind=strcspn(acc.name,"\n");
    acc.name[ind]='\0';
    printf("Enter Your Account Number:");
    scanf("%d",&acc.acc_no);
    acc.balance=0;

    fwrite(&acc,sizeof(acc),1,file);
    fclose(file);
    printf("\nAccount Created Successfully!!");

    
     

}
void deposit_money(){
   FILE *file= fopen(ACCOUNT_FILE,"rb+");
   if(file==NULL){
    printf("\n Unable to open file !!");
    return ;
   }
   int acc_no;
   float money;
   printf("\nEnter Your Account Number:");
   scanf("%d",&acc_no);
   printf("\nEnter the amount you want to deposit:");
   scanf("%f",&money);
   Account acc_to_read;
   while(fread(&acc_to_read,sizeof(acc_to_read),1,file)){
    if(acc_to_read.acc_no == acc_no){
        acc_to_read.balance+=money;
        fseek(file,-sizeof(acc_to_read),SEEK_CUR);
        fwrite(&acc_to_read,sizeof(acc_to_read),1,file);
        fclose(file);
        printf("Successfully Deposited Rs.%.2f.The New Balance is Rs.%.2f",money,acc_to_read.balance);
        return;
    }

   }

   fclose(file);
   printf("Money Could Not Be Deposited As Account Number:%d is not found In Records",acc_no);
}



void withdraw_money(){
   FILE *file= fopen(ACCOUNT_FILE,"rb+");
   if(file == NULL){
    printf("\n Unable to open the file");\
    return ;

   }
   int acc_no;
   float money;
   printf("\nEnter Your Account Number:");
   scanf("%d",&acc_no);
   printf("\nEnter the amount you wish to Withdraw:");
   scanf("%f",&money);
   Account acc_r;
   while(fread(&acc_r,sizeof(acc_r),1,file)){
    if(acc_r.acc_no==acc_no){
        if(acc_r.balance >=money){
            acc_r.balance-=money;
            fseek(file,-sizeof(acc_r),SEEK_CUR);
            fwrite(&acc_r,sizeof(acc_r),1,file);
            printf("SuccessFully Withdrawn Rs.%.2f .Reamaining Balance is Rs.%.2f",money,acc_r.balance);
        }
        else{
            printf("Insufficient Balance!");
        }
        fclose(file);
        return ;
    }
   }
   fclose(file);
   printf("Money Could Not Be Withdrawn As Account Number %d Was Not Found",acc_no);


}
void check_balance(){
    FILE *file=fopen(ACCOUNT_FILE,"rb");
    if(file == NULL){
        printf("\n Unable to oprn file ");
        return ;
    }
    int acc_no;
    Account acc_read;
    printf("Enter Your Account Number:");
    scanf("%d",&acc_no);

    while(fread(&acc_read,sizeof(acc_read),1,file)){
        if(acc_read.acc_no ==acc_no){
            printf("\n Your Balance is Rs.%.2f",acc_read.balance);
            fclose(file);
            return; 
        }
    }
    fclose(file);
    printf("\nAccount No:%d was not found.\n",acc_no);
}