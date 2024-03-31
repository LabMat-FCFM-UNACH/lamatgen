#include "lamatgen.h"
using namespace std;

int main(){
  
  // Incluir un conjunto de puntos que al unirse dibujen una curva (en este caso el conjunto será la parametrización de una curva).

	double r = 1;
	double k = 4;

  vector<vector<double>> hipocicloide = {};
  
  for (double t = 0; t<=2*M_PI+0.01; t+=0.01){

    // Escribir aquí la parametrización.
    
    hipocicloide.push_back({(k-1)*r*cos(t)+r*cos(t*(1-k)), (k-1)*r*sin(t)+r*sin(t*(1-k)), 0});
    
  }
 
  
  // Definir un valor de espesor de línea y el número de caras que queremos que tenga la malla.
  
  double espesor = 0.7;
  int n = 100;

  // Definir el nombre del archivo de salida.
  
  string archivo_salida = "hipocicloide4";

  generar_curva(hipocicloide, espesor, n, archivo_salida);

  
}

