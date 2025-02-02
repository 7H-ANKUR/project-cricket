#include <stdio.h>
#include <string.h>

// Define a structure for a batsman
typedef struct {
    int runs;            // Runs scored by the batsman
    int isOut;           // Flag to indicate if the batsman is out (1) or not (0)
    char name[20];       // Name of the batsman
    char dismissal[20];  // To store how the batsman got out (e.g., "LBW", "Catch Out")
    int ballsPlayed;     // Number of balls played by the batsman
} Batsman;

// Define a structure for a bowler
typedef struct {
    char name[20];       // Name of the bowler
    int oversBowled;     // Number of overs bowled by the bowler
} Bowler;

int main() {
    // Initialize an array of batsmen with initial values
    Batsman batsmen[11] = {
        {0, 0, "", "", 0}, {0, 0, "", "", 0}, {0, 0, "", "", 0},
        {0, 0, "", "", 0}, {0, 0, "", "", 0}, {0, 0, "", "", 0},
        {0, 0, "", "", 0}, {0, 0, "", "", 0}, {0, 0, "", "", 0},
        {0, 0, "", "", 0}, {0, 0, "", "", 0}
    };

    // Initialize a bowler with default values
    Bowler currentBowler = {"", 0};
    
    // Variables to track the game state
    int striker = 0;     // Index of the current striker (batsman facing the ball)
    int nonStriker = 1;  // Index of the current non-striker (batsman at the other end)
    int totalRuns = 0;   // Total runs scored by the team
    int extras = 0;      // Extra runs (wide balls, no balls, leg byes, etc.)
    int wickets = 0;     // Total wickets fallen
    int overs = 0;       // Total overs bowled
    int balls = 0;       // Total balls bowled in the current over
    int nextBatsman = 2; // Index of the next batsman to come (after a wicket falls)
    int maxOvers;        // Maximum number of overs in the innings
    int isFreeHit = 0;   // Flag to indicate if the next ball is a free hit (1) or not (0)

    // Prompt the user to enter the maximum number of overs
    printf("Enter the maximum number of overs: ");
    scanf("%d", &maxOvers);

    // Prompt the user to enter the names of the first two batsmen
    printf("Enter the name of the first batsman: ");
    scanf("%s", batsmen[0].name);
    printf("Enter the name of the second batsman: ");
    scanf("%s", batsmen[1].name);

    // Prompt the user to enter the name of the bowler
    printf("Enter the name of the bowler: ");
    scanf("%s", currentBowler.name);                                    

    // Display the header for the scoreboard
    printf("\n\n");
    printf("___________________________________________________Cricket Score Board_________________________________________________________\n");
    printf("                                                   -------------------\n");

    // Main game loop
    while(1) {
        // Display current score, overs, and bowler
        printf("\nCurrent Score: %d/%d (Extras: %d)", totalRuns, wickets, extras);
        printf("\tOvers: %d.%d", overs, balls);
        printf("\tBowler: %s (%d overs)\n", currentBowler.name, currentBowler.oversBowled);
        
        // Display striker status with marker
        printf("\nStriker:    %s - %d (%d balls) %s %s", 
               batsmen[striker].name, 
               batsmen[striker].runs,
               batsmen[striker].ballsPlayed,
               batsmen[striker].isOut ? "(out)" : "",
               "[STRIKER]");
        
        // Display non-striker status
        printf("\nNon-Striker: %s - %d (%d balls) %s\n", 
               batsmen[nonStriker].name, 
               batsmen[nonStriker].runs,
               batsmen[nonStriker].ballsPlayed,
               batsmen[nonStriker].isOut ? "(out)" : "");

        // Display options for the user
        printf("\n1. Add runs\n2. Add wicket\n3. Add ball\n4. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        // Exit the loop if user chooses to exit
        if (choice == 4) break;

        // Handle user's choice
        switch(choice) {
            case 1: {
                // Add runs to the striker's score and total runs
                printf("Choose the type of runs:\n");
                printf("1. Normal runs\n2. Leg byes\n");
                printf("Enter your choice: ");
                int runType;
                scanf("%d", &runType);

                int runs;
                printf("Enter runs scored (0-6): ");
                scanf("%d", &runs);
                
                // Validate runs input
                if (runs < 0 || runs > 6) {
                    printf("Invalid runs! Must be 0-6\n");
                    break;
                }

                if (runType == 1) {
                    // Normal runs: Add to batsman's score and team total
                    totalRuns += runs;
                    batsmen[striker].runs += runs;
                    batsmen[striker].ballsPlayed++;
                    
                    // Swap positions for odd runs
                    if (runs % 2 != 0) {
                        int temp = striker;
                        striker = nonStriker;
                        nonStriker = temp;
                    }
                } else if (runType == 2) {
                    // Leg byes: Add to team total and extras, but not to batsman's score
                    totalRuns += runs;
                    extras += runs;
                    balls++;
                } else {
                    printf("Invalid choice! No runs added.\n");
                    break;
                }
                
                balls++;
                isFreeHit = 0;
                break;
            }
            case 2: {
                // Add a wicket and update the striker
                if (wickets >= 10) {
                    printf("All out! No more wickets.\n");
                    break;
                }
                
                // Check if it's a free hit (only run-out is allowed)
                if (isFreeHit) {
                    printf("It's a free hit! Only run-out is allowed.\n");
                    printf("Was it a run-out? (1 for Yes, 0 for No): ");
                    int isRunOut;
                    scanf("%d", &isRunOut);
                    
                    if (!isRunOut) {
                        printf("No wicket can be taken on a free hit except run-out.\n");
                        break;
                    }

                    int runsCompleted;
                    printf("Did the batsmen complete any runs by running? (Enter runs, 0 if none): ");
                    scanf("%d", &runsCompleted);

                    if (runsCompleted > 0) {
                        totalRuns += runsCompleted;
                        batsmen[striker].runs += runsCompleted;
                    }

                    printf("Who was run out?\n");
                    printf("1. Striker (%s)\n2. Non-Striker (%s)\n", batsmen[striker].name, batsmen[nonStriker].name);
                    printf("Enter your choice: ");
                    int runOutChoice;
                    scanf("%d", &runOutChoice);

                    if (runOutChoice == 1) {
                        strcpy(batsmen[striker].dismissal, "Run Out");
                        batsmen[striker].isOut = 1;
                        wickets++;
                        if (nextBatsman < 11) {
                            printf("Enter the name of the new batsman: ");
                            scanf("%s", batsmen[nextBatsman].name);
                            striker = nextBatsman;
                            nextBatsman++;
                        }
                    } else if (runOutChoice == 2) {
                        strcpy(batsmen[nonStriker].dismissal, "Run Out");
                        batsmen[nonStriker].isOut = 1;
                        wickets++;
                        if (nextBatsman < 11) {
                            printf("Enter the name of the new batsman: ");
                            scanf("%s", batsmen[nextBatsman].name);
                            nonStriker = nextBatsman;
                            nextBatsman++;
                        }
                    } else {
                        printf("Invalid choice! No wicket added.\n");
                        break;
                    }

                    balls++;
                    isFreeHit = 0;
                    break;
                }
                
                printf("How did the batsman get out?\n");
                printf("1. LBW\n2. Catch Out\n3. Hit Wicket\n4. Stumped\n5. Run Out\n");
                printf("Enter your choice: ");
                int dismissalChoice;
                scanf("%d", &dismissalChoice);
                
                switch(dismissalChoice) {
                    case 1:
                        strcpy(batsmen[striker].dismissal, "LBW");
                        break;
                    case 2:
                        strcpy(batsmen[striker].dismissal, "Catch Out");
                        break;
                    case 3:
                        strcpy(batsmen[striker].dismissal, "Hit Wicket");
                        break;
                    case 4:
                        strcpy(batsmen[striker].dismissal, "Stumped");
                        break;
                    case 5: {
                        printf("Who was run out?\n");
                        printf("1. Striker (%s)\n2. Non-Striker (%s)\n", batsmen[striker].name, batsmen[nonStriker].name);
                        printf("Enter your choice: ");
                        int runOutChoice;
                        scanf("%d", &runOutChoice);

                        if (runOutChoice == 1) {
                            strcpy(batsmen[striker].dismissal, "Run Out");
                            batsmen[striker].isOut = 1;
                            wickets++;
                            if (nextBatsman < 11) {
                                printf("Enter the name of the new batsman: ");
                                scanf("%s", batsmen[nextBatsman].name);
                                striker = nextBatsman;
                                nextBatsman++;
                            }
                        } else if (runOutChoice == 2) {
                            strcpy(batsmen[nonStriker].dismissal, "Run Out");
                            batsmen[nonStriker].isOut = 1;
                            wickets++;
                            if (nextBatsman < 11) {
                                printf("Enter the name of the new batsman: ");
                                scanf("%s", batsmen[nextBatsman].name);
                                nonStriker = nextBatsman;
                                nextBatsman++;
                            }
                        } else {
                            printf("Invalid choice! No wicket added.\n");
                        }
                        break;
                    }
                    default:
                        printf("Invalid choice! Setting dismissal as 'Unknown'\n");
                        strcpy(batsmen[striker].dismissal, "Unknown");
                        break;
                }
                
                if (dismissalChoice != 5) {
                    batsmen[striker].isOut = 1;
                    wickets++;
                    
                    if (nextBatsman < 11) {
                        printf("Enter the name of the new batsman: ");
                        scanf("%s", batsmen[nextBatsman].name);
                        striker = nextBatsman;
                        nextBatsman++;
                    }
                }
                
                balls++;
                isFreeHit = 0;
                break;
            }
            case 3: {
                // Add a ball to the current over
                printf("What type of ball is it?\n");
                printf("1. Dot Ball\n2. Wide Ball\n3. No Ball\n");
                printf("Enter your choice: ");
                int ballType;
                scanf("%d", &ballType);
                
                switch(ballType) {
                    case 1:
                        balls++;
                        batsmen[striker].ballsPlayed++;
                        isFreeHit = 0;
                        break;
                    case 2:
                        totalRuns += 1;
                        extras += 1;
                        break;
                    case 3:
                        totalRuns += 1;
                        extras += 1;
                        isFreeHit = 1;
                        break;
                    default:
                        printf("Invalid choice! Ball not counted.\n");
                        break;
                }
                break;
            }
            default:
                printf("Invalid choice!\n");
                break;
        }

        // Update overs and rotate strike if needed
        if (balls >= 6) {
            overs++;
            balls = 0;
            currentBowler.oversBowled++;

            int temp = striker;
            striker = nonStriker;
            nonStriker = temp;

            printf("Enter the name of the new bowler: ");
            scanf("%s", currentBowler.name);

            isFreeHit = 0;
        }

        // Check if innings is over
        if (wickets >= 10 || overs >= maxOvers) {
            printf("\nInnings Over!\n");
            break;
        }
    }

    // Display final scorecard
    printf("\nFinal Scorecard:\n");
    printf("Total: %d/%d (Extras: %d) in %d.%d overs\n", totalRuns, wickets, extras, overs, balls);
    printf("\nBatsmen:\n");
    for (int i = 0; i < 11; i++) {
        printf("%s - %d (%d balls) %s %s%s%s\n", 
               batsmen[i].name,
               batsmen[i].runs,
               batsmen[i].ballsPlayed,
               batsmen[i].isOut ? "(out)" : "(not out)",
               batsmen[i].isOut ? batsmen[i].dismissal : "",
               (i == striker) ? "[STRIKER] " : "",
               (i == nonStriker) ? "[NON-STRIKER]" : "");
    }

    // Display bowler's performance
    printf("\nBowler: %s bowled %d overs\n", currentBowler.name, currentBowler.oversBowled);

    return 0;
}
