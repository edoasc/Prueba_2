Compilar el programa con el makefile y luego realizar la ejecución con ./ejecutable.out

El programa entrega un gráfico de las trayectorias del misil y el proyectil respecto a la tierra. se observa que el proyectil intercepta al misil en la altura deseada pero en ascenso, lo cual suguiere que el lanzamiento del proyectil debe realizarte en un tiempo posterior al lanzamiento del misil.

Como comentario final, el método de la secante resulta demasiado sensible a los hiperparámetros, alterar el valor del tiempo inicial, la cantidad de iteraciones o el error deseado no hace mas que cambiar considerablemente el comportamiento de la trayectoria del proyectil. Los hiperparámetros puestos en el códigos son los que mejor se acomodan al resultado deseado.