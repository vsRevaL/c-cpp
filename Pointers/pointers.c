#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cash = 100;

void play(int bet)
{
    //char c[3] = {'J','Q','K'};
    char *c = (char*)malloc(3*sizeof(char)); /////////////
    c[0] = 'J';
    c[1] = 'Q';
    c[2] = 'K';
    printf("Shuffling...\n");
    srand(time(NULL));
    for(int i=0; i<5; i++){
        int x = rand() % 3;
        int y = rand() % 3;
        int temp = c[x];
        c[x] = c[y];
        c[y] = temp;
    }
    int playersGuess;
    printf("What's the position of queen - 1,2 or 3?  ");
    scanf("%d", &playersGuess);
    if(c[playersGuess - 1] == 'Q'){
        cash += 3*bet;
        printf("You Win! Result = %c%c%c Toal Cash= %d\n", c[0],c[1],c[2],cash);
    }
    else{
        cash -= bet;
        printf("You Loose! Result = %c%c%c Toal Cash= %d\n", c[0],c[1],c[2],cash);
    }

    free(c); /////////////
}

int main()
{
    int bet;
    printf("**Welcome to the Virtual Casino**\n\n");
    printf("Total cash = $%d\n",cash);
    while(cash > 0){
        printf("What is your bet? $");
        scanf("%d", &bet);
        if(bet == 0 || bet > cash){
            break;
        }
        play(bet);
        printf("\n***************************************\n");
    }
}
