#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

int funcion(double t, const double y[], double f[], void *params){
    (void)(t);
    double *p = (double *)params; 
    double G = p[0];
    double MT = p[1];
    f[0] = y[1];
    f[1] = y[0] * pow(y[3], 2) - (G * MT) / pow(y[0], 2);
    f[2] = y[3];
    f[3] = - 2 * y[1] * y[3] / y[0];
    return GSL_SUCCESS;
}

int solucion_pvi(int N, double *t, double h, gsl_odeiv2_driver *d, double *Y){
    for(int i = 1; i <= N; i++){
        double ti = *t + h;
        int status = gsl_odeiv2_driver_apply (d, t, ti, Y);
        if (status != GSL_SUCCESS){
        printf ("error, return value=%d\n", status);
        break;
      }
    }
    return 0;
}

int main(){

    double H = 1072e3;
    double G = 6.674e-11;
    double MT = 5.972e24;
    double RT = 6371e3;
    double v0 = 7300;
    double phi = 15 * M_PI / 180;
    double params[2] = {G, MT};

    // Condiciones iniciales para el misil
    double y[4] = {RT+H, v0 * sin(phi), 0, v0 * cos(phi) / (RT + H)};

    // Calculamos el tiempo para el cual el misil se encuentra a 2000 km sobre la tierra, mismo que
    // coincide con el tiempo en que el proyectil impacta al misil

    double t0 = 0;
    double tf = 86400;
    double h = 1;
    int N = (tf - t0) / h;

    gsl_odeiv2_system sys = {funcion, NULL, 4, &params};
    gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk4, h, 1, 0.0);

    double t = 0.0;
    double t_impacto = 0.0;
    double radio_impacto = 0.0;
    //double theta_impacto = 0.0;
    double ti = 0;

    FILE *archivo = fopen("datos.dat", "w");
    for (int i = 1; i <= N; i++){
        fprintf(archivo, "%.6f %.6f %.6f %.6f %.6f\n", ti, y[0], y[1], y[2], y[3]);
        ti = t + h;
        int status = gsl_odeiv2_driver_apply (d, &t, ti, y);
        if (status != GSL_SUCCESS){
            printf ("error, return value=%d\n", status);
            break;
        }
        //2000e3 + RT = 8371e3, se acepta un error maximo de 10 kilometros
        if (fabs(y[0] - 2000e3 - RT) <= 10000 && y[1] < 0){ 
            t_impacto = ti;
            radio_impacto = y[0];
            //theta_impacto = y[2]; 
            break;
        }
    }
    fclose(archivo);
    gsl_odeiv2_driver_free (d);

    double v0_p = 2400; // Velocidad inicial del proyectil, se elige de forma arbitraria para resolver con PVF
    double z[4] = {RT, 0, M_PI, v0_p * cos(0) / RT}; // Condiciones iniciales para el proyectil, z[3] es el shooting inicial
    double t_0 = 0.0; // Variable auxiliar de tiempo por si se quiere cambiar el instante de lanzamiento del proyectil
    N = (t_impacto - t_0) / h;

    gsl_odeiv2_system sys2 = {funcion, NULL, 4, &params};
    gsl_odeiv2_driver * d2 = gsl_odeiv2_driver_alloc_y_new (&sys2, gsl_odeiv2_step_rk4, h, 1, 0.0);

    double tol = 1e-9;
    double error = tol + 1;
    double z_0 = 0;
    double z_1 = (2300 * cos(0)) / RT;
    double z_2 = 0;

    // Primera ejecucion
    t = 0.0;
    z_0 = z[3];
    solucion_pvi(N, &t, h, d2, z);
    double error_0 = fabs(z[0] - radio_impacto);

    int max_iter = 200;
    int i = 0;
    while (error > tol && i < max_iter){
        t = 0.0;
        z[0] = RT;
        z[1] = 0;
        z[2] = M_PI;
        z[3] = z_1;
        solucion_pvi(N, &t, h, d2, z);
        //printf("%.6f %.6f\n", z[3], z_1);
        error = fabs(z[0] - radio_impacto);
        z_2 = z_1 - (z_1 - z_0)/(error - error_0) * error;
        z_0 = z_1;
        z_1 = z_2;
        error_0 = error;
        ++i;
    }

    z[0] = RT;
    z[1] = 0;
    z[2] = M_PI;
    z[3] = z_1;
    t = 0.0;
    ti = 0;

    FILE *archivo2 = fopen("datos2.dat", "w");
    for (int i = 1; i <= N; i++){
        fprintf(archivo2, "%.6f %.6f %.6f %.6f %.6f\n", ti, z[0], z[1], z[2], z[3]);
        ti = t + h;
        int status = gsl_odeiv2_driver_apply (d2, &t, ti, z);
        if (status != GSL_SUCCESS){
            printf ("error, return value=%d\n", status);
            break;
        }
        if (fabs(z[0] - radio_impacto) < 1000){
            break;
        }
    }
    fclose(archivo);
    gsl_odeiv2_driver_free (d2);

    FILE* gnuplot = popen("gnuplot -persist", "w");
    fprintf(gnuplot, "set terminal pngcairo enhanced size 1024, 1024\n");
    fprintf(gnuplot, "set output 'grafico.png'\n");
    fprintf(gnuplot, "set style data lines\n");
    fprintf(gnuplot, "set xlabel 'Eje x [m]'\n");
    fprintf(gnuplot, "set ylabel 'Eje y [m]'\n");
    fprintf(gnuplot, "set polar\n");
    fprintf(gnuplot, "set grid polar\n");
    fprintf(gnuplot, "plot 'datos.dat' using 4:2 ti 'Trayectoria Misil', 'datos2.dat' using 4:2 ti 'Trayectoria Proyectil', 6371000 ti 'Tierra'\n");
    pclose(gnuplot);

    remove("datos.dat");
    remove("datos2.dat");
    remove("ejecutable.out");
}