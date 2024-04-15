#include <stdio.h>

int main() {

  int a;
  int s;
  int i;
  printf("donner l'entier a saisir");
  scanf("%d",&a);
  s = 0;
  for(i=1;i<a;i++) {
    if(a%i==0) {
      s = s+1;
    }
  }
  if(s==1)
    {  printf("le nombre saisie est premier");
    }
  else {
    printf("le nombre saisie n'est pas premier");
  }
}
