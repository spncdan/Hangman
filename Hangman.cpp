// This program is a Console-Based Hangman game which will have 4 difficulties which can be chosen by the player.
// Language: C++
// I've made it so this game is based on individual inputting of letters as a guess rather than entire word guesses.
// Sources: Error checking with ChatGPT and for general guidance when it came to invalid inputs and the hint system implementation.
// (Easy = 4 lettered words, Normal = 6 lettered words, Hard = 8 lettered words, Insane = 12 lettered words)

// Features:
// 4 Difficulties
// Invalid inputs are error checked (ChatGPT assisted me heavily on these aspects of the code)
// Hint system is implemented (ChatGPT assisted me in conceptualizing how to go about creating this)
// inputting already correct letters causes error which prompts you to renter a new letter

#include <iostream>
#include <string> // ensures that the program knows about the std::string class and it's methods.
#include <cstdlib> // Used for rand() function
#include <ctime> // to seed the random number generator
#include <limits> // necessary for the operation of numeric_limits in order to help error check invalid inputs for switches
using namespace std;

// Function prototypes
void wordtoguess(string[10], char[], int, string&);
void printarray(char[12], int);
bool checkloss(int);
bool checkwin(char[], char[], int);
bool isLetterGuessed(char, char[], int);

// Main function
int main() {
    // Seed the random number generator for wordtoguess function
    srand(static_cast<unsigned int>(time(0)));

    // Boolean variable which will be used as a condition for the while loop which will operate the menu and loop back to the menu when the game is over.
    bool menu_exit = false;
    // A series of array which will hold potential words which can be used as potential word options for the game
    // Each array pertains to the difficulty chosen by the user (I removed captialization to simplify the need for case insensitvity)
    string easy_words[10] = { "tree", "book", "fish", "jump", "star", "blue", "snow", "love", "moon", "play" };
    string normal_words[10] = { "planet", "member", "budget", "camera", "forest", "school", "bridge", "simple", "yellow", "tunnel" };
    string hard_words[10] = { "backpack", "daughter", "hospital", "keyboard", "magazine", "sandwich", "treasure", "umbrella", "discount", "notebook" };
    string insane_words[10] = { "abolitionist", "accessorises", "bewilderment", "heartwarming", "consistently", "breakthrough", "appreciation", "preservation", "enlightening", "invigorating" };

    // This will be a visual array which will show the building of the hangman
    string hangman_stages[7] = // This idea came from ChatGPT when I asked from potential ways to visually represent the hangman without having to manually change the string each time
    {
        // Stage 1 (0 wrong guesses) (index 0)
        "  +------+  \n" // I'm using \n here in order to allow this string to be multi-lined without making it an entirely new instance in the array
        "         |  \n"
        "         |  \n"
        "         |  \n"
        "         |  \n"
        "         |  \n"
        " ============\n",
        // Stage 2 (1 wrong guess) (index 1)
        "  +------+  \n"
        "  O      |  \n"
        "         |  \n"
        "         |  \n"
        "         |  \n"
        "         |  \n"
        " ============\n",
        // Stage 3 (2 wrong guesses) (index 2)
        "  +------+  \n"
        "  O      |  \n"
        "  |      |  \n"
        "         |  \n"
        "         |  \n"
        "         |  \n"
        " ============\n",
        // Stage 3 (3 wrong guesses) (index 3)
        "  +------+  \n"
        "  O      |  \n"
        " /|      |  \n"
        "         |  \n"
        "         |  \n"
        "         |  \n"
        " ============\n",
        // Stage 4 (4 wrong guesses) (index 4)
        "  +------+  \n"
        "  O      |  \n"
        " /|\\     |  \n"
        "         |  \n"
        "         |  \n"
        "         |  \n"
        " ============\n",
        // Stage 5 (5 wrong guesses) (index 5)
        "  +------+  \n"
        "  O      |  \n"
        " /|\\     |  \n"
        " /       |  \n"
        "         |  \n"
        "         |  \n"
        " ============\n",
        // Stage 6 (6 wrong guesses) (index 6)
        "  +------+  \n"
        "  O      |  \n"
        " /|\\     |  \n"
        " / \\     |  \n"
        "         |  \n"
        "         |  \n"
        " ============\n",
    };
    while (menu_exit == false) {
        // This will be a variable which will be chosen by the player to operate within the menu (will reset after each loop back)
        int menu_option = 0;
        // This is the word that will be chosen to be guessed by the player, this will be randomly changed each time the player begins the game, choosing specifically from the words list above. (will reset after each loop back)
        char word_to_guess[12] = { '-','-','-','-','-','-','-','-','-','-','-','-' };
        // This will be the word to be guessed in string format.
        string correct_word = " ";
        // This will represent the progress made by the player, what letters are right will be in accordance to the word_to_guess array above (will reset after each loop back)
        char current_progress[12] = { '-','-','-','-','-','-','-','-','-','-','-','-' };
        // This will be the variable which will be inputted in the guessed_letters and word_to_guess arrays and will change accordingly to the difficulty chosen by the player (will reset after each loop back)
        int size = 0;

        cout << "*************************************************" << endl;
        cout << endl;
        cout << "       Welcome to Hangman! " << endl;
        cout << endl;
        cout << "Please choose a number from the following options: " << endl;
        cout << endl;
        cout << "1. Play " << endl;
        cout << "2. How To Play " << endl;
        cout << "3. Exit " << endl;
        cout << endl;
        cout << "*************************************************" << endl;
        cout << endl;

        // This will allow our player to input their choice (1, 2, or 3) (chatgpt helped me resolve an issue I was having errors where if the user inputted a non-integer value the console would enter a fail-state.)
        while (!(cin >> menu_option)) {
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input. Please enter a number: " << endl; // Asking user to input a number
        }

        // Switch for the menu
        switch (menu_option) {
        case 1: { // Commences the game
            bool error_check = true;
            while (error_check == true) {
                // Variable to decide difficulty of the game
                int difficulty = 0;

                // Asking player to choose their preferred difficulty
                cout << " " << endl;
                cout << "Please Choose a Difficulty" << endl;
                cout << "1. Easy" << endl;
                cout << "2. Normal" << endl;
                cout << "3. Hard" << endl;
                cout << "4. Insane" << endl;
                cout << " " << endl;
                // Console input by player (chatgpt helped me resolve an issue I was having errors where if the user inputted a non-integer value the console would enter a fail-state.)
                while (!(cin >> difficulty)) {
                    cin.clear(); // Clear the error state (sourced from chatgpt)
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input (sourced from chatgpt)
                    cout << "Invalid input. Please enter a number: " << endl;
                }

                if (difficulty == 1) { // Easy mode
                    size = 4; // Changing size to 4 (4 lettered word)
                    wordtoguess(easy_words, word_to_guess, size, correct_word); // Function to determine word to be guessed by player
                    error_check = false; // Passes error check
                }
                else if (difficulty == 2) { // Normal mode
                    size = 6; // Changing size to 6 (6 lettered word)
                    wordtoguess(normal_words, word_to_guess, size, correct_word); // Function to determine word to be guessed by player
                    error_check = false; // Passes error check
                }
                else if (difficulty == 3) { // Hard mode
                    size = 8; // Changing size to 8 (8 lettered word)
                    wordtoguess(hard_words, word_to_guess, size, correct_word); // Function to determine word to be guessed by player
                    error_check = false; // Passes error check
                }
                else if (difficulty == 4) { // Insane mode
                    size = 12; // Changing size to 12 (12 lettered word)
                    wordtoguess(insane_words, word_to_guess, size, correct_word); // Function to determine word to be guessed by player
                    error_check = false; // Passes error check
                }
                else { // There is still an issue where if the user inputs a non-integer, it will loop indefinitely at high speeds. (This will be fixed after working on core mechanics and game)
                    cout << "Please enter a valid selection." << endl; // will loop back to choose difficulty menu
                    cout << " " << endl;
                }
            }
            // Variable which keeps track of wrong guesses made by the player
            int wrong_guesses = 0;
            // Variable which keeps track of consecutive wrong guesses by the player (3 will = a random letter hint) (sourced by chatgpt)
            int consecutive_wrong_guesses = 0;
            // Boolean variable which will work as a flag to turn true if a letter is correct
            bool correct_guess = false;
            // Boolean variable which ensure while loop of the game
            bool keepgoing = true;

            // While loop which will loop the entirety of the game
            while (keepgoing == true) {
                // This will be the variable in which the user will guess the letter (will reset after each loop back)
                char guessed_letter = '0';
                correct_guess = false; // reverts correct_guess back to false
                cout << " " << endl;
                cout << hangman_stages[wrong_guesses] << endl; // printing out the hangman visual in accordance to where the player stands in terms of their guesses
                cout << " " << endl;
                printarray(current_progress, size); // Printing out the current progress of the player
                cout << " " << endl;

                do { // Sourced from ChatGPT
                    cout << "Please enter a letter: " << endl;
                    cin >> guessed_letter; // Prompting the user to enter a letter
                    if (cin.fail() || !isalpha(guessed_letter)) { // Checking if input failed or if the input is not an alphabetic character
                        cin.clear(); // Clearing the error state
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignoring invalid input
                        cout << "Invalid input. Please enter a single letter: " << endl; // Asking the user to enter a single letter
                        continue; // Restarting the loop if input was invalid
                    }
                    guessed_letter = tolower(guessed_letter); // Converting the guessed letter to lowercase to handle case insensitivity
                    cout << " " << endl;
                    if (isLetterGuessed(guessed_letter, current_progress, size)) { // Checking if the guessed letter has already been correctly guessed
                        cout << "You've already guessed that letter. Please enter a different letter." << endl;
                    }
                } while (isLetterGuessed(guessed_letter, current_progress, size)); // Looping if the guessed letter has already been correctly guessed

                for (int i = 0; i < size; i++) { // for loop which will check if the guessed_letter is equal to any of the contents of the word to be guessed.
                    if (guessed_letter == word_to_guess[i]) {
                        current_progress[i] = guessed_letter;
                        correct_guess = true; // if the guessed_letter at least covers one letter in the word to be guessed this will be true and prevent an addition to the wrong_guesses variable.
                    }
                }

                // Checks if player got a wrong guess and if so, wrong_guess will increment along with the consecutive_wrong_guesses
                if (correct_guess == false) {
                    wrong_guesses += 1; // adding 1 to wrong_guesses if user gets an incorrect guess
                    consecutive_wrong_guesses += 1; // adds 1 to the consecutive wrong guesses
                }
                else {
                    consecutive_wrong_guesses = 0; // if player gets a correct guess this will revert it back to 0
                }

                // Hint system (activates if the user gets 3 wrong in a row without a correct letter match up)
                // Provide a one letter hint if 3 consecutive wrong guesses
                if (consecutive_wrong_guesses == 3) {
                    cout << " " << endl;
                    cout << "Hint: Here's one letter to help you out!" << endl;
                    cout << " " << endl;
                    bool hint_given = false;
                    while (!hint_given) {
                        int random_index = rand() % size;
                        if (current_progress[random_index] == '-') { // checks if random_index is an empty spot and if not it will loop back and choose a new random number
                            current_progress[random_index] = word_to_guess[random_index]; // makes current_progress random index = to word_to_guess random index
                            hint_given = true; // since the hint is now given, we will break out of the while loop and continue the game.
                        }
                    }
                    consecutive_wrong_guesses = 0; // reverts consecutive_wrong_guesses to 0
                }

                // Checking for loss condition
                if (checkloss(wrong_guesses) == true) {
                    cout << " " << endl;
                    cout << "You lose!" << endl;
                    cout << " " << endl;
                    cout << hangman_stages[wrong_guesses] << endl;
                    cout << " " << endl;
                    cout << "The correct word was " << correct_word << "." << endl;
                    keepgoing = false; // breaks out of loop
                    break;
                }

                // Checking for win correction
                if (checkwin(word_to_guess, current_progress, size) == true) {
                    cout << " " << endl;
                    cout << "You win by correctly guessing " << correct_word << " as the word!" << endl;
                    cout << " " << endl;
                    keepgoing = false; // breaks out of loop
                    break;
                }
            }
        }
              break;
        case 2: { // Shows how to play the game

            cout << " " << endl;
            cout << "In this game, you will be given a specific word to guess depending on your chosen difficulty." << endl;
            cout << " " << endl;
            cout << "The difficulties are the following:" << endl;
            cout << "Easy: 4 lettered words" << endl;
            cout << "Normal: 6 lettered words" << endl;
            cout << "Hard: 8 lettered words" << endl;
            cout << "Insane: 12 lettered words" << endl;
            cout << " " << endl;
            cout << "Each time you input an incorrect letter, the board will visually change to show a part of a stick figure being made on the hanging mechanism" << endl;
            cout << " " << endl;
            cout << hangman_stages[0] << endl;
            cout << " " << endl;
            cout << hangman_stages[6] << endl;
            cout << " " << endl;
            cout << "If you guess wrongly 6 times, you lose!" << endl;
            cout << " " << endl;
            break;
        }
        case 3: { // Exits the program
            cout << "Thank you for playing!" << endl;
            cout << "Exiting Program..." << endl;
            menu_exit = true;
            break;
        }
        default: { // The player enters something other than the numbers above (I need to address an issue where if the player enters a non-integer value the menu will loop extremely fast)
            cout << " " << endl;
            cout << "Please enter a valid selection." << endl;
            cout << " " << endl;
        }
        }
    }
    return 0;
}

// Function which takes a random word from a string array and inputs each one of it's letters into a char array
void wordtoguess(string arr[10], char arr1[], int size, string& word) { // & is a reference parameter to ensure that the changes to the word parameter stay the same in main
    // Random selection of word from the array
    int randomnum = rand() % 10;
    string selected_word = arr[randomnum];
    word = selected_word;
    // Allowing char array to hold the selected word
    for (int i = 0; i < size; i++) {
        arr1[i] = selected_word[i];
    }
}

// Function to print one dimensional array
void printarray(char arr[12], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl; // Added a newline for better formatting

}

// Function to find whether a user has already guessed a correct letter (sourced from chatgpt when trying to prevent users from being able to reinput correct letters) (Currently not working)
bool isLetterGuessed(char guessed_letter, char current_progress[], int size) {
    for (int i = 0; i < size; i++) {
        if (current_progress[i] == guessed_letter) {
            return true;
        }
    }
    return false;
}

// Function which checks if player has won by getting the entire word
bool checkwin(char arr1[], char arr2[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false; // if for any reason any of the contents of the array don't match, it will return false
        }
    }
    return true; // if the array is entirely equal, it will return true
}

// Function which checks if player has lost by getting 6 wrong guesses
bool checkloss(int wrong_guesses) {
    if (wrong_guesses >= 6) {
        return true; // if wrong_guesses exceeds or equals 6 it will return true
    }
    else {
        return false; // if wrong_guesses does not exceed 6 it will return false
    }
}
