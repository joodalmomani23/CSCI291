#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NUM_TEAMS 6        // Maximum number of teams allowed
#define SQUAD_SIZE 20      // Maximum players per team

// Structure to store player information.
typedef struct {
    char name[26];          // Player's full name
    int kit_number;         // Player's kit number (1-99)
    char club[30];          // Club name
    int day, month, year;   // Birthdate fields
    char position[20];      // Player's position
} player_t;

// Structure to store team information including the list of players.
typedef struct {
    char name[21];          // Team name
    player_t players[SQUAD_SIZE]; // Array of players
    int active_size;        // Current number of players in the team
} team_t;

// Array to hold teams and a variable to track the current number of teams enrolled.
team_t teams[NUM_TEAMS];
int team_count = 0;  // Keeps track of the number of teams currently enrolled.

const char predefined_team_names[][21] = {
    "team1", "team2", "team3", "team4", "team5", "team6"  // Names of the predefined teams
};

// Function prototypes for the main functions used in the program.
void display_menu(), enroll_club(), add_player(), search_update(), display_club_statistics();
void handle_error(const char message[], int code);
void clear_input_buffer(), input_string(char prompt[], char result[], int alpha_only);
int input_int(char prompt[], int min, int max);

int main() {
    int choice;
    // Infinite loop to keep the menu running until the user chooses to exit.
    while (1) {
        display_menu();  // Display the main menu options.
        choice = input_int("Enter your choice: ", 0, 4);  // Get the user's menu choice.
        // Process the user's choice using a switch-case structure.
        switch (choice) {
            case 1: enroll_club(); break;  // Enroll a new club.
            case 2: add_player(); break;  // Add a new player to a club.
            case 3: search_update(); break;  // Search for a player and update details.
            case 4: display_club_statistics(); break;  // Display the statistics of the clubs.
            case 0: printf("Exiting program.\n"); return 0;  // Exit the program.
            default: handle_error("Invalid choice", 0);  // Handle invalid input.
        }
    }
}

// Displays the menu with options for the user to choose from.
void display_menu() {
    printf("\nMenu:\n1. Enroll Club\n2. Add Player\n3. Search and Update Player\n4. Display Club Statistics\n0. Exit\n");
}

// Clears the input buffer to ensure no leftover characters from previous inputs.
void clear_input_buffer() {
    while (getchar() != '\n');
}

// Handles errors by printing appropriate messages based on the error code.
void handle_error(const char message[], int code) {
    printf("%s\n", message);
    if (code == 1) printf("Please enter alphabetic characters.\n");
    else if (code == 2) printf("Enter a valid club name\n");
    else if (code == 3) printf("Enter a valid date format: day month year.\n");
}

// Prompts the user for an integer input and ensures it is within a valid range.
int input_int(char prompt[], int min, int max) {
    int num;
    printf("%s", prompt);
    while (scanf("%d", &num) != 1 || num < min || num > max) {
        handle_error("Invalid input!", 0);  // Show error if input is invalid.
        clear_input_buffer();  // Clear input buffer to prevent incorrect data.
        printf("%s", prompt);  // Prompt the user again.
    }
    clear_input_buffer();  // Clear buffer once input is valid.
    return num;
}

// Prompts the user for a string input and validates if it contains alphabetic characters, if required.
void input_string(char prompt[], char result[], int alpha_only) {
    printf("%s", prompt);
    while (scanf(" %[^\n]", result) != 1 || (alpha_only && !isalpha(result[0]))) {
        handle_error("Invalid input!", 1);  // Show error if input is invalid.
        clear_input_buffer();  // Clear input buffer.
        printf("%s", prompt);  // Prompt the user again.
    }
}

// Enrolls a new club (team) by assigning a predefined team name.
void enroll_club() {
    if (team_count >= NUM_TEAMS) {
        handle_error("Cannot enroll more teams. Maximum number of teams reached.", 0);
        return;
    }
    // Assign the predefined team name and initialize the number of players as zero.
    strncpy(teams[team_count].name, predefined_team_names[team_count], sizeof(teams[team_count].name) - 1);
    teams[team_count].active_size = 0;
    printf("Club '%s' enrolled successfully.\n", teams[team_count].name);
    team_count++;  // Increment the team count after successfully enrolling a team.
}

// Adds a player to a selected team.
void add_player() {
    int team_index;
    printf("Select a team (0 to %d): ", team_count - 1);
    while (scanf("%d", &team_index) != 1 || team_index < 0 || team_index >= team_count) {
        handle_error("Invalid input! Please enter a valid integer within range.", 0);
        clear_input_buffer();
        printf("Select a team (0 to %d): ", team_count - 1);
    }

    player_t new_player;  // Declare a new player structure to store the player's data.
    input_string("Enter player's full name: ", new_player.name, 1);  // Prompt for player's full name.
    new_player.kit_number = input_int("Enter kit number (1-99): ", 1, 99);  // Prompt for kit number.
    input_string("Enter club name: ", new_player.club, 0);  // Prompt for club name.

    // Validate that the entered club name matches one of the predefined team names.
    int valid_club = 0;
    for (int i = 0; i < NUM_TEAMS; i++) {
        if (strcmp(new_player.club, predefined_team_names[i]) == 0) valid_club = 1;
    }
    if (!valid_club) {
        handle_error("Invalid club name!", 2);
        return;
    }

    // Inline birthdate input, ensuring that the date entered is valid.
    printf("Enter birthdate (day month year): ");
    while (scanf("%d %d %d", &new_player.day, &new_player.month, &new_player.year) != 3 ||
           new_player.year < 1900 || new_player.year > 2023 ||
           new_player.month < 1 || new_player.month > 12 ||
           new_player.day < 1 || 
           (new_player.day > 30 && (new_player.month == 4 || new_player.month == 6 || new_player.month == 9 || new_player.month == 11)) ||
           (new_player.month == 2 && new_player.day > ((2024 - new_player.year) % 4 == 0 ? 29 : 28))) {
        handle_error("Invalid birthdate!", 3);
        clear_input_buffer();
        printf("Enter birthdate (day month year): ");
    }

    input_string("Enter position: ", new_player.position, 1);  // Prompt for player's position.

    // Add the new player to the selected team.
    teams[team_index].players[teams[team_index].active_size] = new_player;
    teams[team_index].active_size++;  // Increment the active size of the team.
    printf("Player added successfully.\n");
}

// Search for a player by name or kit number and update their information.
void search_update() {
    char search_name[26];  // Store the name of the player being searched.
    int kit_number, search_choice = input_int("Search by:\n1. Name\n2. Kit Number\nEnter choice: ", 1, 2);
    if (search_choice == 1) {
        input_string("Enter player name to search: ", search_name, 1);  // Get the name of the player to search.
    } else {
        kit_number = input_int("Enter kit number to search: ", 1, 99);  // Get the kit number to search.
    }

    // Search through each team's player list.
    for (int i = 0; i < team_count; i++) {
        for (int j = 0; j < teams[i].active_size; j++) {
            // Check if the player matches the search criteria.
            if ((search_choice == 1 && strcasecmp(teams[i].players[j].name, search_name) == 0) || 
                (search_choice == 2 && teams[i].players[j].kit_number == kit_number)) {
                printf("Player found: %s, Kit Number: %d, Club: %s\n", teams[i].players[j].name, teams[i].players[j].kit_number, teams[i].players[j].club);
                // Update the player's kit number and position.
                teams[i].players[j].kit_number = input_int("Update kit number: ", 1, 99);
                input_string("Update position: ", teams[i].players[j].position, 1);
                printf("Player updated successfully.\n");
                return;
            }
        }
    }
    printf("Player not found.\n");
}

// Display statistics about all enrolled clubs, including the number of players and average age.
void display_club_statistics() {
    for (int i = 0; i < team_count; i++) {
        printf("\nClub Name: %s\nNumber of players: %d\n", teams[i].name, teams[i].active_size);
        int total_age = 0;
        for (int j = 0; j < teams[i].active_size; j++) {
            player_t player = teams[i].players[j];
            printf("Name: %s, Kit Number: %d, Position: %s\n", player.name, player.kit_number, player.position);
            total_age += 2024 - player.year;  // Calculate the total age of all players.
        }
        // Display the average age of players in the team.
        printf("Average age of players: %d\n", teams[i].active_size ? total_age / teams[i].active_size : 0);
    }
}
