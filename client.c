#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#define MAX_STOCKS 100 // maximum number of stocks that are possible

int c,q=0;
char loginpath[80];

struct stock{
    char name[51];
    int baseVal;
    int currVal;
    int incr;
};// structure stores the current details of the stocks available in the market

struct user{
	char name[51];
	char password[9];
	int num[20];
	int basePrice[20];
	char stocks[20][51];
	long profit;
	long capital;
};// this stores the details of the users - name, password, stocks bought, etc.

struct user u;
struct stock stocks[MAX_STOCKS];

extern void setRunning(int);
extern void setSignal(char);
extern int isRunning();
extern char getSignal();
void quit();
void company();
void customer();
void getQuote();
void getGraph();
void createStock();
void registerUser();
void availablestocks();
void buyStocks();
void login();
void loggedin();
void logout();
void sellStocks();
void myPortfolio();

int main(){
    if(!isRunning())//checking whether the server is running or not
    	printf("Stocks server is not running! Please run the stocks server before running the client!\n");
    else{
    	do{
	    	printf("\n\tVirtual Stock Market Management System\n\t---------------------------------------\n\n");
    		printf("1. Company Login - Create stocks and view current prices/analysis\n");
    		printf("2. Customer Login - Buy and Sell Stocks\n");
    		printf("3. Exit\n\n");
    		printf("Enter choice:");
    		scanf("%d",&c);
    		switch(c){
    			case 1:
    				company();
    				break;
    			case 2:
    				customer();
    				break;
    			case 3:
    				quit();
    				break;
    			default:
    				printf("Invalid choice! Enter again.\n\n");
    		}
    	}while(!q);
    }
    return 0;
}

void quit(){//code to quit the client - this asks to option to quit the server as well
	char chr;
	printf("Stop the server as well? (y/n) ");
	getchar();
	chr=getchar();
	if(chr=='y'){
		setRunning(0);
		setSignal('q');// passing the quit signal to the server
	}
	q=1;
}

void company(){// function to print the company login menu
	int p=1;
	do{
		printf("\nWelcome to the company management console!\n\n");
		printf("1. View current stock price of a company\n");
		printf("2. View past stock history of the company\n");
		printf("3. Create a new stock for a company\n");
		printf("4. Go Back\n\n");
		printf("Enter choice:");
		scanf("%d",&c);
		switch(c){
			case 1:
				getQuote();
				break;
			case 2:
				getGraph();
				break;
			case 3:
				createStock();
				break;
			case 4:
				p=0;
				break;
			default:
				printf("Invalid Choice. Enter again!\n");
		}
	}while(p);
}

void getQuote(){//function fetches the current value of stock in the market
	char cmp[51];
	printf("Enter the name of the company whose stock you want to get: ");
	scanf("%s",cmp);
	setSignal('w');// setting the wait signal on server to prevent file-locking conflicts
	int i,found=0;
	FILE *f=fopen("common/stocks.dat","r+");
	fread(stocks,sizeof(struct stock),MAX_STOCKS,f);
	for(i=0;i<MAX_STOCKS;i++){
		if(strcmp(stocks[i].name,cmp)==0){
			printf("\nThe current stock value of %s is: %d\n",stocks[i].name,stocks[i].currVal);
			found=1;
			break;
		}
	}
	if(!found)
		printf("\nThe stock for the company doesn't exist!\n");
	printf("\nPress enter to continue...\n");
	getchar();
	getchar();
	setSignal('\0');// setting the null signal on the server
}

void getGraph(){
	char cmp[51];
	printf("Enter the name of the company whose stock you want to get: ");
	scanf("%s",cmp);
	setSignal('w');
	int i,found=0;
	FILE *f=fopen("common/stocks.dat","r+");
	fread(stocks,sizeof(struct stock),MAX_STOCKS,f);
	for(i=0;i<MAX_STOCKS;i++){
		if(strcmp(stocks[i].name,cmp)==0){
			FILE *fp=fopen("common/plot","w+");
			fprintf(fp,"plot \"common/records/%s.txt\" with line\npause -1 \"Press enter to continue...\"",stocks[i].name);
			fclose(fp);
			system("gnuplot common/plot");
			found=1;
			break;
		}
	}
	if(!found){
		printf("\nThe stock for the company doesn't exist!\n");
		printf("\nPress enter to continue...\n");
		getchar();
		getchar();
	}
	setSignal('\0');
}

void createStock(){
	char name[51];
	int val,i;
	printf("\nEnter the name of the stock that you want to create:");
	scanf("%s",name);
	printf("Enter the base price for the stock:");
	scanf("%d",&val);
	setSignal('w');
	FILE *f=fopen("common/stocks.dat","r");
	fread(stocks,sizeof(struct stock),MAX_STOCKS,f);
	fclose(f);
	for(i=0;i<MAX_STOCKS;i++){
		if(strcmp(stocks[i].name,"")==0){
			strcpy(stocks[i].name,name);
			stocks[i].baseVal=val;
			stocks[i].currVal=val;
			break;
		}
	}
	f=fopen("common/stocks.dat","w+");
	fwrite(stocks,sizeof(struct stock),MAX_STOCKS,f);
	fclose(f);
	printf("\nStock created successfully!\n");
	printf("\nPress enter to continue...\n");
	getchar();
	getchar();
	setSignal('\0');
}

void customer(){
	int p=1;
	do{
		printf("\n1. New User? Register!\n");
		printf("2. Old User? Login!\n");
		printf("3. Go back\n");
		printf("Enter choice: ");
		scanf("%d",&c);
		switch(c){
			case 1:
				registerUser();
				break;
			case 2:
				login();
				break;
			case 3:
				p=0;
				break;
			default:
				printf("\nWrong choice. Enter again.\n");
		}
	}while(p);
}

void registerUser(){       //Registering for a new account
	struct user usr;
	struct termios oflags, nflags;
	int i;
	printf("\nEnter account details below.\n\n");
	printf("Enter name:");
	scanf("%s",usr.name);
	tcgetattr(fileno(stdin), &oflags);
	nflags = oflags;
	nflags.c_lflag &= ~ECHO;
	nflags.c_lflag |= ECHONL;
	if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        	perror("tcsetattr");
       	printf("Enter password: ");
        scanf("%s",usr.password);
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        	perror("tcsetattr");
	char path[80]="common/users/";
	strcat(path,usr.name);
	usr.capital=1000000;
	usr.profit=0;
	for(i=0;i<20;i++)
	strcpy(usr.stocks[i],"");
	FILE *f=fopen(path,"w+");
	fwrite(&usr,sizeof(struct user),1,f);
	fclose(f);
	printf("New account created successfully!\n");
	printf("Press enter to continue...\n");
	getchar();
	getchar();
}

void login(){
	//struct user usr;
	struct termios oflags, nflags;
	char username[51],pwd[9];
	printf("Enter username: ");
	scanf("%s",username);
	tcgetattr(fileno(stdin), &oflags);
	nflags = oflags;
	nflags.c_lflag &= ~ECHO;
	nflags.c_lflag |= ECHONL;
	if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        	perror("tcsetattr");
        printf("Enter password: ");
        scanf("%s",pwd);
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        	perror("tcsetattr");
        char path[80]="common/users/";
	strcat(path,username);
	FILE *f=fopen(path,"r+");
	if(f==NULL){
		printf("User doesn't exist!\n\n");
		return;
	}
	fread(&u,sizeof(struct user),1,f);
	if(strcmp(pwd,u.password)==0){
		printf("Login successful!!\n\n");
		strcpy(loginpath,path);
		loggedin();
	}else
		printf("Wrong username/password combination!\n\n");
}

void loggedin(){
	int p=1;
	do{
	printf("1. Buy Stocks\n2. Sell stocks\n3. Get Stock Quotes\n4. Get previous stocks history\n5. View available stocks\n6. My Portfolio\n7. Logout\n\n");
	
	printf("Enter your choice: ");
	scanf("%d",&c);
	
	switch(c)
	{
	case 1:
	buyStocks();
	break;
	
	case 2:
	sellStocks();
	break;
	
	case 3:
	getQuote();
	break;
	
	case 4:
	getGraph();
	break;
	
	case 5:
	availablestocks();
	break;
	
	case 6:
	myPortfolio();
	break;
	
	case 7:
	logout();
	p=0;
	break;
	
	default:
	printf("\nWrong choice. Enter again.\n");
	}
	}while(p);
}

void availablestocks(){
	setSignal('w');
	int i;
	FILE *f=fopen("common/stocks.dat","r+");
	fread(stocks,sizeof(struct stock),MAX_STOCKS,f);
	for(i=0;i<MAX_STOCKS;i++){
		if (strcmp(stocks [i].name,"")==0)
		break;
		else
		printf("%s\n",stocks[i].name);
	}
}

void buyStocks(){
	char name[51];
	int qty,j;
	printf("Enter the name of the stock you wish to buy:");
	scanf("%s",name);
	
	int i,found=0;
	FILE *f=fopen("common/stocks.dat","r+");
	fread(stocks,sizeof(struct stock),MAX_STOCKS,f);
	for(i=0;i<MAX_STOCKS;i++){
		if(strcmp(stocks[i].name,name)==0){
			printf("\nThe current stock value of %s is: %d\nYour capital is Rs. %ld\n",stocks[i].name,stocks[i].currVal,u.capital);
			found=1;
			break;
		}
	}
	if(!found){
		printf("\nThe stock for the company doesn't exist!\n");	
		return;
	}
		
	printf("Enter the quantity that you wish to buy\n");
	scanf("%d",&qty);
	
	long price=stocks[i].currVal*qty;
	
	if(u.capital<price)
	printf("Transaction failed due to insufficient funds!\n");
	else
	{
	u.capital-=price;
	for(j=0;j<20;j++){
		if(strcmp(u.stocks[j],name)==0){
			u.num[j]+=qty;			
			break;
		}else if(strcmp(u.stocks[j],"")==0){
			strcpy(u.stocks[j],name);
			u.num[j]=qty;
			break;		
		}
	}
	u.basePrice[j]=stocks[i].currVal;
	printf("\nStock bought successfully!\n\n");
	}
	printf("Press enter to continue...\n");
	getchar();
	getchar();
}

void logout(){
	char path[80]="common/users/";
	strcat(path,u.name);
	FILE *f=fopen(path,"w+");
	fwrite(&u,sizeof(struct user),1,f);
	fclose(f);
	printf("\n You have logged out successfully!!\n \n");
}
	
void sellStocks(){
char name[51];
int i,j, qty,found=0;
	printf("\nEnter the name of the stock you wish to sell:");
	scanf("%s",name);
FILE *f=fopen("common/stocks.dat","r+");
	fread(stocks,sizeof(struct stock),MAX_STOCKS,f);
	for(i=0;i<MAX_STOCKS;i++){
		if(strcmp(stocks[i].name,name)==0){
			printf("\nThe current stock value of %s is: %d\nYour capital is Rs. %ld\n",stocks[i].name,stocks[i].currVal,u.capital);
			found=1;
			break;
		}
	}
	if(!found){
		printf("\nThe stock for the company doesn't exist!\n");	
		return;
	}
	printf("Enter the quantity that you wish to sell\n");
	scanf("%d",&qty);
	
	long price=stocks[i].currVal*qty;
	for(j=0;j<20;j++){
		if(strcmp(u.stocks[j],name)==0){
		if(qty>u.num[j])
		{ 
	printf("\nYou cannot sell more stocks than you possess\n");
	break;
		}
		else {
			u.capital+=price;
			u.num[j]-=qty;
			if (u.num[j]==0)
			strcpy(u.stocks[j],"");
			printf("\nStocks sold successfully!!\n You have earned Rs. %ld \n Your current capital is Rs.%ld\n\n",price,u.capital);
			break;
		      }
		 }else
		 printf("\nYou have not bought any stocks of this company!\n");
		 }
		 printf("Press enter to continue...\n");
	getchar();
	getchar();
}		

void myPortfolio(){
	int i,j;
	printf("\n\tMY PORTFOLIO\n\n");
	printf("Name: %s\n",u.name);
	printf("Capital: %ld\n\n",u.capital);
	printf("------------------------------------------------------------\n");
	printf("Name | Quantity | Unit market price | Total price\n");
	printf("------------------------------------------------------------\n");
	for(j=0;j<20;j++){
	FILE *f=fopen("common/stocks.dat","r+");
	fread(stocks,sizeof(struct stock),MAX_STOCKS,f);
	fclose(f);
	for(i=0;i<MAX_STOCKS;i++){
		if(strcmp(stocks[i].name,u.stocks[j])==0)
			break;
	}
	if(strcmp(u.stocks[j],"")==0)
	break;
	else
		printf("%s       %d        %d             %d",u.stocks[j],u.num[j],stocks[i].currVal,(stocks[i].currVal*u.num[j]));
		printf("\n\n");
		}
		printf("Press enter to continue...\n");
	getchar();
	getchar();
}
