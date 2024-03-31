#include "lamatgen.h"

using namespace std;

int main(){
  
  // Incluir un conjunto de puntos que al unirse dibujen una curva (en este caso el conjunto será la parametrización de una curva).

	double a = 2;

  vector<vector<double>> hipocicloide = {};
  
  for (double t = 0; t<=2*M_PI+0.01; t+=0.01){

    // Escribir aquí la parametrización.
    
    hipocicloide.push_back({2*a*(1-cos(t))*cos(t), 2*a*(1-cos(t))*sin(t), 0});
    
  }
 
  
  // Definir un valor de espesor de línea y el número de caras que queremos que tenga la malla.
  
  double espesor = 0.2;
  int n = 100;

  // Definir el nombre del archivo de salida.
  
  string archivo_salida = "cardioide";

  generar_curva(hipocicloide, espesor, n, archivo_salida);



  
}

