//Library : General utilities
#include <stdlib.h>
//Library : fonctions usuelles de C (input + output)
#include <stdio.h>

void squares(int* a , int* b) {
    *a = *a * *a;
    *b = *b * *b;
}

int main() {
    int a =4;
    int b = 0;
    int* pa = &a;
    int* pb = &b;

   // printf("valeurs sont: %d , %d",a,b);

   printf("les valeurs des pointeurs sont : %d , %d\n",*pa,*pb);
   printf("Les adresses des pointeurs sont : %p, %p\n",pa,pb);


       b = *pb + 9;
    printf("valeurs sont: %d , %d\n",a,b);

    squares(&a,&b);
    printf("valeurs sont: %d , %d\n",a,b);
    return 0;
}


