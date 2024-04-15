#include <stdlib.h>
#include <stdio.h>


int facto(int n) {
    if (n == 1) { return 1; }

    else {

        return n * facto(n - 1);
    }

}


double power(double x, int n) {

    if (n == 0) {
        return 1;
    } else {
        return x * power(x, n - 1);
    }

}

double expon(double x, int n) {

    int i;
    double s = 1;
    if (n == 0) {
        return 1;
    } else {
        for (i = 1; i <= n; i++) {
            s = s + (power(x, i) / facto(i));

        }

        return s;

    }


}


int main() {

    double x;
    printf("donner le nombre de x\n");
    scanf("%lf", &x);
    printf("le resultat de exp x est :\n");
    printf("%lf", expon(x, 20));


    return 0;
}






