
# LamaGen v1.0

LamatGen es un recurso planteado para C++ que permite la generación de modelos 3D de elementos matemáticos parametrizables. Consiste de 3 clases principales:

 *  **lamat_numethod**: permite al usuario generar puntos paramétricos a partir de herramientas de cálculo numérico, actualmente únicamente permite resolver sistemas de ecuaciones diferenciales ordinarias a partir del método de Runge - Kutta de cuarto orden pero se plantea tener métodos que permitan una resolución a partir de problemas en la frontera o métodos de interpolación de puntos.
 *  **lamat_curve**: permite generar un archivo .obj de una curva paramétrica, este archivo se muestra en milímetros y se le permite al usuario seleccionar la resolución y tamaño de la vecindad tubular del archivo. Así mismo, está diseñado para que los archivos generados sean compatibles con laminadores como Prusa Slicer o Ultimaker Cura permitiendo al usuario no solo mantener un archivo renderizable sino traer las curvas generadas al mundo real.
 *  **lamat_surface**: es el siguiente paso a implementar con el objetivo de tratar el mismo sistema de lamat_curve pero tratando con superficies.

### ¿Puedo vender o generar algún ingreso de los modelos generados por el programa?
Sí, este es un trabajo realizado que tiene como objetivo facilitar el trabajo de las personas que trabajen con recursos en matemáticas independientemente de su situación económica o grado académico por lo que está registrado frente a la licencia GPL_3.0 lo que lo coloca como código abierto, es gratuito para quien quiera utilizarlo pero el producto de utilizar el programa puede ser comercializado sin problema, sólo te pedimos que pases la voz en cuanto al programa para que más gente lo conozca.

### Tengo alguna sugerencia sobre el programa.
Siempre hay más formas de hacer un trabajo, si tienes alguna sugerencia comunícate con nosotros a labmat.fcfmunach@gmail.com, también puedes comunicar dudas o inquietudes que tengas sobre el programa.

### ¿Existe documentación del programa?

Sí, puedes consultar la documentación de LamatGen así como distintas actualizaciones en nuestro [sitio web](https://labmatunach.net/).
