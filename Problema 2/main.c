#include <stdio.h>
#include <math.h>
#include "header.h"

int main(){

    double L = 1;
    int N = 100;
    double h = L / N;

    double phi[N+1];
    double E1 = 1;
    double E2 = 5;
    double E3 = 0.0;
    double phi1 = 0.0;
    double phi2 = 0.0;
    double phi3 = 0.0;
    double etha = 1e-10;
    double E[5] = {0, 0, 0, 0, 0};
    int i = 0;

    // VALORES PROPIOS

    printf("Primeros 5 valores propios:\n");
    while (i < 5){
        phi1 = numerov(phi, E1, h, N);
        phi2 = numerov(phi, E2, h, N);

        if (phi1 * phi2 > 0){
            E1 = E2;
            E2 = E2 + 5.0;
        }
        else if (phi1 * phi2 < 0){
            E3 = (E1 + E2) / 2;
            phi3 = numerov(phi, E3, h, N);

            if (phi1 * phi3 < 0){
                E2 = E3;
            }
            else if (phi3 * phi2 < 0){
                E1 = E3;
            }

            if (fabs(E1 - E2) < etha){
                E[i] = (E1 + E2) / 2;
                printf("%.4f\n", E[i]);

                E1 = E2;
                E2 = E2 + 5.0;
                ++i;
            }
        }
    }

    // VECTORES PROPIOS 
    
    double K[N+1];
    for (int i = 0; i < N + 1; i++){
        phi[i] = 0;
        K[i] = 0;
    }

    double x = 0.0;
    double autovectores[5][N+1];
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < N+1; j++){
            x = i * h;
            K[i] = (E[i] - potencial(x));
        }

        phi[0] = 0.0;
        phi[1] = 2.0 / N;
        //printf("%f %f\n", 0.0, phi[0]);
        //printf("%f %f\n", 1 / (double)N, phi[1]);
        double constant = 6.0 * pow((double)N, 2);

        for (int i = 2; i < N + 1; i++) {
        double numerador = 2.0 * (1.0 - 5.0 * K[i-1] / constant) * phi[i-1] - (1.0 + K[i-2] / constant) * phi[i-2];
        phi[i] = numerador / (1.0 + K[i] / constant);
        //printf("%f %f\n", (double)i / (double)N, phi[i]);
        }
        for (int j = 0; j < N + 1; j++) {
            autovectores[i][j] = phi[j];
        }
    }

    FILE *archivo = fopen("autovectores.dat", "w");
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < N + 1; j++) {
            fprintf(archivo, "%f", autovectores[i][j]);
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);

    remove("ejecutable.out");

    return 0;
}