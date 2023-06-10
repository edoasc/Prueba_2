------> ¿A qué tipo de problema corresponde?

El sistema puede ser descrito de la siguiente manera:

θ1'' = f1(θ1, θ2, θ1', θ2')
θ2'' = f2(θ1, θ2, θ1', θ2'),

donde es posible escribirlo como un sistema de 4 ecuaciones diferenciales de primer orden. Para resolverlo es necesario disponer de 4 condiciones iniciales para theta1, theta1', theta2 y theta2', sin embargo podemos resolver theta1 como un problema PVI ya que conocemos las condiciones iniciales para dicha variable y podemos resolver theta2 como PVF ya que conocemos sus condiciones de borde. 

------> ¿Es posible resolver el problema con los métodos visto en clases? En caso de ser así ¿Qué método utilizaría y porqué?

Como se menciono en el punto previo si es posible la resolución del sistema. Para theta 1 aplicaría RK4 en un tramo de tiempo desde t=0 hasta t=3 para determinar theta1 y theta1' en todos los puntos del intervalo.

Para theta2 haria dos shooting no lineales de las variables theta2 y theta2', ya que no conozco sus valores iniciales pero si puedo determinar la función de error a minimizar aprovechando los valores dedos en t=3. Luego minimizaría el error por medio del método de la secante hasta obtener las condiciones iniciales propicias para aplicar PVI.

------> Si se aplicara diferencias finitas sobre la ecuación diferencial, ¿Es posible resolver el problema mediante métodos de resolución de sistemas lineales? (como Gauss-Jordan) ¿Porqué? 

No, ya que si bien se aproximan tanto la primera como la segunda derivada de theta por medio de diferencias finitas, los factores seno y coseno que acompañan a la ecuación mantendran la forma no lineal de la misma, además de esto, es imperiosamente necesario disponer de la solución analítica de theta1 y theta2 para realizar este proceso, por lo cual si solo se dispone de las ecuaciones diferenciales y se desea resolver de forma puramente numérica, no es posible aplicar diferencias finitas.

------> Si se utilizara el método de shooting no lineal, ¿Es posible resolver el problema mediante el método de Newton-Raphson (de una variable)?
¿Porqué?.

Si es posible. Al aplicar el método de shooting no lineal lo que se busca es realizar una busqueda de las raices de una ecuación no lineal para obtener los parametros iniciales que mejor se ajustan a las condiciones de borde dadas. Newton-Raphson es un método de busqueda de raíces para ecuaciones no lineales por lo que se ajusta al modelo de shooting no lineal.