#include <stdio.h>
#include <math.h>

double potencial(double x){
    return 0;
}

double numerov(double *phi, double E, double h, int N){
    double K[N+1];
    double numerador;

    for (int i = 0; i < N + 1; i++){
        double x = i * h;
        K[i] = (E - potencial(x));
    }

    phi[0] = 0.0;
    phi[1] = 2.0 / N;
    double constant = 6.0 * pow(N, 2);

    for (int i = 2; i < N + 1; i++) {
        numerador = 2.0 * (1.0 - 5.0 * K[i-1] / constant) * phi[i-1] - (1.0 + K[i-2] / constant) * phi[i-2];
        phi[i] = numerador / (1.0 + K[i] / constant);
    }
   return phi[N];
}