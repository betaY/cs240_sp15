#include <stdio.h>

void high_low(int, int );
int
main(int argc, char **argv) {
    int high, low, mid;
    char yn = ' ';
    printf("Welcome to the High Low game...\n");

    // Write your implementation here...

    // printf("Think of a number between 1 and 100 and press press <enter>\n");
    do {
        high = 100;
        low = 1;
        high_low(high, low);
        printf("Do you want to continue playing (y/n)?");
        scanf("%c", &yn);
        getchar();
    } while (yn == 'y');
    printf("Thanks for playing!!!\n");
}

void high_low(int high, int low) {
    printf("Think of a number between 1 and 100 and press press <enter>\n");
    while (high >= low) {
        int mid = (high + low)/2;
        char yn = ' ';
        printf("Is it higher than %d? (y/n)\n", mid);
        scanf("%c", &yn);
        getchar();
        //yn = getchar();                                                       
        if (yn == 'y') {
            low = mid+1;
        } else if(yn == 'n') {
            high = mid-1;
        } else {
            printf("Type y or n\n");
        }
    }
    printf("\n>>>>>> The number is %d\n\n", low);
    // return low;
}
