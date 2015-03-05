#include <stdio.h>
#include <math.h>
#include <string.h>

int ctoi(char a){
    if(a == '0') {
        return 0;
    } else if(a == '1') {
        return 1;
    } else if(a == '2') {
        return 2;
    } else if(a == '3') {
        return 3;
    } else if(a == '4') {
        return 4;
    } else if(a == '5') {
        return 5;
    } else if(a == '6') {
        return 6;
    } else if(a == '7') {
        return 7;
    } else if(a == '8') {
        return 8;
    } else if(a == '9') {
        return 9;
    } else if(a == 'A') {
        return 10;
    } else if(a == 'B') {
        return 11;
    } else if(a == 'C') {
        return 12;
    } else if(a == 'D') {
        return 13;
    } else if(a == 'E') {
        return 14;
    } else if(a == 'F') {
        return 15;
    } else if(a == 'G') {
        return 16;
    } else if(a == 'H') {
        return 17;
    } else if(a == 'I') {
        return 18;
    } else if(a == 'J') {
        return 19;
    } else if(a == 'K') {
        return 20;
    } else if(a == 'L') {
        return 21;
    } else if(a == 'M') {
        return 22;
    } else if(a == 'N') {
        return 23;
    } else if(a == 'O') {
        return 24;
    } else if(a == 'P') {
        return 25;
    }
}

float my_pow(float a, float b) {
    float r = a;

    if (b > 0) {
        while (--b)
            r *= a;
    } else if (b < 0) {
        while (++b) 
            r *= a;
        r = 1/r;
    } else 
        r = 1;

    return r;
}

int main(int argc, char ** argv) {
    
    char base_digits[25] = {'0','1','2','3','4','5','6','7','8',
                            '9','A','B','C','D','E','F','G','H',
                            'I','J','K','L','M','N','O'};

    if(argc <= 3) {
        printf("Usage:  convert <basefrom> <baseto> <number>\n");
        return 1;
    }
    //int num = atoi(argv[3]);
    int num[64];

    int base_from = atoi(argv[1]);
    int base_to = atoi(argv[2]);
    // int a = 2;
    //printf("%d, %d, %d\n", base_from, base_to, num);
    printf("Number read in base %d: %s\n", base_from, argv[3]);

    int base_ten = 0;
    int i = 0;
    int j = 0;
    for (i = strlen(argv[3])-1; i >= 0; i--) {
        //printf("%d, %d\n", i, j);
        //printf("%d\n", ((int)argv[3][i] - 48));
        //base_ten += ((int)argv[3][i] - 48) * pow(base_from, j);
        if (ctoi(argv[3][i]) > base_from)
        {
            /* code */
            printf("Wrong digit in number.\n");
            return 1;
        }
        base_ten += ctoi(argv[3][i]) * (int)my_pow(base_from, j);
        //int pow = (int)pow(base_from, j);
        j++;
    }

    //printf("base_10 = %d\n", base_ten);
    printf("Converted to base 10: %d\n", base_ten);

    int converted_num[128];
    i = 0;
    while(base_ten != 0) {
        converted_num[i] = base_ten % base_to;
        base_ten = base_ten / base_to;
        i++;
        //printf("-----------------%d\t%d------------------\n", converted_num[i-1], base_ten);
    }

    i--;
    printf("Converted to base %d: ", base_to);
    for(  ; i>=0; i--) /* go backward through array */
    {
        printf("%c", base_digits[converted_num[i]]);
    }
    printf("\n");

    return 0;
}
