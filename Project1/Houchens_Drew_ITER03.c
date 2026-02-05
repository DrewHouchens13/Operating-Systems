/*
* Name: Drew Houchens 
* Class: CS2060 T/TH 1:30-2:55
* Windows OS
* Due April 3rd, 2024 
* Description: This program is for multiple rideshares looking to do business 
* The rates are based on number of minutes and miles traveled. 
* User selects a rideshare and enters a number of miles(1-100) that the program authenticates. 
* There is a base rate that will be charged if the calculated fee is not more. 
* The program displays the rider, miles, minutes, and fee until Sentinel value is entered.
* Once sentinel value entered, displays business summary and writes each companies' details to a file
*/
    

//included headers
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <float.h>
#include <ctype.h>


//used to generate	random number of minutes based on number of miles
#define MIN_RAND_MINUTES_FACTOR 1.2
#define MAX_RAND_MINUTES_FACTOR 1.5
#define MIN_FARE_VALUE 0
#define MAX_FARE_VALUE 50 
#define MIN_DBL_VALUE -100000
#define MAX_DBL_VALUE 100000

//constant for rows and columns in the survey array
#define SURVEY_RIDER_ROWS 5
#define SURVEY_CATEGORIES 3

//login and sentinel values
#define SENTINEL_VALUE -1 
#define CORRECT_ID "id1"
#define CORRECT_PASSCODE "abcd"
#define LOGIN_MAX_ATTEMPTS 3

//String constant to define size of strings
#define STRING_SIZE 80 

//constant for path 
#define PATH "C:\\RideShare\\"


//creating rideShare company structure 
typedef struct RideShare {

    double baseFare;
    double minFare;
    double costPerMin;
    double costPerMile;
    char name[STRING_SIZE];
    unsigned int totalRiders;
    unsigned int totalSurveyRiders;
    int userSurveys[SURVEY_RIDER_ROWS][SURVEY_CATEGORIES];
    double totalMiles;
    double totalMinutes;
    double totalFare;

}RideShare;

//structure for the nodes of our linked list 
typedef struct node
{
    RideShare data;   
    struct node* nextNodePtr;

}Node;

//function prototypes 
double getValidDoubleSentinel(double min, double max, double sentinel);
double getValidDouble(double min, double max);
int generateMinutes(double miles, double minMinFactor, double maxMinFactor);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
bool loginAdmin(const char* correctID, const char* correctPasscode, unsigned int maxAttempts);
void printFare(int count, double miles, double minutes, double fare);
void fgetsRemoveNewLine(char* buff);
void setupRideShare(Node** headPtr, int minimum, int maximum);
bool scanDouble(char* buffer, double* output);
void displayRideShareRatings(size_t surveyCount, const int survey[][SURVEY_CATEGORIES]);
void riderMode(Node* headPtr);
char getYOrN();
void calculateAndPrintAverages(const int surveyRatings[][SURVEY_CATEGORIES], unsigned int surveyCount, const char* surveyCategories[SURVEY_CATEGORIES]);
void printRideShares(Node* headPtr); 
void insertNode(Node** headPtr, RideShare newRideCompany); 
void freeList(Node* head);
RideShare* findRideShareByName(Node* headPtr);
void displayRideShareSummary(Node* headPtr); 
void writeRideShares(Node* headPtr);
void writeOneRideShare(RideShare* rideShare);




//declaring global constant for survey categories
const char* surveyCategories[SURVEY_CATEGORIES] = { "Safety", "Cleanliness", "Comfort" };


//beginning main function 
int main(void) {

    //define our max amount of attempts
    int maxAttempts = 4; 

    bool adminLogin = loginAdmin(CORRECT_ID, CORRECT_PASSCODE, maxAttempts); 

    //if admin logged in properly, we will continue on with the program
    if (adminLogin == true) {

        Node* headNodePtr = NULL;
        setupRideShare(&headNodePtr, MIN_FARE_VALUE, MAX_FARE_VALUE);           
        riderMode(headNodePtr); 
        displayRideShareSummary(headNodePtr);
        writeRideShares(headNodePtr); 
        freeList(headNodePtr);
        
    }
   

    

    return 0; 
}



/*
* Returns only valid input from the user based on the min and max values + scanDouble boolean 
* Parameters: int min, int max, int sentinel
* Returns: A double representing the number of miles the client will travel
*/
double getValidDoubleSentinel(double min, double max, double sentinel) {

    bool goodInput = false;
    double miles = 0;

    do {

        printf("\nEnter a valid number between %.2lf and %.2lf:", min, max);

        //Prompt for the user to enter a double
        char userInput[STRING_SIZE];
        fgets(userInput, STRING_SIZE, stdin);

        //Set pointer to our miles value
        double *milesPTR = &miles;

        //scan for a double
        bool isDouble = scanDouble(userInput, milesPTR);

        if (isDouble == true) {

            if ((miles >= min && miles <= max) || miles == sentinel) {

                goodInput = true;

            }

            else {

                printf("\nError: Not within the range %.2lf and %.2lf", min, max);
            }
        }

        else {

            puts("\nError: You did not enter a number.");
        }

    }
    while (goodInput != true);
   
    return miles;

}

/*
* Returns only valid input from the user based on the min and max values 
* Parameters: int min, int max
* Returns: A double representing the number of miles the client will travel
*/
double getValidDouble(double min, double max) {

    bool goodInput = false;
    double miles = 0;

    do {

        printf("\nEnter a valid number between %.2lf and %.2lf:", min, max);

        //Prompt for the user to enter a double
        char userInput[STRING_SIZE]; 
        fgets(userInput, STRING_SIZE, stdin); 

        //Set pointer to our miles value
        double *milesPTR = &miles; 

        //scan for a double
        bool isDouble = scanDouble(userInput, milesPTR); 
        
        if (isDouble == true) {

            if (miles >= min && miles <= max) {

                goodInput = true;

            }

            else {

                printf("\nError: Not within the range %.2lf and %.2lf", min, max);
            }
        }

        else {

            puts("\nError: You did not enter a number.");
        }

    } while (goodInput != true);

    return miles;

}




/*
* Finds the RideShare selected by the user or tells them to reenter the name 
* Parameters: Node* headPtr
* Returns: Nothing
*/
RideShare* findRideShareByName(Node* headPtr) {

    Node* currentPtr = headPtr;
    bool found = false;

    while (found == false) {

        char searchName[STRING_SIZE];
        puts("\nPlease enter the name of a RideShare you wish to ride with: ");
        fgets(searchName, STRING_SIZE, stdin);
        fgetsRemoveNewLine(searchName);

        while (currentPtr != NULL && found == false) {

            char* currentName = currentPtr->data.name; 

            if (strcmp(_strupr(currentName), _strupr(searchName)) == 0) {

                found = true;

            }

            else {

                currentPtr = currentPtr->nextNodePtr; // Move to the next node
            }


        }

        if (found == false) {

            puts("\nName was not found please try again");
            currentPtr = headPtr;
        }

    }

    return &(currentPtr->data);
}


/*
* Frees the memory allocated for our created linked list
* Parameters: Node* head
* Returns: Nothing
*/
void freeList(Node* headPtr) {

    Node* current = headPtr;
    Node* next;

    
    while (current != NULL) {
       
        next = current->nextNodePtr;

        // Free the memory allocated for the current node
        free(current);

        // Move to the next node
        current = next;
    }
}



/*
* Initializes all value of the declared rideShare structure 
* Parameters: Node** headPtr, int minimum, int maximum)
* Returns: Nothing 
*/
void setupRideShare(Node** headPtr, int minimum, int maximum) {

    bool continueAdding = true; 

    while (continueAdding == true) {

        RideShare currentRS; 
        RideShare *currentCompany = &currentRS; 

        //collecting values for all fares 
        puts("\nPlease enter a value for your base fare.");
        currentCompany->baseFare = getValidDouble(minimum, maximum);
        puts("\nPlease enter a value for your minimum fare.");
        currentCompany->minFare = getValidDouble(minimum, maximum);
        puts("\nPlease enter a value for your cost per minute");
        currentCompany->costPerMin = getValidDouble(minimum, maximum);
        puts("\nPlease enter a value for your cost per mile");
        currentCompany->costPerMile = getValidDouble(minimum, maximum);

        //collect string for their name 
        puts("\nPlease enter a name for your RideShare company");
        fgets(currentCompany->name, STRING_SIZE, stdin);
        fgetsRemoveNewLine(currentCompany->name);

        //initialize all other values to zero
        currentCompany->totalFare = 0;
        currentCompany->totalMiles = 0;
        currentCompany->totalMinutes = 0;
        currentCompany->totalRiders = 0;
        currentCompany->totalSurveyRiders = 0;

        //initialize survey array values to zero 
        for (size_t i = 0; i < SURVEY_RIDER_ROWS; i++) {

            for (size_t j = 0; j < SURVEY_CATEGORIES; j++) {

                currentCompany->userSurveys[i][j] = 0;

            }
        }

        //now we will display inputted values 
        printf("\n%s", currentCompany->name);
        printf("\nBase Fare:%lf", currentCompany->baseFare);
        printf("\nMinimum Fare:%lf", currentCompany->minFare);
        printf("\nCost Per Minute:%lf", currentCompany->costPerMin);
        printf("\nCost Per Mile:%lf", currentCompany->costPerMile);

        //add the created company to our linked list
        insertNode(headPtr, currentRS); 

        //now ask if they wish to continue adding rideshares
        puts("\n\nWould you like to enter RideShare information for another compnay?");
        if (getYOrN() == 'N') {

            continueAdding = false; 

        }


    }
    puts("\nExiting Admin Mode\n");

}


/*
* Verifies that the user login by matching the correct id and password 
* Parameters: const char* correctID, const char* correctPasscode, unsigned int maxAttempts
* Returns: A boolean telling if the login was successful or not 
*/
bool loginAdmin(const char* correctID, const char* correctPasscode, unsigned int maxAttempts) {

    unsigned int currentAttempts = 1; 
    bool successfulLogin = false; 

    while (currentAttempts <= maxAttempts && !successfulLogin) {

        //declaring char arrays for inputted user id and password
        char inputID[STRING_SIZE];
        char inputPassword[STRING_SIZE];

        //getting user id and password from the user
        puts("Please enter your ID");
        fgets(inputID, STRING_SIZE, stdin);
        puts("Please enter your password");
        fgets(inputPassword, STRING_SIZE, stdin);

        //removing newline character from both inputs
        fgetsRemoveNewLine(inputID);
        fgetsRemoveNewLine(inputPassword);

        //check if the inputted values match the correct id and password
        if (strcmp(inputID, correctID) == 0) {

            if (strcmp(inputPassword, correctPasscode) == 0) {

                puts("Login Successful!");
                successfulLogin = true;

            }

            else {

                puts("ID or Passcode was incorrect.");
                currentAttempts++;

            }
        }

        else {

            puts("ID or Passcode was incorrect.");
            currentAttempts++;

        }


    }

    if (!successfulLogin) {

        puts("Incorrect Attempts Reached. Exiting RideShare Login");

    }

    return successfulLogin;


}

/*
* Removes the newline character from an incoming string 
* Parameters: char* buff
* Returns: Nothing
*/
void fgetsRemoveNewLine(char* buff) {

    if (buff[strlen(buff) - 1] == '\n') {
        buff[strlen(buff) - 1] = '\0';
    }

}

/*
* Calculates a random number between the minimum and maximum amount of minutes
* Parameters: double miles, double minFactor, double maxFactor
* Returns: A random number of minutes that the user will travel based on number of miles
*/
int generateMinutes(double miles, double minFactor, double maxFactor) {

     int minMinutes = (int)(miles * minFactor); 
     int maxMinutes = (int)(miles * maxFactor); 
    
    //generating random number between minMinutes and maxMinutes to use as our minutes
    int minutes = (rand() % (maxMinutes - minMinutes + 1)) + minMinutes;

    return minutes;
}

/*
* Scans if user input is a double. Returns true if so 
* Parameters:const char *buffer, double *output
* Returns: A boolean indicating if the user entered a validDouble
*/
bool scanDouble(char *buffer, double *output) {

    char* end; 
    errno = 0; 
    double testNum = 0; 
    bool validDouble = false; 
    
    fgetsRemoveNewLine(buffer);

    testNum = strtod(buffer, &end); 


    if (buffer == end) {

        puts("Invalid input");

    }

    else if (*end != '\0') {

        puts("Invalid input");

    }

    else if (testNum > MAX_DBL_VALUE || testNum < MIN_DBL_VALUE) {

        puts("Input out of range for type double"); 

    }

    else {

        *output = testNum;
        validDouble = true;

    }
 
    return validDouble; 

}

/*
* Calculates fare of the ride based on base fare, miles and minutes. If calculated fare less than minimum fare, gives minimum fare.
* Parameters: dodouble base, double minuteCost, double mileCost, double minRate, double miles, int minutes
* Returns: Calculated Fare or Minimum Fare
*/
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes) {

    double fare = base + (minuteCost * minutes) + (mileCost * miles);

    //if the fare is less than the minimim flat rate, we will charge the flat rate
    if (fare >= minRate) {

        return fare; 

    }

    else {

        return minRate;

    }


}


/*
* Displays the current ride's summary or the business summary from the day if sentinel value is entered.
* Parameters:int count, double miles, int minutes, double fare
* Returns: Nothing
*/
void printFare(int count, double miles, double minutes, double fare) {

    if (miles > 0) {
        
        puts("Rider     Number of Miles         Number of Minutes       Ride Fare");
        //formatted to fit under each header
        printf("%3d%16.2lf%24lf%22.2lf\n", count, miles, minutes, fare);
        puts("\n");

    }

    else {

        puts("\n0 rides were completed\n");
        puts("\n");

    }
}


/*
* Displays all information of entered rideshares
* Parameters:Node* headPtr
* Returns: Nothing
*/
void printRideShares(Node* headPtr) {

    if (headPtr != NULL)
    {
        
        //
        Node* currentPtr = headPtr;

        //
        while (currentPtr != NULL)
        {
            // Print information of the current ride-share organization
            printf("\nCompany Name: %s\n", currentPtr->data.name);
            printf("Base Fare: %lf\n", currentPtr->data.baseFare);
            printf("Cost Per Minute: %lf\n", currentPtr->data.costPerMin);
            printf("Cost Per Mile: %lf\n", currentPtr->data.costPerMile);
            printf("Minimum Flat Rate: %lf\n", currentPtr->data.minFare);
            puts("\n"); 

            currentPtr = currentPtr->nextNodePtr;
        }
    }
 
    else
    {
        puts("List is empty");
    }
}



/*
* Inserts a new node into our linkedlist of rideshares 
* Parameters:Node** headPtr, RideShare newRideCompany
* Returns: Nothing
*/
void insertNode(Node** headPtr, RideShare newRideCompany) {

        Node* newNodePtr = malloc(sizeof(Node));

        // 
        if (newNodePtr != NULL)
        {
            //
            newNodePtr->data = newRideCompany;
            //
            newNodePtr->nextNodePtr = NULL;

            // 
            Node* previousPtr = NULL;
            // 
            Node* currentPtr = *headPtr;

            //
            while (currentPtr != NULL && strcmp(currentPtr->data.name, newRideCompany.name) < 0)
            {
                //
                previousPtr = currentPtr;
                //
                currentPtr = currentPtr->nextNodePtr;
            }

            // 
            if (previousPtr == NULL)
            {
                //
                *headPtr = newNodePtr;
            }
            //
            else
            {
                //
                previousPtr->nextNodePtr = newNodePtr;
            }

            //
            newNodePtr->nextNodePtr = currentPtr;
        }
        //
        else
        {
            puts("No memory to create node");
        }
    

}


/*
* Displays either the current surveys entered or nothing if no surveys have been entered
* Parameters: size_t surveyCount, const int survey[][SURVEY_CATEGORIES
* Returns: Nothing
*/
void displayRideShareRatings(size_t surveyCount, const int survey[][SURVEY_CATEGORIES]) {

    if (surveyCount == 0) {

        puts("\nNo surveys yet entered");
    }

    else {

        const char* riderStr = "Rider"; 
        printf("%s  ", riderStr);
        for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

            printf("%s ", surveyCategories[i]);        

        }

        puts("\n"); 

        for (size_t i = 0; i < surveyCount; i++) {
           
            printf("%zu\t", i + 1);

            for (size_t j = 0; j < SURVEY_CATEGORIES; j++) {

                printf("%d         ", survey[i][j]);

            }

            puts("\n");

        }

    }
}

/*
* Allows the user to enter their survey ratings if they choose to do so
* Parameters: int survey[][SURVEY_CATEGORIES], int* surveyCount
* Returns: Nothing
*/
void getRatings(int survey[][SURVEY_CATEGORIES], unsigned int surveyCount) {

    puts("Enter a value for Safety, Cleanliness, Comfort in that order.");
    for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

        survey[surveyCount][i] = (int)getValidDouble(1,5); 
    }    

}

/*
* Conducts the rider mode of the ride share. ends once the admin logs back in
* Parameters: RideShare* companyName
* Returns: Nothing
*/
void riderMode(Node* headPtr) {

    //initialize variables 
    int minMiles = 1;
    int maxMiles = 100;
    int maxAttempts = 4;
    bool loginStatus = false;

    do {

        //we are displaying our different ride shares and having our user select one
        printRideShares(headPtr);
        RideShare* selectedShare = findRideShareByName(headPtr); 
        displayRideShareRatings(selectedShare->totalSurveyRiders, selectedShare->userSurveys);

        //new rider 
        selectedShare->totalRiders++;

       //calculate miles
       printf("Welcome to %s, we only accept rides 1-100 miles", selectedShare->name); 
       double miles = getValidDoubleSentinel(minMiles, maxMiles, SENTINEL_VALUE);

        if (miles == SENTINEL_VALUE) {

            loginStatus = loginAdmin(CORRECT_ID, CORRECT_PASSCODE, maxAttempts);

        }

        else {

             int minutes = generateMinutes(miles, MIN_RAND_MINUTES_FACTOR, MAX_RAND_MINUTES_FACTOR);
             double fare = calculateFare(selectedShare->baseFare, selectedShare->costPerMin, selectedShare->costPerMile, selectedShare->minFare, miles, minutes);

             //Printing user's fare
             puts("\nCurrent Ride Information\n");
             printFare(selectedShare->totalRiders, miles, minutes, fare);
             puts("Thanks for riding with us!\n");

             //increment totals
             selectedShare->totalFare += fare;
             selectedShare->totalMiles += miles;
             selectedShare->totalMinutes += minutes;

            //ask if user wants to enter survey 
            puts("\nWould you like to complete a survey for the ride?");
            if (getYOrN() == 'Y') {
                  
                  getRatings(selectedShare->userSurveys, selectedShare->totalSurveyRiders); 
                  puts("\nThank you for filling out the survey\n"); 
                  selectedShare->totalSurveyRiders++;

             }
            
        }


    }

    while (loginStatus == false);

}

/*
* Retrieves a Y or N char from the user 
* Parameters: None
* Returns: Char Y or N
*/
char getYOrN() {

    puts("\nPlease enter 'Y' or 'N': ");

    char yOrN = 0;
    bool validInput = false; 

    do {

        yOrN = getchar(); 

        while (getchar() != '\n');
            
        if (yOrN == 'Y' || yOrN == 'y') {

            yOrN = 'Y';
            validInput = true;

        }

        else if (yOrN == 'N' || yOrN == 'n') {

            yOrN = 'N';
            validInput = true;

        }

        else {

            puts("\nInvalid input! Please enter 'Y' or 'N': ");

        }

    } while (!validInput);

    return yOrN;
}

/*
* Calculates and displays the column averages of the ratings aquired
* Parameters: calculateAndPrintAverages(const int surveyRatings[][SURVEY_CATEGORIES], unsigned int surveyCount, const char* surveyCategories[SURVEY_CATEGORIES]
* Returns: Nothing
*/
void calculateAndPrintAverages(const int surveyRatings[][SURVEY_CATEGORIES], unsigned int surveyCount, const char* surveyCategories[SURVEY_CATEGORIES]) {

    double categoryAverages[SURVEY_CATEGORIES];

    for (size_t j = 0; j < SURVEY_CATEGORIES; j++) {

        double sum = 0.0;
        double nonZeroNumbers = 0.0;

        for (size_t i = 0; i < SURVEY_RIDER_ROWS; i++) {

            sum += surveyRatings[i][j];

            if (surveyRatings[i][j] > 0) {

                nonZeroNumbers++;

            }

        }

        categoryAverages[j] = sum / nonZeroNumbers;

    }

    puts("\nCategory Rating Averages");
    for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

        printf("%s    ", surveyCategories[i]);

    }

    puts("\n"); 

    for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

        printf("%.4lf      ", categoryAverages[i]);

    }

    puts("\n\n");


}

/*
* Displays all information on the console for each rideshare company at the end of the program
* Parameters:Node* headPtr 
* Returns: Nothing
*/
void displayRideShareSummary(Node* headPtr) {

    Node* currentPtr = headPtr; 

    while (currentPtr != NULL) {

        RideShare currentCompany = currentPtr->data; 

        printf("Summary for %s\n\n", currentCompany.name);
        printFare(currentCompany.totalRiders, currentCompany.totalMiles, currentCompany.totalMinutes, currentCompany.totalFare);
        calculateAndPrintAverages(currentCompany.userSurveys, currentCompany.totalSurveyRiders, surveyCategories); 
        puts("-----------------------------------------------------------\n"); 

        currentPtr = currentPtr->nextNodePtr; 
    }
}

void writeOneRideShare(RideShare* rideShare) {

    char filename[STRING_SIZE];
    sprintf(filename,"%s%s.txt", PATH, rideShare->name);
    FILE* fPtr = fopen(filename, "w"); 

    if (fPtr == NULL) {

        puts("\nThere was an error opening the file"); 
    }

    else {

        if (rideShare->totalMiles > 0) {

            fputs("Rider     Number of Miles         Number of Minutes       Ride Fare\n", fPtr);           
            fprintf(fPtr,"%3d%16.2lf%24lf%22.2lf\n", rideShare->totalRiders, rideShare->totalMiles, rideShare->totalMinutes, rideShare->totalFare);

            double categoryAverages[SURVEY_CATEGORIES];

            for (size_t j = 0; j < SURVEY_CATEGORIES; j++) {

                double sum = 0.0;
                double nonZeroNumbers = 0.0;

                for (size_t i = 0; i < SURVEY_RIDER_ROWS; i++) {

                    sum += rideShare->userSurveys[i][j];

                    if (rideShare->userSurveys[i][j] > 0) {

                        nonZeroNumbers++;

                    }

                }

                categoryAverages[j] = sum / nonZeroNumbers;

            }

            fputs("\nCategory Rating Averages\n", fPtr);
            for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

                fprintf(fPtr, "%s    ", surveyCategories[i]);

            }

            fputs("\n",fPtr);

            for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

                fprintf(fPtr, "%.4lf      ", categoryAverages[i]);

            }

            fputs("\n\n", fPtr);


        }


        else {

            fputs("\n0 rides were completed\n", fPtr);
            fputs("\n", fPtr);

            double categoryAverages[SURVEY_CATEGORIES];

            for (size_t j = 0; j < SURVEY_CATEGORIES; j++) {

                double sum = 0.0;
                double nonZeroNumbers = 0.0;

                for (size_t i = 0; i < SURVEY_RIDER_ROWS; i++) {

                    sum += rideShare->userSurveys[i][j];

                    if (rideShare->userSurveys[i][j] > 0) {

                        nonZeroNumbers++;

                    }

                }

                categoryAverages[j] = sum / nonZeroNumbers;

            }

            fputs("\nCategory Rating Averages\n", fPtr);
            for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

                fprintf(fPtr, "%s    ", surveyCategories[i]);

            }

            fputs("\n", fPtr);

            for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

                fprintf(fPtr, "%.4lf      ", categoryAverages[i]);

            }

            fputs("\n\n", fPtr);


        }
       
        
    }

    fclose(fPtr); 

}

// Function to write all RideShares in the linked list to individual files
void writeRideShares(Node* headPtr) {


    Node* currentPtr = headPtr;

    while (currentPtr != NULL) {

        writeOneRideShare(&(currentPtr->data)); 
        currentPtr = currentPtr->nextNodePtr; 

    }
}



