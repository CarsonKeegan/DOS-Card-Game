
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <random>
#include <stdbool.h>
#include <time.h> 
#include <string.h>
#include <math.h>


typedef struct card_s {
    char color[10];
    int value;
    char action[15];
    struct card_s* pt;
} card;


//functions list TOTAL OF "8 FUNCTIONS"
void ShuffleDeck(card deck[]); //shuffles the deck of 108 cards

card* addCard(card* head, card newCard); // adds an individual card to a linked list, can be used for specific cards too

void printLinkedList(card* head, int player); // int value is for the player num or centerline of which to print, 1 = player 1, 2 = player 2, 0 = centerline

card* addNumCards(card* head, card* deck, int* cardCount, int* cardsInHand, int numCardsAdded); // adds a certain amount of cards to a linked list from the deck

int checkSingleMatch(card* centerCard, card* playerCard); //checks if there are single matches of the players hands and the center line cards.
// for function checkSingleMatch: -1 == no match, 0 == single match, 1 == color match

int checkDoubleMatch(card* centerCard, card* playerCard1, card* playerCard2); //checks if there are double matches of the players hands and the center line cards.
//-1 == no match, 0 == double match, 1 == double color match

card* deleteCard(card* linkedList, card* target, int* cardsInHand); // removes a card struct from a linked list

void CalculateWinner(card* LosersHand, int player); // calculates the points based off the losers hand

void printCardGraphics(card* head);


int main(void) { //Start of the main void function!
    FILE* inp;
    const int size = 108; //change for different deck file inputs
    card deck[size];
    int i, deckInput, deckCardCount = 0, cardsToPlay, cardsInHand1 = 0, cardsInHand2 = 0, cardsInCenterLine = 0, userSelectedCard1, userSelectedCard2, userInput, colorMatchCount = 0, doubleColorMatchCount = 0, drawCounter = 0, loop;
    char userFile[100];
    char dealCardsInput;
    card* playerHand1 = NULL, * playerHand2 = NULL, * centerLine = NULL, * centertemp = NULL, * cardpt1 = NULL, * cardpt2 = NULL, * temp = (card*)malloc(sizeof(card));

    srand((int)time(NULL));

    printf("Lets Play a Game of DOS\nPress 1 to shuffle the DOS deck or 2 to load a deck from a file: "); // determine deck to use
    scanf(" %d", &deckInput);

    if (deckInput == 1) { // shuffles in a normal deck
        inp = fopen("CardDeck.txt", "r"); //open the file "CardDeck.txt".

        if (inp == NULL) {
            printf("Could not open file \n"); //prints cannot open the file.
        }

        for (i = 0; i < size; i++) {  //scan in cards from file into deck
            fscanf(inp, "%s", deck[i].color);
            fscanf(inp, "%d", &deck[i].value);
            fscanf(inp, "%s", deck[i].action);
        }

        ShuffleDeck(deck); //shuffles the deck of cards and awaits command from user/player to deal the shuffled deck of cards.
        printf("The deck is shuffled. Press any key to deal cards: ");
    }
    else if (deckInput == 2) { // enters the user deck, doesn't shuffle
        printf("Enter the File name: "); //prompts user to enter a file name 
        scanf("%s", userFile);

        inp = fopen(userFile, "r"); //programmer 

        if (inp == NULL) {
            printf("Could not open file: "); // prints that file could not be opens 
        }

        for (i = 0; i < size; i++) {
            fscanf(inp, "%s", deck[i].color);
            fscanf(inp, "%d", &deck[i].value);
            fscanf(inp, "%s", deck[i].action);
        }

        printf("The deck has been inputed. Press any key to deal cards: ");

    }
    else { // incorrect input, pops up error
        printf("invalid input, restart program");
        return 0;
    }

    // pause for user input to start the game
    scanf(" %c", &dealCardsInput);

    // initial dealing of cards to each player and centerline
    playerHand1 = addNumCards(playerHand1, deck, &deckCardCount, &cardsInHand1, 7);
    playerHand2 = addNumCards(playerHand2, deck, &deckCardCount, &cardsInHand2, 7);
    centerLine = addNumCards(centerLine, deck, &deckCardCount, &cardsInCenterLine, 2);

    while (cardsInHand1 > 0 && cardsInHand2 > 0 && deckCardCount < 108) { // main loop of game
        // both loop and drawcounter are to check if any cards are played to prompt the draw card from deck feature
        loop = 0;
        drawCounter = 0;

        if (cardsInHand1 <= 0) { // check for winner
            CalculateWinner(playerHand2, 1);
            return 0;
        }
        if (cardsInHand2 <= 0) { // check for winner
            CalculateWinner(playerHand1, 2);
            return 0;
        }

        printf("\nPlayer 1's Turn: \n\n");
        printLinkedList(playerHand1, 1);
        printLinkedList(centerLine, 0);

        centertemp = centerLine;

        while (centertemp != NULL) { //player 1's turn
            if (cardsInHand1 <= 0) { // check for winner
                CalculateWinner(playerHand2, 1);
                return 0;
            }
            if (cardsInHand2 <= 0) { // check for winner
                CalculateWinner(playerHand1, 2);
                return 0;
            }
            cardpt1 = playerHand1;
            cardpt2 = playerHand1;

            printf("How many cards do you want to play on: ");
            printCardGraphics(centertemp); // print the card to be played on
            printf("if you can't play any enter 0: ");
            scanf(" %d", &cardsToPlay);

            if (cardsToPlay == 1) { // if the player only wants to play 1 card on the given center card
                printf("Select a card from 1-%d: ", cardsInHand1); //prompt player to select 1-how many cards they want to match
                scanf(" %d", &userSelectedCard1);
                while (userSelectedCard1 - 1 > 0) {
                    cardpt1 = cardpt1->pt;
                    userSelectedCard1 += -1;
                }
                if (checkSingleMatch(centertemp, cardpt1) == 0) { // normal single match
                    playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1);
                    *temp = *centertemp;
                    centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                    centertemp = temp->pt;
                }
                else if (checkSingleMatch(centertemp, cardpt1) == 1) { // color single match, adds 1 to the colorMatch for discarding at end of player turn
                    playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1);
                    *temp = *centertemp;
                    centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                    centertemp = temp->pt;
                    colorMatchCount += 1;
                }
                else {
                    printf("The selected card can't be placed on: ");
                    printCardGraphics(centertemp); // print the card in the centerline that can't be played on with the user's selection
                }
            }
            else if (cardsToPlay == 2) { // if the player decides to play 2 cards on the given center card
                printf("Select two cards from 1-%d: ", cardsInHand1); //prompt player to select 1-how many cards they want to match 
                scanf(" %d %d", &userSelectedCard1, &userSelectedCard2);
                while (userSelectedCard1 - 1 > 0) { // moves cardpt1 to the 1st card the user wants to play
                    cardpt1 = cardpt1->pt;
                    userSelectedCard1 += -1;
                }
                while (userSelectedCard2 - 1 > 0) {  // moves cardpt2 to the 2nd card the user wants to play
                    cardpt2 = cardpt2->pt;
                    userSelectedCard2 += -1;
                }
                if (checkDoubleMatch(centertemp, cardpt1, cardpt2) == 0) { // normal double match
                    playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1);
                    playerHand1 = deleteCard(playerHand1, cardpt2, &cardsInHand1);
                    *temp = *centertemp;
                    centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                    centertemp = temp->pt;
                }
                else if (checkDoubleMatch(centertemp, cardpt1, cardpt2) == 1) { // double color match, adds 1 to double color match for discarding cards at the end of the turn
                    playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1);
                    playerHand1 = deleteCard(playerHand1, cardpt2, &cardsInHand1);
                    *temp = *centertemp;
                    centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                    centertemp = temp->pt;
                    doubleColorMatchCount += 1;
                }
                else {
                    printf("The selected cards can't be placed on:");
                    printCardGraphics(centertemp); // print the card in the centerline that can't be played on with the user's selection
                }
            }
            else if (cardsToPlay == 0) { //if the player does CANT play any cards or chooses not to play any cards on the centerline
                centertemp = centertemp->pt;
                drawCounter += 1; // increment drawcounter for each center card that is not played on
            }
            else { // player selects a value other than 1, 2, or 0 for the selected center card
                printf("invalid input, please select a different amount of cards\n");
            }

            printLinkedList(playerHand1, 1); // prints the newly updated hand after the player has used any cards on the centerline, or if the hand still remains the same
            loop += 1; // increment loop to compare later
        }

        if (cardsInHand1 <= 0) { // check for winner
            CalculateWinner(playerHand2, 1);
            return 0;
        }
        if (cardsInHand2 <= 0) { //check for winner
            CalculateWinner(playerHand1, 2);
            return 0;
        }

        if (drawCounter == loop) { // compares drawcounter and loop, if they are equal it means the player hasn't played anything on the centerline, so they must draw a card
            printf("\nYou drew a card\n"); //prints to player that they have drawn a card.
            playerHand1 = addNumCards(playerHand1, deck, &deckCardCount, &cardsInHand1, 1);
            printLinkedList(playerHand1, 1);
            cardpt1 = playerHand1;
            centertemp = centerLine;


            while (cardpt1->pt != NULL) { // points the cardpt1 to the newly drawn card since it is the last in the hand
                cardpt1 = cardpt1->pt;
            }

            if (cardsInCenterLine == 0) { //if centerline is empty then add 2 cards into centerline
                centerLine = addNumCards(centerLine, deck, &deckCardCount, &cardsInCenterLine, 2);
            }

            else {
                while (centertemp != NULL) { // go through the centerline and see if drawn card can be played, only allowing the one drawn card to be played for a match

                    printf("Would you like to play: ");
                    printCardGraphics(cardpt1); // prints the drawn card
                    printf("on: ");
                    printCardGraphics(centertemp); // prints the centerline card from which to play the drawn card on
                    printf("press 1 for yes, 0 for no or if the card is unplayable: ");

                    scanf(" %d", &userInput);
                    if (userInput == 1) { // user selects to play the drawn card
                        if (checkSingleMatch(centertemp, cardpt1) == 0) { // normal single match
                            playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1);
                            *temp = *centertemp;
                            centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                            centertemp = temp->pt;
                            break;
                        }
                        else if (checkSingleMatch(centertemp, cardpt1) == 1) { // color single match
                            playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1);
                            *temp = *centertemp;
                            centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                            centertemp = temp->pt;
                            colorMatchCount += 1;
                            break;
                        }
                    }
                    else { // the user doesn't play card or can't play it
                        printf("The selected card can't be placed on:");
                        printCardGraphics(centertemp); // print the card in the centerline that can't be played on with the user's selection
                        centertemp = centertemp->pt;
                    }
                }
                printLinkedList(playerHand1, 1); // print the newly updated hand
            }
        }

        cardpt1 = playerHand1; // set the cardpt1 back to the start of the hand
        printf("pick a card from your hand to discard to the center row: "); //prompt user to pick a specific card to discard to the center row
        scanf(" %d", &userSelectedCard1);
        while (userSelectedCard1 - 1 > 0) { // cardpt1 is incremented through the linked list to the desired card to be discarded
            cardpt1 = cardpt1->pt;
            userSelectedCard1 += -1;
        }
        centerLine = addCard(centerLine, *cardpt1); // the selected card is placed in the centerline
        cardsInCenterLine += 1;
        playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1); // the selected card is removed from the players hand

        if (cardsInCenterLine == 0) { // refreshes the centerline if a match is made off the drawn card
            centerLine = addNumCards(centerLine, deck, &deckCardCount, &cardsInCenterLine, 2);
        }

        else if (cardsInCenterLine == 1) { // adds in one card to the centerline if only one is present
            centerLine = addNumCards(centerLine, deck, &deckCardCount, &cardsInCenterLine, 1);
        }

        if (colorMatchCount > 0) { // accounts for the amount of single color matches made throughout player turn
            printf("\nYou made %d color match(s) you can discard a card on the center row.\n", colorMatchCount);
            while (colorMatchCount > 0) { // for match prompt user to discard a given card from hand and add to centerline
                printLinkedList(playerHand1, 1);
                cardpt1 = playerHand1;
                printf("pick a card from your hand to discard to the center row: "); //prompt user to pick a specific card to discard to the center row
                scanf(" %d", &userSelectedCard1);
                while (userSelectedCard1 - 1 > 0) {
                    cardpt1 = cardpt1->pt;
                    userSelectedCard1 += -1;
                }
                centerLine = addCard(centerLine, *cardpt1);
                cardsInCenterLine += 1;
                playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1);
                printLinkedList(centerLine, 0); // print updated hand for each match
                colorMatchCount += -1;
            }
        }

        if (cardsInHand1 < 0) { // checks for a winner
            CalculateWinner(playerHand2, 1);
            return 0;
        }
        if (cardsInHand2 < 0) { // checks for a winner
            CalculateWinner(playerHand1, 2);
            return 0;
        }

        if (doubleColorMatchCount > 0) { // checks the amount of double color matches made allowing the player to discard a card for each match and add 1 card to the other players hand for each match
            printf("\nYou made %d double color match you can discard a card on the center row and everyone else must draw a card.\n", doubleColorMatchCount);
            while (doubleColorMatchCount > 0) {
                printLinkedList(playerHand1, 1);
                cardpt1 = playerHand1;
                printf("pick a card from your hand to discard to the center row: "); //prompt user to pick a specific card to discard to the center row
                scanf(" %d", &userSelectedCard1);
                while (userSelectedCard1 - 1 > 0) {
                    cardpt1 = cardpt1->pt;
                    userSelectedCard1 += -1;
                }
                centerLine = addCard(centerLine, *cardpt1);
                cardsInCenterLine += 1;
                playerHand1 = deleteCard(playerHand1, cardpt1, &cardsInHand1);
                playerHand2 = addNumCards(playerHand2, deck, &deckCardCount, &cardsInHand2, 1);
                printLinkedList(centerLine, 0); // print newly updated hand after discard
                doubleColorMatchCount += -1;
            }
        }

        printf("Player 1's Turn is over\n"); // End of player 1's turn, start of 2nd players turn

        printf("\nPlayer 2's Turn: \n"); //starts with player 2
        printLinkedList(playerHand2, 2);
        printLinkedList(centerLine, 0);

        centertemp = centerLine; // resets centertemp to the head of the centerline
        drawCounter = 0; // resets drawcounter and loop for the case that a player never plays a card
        loop = 0;

        while (centertemp != NULL) { //player 2's turn
            if (cardsInHand1 <= 0) { // check for winner
                CalculateWinner(playerHand2, 1);
                return 0;
            }
            if (cardsInHand2 <= 0) { // check for winner
                CalculateWinner(playerHand1, 2);
                return 0;
            }
            cardpt1 = playerHand2;
            cardpt2 = playerHand2;

            printf("How many cards do you want to play on: ");
            printCardGraphics(centertemp); // print the card to be played on in the center line
            printf("if you can't play any enter 0: ");
            scanf(" %d", &cardsToPlay);

            if (cardsToPlay == 1) { // if user selects to only play one card from their hand
                printf("Select a card from 1-%d: ", cardsInHand2);
                scanf(" %d", &userSelectedCard1);
                while (userSelectedCard1 - 1 > 0) { // increments cardpt1 to the given card to be played
                    cardpt1 = cardpt1->pt;
                    userSelectedCard1 += -1;
                }
                if (checkSingleMatch(centertemp, cardpt1) == 0) { // normal single match
                    playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);
                    *temp = *centertemp;
                    centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                    centertemp = temp->pt;
                }
                else if (checkSingleMatch(centertemp, cardpt1) == 1) { // color single match
                    playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);
                    *temp = *centertemp;
                    centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                    centertemp = temp->pt;
                    colorMatchCount += 1; // increments color match counter for end of turn
                }
                else {
                    printf("The selected card can't be placed on:");
                    printCardGraphics(centertemp); // print the card in the centerline that can't be played on with the user's selection
                }
            }
            else if (cardsToPlay == 2) { // if player decides to add 2 cards to the given centerline card
                printf("Select two cards from 1-%d: ", cardsInHand2);
                scanf(" %d %d", &userSelectedCard1, &userSelectedCard2);
                while (userSelectedCard1 - 1 > 0) { // increments pt1 to first user selected card
                    cardpt1 = cardpt1->pt;
                    userSelectedCard1 += -1;
                }
                while (userSelectedCard2 - 1 > 0) { // increments pt2 to 2nd user selected card
                    cardpt2 = cardpt2->pt;
                    userSelectedCard2 += -1;
                }
                if (checkDoubleMatch(centertemp, cardpt1, cardpt2) == 0) { // check for normal double match
                    playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);
                    playerHand2 = deleteCard(playerHand2, cardpt2, &cardsInHand2);
                    *temp = *centertemp;
                    centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                    centertemp = temp->pt;
                }
                else if (checkDoubleMatch(centertemp, cardpt1, cardpt2) == 1) { // check for double color match
                    playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);
                    playerHand2 = deleteCard(playerHand2, cardpt2, &cardsInHand2);
                    *temp = *centertemp;
                    centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                    centertemp = temp->pt;
                    doubleColorMatchCount += 1; // increments double color match counter for end of player turn
                }
                else {
                    printf("The selected cards can't be placed on:");
                    printCardGraphics(centertemp); // print the card in the centerline that can't be played on with the user's selection
                }
            }
            else if (cardsToPlay == 0) { // the user decided to not play any cards or can't play a card
                centertemp = centertemp->pt;
                drawCounter += 1; // adds 1 to drawcounter for each time the player doensn't play on the centerline
            }
            else { // user selected a value other than 1, 2, or 0 for playing cards
                printf("invalid input, please select a different amount of cards\n");
            }

            printLinkedList(playerHand2, 2); // print updated hand
            loop += 1; // increment loop for each centerline card gone through, used for comparison later
        }

        if (cardsInHand1 <= 0) { // check for winner
            CalculateWinner(playerHand2, 1);
            return 0;
        }
        if (cardsInHand2 <= 0) { //checks for winner
            CalculateWinner(playerHand1, 2);
            return 0;
        }

        if (drawCounter == loop) { // player didn't play, or couldn't play any cards on any of the centerline cards prompting them to draw a card
            printf("\nYou drew a card\n");
            playerHand2 = addNumCards(playerHand2, deck, &deckCardCount, &cardsInHand2, 1);
            printLinkedList(playerHand2, 2); //prints players updated hand
            cardpt1 = playerHand2;
            centertemp = centerLine;


            while (cardpt1->pt != NULL) { // increment pointer to the newly drawn card
                cardpt1 = cardpt1->pt;
            }

            if (cardsInCenterLine == 0) { // repopulate the centerline if empty
                centerLine = addNumCards(centerLine, deck, &deckCardCount, &cardsInCenterLine, 2);
            }

            else {
                while (centertemp != NULL) { // go through the centerline and see if drawn card can be played, only allowing the one drawn card to be played for a match

                    printf("Would you like to play: ");
                    printCardGraphics(cardpt1); // print the user drawn card 
                    printf("on: ");
                    printCardGraphics(centertemp); // print the center line card from which the user is trying to play their drawn card
                    printf("press 1 for yes, 0 for no or if the card is unplayable: ");

                    scanf(" %d", &userInput);
                    if (userInput == 1) { //checks for a single match from the player's input number.
                        if (checkSingleMatch(centertemp, cardpt1) == 0) { // normal single match
                            playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);
                            *temp = *centertemp;
                            centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                            centertemp = temp->pt;
                            break;
                        }
                        else if (checkSingleMatch(centertemp, cardpt1) == 1) { // color single match
                            playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);
                            *temp = *centertemp;
                            centerLine = deleteCard(centerLine, centertemp, &cardsInCenterLine);
                            centertemp = temp->pt;
                            colorMatchCount += 1; // add to color match counter for end of turn discarding
                            break;
                        }
                    }
                    else { // player didn't or couldn't play the drawn card on the centerline
                        printf("The selected cards can't be placed on:");
                        printCardGraphics(centertemp); // print the card in the centerline that can't be played on with the user's selection
                        centertemp = centertemp->pt;
                    }
                }
                printLinkedList(playerHand2, 2); // print updated hand
            }
        }

        cardpt1 = playerHand2;
        printf("pick a card from your hand to discard to the center row: "); // player is prompted to discard a card at the end of turn
        scanf(" %d", &userSelectedCard1);
        while (userSelectedCard1 - 1 > 0) {
            cardpt1 = cardpt1->pt;
            userSelectedCard1 += -1;
        }
        centerLine = addCard(centerLine, *cardpt1);
        cardsInCenterLine += 1;
        playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);

        if (cardsInCenterLine == 0) { // repopulate center line if empty
            centerLine = addNumCards(centerLine, deck, &deckCardCount, &cardsInCenterLine, 2);
        }

        else if (cardsInCenterLine == 1) { // add one card to centerline if only one is their
            centerLine = addNumCards(centerLine, deck, &deckCardCount, &cardsInCenterLine, 1);
        }

        if (colorMatchCount > 0) { //checks the amount of single color matches made during turn, allowing player to discard a card for each match
            printf("\nYou made %d color match(s) you can discard a card on the center row.\n", colorMatchCount);
            while (colorMatchCount > 0) { //while there is a color match greater than 1 the player must pick a card to remove
                printLinkedList(playerHand2, 2);
                cardpt1 = playerHand2;
                printf("pick a card from your hand to discard to the center row: ");
                scanf(" %d", &userSelectedCard1);
                while (userSelectedCard1 - 1 > 0) {
                    cardpt1 = cardpt1->pt;
                    userSelectedCard1 += -1;
                }
                centerLine = addCard(centerLine, *cardpt1);
                cardsInCenterLine += 1;
                playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);
                printLinkedList(centerLine, 0); //prints the updated center line
                colorMatchCount += -1;
            }
        }

        if (cardsInHand1 < 0) { //checks for a winner
            CalculateWinner(playerHand2, 1);
            return 0;
        }
        if (cardsInHand2 < 0) { // checks for a winner
            CalculateWinner(playerHand1, 2);
            return 0;
        }

        if (doubleColorMatchCount > 0) { //checks if there was a double color match
            printf("\nYou made %d double color match you can discard a card on the center row and everyone else must draw a card.\n", doubleColorMatchCount);
            while (doubleColorMatchCount > 0) { //while there is a double color match
                printLinkedList(playerHand2, 2); //prints the updated player's hand 
                cardpt1 = playerHand2;
                printf("pick a card from your hand to discard to the center row: "); // prompt user to pick a card to discard to the center row.
                scanf(" %d", &userSelectedCard1);
                while (userSelectedCard1 - 1 > 0) {
                    cardpt1 = cardpt1->pt;
                    userSelectedCard1 += -1;
                }
                centerLine = addCard(centerLine, *cardpt1);
                cardsInCenterLine += 1;
                playerHand2 = deleteCard(playerHand2, cardpt1, &cardsInHand2);
                playerHand1 = addNumCards(playerHand1, deck, &deckCardCount, &cardsInHand1, 1);
                printLinkedList(centerLine, 0);
                doubleColorMatchCount += -1;
            }
        }

        printf("Player 2's Turn is over\n\n"); //switches back to the 1st player
    }

    if (cardsInHand1 < 0) { //checks if there is a winner
        CalculateWinner(playerHand2, 1);
        return 0;
    }
    if (cardsInHand2 < 0) { //checks if there is a winner
        CalculateWinner(playerHand1, 2);
        return 0;
    }

    return 0;
}

void ShuffleDeck(card deck[]) { //shuffles the deck of 108 cards
    const int size = 108;  //change for different deck file inputs
    int randnums[size];
    int i, j, tempval;
    card tempdeck[size];

    for (i = 0; i < size; ++i) { // fills randnums array with random integers from 0-107
        tempval = (rand() % size);
        j = 0;

        while (j < i) {  // check if tempval is in randnums already
            if (tempval == randnums[j]) {
                tempval = (rand() % size);
                j = 0;
            }
            else {
                j++;
            }
        }
        randnums[i] = tempval;
    }

    for (i = 0; i < size; ++i) { // fill a temp deck so values aren't overwritten
        tempdeck[i] = deck[randnums[i]];
    }
    for (i = 0; i < size; ++i) {
        deck[i] = tempdeck[i];
    }
}

card* addCard(card* head, card Card) { // adds an individual card to a linked list, can be used for specific cards too
    card* newCard = (card*)malloc(sizeof(card));
    *newCard = Card;
    newCard->pt = NULL;
    if (head == NULL) {
        head = newCard;
    }
    else {
        card* current = head;
        while (current->pt != NULL) {
            current = current->pt;
        }
        current->pt = newCard;
    }
    return head;
}

void printLinkedList(card* head, int player) { // prints a linked list
    if (player == 1 || player == 2) { // for player 1 or 2
        printf("Player %d's hand: "); //prints which player hand will be displayed
    }
    else { // for the centerline
        printf("Centerline: ");
    }

    while (head != NULL) {
        printCardGraphics(head);
        head = head->pt;
    }
}

card* addNumCards(card* head, card* deck, int* cardCount, int* cardsInHand, int numCardsAdded) { // adds a certain amount of cards to a linked list from the deck
    int  startcard = *cardCount;
    while (*cardCount < startcard + numCardsAdded) {
        if (*cardCount > 108) { // if the cardCount from the deck ever goes over its alloted amount of cards the below statement is printed and the winner is calculated immediatly
            printf("the deck ran out of cards\n");
            CalculateWinner(head, 1);
        }
        head = addCard(head, deck[*cardCount]);
        *cardCount += 1;
        *cardsInHand += 1;
    }
    return head;
}

int checkSingleMatch(card* centerCard, card* playerCard) { // compares the player card and centerline card and returns -1 == no match, 0 == single match, 1 == color match
    if ((centerCard->value == playerCard->value) || strcmp(playerCard->action, "AnyNumber") == 0 || strcmp(centerCard->action, "AnyNumber") == 0) {
        if ((strcmp(centerCard->color, playerCard->color) == 0) || (strcmp(playerCard->action, "AnyColor") == 0) || (strcmp(centerCard->action, "AnyColor") == 0)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return -1;
    }
}

int checkDoubleMatch(card* centerCard, card* playerCard1, card* playerCard2) { //compares the two player cards with one another and the centerline and returns -1 == no match, 0 == double match, 1 == double color match
    if ((centerCard->value == playerCard1->value + playerCard2->value) || strcmp(playerCard1->action, "AnyNumber") == 0 || strcmp(playerCard2->action, "AnyNumber") == 0 || strcmp(centerCard->action, "AnyNumber") == 0) {
        if (((strcmp(centerCard->color, playerCard1->color) == 0) && (strcmp(centerCard->color, playerCard2->color) == 0)) || (strcmp(playerCard1->action, "AnyColor") == 0 && strcmp(playerCard2->color, centerCard->color) == 0) || (strcmp(playerCard2->action, "AnyColor") == 0 && strcmp(playerCard1->color, centerCard->color) == 0) || (strcmp(playerCard2->action, "AnyColor") == 0 && strcmp(playerCard1->action, "AnyColor") == 0) || (strcmp(centerCard->action, "AnyColor") == 0 && strcmp(playerCard1->color, playerCard2->color) == 0)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return -1;
    }
}

card* deleteCard(card* linkedList, card* target, int* cardsInHand) { // removes a card struct from a linked list
    card* tempPt = NULL; //sets tempPt to NULL 
    tempPt = linkedList;
    card* header = linkedList;

    if (tempPt->value == target->value && strcmp(tempPt->color, target->color) == 0 && strcmp(tempPt->action, target->action) == 0) {
        linkedList = linkedList->pt;
        free(target);
        *cardsInHand = *cardsInHand - 1;
        return linkedList;
    }

    while (tempPt != NULL) {
        tempPt = tempPt->pt;
        if (tempPt->value == target->value && strcmp(tempPt->color, target->color) == 0 && strcmp(tempPt->action, target->action) == 0) {
            linkedList->pt = target->pt;
            free(target);
            *cardsInHand = *cardsInHand - 1;
            return header;
        }
        linkedList = linkedList->pt;
    }
}

void CalculateWinner(card* LosersHand, int player) { // calculates the points based off the losers hand
    int points = 0;
    printf("\n");
    while (LosersHand != NULL) {
        if (strcmp(LosersHand->action, "AnyNumber") == 0) { // for the 2 black card
            points += 40;
        }
        if (strcmp(LosersHand->action, "AnyColor") == 0) { // for # number cards
            points += 20;
        }
        else {
            points += LosersHand->value;
        }
        LosersHand = LosersHand->pt;
    }
    printf("Player %d won with %d points!", player, points); // prints which player has won, with how many total points
    exit;
}

void printCardGraphics(card* head) { // prints the graphics for a DOS card

    if (strcmp(head->action, "AnyColor") == 0) {
        printf("\n---------\n");
        printf("|%d\t|\n", head->value);
        printf("|%s\t|\n", head->color);
        printf("|AnyClr\t|\n");
        printf("|%d\t|\n", head->value);
        printf("---------\n");
    }
    else if (head->value != 11) {
        printf("\n---------\n");
        printf("|%d\t|\n", head->value);
        printf("|%s\t|\n", head->color);
        printf("|%s\t|\n", head->action);
        printf("|%d\t|\n", head->value);
        printf("---------\n");
    }
    else {
        printf("\n---------\n");
        printf("|#\t|\n");
        printf("|%s\t|\n", head->color);
        printf("|AnyNum\t|\n");
        printf("|#\t|\n");
        printf("---------\n");

    }

}

