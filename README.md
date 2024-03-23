LamatGen es un recurso planteado para Python y C++ que permite la generación de modelos 3D de elementos matemáticos parametrizables. Consiste de 3 clases principales:
1. lamat_numethod permite al usuario generar puntos paramétricos a partir de herramientas de cálculo numérico, actualmente únicamente permite resolver sistemas de ecuaciones diferenciales ordinarias a partir del método de Runge - Kutta de cuarto orden pero se plantea tener métodos que permitan una resolución a partir de problemas en la frontera o métodos de interpolación de puntos.
2. lamat_curve permite generar un archivo .obj de una curva paramétrica, este archivo se muestra en milímetros y se le permite al usuario seleccionar la resolución y tamaño de la vecindad tubular del archivo. Así mismo, está diseñado para que los archivos generados sean compatibles con laminadores como Prusa Slicer o Ultimaker Cura permitiendo al usuario no solo mantener un archivo renderizable sino traer las curvas generadas al mundo real.
3. lamat_surface es el siguiente paso a implementar con el objetivo de tratar el mismo sistema de lamat_curve pero tratando con superficies.