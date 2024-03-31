#include "lamatgen.h"

int main(){
  
  // Incluir un conjunto de puntos que al unirse dibujen una curva (en este caso el conjunto será la parametrización de una curva).

	double kx = 3;
	double ky = 2;

  vector<vector<double>> hipocicloide = {};
  
   for (double t = 0; t<=2*M_PI+0.1; t+=0.01){

    // Escribir aquí la parametrización.
    
    hipocicloide.push_back({cos(kx*t), 2*sin(ky*t), 0});
    
  }
 
  
  // Definir un valor de espesor de línea y el número de caras que queremos que tenga la malla.
  
  double espesor = 0.5;
  int n = 50;

  // Definir el nombre del archivo de salida.
  
  string archivo_salida = "lissajous";

  generar_curva(hipocicloide, espesor, n, archivo_salida);

	return 0;

  
}

