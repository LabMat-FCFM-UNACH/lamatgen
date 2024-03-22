/*
 * Copyright (c) 2024 Daniel Alberto García Sánchez y Carlos Fabian Ramirez Alvarez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LAMATGEN_H
#define LAMATGEN_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <algorithm> 

using namespace std;

string progressBar(int percentage) {
    // Asegurarse de que el porcentaje está en el rango de 0 a 100
    percentage = min(max(percentage, 0), 100);

    // Determinar cuántos caracteres de la barra deben estar llenos
    int numFilledChars = percentage / 2; // Suponiendo una barra de 20 caracteres de ancho para representar del 0% al 100%

    // Construir la barra de porcentaje
    string bar;
    bar += "\033[36m[";
    for (int i = 0; i < 50; ++i) {
        if (i < numFilledChars)
            bar += "■";
        else
            bar += " ";
    }
    bar += "] " + to_string(percentage) + "%\033[0m";
    
    return bar;
}


class lamatgen_rk4{
public:

vector<vector<double>> solve(vector<double> (*f)(double, vector<double>), vector<double> t_span, vector<double> y0, double h) {
    cout << "\033[1m\033[44m\033[33mSe ha agregado un conjunto de puntos de un sistema de ecuaciones diferenciales. Resolviendo por el método de Runge-Kutta de orden 4...\033[0m" << endl;

    double t0 = t_span[0];
    double tf = t_span[1];
    
    int n_steps = ((tf - t0) / h) + 1;
    
    cout << "\033[1m\033[32mGenerando solución con \033[35m" << n_steps << "\033[32m puntos.\n";

    vector<vector<double>> y_val(n_steps, vector<double>(y0.size(), 0.0));
    vector<double> yk2(y0.size(), 0.0), yk3(y0.size(), 0.0), yk4(y0.size(), 0.0);
    
    y_val[0] = y0;

    double half_h = 0.5 * h;
    double sixth_h = h / 6.0;

    for (int i = 1; i < n_steps; ++i) {
        double t_n = t0 + h * (i - 1);
        vector<double> y_n = y_val[i - 1];

        vector<double> k1 = f(t_n, y_n);

        for (int j = 0; j < y0.size(); ++j) {
            yk2[j] = y_n[j] + half_h * k1[j];
        }
        vector<double> k2 = f(t_n + half_h, yk2);

        for (int j = 0; j < y0.size(); ++j) {
            yk3[j] = y_n[j] + half_h * k2[j];
        }
        vector<double> k3 = f(t_n + half_h, yk3);

        for (int j = 0; j < y0.size(); ++j) {
            yk4[j] = y_n[j] + h * k3[j];
        }
        vector<double> k4 = f(t_n + h, yk4);

        for (int j = 0; j < y0.size(); ++j) {
            y_val[i][j] = y_n[j] + sixth_h * (k1[j] + 2.0 * k2[j] + 2.0 * k3[j] + k4[j]);
        }
        
        if (i % 10 == 0) {
            cout << "\033[34m(" << i << "/" << n_steps << ") "<< progressBar(round((i*100)/n_steps)) << endl;
            cout << "\033[A" << "\033[K";
        }
    }
    cout << "\033[A" << "\033[K";
    cout << "\033[32m [ Solución \033[0m\033[35m(" << n_steps << " puntos)\033[0m\033[32m generada ]\033[0m \n" << endl;

    return y_val;
}
};

bool puntos_consecutivos(int puntos, bool op) {
    cout << "\n\033[31m\033[1mAdvertencia:\033[0m\033[33mSe han detectado un mínimo de \033[1m\033[35m" << puntos << " \033[0m\033[33mpuntos identicos consecutivos. ¿Seguir detectando puntos consecutivos? (Y/n). ";

    char res;
    while (true) {
        cin >> res;
        if (res == 'n' || res == 'N') {
            op = false;
            cout << "\033[31m [ Detección de puntos consecutivos apagada ]\033[0m \n" << endl;
            break;
        } else if (res == 'y' || res == 'Y') {
            op = true;
            cout << "\033[32m Continuando... \033[0m \n" << endl;
            break;
        } else {
            cout << "\033[1m\033[31m Respuesta inválida.\n \033[0m" << endl;
        }
    }
    return op;
}

void advertencia_punto_fijo(int p1_i, int p2_i, vector<double> p1, vector<double> p2) {
    static int respuesta_positiva_count = 0;
    static int puntos_consecutivos_count = 0;
    static bool selec_op = true;
    static vector<double> guarda_vec;

    if (p1 == p2) {
        if (guarda_vec != p1) {
            respuesta_positiva_count = 0;
        }
        if (respuesta_positiva_count < 3) {
            selec_op = true;

            cout << "\033[31m\033[1mAdvertencia:\033[0m\033[33m Los puntos número\033[35m\033[1m " << p1_i << "\033[0m\033[33m y \033[1m\033[35m" << p2_i << "\033[0m\033[33m se repiten de forma consecutiva. Esto puede causar discontinuidades en el modelo generado. ¿Continuar? (Y/n): ";

            char respuesta;
            while (true) {
                cin >> respuesta;
                if (respuesta == 'n' || respuesta == 'N') {
                    cout << "\033[31mAbortando...\033[0m \n" << endl;
                    exit(0);
                } else if (respuesta == 'y' || respuesta == 'Y') {
                    respuesta_positiva_count++;
                    puntos_consecutivos_count++;
                    if (respuesta_positiva_count >= 3) {
                        selec_op = puntos_consecutivos(puntos_consecutivos_count, selec_op);
                        return;
                    }
                    guarda_vec = p1;
                    cout << "\033[32m Continuando... \033[0m \n" << endl;
                    break;
                } else {
                    cout << "\033[1m\033[31m Respuesta inválida.\n \033[0m" << endl;
                }
            }
        } else if (selec_op == true) {
            puntos_consecutivos_count++;
            selec_op = puntos_consecutivos(puntos_consecutivos_count, selec_op);
            guarda_vec = p1;
            return;
        }
    }
}

void advertencia_exceso(long int triangulos) {
    if (triangulos >= 1000000) {
        cout << "\033[31m\033[1mAdvertencia: \033[0m\033[33m Los parámetros ingresados generan un modelo de más de 1M de triángulos, aunque posible de procesar, en equipos de bajos recursos puede causar problemas de renderizado o visualización del objeto, ¿Continuar? (Y/n): ";

        char respuesta;
        while (true) {
            cin >> respuesta;
            if (respuesta == 'n' || respuesta == 'N') {
                cout << "\033[31mConsejo: Una forma de reducir la cantidad de triángulos generados es reducir el número de puntos o la resolución del objeto\nAbortando...\033[0m \n" << endl;
                exit(0);
            } else if (respuesta == 'y' || respuesta == 'Y') {
                cout << "\033[32m Continuando... \033[0m \n" << endl;
                break;
            } else {
                cout << "\033[1m\033[31m Respuesta inválida.\n \033[0m" << endl;
            }
        }
    }
}

double producto_punto(vector<double> a, vector<double> b){
   double producto = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];

  return producto;

}

vector<double> producto_cruz(vector<double> u, vector<double> v){

  double i = u[1]*v[2]-u[2]*v[1];
  double j = u[2]*v[0]-u[0]*v[2];
  double k = u[0]*v[1]-u[1]*v[0];

  double norma = sqrt(i*i+j*j+k*k);

  vector<double> w = {i/norma,j/norma,k/norma};
  return w;

}



vector<vector<double>> generador_planos(vector<double> p1, vector<double> p2){
	cout << "\033[1m\033[44m\033[33mGenerando base inicial del modelo...\033[0m" << endl; 

  double x =0.0, y=0.0 ,z=0.0;

  if (p1[2] != p2[2]){
    x = 1;
    y = 1;
    z = ((p2[0]-p1[0])*(x-p1[0])+(p2[1]-p1[1])*(y-p1[1])-p1[2]*(p2[2]-p1[2]))/(p1[2]-p2[2]);
  } else if (p1[1] != p2[1]){
    x = 1;
    z = 1;
    y = ((p2[0]-p1[0])*(x-p1[0])+(p2[2]-p1[2])*(z-p1[2])-p1[1]*(p2[1]-p1[1]))/(p1[1]-p2[1]);
  } else if (p1[0] != p2[0]){
    z = 1;
    y = 1;
    x = ((p2[1]-p1[1])*(y-p1[1])+(p2[2]-p1[2])*(z-p1[2])-p1[0]*(p2[0]-p1[0]))/(p1[0]-p2[0]);
  }
  


  vector<double> base1 = {x-p1[0], y-p1[1], z-p1[2]};


  double norma_base1 = sqrt(pow(base1[0], 2) + pow(base1[1], 2) + pow(base1[2], 2));


  base1 = {base1[0]/norma_base1, base1[1]/norma_base1, base1[2]/norma_base1};

  vector<double> eje = {p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]};
  vector<double> base2 = producto_cruz(eje, base1);
	
	cout << "\033[32m [ Base inicial generada ] \033[0m \n" << endl;
		
  return {base1, base2};
 }


vector<vector<vector<double>>> generar_puntos(vector<double> base1, vector<double> base2, double radio, int n_puntos, vector<double> pinicial, vector<double> pfinal){

  vector<vector<vector<double>>> vectores_alrededor = {};

  for (int i = 0; i < n_puntos; i++){

    vector<vector<double>> tapas = {};

    vector<double> punto_alrededor = { radio*(base1[0] * cos((2*i* M_PI)/n_puntos) + base2[0] * sin((2*i* M_PI)/n_puntos)),radio*( base1[1] * cos((2*i* M_PI)/n_puntos) + base2[1] * sin((2*i* M_PI)/n_puntos)), radio*(base1[2] * cos((2*i* M_PI)/n_puntos) + base2[2] * sin((2*i* M_PI)/n_puntos))};

    vector<double> tapa1 = {punto_alrededor[0]+pinicial[0], punto_alrededor[1]+pinicial[1], punto_alrededor[2]+pinicial[2]};

    tapas.push_back(tapa1);

     vector<double> tapa2 = {punto_alrededor[0]+pfinal[0], punto_alrededor[1]+pfinal[1], punto_alrededor[2]+pfinal[2]};

    tapas.push_back(tapa2);
    vectores_alrededor.push_back(tapas);
  }

  return vectores_alrededor;
}

vector<vector<double>> base_rotada(vector<double> base, vector<double> p1, vector<double> p2){

  vector<double> eje = {p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]};

  double norma_eje = sqrt(pow(eje[0], 2) + pow(eje[1], 2) + pow(eje[2], 2));

  eje = {eje[0]/norma_eje, eje[1]/norma_eje, eje[2]/norma_eje};

  // Aquí comenzamos la revisión del ángulo entre los dos vectores.

  double angulo_base1 =  M_PI/2 - acos(producto_punto(base, eje));

  vector<double> eje_rotacion1 = producto_cruz(eje, base);

  double norma_rotacion = sqrt(pow(eje_rotacion1[0], 2) + pow(eje_rotacion1[1], 2) + pow(eje_rotacion1[2], 2));

  eje_rotacion1 = {eje_rotacion1[0]/norma_rotacion, eje_rotacion1[1]/norma_rotacion, eje_rotacion1[2]/norma_rotacion};

  vector<double> vector_rotado = {
  base[0]*cos(angulo_base1) + (eje_rotacion1[0]*eje_rotacion1[0]*base[0] + eje_rotacion1[0]*eje_rotacion1[1]*base[1] + eje_rotacion1[0]*eje_rotacion1[2]*base[2])*(1-cos(angulo_base1)) + (eje_rotacion1[1]*base[2]-eje_rotacion1[2]*base[1])*sin(angulo_base1), base[1]*cos(angulo_base1) + (eje_rotacion1[1]*eje_rotacion1[0]*base[0] + eje_rotacion1[1]*eje_rotacion1[1]*base[1] + eje_rotacion1[1]*eje_rotacion1[2]*base[2])*(1-cos(angulo_base1)) + (eje_rotacion1[2]*base[0]-eje_rotacion1[0]*base[2])*sin(angulo_base1), base[2]*cos(angulo_base1) + (eje_rotacion1[2]*eje_rotacion1[0]*base[0] + eje_rotacion1[2]*eje_rotacion1[1]*base[1] + eje_rotacion1[2]*eje_rotacion1[2]*base[2])*(1-cos(angulo_base1)) + (eje_rotacion1[0]*base[1]-eje_rotacion1[1]*base[0])*sin(angulo_base1)
  };

  double norma_rotada = sqrt(pow(vector_rotado[0], 2) + pow(vector_rotado[1], 2) + pow(vector_rotado[2], 2));

  vector<double> base1 = {vector_rotado[0]/norma_rotada, vector_rotado[1]/norma_rotada, vector_rotado[2]/norma_rotada};

  vector<double> base2 = producto_cruz(eje, vector_rotado);

  return {base1, base2};
}

void generar_curva( vector<vector<double>> parametrizacion, double espesor, int n_puntos, string nombre_del_archivo ){
	int total_vertices = parametrizacion.size()*(n_puntos + 1);
	long int total_triangulos = n_puntos*2*parametrizacion.size();

	advertencia_exceso(total_triangulos);
	
	nombre_del_archivo+=".obj";
	
	cout << "\033[1m\033[32mIniciando renderizado del modelo: \033[0m\033[35m\033[1m\033[4m" << nombre_del_archivo << "\033[0m\n" << endl;
	
  ofstream outFile(nombre_del_archivo);

  double radio_del_tubo = espesor/2;

	advertencia_punto_fijo(0, 1, parametrizacion[0], parametrizacion[1]);
  vector<vector<double>> plano = generador_planos(parametrizacion[0], parametrizacion[1]);
	
   vector<vector<vector<double>>> puntos_circulares = generar_puntos(plano[0], plano[1], radio_del_tubo, n_puntos, parametrizacion[0], parametrizacion[1]);

	cout << "\033[1m\033[44m\033[33mGenerando vértices de la curva...\033[0m" << endl;

  for (int k = 0; k < 2; k++){

    outFile << "v " << parametrizacion[k][0] << " " << parametrizacion[k][1] << " " << parametrizacion[k][2] << endl;

    for (int i = 0; i < n_puntos; i++){

     outFile << "v " << puntos_circulares[i][k][0] << " " << puntos_circulares[i][k][1] << " " << puntos_circulares[i][k][2] << endl;

    }
  }
  
  int vertices_iniciales = 2+2*n_puntos;
  
  cout << "\033[34m(" << vertices_iniciales << "/" << total_vertices << ") " << progressBar(round((vertices_iniciales*100)/total_vertices)) << endl; 

	// Esto se tiene que arreglar, debe ser posible generar una cantidad de vectores siempre qe el coso sea mayor a 1.
	
	if ( parametrizacion.size() != 2 ){
  vector<double> base_rotacion = plano[0];

  for (int j = 1; j<parametrizacion.size()-1; j++){
  	advertencia_punto_fijo(j, j+1, parametrizacion[j], parametrizacion[j+1]);

    vector<vector<double>> planos = base_rotada(base_rotacion, parametrizacion[j], parametrizacion[j+1]);

    puntos_circulares = generar_puntos(planos[0], planos[1], radio_del_tubo, n_puntos, parametrizacion[j], parametrizacion[j+1]);

    base_rotacion = planos[0];

    outFile << "v " << parametrizacion[j+1][0] << " " << parametrizacion[j+1][1] << " " << parametrizacion[j+1][2] << endl;

    for (int i = 0; i < n_puntos; i++){

       outFile << "v " << puntos_circulares[i][1][0] << " " << puntos_circulares[i][1][1] << " " << puntos_circulares[i][1][2] << endl;

    }
	cout << "\033[A" << "\033[K";
	int vertices_gen = vertices_iniciales + (j+2)*n_puntos + 1;
	cout << "\033[34m(" << vertices_gen << "/" << total_vertices << ") " << progressBar(round((vertices_gen*100)/total_vertices)) << endl; 
  }
  
  }
  
  cout << "\033[A" << "\033[K";

	cout << "\033[32m [ Vértices generados ] \033[0m \n" << endl;
	
	cout << "\033[1m\033[44m\033[33mGenerando triángulos del modelo...\033[0m" << endl;
	
	
  // Esta función genera las tapas, supongo que podría usar un condicional más adelante.

  for (int i = 0; i < n_puntos; i++){
  
     if (i != n_puntos-1){
       outFile << "f " << 1 << " " << i+3 << " " << i+2 << endl;
     } 
     else {
       outFile << "f " << 1 << " " << 2 << " " << n_puntos+1 << endl;
     }
     cout << "\033[34m(" << i+1 << "/" << total_triangulos << ") "<< progressBar(round((i*100)/total_triangulos)) << endl;
     cout << "\033[A" << "\033[K"; 
  }
  	cout << endl;
  	cout << "\033[A" << "\033[K";
  	
  	cout << "\033[32m [ Primera tapa generada ] \033[0m" << endl;
  	
  
  // Aquí hacemos una función que genera la frontera del cilindro.

  for (int k = 0; k<parametrizacion.size()-1; k++){

    for( int i=0; i<n_puntos; i++){

      if (i != n_puntos-1){

        outFile << "f " << i+3 + (k+1)*(n_puntos+1) << " " << i+2 + (k+1)*(n_puntos+1) << " " <<  i+2 + k*(n_puntos+1) << endl;

        outFile << "f " << i+2 + k*(n_puntos+1) << " " <<  i+3 + k*(n_puntos+1) << " " <<   i+3 + (k+1)*(n_puntos+1)<< endl;

      }

      else {

        outFile << "f " << n_puntos+1 + (k+1)*(n_puntos + 1) << " " << n_puntos+1 + (k)*(n_puntos + 1)  << " " <<  2+k*(n_puntos+1) << endl;

        outFile << "f " <<  2+k*(n_puntos+1)  << " " <<   n_puntos+3+k*(n_puntos+1)<< " " <<   2*(n_puntos+1)  +k*(n_puntos+1)<< endl;

      }
  }
  cout << "\033[34m(" << n_puntos*(2*k+3) << "/" << total_triangulos << ") "<< progressBar(round(((n_puntos*(2*k+3))*100)/total_triangulos)) << endl;
     cout << "\033[A" << "\033[K";
  }
  
  	cout << endl;
  	cout << "\033[A" << "\033[K";

	cout << "\033[32m [ Frontera generada ] \033[0m" << endl;

  for (int i = 0; i < n_puntos; i++){
     if (i != n_puntos-1){
       outFile << "f " << i+2 + (parametrizacion.size()-1)*(n_puntos+1) << " " << i+3 + (parametrizacion.size()-1)*(n_puntos+1) << " " << 1 + (parametrizacion.size()-1)*(n_puntos + 1) << endl;
     } 
     else {
       outFile << "f " << n_puntos+1 + (parametrizacion.size()-1)*(n_puntos + 1) << " " << 2 + (parametrizacion.size()-1)*(n_puntos + 1) << " " << 1 + (parametrizacion.size()-1)*(n_puntos + 1) << endl;
     }
     cout << "\033[34m(" << n_puntos*(2*parametrizacion.size()+1) + i + 1 << "/" << total_triangulos << " ] "<< progressBar(round(((n_puntos*(2*parametrizacion.size()+1) + i + 1)*100)/total_triangulos)) << "%" << endl;
     cout << "\033[A" << "\033[K";
  }
  
	cout << endl;
  cout << "\033[A" << "\033[K";

	cout << "\033[32m [ Segunda tapa generada ] \033[0m\n" << endl;
	
	cout << "\033[1m\033[32m Curva \033[35m\033[4m" << nombre_del_archivo << "\033[0m\033[1m\033[32m generada exitosamente." << endl;
	
}

#endif
