#include <stdlib.h>
#include <stdio.h>


void int_to_bits(int n) {

    int size = sizeof(n)*8;

    unsigned int mask = 1 << (size - 1);
        for(int i = 0; i < size; i++) {
        // Si le bit est 1, affiche 1, sinon affiche 0
        printf("%d", (n & mask) ? 1 : 0);
        
        // Décalage du masque vers la droite
        mask >>= 1;
        
        // Ajoute un espace tous les 8 bits pour faciliter la lecture
        if ((i + 1) % 8 == 0)
            printf(" ");
    }
    printf("\n");


    
}


int main() {

    int x;
    printf("donner le numero a convertir\n");
    scanf("%d" , &x);
    int_to_bits(x);
    

    return 0;
}


