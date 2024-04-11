#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main() {

  int valeur_a_trouver;
  int s = 10;
  int i;
  int a;
  
  srand(getpid());
  valeur_a_trouver = rand() % 100;
  printf("vous avez %d essais" , s);

  for(i=0;i<10;i++){
    printf("essai numero %d \n" , s);
    s = s - 1;
    printf("saisir votre numero");
    scanf("%d",&a);

    if(a==valeur_a_trouver) {
      printf("congrats you won");
      break;
    }

    printf("la valeur est %d" , valeur_a_trouver);
    
  }
  

}
