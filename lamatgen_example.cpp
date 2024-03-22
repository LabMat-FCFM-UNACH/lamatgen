#include "lamatgen.h"

using namespace std;

int main(){
  
  // Incluir un conjunto de puntos que al unirse dibujen una curva (en este caso el conjunto será la parametrización de una curva).

  vector<vector<double>> parabola = {};
  
  for (double t = -2; t<=2; t+=0.01){

    // Escribir aquí la parametrización.
    
    parabola.push_back({t*t, t, 0});
    
  }

  
  // Definir un valor de espesor de línea y el número de caras que queremos que tenga la malla.
  
  double espesor = 0.2;
  int n = 100;

  // Definir el nombre del archivo de salida.
  
  string archivo_salida_parabola = "parabola";


  generar_curva(parabola, espesor, n, archivo_salida_parabola);


  
}

