#include "lamatgen.h"

using namespace std;

int main(){
  
  // Incluir un conjunto de puntos que al unirse dibujen una curva (en este caso el conjunto será la parametrización de una curva).

  vector<vector<double>> parabola = {};
  
  for (double t = -2; t<=2; t+=0.0001){

    // Escribir aquí la parametrización.
    
    parabola.push_back({t*t, t, 0});
    
  }
  
	vector<vector<double>> espiral = {};
  
  for (double t = 0; t<=6*M_PI; t+=0.01){

    // Escribir aquí la parametrización.
    
    espiral.push_back({pow(M_E, -0.35*t)*cos(t), pow(M_E, -0.35*t)*sin(t), 0});
    
  }
  
  vector<vector<double>> coseno = {};
  
  for (double t = -2*M_PI; t<=2*M_PI; t+=0.01){

    // Escribir aquí la parametrización.
    
    coseno.push_back({cos(t), t, 0});
    
  }
  
  vector<vector<double>> circulo = {};
  
  for (double t = 0; t<=2*M_PI+0.01; t+=0.01){

    // Escribir aquí la parametrización.
    
    circulo.push_back({cos(t), sin(t), 0});
    
  }
  
  // Definir un valor de espesor de línea y el número de caras que queremos que tenga la malla.
  
  double espesor = 0.2;
  int n = 100;

  // Definir el nombre del archivo de salida.
  
  string archivo_salida_parabola = "parabola";
  string archivo_salida_espiral = "espiral";
  string archivo_salida_coseno = "coseno";
  string archivo_salida_circulo = "circulo";

  generar_curva(parabola, espesor, n, archivo_salida_parabola);
  generar_curva(espiral, espesor, n, archivo_salida_espiral);
  generar_curva(coseno, espesor, n, archivo_salida_coseno);
  generar_curva(circulo, espesor, n, archivo_salida_circulo);


  
}

