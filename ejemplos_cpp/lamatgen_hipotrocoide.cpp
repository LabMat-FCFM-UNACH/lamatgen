#include "lamatgen.h"

int main(){
  
  // Incluir un conjunto de puntos que al unirse dibujen una curva (en este caso el conjunto será la parametrización de una curva).

	double R = 16;
	double r = 1;
	double d = 2;

  vector<vector<double>> hipocicloide = {};
  
   for (double t = 0; t<=10*M_PI+0.01; t+=0.01){

    // Escribir aquí la parametrización.
    
    hipocicloide.push_back({0, (R-r)*cos(t)+d*cos(((R-r)/r)*t), (R-r)*sin(t)-d*sin(((R-r)/r)*t)});
    
  }
 
  
  // Definir un valor de espesor de línea y el número de caras que queremos que tenga la malla.
  
  double espesor = 1;
  int n = 50;

  // Definir el nombre del archivo de salida.
  
  string archivo_salida = "hipotrocoide";

  generar_curva(hipocicloide, espesor, n, archivo_salida);



  
}

