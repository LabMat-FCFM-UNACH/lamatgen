#include "lamatgen.h"
using namespace std;

int main(){
  
  // Incluir un conjunto de puntos que al unirse dibujen una curva (en este caso el conjunto será la parametrización de una curva).

  vector<vector<double>> cuadrado = {};
  
  for (double t = 0.5; t<=1; t+=0.01){

    // Escribir aquí la parametrización.
    
    cuadrado.push_back({t, 0, 0});
    
  }

  
  for (double t = 0.01; t<=1; t+=0.01){

    // Escribir aquí la parametrización.
    
    cuadrado.push_back({1, t, 0});
    
  }
  
  
  for (double t = 0.01; t<=1; t+=0.01){

    // Escribir aquí la parametrización.
    
    cuadrado.push_back({1-t, 1, 0});
    
  }
  
  
  for (double t = 0.01; t<=1; t+=0.01){

    // Escribir aquí la parametrización.
    
    cuadrado.push_back({0, 1-t, 0});
    
  }
  
  for (double t = 0.01; t<=0.501; t+=0.01){

    // Escribir aquí la parametrización.
    
    cuadrado.push_back({t, 0, 0});
    
  }
  
  // Definir un valor de espesor de línea y el número de caras que queremos que tenga la malla.
  
  double espesor = 0.1;
  int n = 100;

  // Definir el nombre del archivo de salida.
  
  string archivo_salida = "cuadrado";


  generar_curva(cuadrado, espesor, n, archivo_salida);



  
}

