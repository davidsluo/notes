/*
RockPaperScissors.java
Author: David Luo
Submission Date: 8/28/2016
Purpose: This is a basic Rock, Paper, Scissors game simulator.

To run, first compile by running "javac RockPaperScissors.java".
Then, run "java RockPaperScissors".
*/
import java.util.Scanner;
import java.util.Random;

public class RockPaperScissors {

    public static void main(String[] args) {

        // Win counter vars
        int compWins = 0;
        int playerWins = 0;
        int ties = 0;

        // Main loop
        while (true) {
            char playerInput = getPlayerInput();
            char compInput = getCompInput();

            // Handle exit.
            if (playerInput == 'e') {
                break;
            }

            // Handle ties.
            else if (playerInput == compInput) {
                System.out.println("We tied. I also chose " + translateChoice(compInput) + ".");
                ties++;
            }

            // Handle all other cases.
            else
            {
                if (playerInput == 'r' && compInput == 'p'){
                    System.out.println("I won! I chose " + translateChoice(compInput) + ".");
                    compWins++;
                }
                else if (playerInput == 'r' && compInput == 's'){
                    System.out.println("You won! I chose " + translateChoice(compInput) + ".");
                    playerWins++;
                }
                else if (playerInput == 'p' && compInput == 's'){
                    System.out.println("I won! I chose " + translateChoice(compInput) + ".");
                    compWins++;
                }
                else if (playerInput == 'p' && compInput == 'r'){
                    System.out.println("You won! I chose " + translateChoice(compInput) + ".");
                    playerWins++;
                }
                else if (playerInput == 's' && compInput == 'r'){
                    System.out.println("I won! I chose " + translateChoice(compInput) + ".");
                    compWins++;
                }
                else if (playerInput == 's' && compInput == 'p'){
                    System.out.println("You won! I chose " + translateChoice(compInput) + ".");
                    playerWins++;
                }
            }
        }

        // Print game statistics.
        System.out.println("Game ended.");
        System.out.println("Player wins:\t" + playerWins);
        System.out.println("Computer wins:\t" + compWins);
        System.out.println("Ties:\t\t" + ties);
    }

    // Gets r, p, or s from the player.
    public static char getPlayerInput() {
        String input;
        Scanner scan = new Scanner(System.in);

        while(true) {
            System.out.println("Enter your choice (R for Rock, P for Paper or S for Scissors) OR E to Exit:");
            input = scan.next();
            input = input.toLowerCase();

            // Input validation.
            if (input.equals("r") || input.equals("p") || input.equals("s") || input.equals("e")) {
                return input.charAt(0);
            }
            else {
                System.out.println("Invalid input. Please try again.");
                System.out.println(input);
            }
        }
    }

    // Randomly generates a number 0, 1, or 2, and returns r, p, or s respectively.
    public static char getCompInput() {
        Random rand = new Random();
        switch (rand.nextInt(3)) {
            case 0:
                return 'r';
            case 1:
                return 'p';
            case 2:
                return 's';
            default:
                return '0';
        }
    }

    // Makes r, p, s into Rock, Paper, Scissors.
    // *rps* must be char 'r', 'p', or 's', or unexpected behavior may occur.
    public static String translateChoice(char rps) {
        switch (rps) {
            case 'r':
                return "Rock";
            case 'p':
                return "Paper";
            case 's':
                return "Scissors";
            default:
                return "";
        }
    }
}