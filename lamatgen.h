/*
 * Copyright (c) 2024 Daniel Alberto García Sánchez 
 * Carlos Fabian Ramirez Alvarez
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

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <algorithm> 
#include <random>

using namespace std;

string progressBar(long percent) {
    percent = min(max(percent, 0L), 100L);
    int fill = percent / 2; // 50 divisiones de la barra

    string bar = "\033[36m[";
    for (int i = 0; i < 50; ++i) {
        bar += (i < fill) ? "■" : " ";
    }
    bar += "] " + to_string(percent) + "%\033[0m";
    return bar;
}

void updateProgress(int current, int total) {
    long percent = round((current * 100.0) / total);
    cout << "\033[A\033[K"; // Borra la línea anterior
    cout << "\033[34m(" << current << "/" << total << ") " << progressBar(percent) << "\n";
}

void writeVertices(ofstream& outFile, const vector<double>& punto, const vector<vector<vector<double>>>& puntos_circulares, int k, int n_puntos) {
    outFile << "v " << punto[0] << " " << punto[1] << " " << punto[2] << "\n";
    for (int i = 0; i < n_puntos; ++i) {
        outFile << "v " << puntos_circulares[i][k][0] << " " << puntos_circulares[i][k][1] << " " << puntos_circulares[i][k][2] << "\n";
    }
}

void generarTapa(ofstream& outFile, int n_puntos, int base_index, int apex_index, int& progress_counter, int total_triangulos) {
    for (int i = 0; i < n_puntos; i++) {
        if (i != n_puntos - 1) {
            outFile << "f " << apex_index << " " << base_index + i + 1 << " " << base_index + i + 2 << "\n";
        } else {
            outFile << "f " << apex_index << " " << base_index + 1 << " " << base_index + n_puntos << "\n";
        }
        updateProgress(++progress_counter, total_triangulos);
    }
}

void generarFrontera(ofstream& outFile, int n_puntos, int layers, int& progress_counter, int total_triangulos) {
    for (int k = 0; k < layers - 1; k++) {
        for (int i = 0; i < n_puntos; i++) {
          int palante = (k+1)*(n_puntos+1);
          int patras = k*(n_puntos+1);

            if (i != n_puntos - 1) {
                outFile << "f " << i + 3 + palante << " " << i + 2 + palante << " " <<  i + 2 + patras << "\n ";
                outFile << "f " << i + 2 + patras << " " <<  i + 3 + patras << " " <<   i + 3 + palante << "\n ";
            } else {
                outFile << "f " << n_puntos + 1 + palante << " " << n_puntos + 1 + patras  << " " <<  2 + patras << "\n ";
                outFile << "f " <<  2 + patras  << " " <<   n_puntos + 3 + patras<< " " <<   2*(n_puntos+1) + patras<< "\n ";
            }
        }
        // Actualización del progreso
        progress_counter += 2 * n_puntos;
        updateProgress(progress_counter, total_triangulos);
    }
}

// Esta clase agrupa las funciones con herramientas matemáticas útiles que se usan alrededor del programa.
class lamatools {
private:
    
    random_device rd;
    mt19937 gen;

public:
    lamatools() : gen(rd()) {}
    
    template <typename T>
    double producto_punto(const vector<T>& u, const vector<T>& v) {
      
      if (u.size() != v.size()) {
        cerr << "\033[31m\033[1mError:\033[0m\033[33m Los elementos del producto punto deben tener la misma cantidad de elementos.\033[35m\033[1m" << "\n";
        exit(0);
      }
    
      double punto = 0; // Inicializar el resultado del producto punto
      
      for (size_t i = 0; i < u.size(); i++) {
        punto += u[i] * v[i]; // Sumar el producto de los elementos
      }
      
      return punto; // Retornar el resultado del producto punto
}

    template <typename T>
    double norma(const vector<T>& u) {
      
      return sqrt(producto_punto(u, u));
      
    }

// Función de producto cruzado
  template <typename T>
  vector<double> producto_cruz(const vector<T>& u, const vector<T>& v, bool norm = false) {
    if (u.size() != 3 || v.size() != 3) {
        cerr << "\033[31m\033[1mError:\033[0m\033[33m Las dimensiones de los vectores deben ser 3 para el producto cruzado.\033[35m\033[1m" << "\n";
        exit(0);
    }
    
    // Calcular el producto cruzado
    T i = u[1] * v[2] - u[2] * v[1];
    T j = u[2] * v[0] - u[0] * v[2];
    T k = u[0] * v[1] - u[1] * v[0];

    vector<double> w = {i*1.0, j*1.0, k*1.0};

    // Si se solicita normalización
    if (norm) {
        double norma_ = norma(w); // Calcular la norma
        return {w[0] / norma_, w[1] / norma_, w[2] / norma_}; // Retornar el vector normalizado
    } else {
        return w; // Retornar el vector no normalizado
    }
}

  // Función para rotar un vector a través de la fórmula de rotación de Rodrigues
  // TODO
  // Revisar una manera de que la función de rotación se adapte de forma adaptativa a la magnitud del vector, en caso de que se trabajen con valores muy grandes o pequeños tal que la función no tenga problemas de número flotante o redondeo con respecto a su magnitud.
  template<typename T>
  vector<double> rot_rodrigues(const vector<T>& v, const vector<T>& k, double angulo, string ang_type = "rad") {
    
    vector<double> rod(3);
    vector<double> kxv = producto_cruz(k, v);
    double kdotv = producto_punto(k, v);
    
    if (ang_type == "deg") {
        angulo = angulo * (M_PI / 180.0);
    } else if (ang_type != "rad") {
        cout << "Unidad de ángulo inválida" << "\n";
        return rod; 
    }

    rod[0] = v[0] * cos(angulo) + kxv[0] * sin(angulo) + k[0] * kdotv * (1 - cos(angulo));
    rod[1] = v[1] * cos(angulo) + kxv[1] * sin(angulo) + k[1] * kdotv * (1 - cos(angulo));
    rod[2] = v[2] * cos(angulo) + kxv[2] * sin(angulo) + k[2] * kdotv * (1 - cos(angulo));
    
    return rod;
}
    
    // Esta función genera un número aleatorio entero
    int ran_int(int min = 1, int max = 6) {
        
      if (min > max) {
        int max_temp = max;
        max = min;
        min = max_temp;
      }

      uniform_int_distribution<> distrib(min, max);
        
      return distrib(gen);
    }
    
    // Esta función, genera un número aleatorio real
    double ran_real(int min = 0.0, int max = 1.0){
      if (min > max) {
            double max_temp = max;
            max = min;
            min = max_temp;
        }
      
      uniform_real_distribution<> distrib(min, max);
      
      return distrib(gen);
    }
    
};

class lametnum{

};

/*
* Aquí empiezan las advertencias de posibles errores en el código, probablemente 
* hay una mejor forma de hacerlo. 
*/

vector<vector<double>> redundancia(vector<vector<double>> v) {
    const int v_mem = v.size();
    int v_fin;

    cout << "\033[1m\033[44m\033[33mLimpiando el conjunto de puntos...\033[0m" << endl;

    for (size_t i = 0; i < v.size(); ++i) {
        if ((i > 0 && v[i] == v[i - 1]) || (i + 1 < v.size() && v[i] == v[i + 1])) {
            v.erase(v.begin() + i);
            --i; // Ajustar el índice después de borrar el elemento
        }

        cout << progressBar(static_cast<int>(round(100.0 * i / v.size()))) << endl;
        cout << "\033[A" << "\033[K";
    }

    v_fin = v_mem - v.size();

    cout << "\033[32m [ Eliminados\033[0m \033[1m\033[35m" << v_fin << "\033[0m \033[32mpuntos consecutivos ] \033[0m\n\n";

    return v;
}

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

bool advertencia_punto_fijo(int p1_i, int p2_i, vector<double> p1, vector<double> p2) {
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

            cout << "\033[31m\033[1mAdvertencia:\033[0m\033[33m Los puntos número\033[35m\033[1m " << p1_i << "\033[0m\033[33m y \033[1m\033[35m" << p2_i << "\033[0m\033[33m se repiten de forma consecutiva. Esto puede causar discontinuidades en el modelo generado. ¿Desea eliminar los puntos consecutivos del modelo? (Y/n): ";

            char respuesta;
            while (true) {
                cin >> respuesta;
                if (respuesta == 'y' || respuesta == 'Y') {
                    cout << "\033[32mEliminando puntos consecutivos...\033[0m \n" << endl;
                    return true;
                } else if (respuesta == 'n' || respuesta == 'N') {
                    respuesta_positiva_count++;
                    puntos_consecutivos_count++;
                    if (respuesta_positiva_count >= 3) {
                        selec_op = puntos_consecutivos(puntos_consecutivos_count, selec_op);
                        return selec_op;
                    }
                    guarda_vec = p1;
                    cout << "\033[32m Continuando... \033[0m \n" << endl;
                    return false;
                } else {
                    cout << "\033[1m\033[31m Respuesta inválida.\n \033[0m" << endl;
                }
            }
        } else if (selec_op) {
            puntos_consecutivos_count++;
            selec_op = puntos_consecutivos(puntos_consecutivos_count, selec_op);
            guarda_vec = p1;
            return selec_op;
        }
    }
    return false; // Default return if no conditions are met
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

/*
* Aquí acaban las advertencias, hay que pensar en más formas de romper
* el código
*/

lamatools lmt_;

vector<vector<double>> generador_planos(vector<double> p, vector<double> q){

  cout << "\033[1m\033[44m\033[33mGenerando base inicial del modelo...\033[0m\n";
  
  double x =1.0, y=1.0 ,z=1.0;

  if (p[2] != q[2]){
    z = ((q[0]-p[0])*(x-p[0])+(q[1]-p[1])*(y-p[1])-p[2]*(q[2]-p[2]))/(p[2]-q[2]);
  } else if (p[1] != q[1]){
    y = ((q[0]-p[0])*(x-p[0])+(q[2]-p[2])*(z-p[2])-p[1]*(q[1]-p[1]))/(p[1]-q[1]);
  } else if (p[0] != q[0]){
    x = ((q[1]-p[1])*(y-p[1])+(q[2]-p[2])*(z-p[2])-p[0]*(q[0]-p[0]))/(p[0]-q[0]);
  }
  
  vector<double> u = {x-p[0], y-p[1], z-p[2]}, eje(p.size());
  
  double norma_u = lmt_.norma(u);
  
  for ( int i = 0; i < p.size(); i++ ){
    u[i] = u[i]/norma_u;
    eje[i] = q[i] - p[i];
  }
  
  vector<double> v = lmt_.producto_cruz(eje, u, true);
  
  cout << "\033[32m [ Base inicial generada ] \033[0m \n\n";
  
  return {u, v};
}


vector<vector<vector<double>>> generar_puntos(vector<double> u, vector<double> v, double r, int n, vector<double> p_ini, vector<double> p_fin){

  vector<vector<vector<double>>> vectores_alrededor = {};

  for (int i = 0; i < n; i++){

    vector<double> punto_alrededor(3), tapa_1(3), tapa_2(3); 
    
    for ( int j = 0; j<u.size(); j++){
      punto_alrededor[j] = r*( u[j]*cos((2*i*M_PI)/n) + v[j]*sin((2*i*M_PI)/n));
      tapa_1[j] = punto_alrededor[j]+p_ini[j];
      tapa_2[j] = punto_alrededor[j]+p_fin[j];
    }
    
    vectores_alrededor.push_back({tapa_1, tapa_2});
  }

  return vectores_alrededor;
}

vector<vector<double>> base_rotada(vector<double> v, vector<double> p1, vector<double> p2){

  vector<double> eje = {p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]}, k = lmt_.producto_cruz(eje, v, true);

  // Aquí comenzamos la revisión del ángulo entre los dos vectores.

  double ang =  M_PI/2 - acos(lmt_.producto_punto(v, eje)/lmt_.norma(eje));
		
  vector<double> v_r = lmt_.rot_rodrigues(v, k, ang);

  vector<double> v1 = {v_r[0]/lmt_.norma(v_r), v_r[1]/lmt_.norma(v_r), v_r[2]/lmt_.norma(v_r)};

  return {v1, k};
}

void generar_curva( vector<vector<double>> parametrizacion, double espesor, int n_puntos, string nombre_del_archivo ){

	int total_vertices = parametrizacion.size()*(n_puntos + 1);
	long int total_triangulos = n_puntos*2*parametrizacion.size();
	vector<vector<vector<double>>> puntos_circulares;
	
	bool redundante = false;
	
	for(int i = 0; i < parametrizacion.size()-1; i++){
	    if (redundante == false){
	      redundante = advertencia_punto_fijo(i, i+1, parametrizacion[i], parametrizacion[i+1]);
	    } else {
	      parametrizacion = redundancia(parametrizacion);
	      break;
	    }
	}
	
	advertencia_exceso(total_triangulos);
	
	nombre_del_archivo+=".obj";
	
 cout << "\033[1m\033[32mIniciando renderizado del modelo: \033[0m\033[35m\033[1m\033[4m" << nombre_del_archivo << "\033[0m\n" << endl;

    ofstream outFile(nombre_del_archivo);

    double radio_del_tubo = espesor / 2;
    vector<vector<double>> plano_inicial = generador_planos(parametrizacion[0], parametrizacion[1]);
    puntos_circulares = generar_puntos(plano_inicial[0], plano_inicial[1], radio_del_tubo, n_puntos, parametrizacion[0], parametrizacion[1]);

    cout << "\033[1m\033[44m\033[33mGenerando vértices de la curva...\033[0m\n\n";

    for (int k = 0; k < 2; ++k) {
        writeVertices(outFile, parametrizacion[k], puntos_circulares, k, n_puntos);
    }

    int vertices_iniciales = 2 + 2 * n_puntos;
    updateProgress(vertices_iniciales, total_vertices);

    if (parametrizacion.size() > 2) {
        vector<double> base_rotacion = plano_inicial[0];

        for (size_t j = 1; j < parametrizacion.size() - 1; ++j) {
            vector<vector<double>> planos = base_rotada(base_rotacion, parametrizacion[j], parametrizacion[j + 1]);

            puntos_circulares = generar_puntos(planos[0], planos[1], radio_del_tubo, n_puntos, parametrizacion[j], parametrizacion[j + 1]);

            base_rotacion = planos[0];

            writeVertices(outFile, parametrizacion[j + 1], puntos_circulares, 1, n_puntos);

            int vertices_gen = vertices_iniciales + (j + 2) * n_puntos + 1;
            updateProgress(vertices_gen, total_vertices);
        }
    }

    cout << "\033[A\033[K";
    cout << "\033[32m [ Vértices generados ] \033[0m \n\n";
	
    cout << "\033[1m\033[44m\033[33mGenerando triángulos del modelo...\033[0m\n";
	
	
  // Generación de la primera tapa
    cout << "\033[1m\033[44m\033[33mGenerando primera tapa...\033[0m\n";
    int progress_counter = 0;
    generarTapa(outFile, n_puntos, 1, 1, progress_counter, total_triangulos);
    cout << "\033[A\033[K";
    cout << "\033[32m [ Primera tapa generada ] \033[0m\n";

    // Generación de la frontera
    cout << "\033[1m\033[44m\033[33mGenerando frontera...\033[0m\n";
    generarFrontera(outFile, n_puntos, parametrizacion.size(), progress_counter, total_triangulos);
    cout << "\033[A\033[K";
    cout << "\033[32m [ Frontera generada ] \033[0m\n";

    // Generación de la segunda tapa
    cout << "\033[1m\033[44m\033[33mGenerando segunda tapa...\033[0m\n";
    generarTapa(outFile, n_puntos, (parametrizacion.size() - 1) * (n_puntos + 1) + 1, (parametrizacion.size() - 1) * (n_puntos + 1) + 2, progress_counter, total_triangulos);
    cout << "\033[A\033[K";
    cout << "\033[32m [ Segunda tapa generada ] \033[0m\n";
	
	cout << "\033[1m\033[32m Curva \033[35m\033[4m" << nombre_del_archivo << "\033[0m\033[1m\033[32m generada exitosamente." << endl;
	
}

class default_solids : public lamatools{

public:

vector<double> random_sphere_add(vector<double> v, vector<vector<double>> check, double step, double radio){    
    
    double theta = 2 * M_PI * ran_real(); 
    double phi = M_PI * ran_real();
    
    vector<double> v_mem = v;
    
    vector<double> mov = {v[0] + step * sin(phi) * cos(theta), v[1] + step * sin(phi) * sin(theta), v[2] + step * cos(phi)};
    if (producto_punto(mov, mov) <= radio){
        return mov;
    } else {
        return random_sphere_add(v_mem, check, step, radio);
    }
}

vector<vector<double>> ran_walk_sphere(double rad = 100.0, double step = 5, int iterations = 500){  
    
    vector<vector<double>> caminante_aleatorio_esfera, check, check_list={};
    
    vector<double> ini_point = {0,0,0};
    
    double v1, v2, v3;
    
    check = {{0,0,0},{0,0,0}};
    
    caminante_aleatorio_esfera.push_back(ini_point);
    check_list.push_back(ini_point);
    
    
    for(int i = 0; i < iterations; i++){
        
        ini_point = random_sphere_add(ini_point, check, step, rad);
            
        check[0] = check[1];
        check[1] = ini_point;
        
        if (check[0] != check[1]) {
		check_list.push_back(ini_point);
		
    		for (int j = 1; j < 10; j++) { 
				
        		v1 = check[0][0] + (check[1][0] - check[0][0]) * (j / 10.0); 
        		v2 = check[0][1] + (check[1][1] - check[0][1]) * (j / 10.0); 
        		v3 = check[0][2] + (check[1][2] - check[0][2]) * (j / 10.0); 
        		
        		caminante_aleatorio_esfera.push_back({v1, v2, v3});
    		}
    		
		} else {
		  cout << progressBar(100) << endl;
	          cout << "\033[A" << "\033[K";
	        break;
		}
	cout << progressBar(round(100*i/iterations)) << endl;
	cout << "\033[A" << "\033[K";
    }
    
    return caminante_aleatorio_esfera;    
}

vector<double> random_lineal_add(vector<double> v, vector<vector<double>> checker, double step, double side, int counter = 6, bool stuck = false) {
    vector<double> v_mem = v;

    if (counter == 0) {
        stuck = true;
    }

    int sel = ran_int(0, 2); // Selects which coordinate to modify (0, 1, or 2)
    int power = ran_int(1, 4); // Random power for direction

    // Calculate the new value
    double mov = v[sel] + pow(-1, power) * step;

    if (!stuck) {
        if (abs(mov) <= side / 2) { // Ensure the new value is within bounds
            v[sel] = mov;
        } else {
            // Retry with decremented counter if out of bounds
            return random_lineal_add(v_mem, checker, step, side, counter - 1, stuck);
        }
    } else {
        // Check all possible moves when stuck
        vector<vector<double>> dir_check = {
            {v[0] + step, v[1], v[2]}, {v[0] - step, v[1], v[2]},
            {v[0], v[1] + step, v[2]}, {v[0], v[1] - step, v[2]},
            {v[0], v[1], v[2] + step}, {v[0], v[1], v[2] - step}
        };
        vector<bool> stuck_check(6, false);

        for (int c = 0; c < dir_check.size(); c++) {
            for (int b = 0; b < checker.size(); b++) {
                if (dir_check[c] == checker[b]) { // Check if the move leads to a repeated vector
                    stuck_check[c] = true;
                    break;
                }
            }
        }

        // Check if all possible moves are out of bounds or stuck
        bool cir1 = (dir_check[0][0] > side / 2) || stuck_check[0];
        bool cir2 = (dir_check[1][0] < -side / 2) || stuck_check[1];
        bool cir3 = (dir_check[2][1] > side / 2) || stuck_check[2];
        bool cir4 = (dir_check[3][1] < -side / 2) || stuck_check[3];
        bool cir5 = (dir_check[4][2] > side / 2) || stuck_check[4];
        bool cir6 = (dir_check[5][2] < -side / 2) || stuck_check[5];

        if (cir1 && cir2 && cir3 && cir4 && cir5 && cir6) {
            return v_mem; // Return original vector if no valid moves
        } else {
            return random_lineal_add(v_mem, checker, step, side, 6, false);
        }
    }

    bool isUnique = true;
    for (int i = 0; i < checker.size(); i++) {
        if (v == checker[i]) {
            isUnique = false;
            break;
        }
    }

    if (isUnique) {
        return v; // Return the modified vector if it's unique
    } else {
        if (counter - 1 >= 0) {
            return random_lineal_add(v_mem, checker, step, side, counter - 1, stuck);
        } else {
            return v_mem; // Return the original vector if no unique point is found
        }
    }
}


vector<vector<double>> ran_walk_cube(bool memory = false, double side = 100, double step = 5, int iterations = 1500){  
    
    vector<vector<double>> mode, caminante_aleatorio_cubo, check, check_list={};
    
    vector<double> ini_point = {0,0,0};
    
    int sel_sign;
    
    double v1, v2, v3;
    
    check = {{0,0,0},{0,0,0}};
    
    caminante_aleatorio_cubo.push_back(ini_point);
    check_list.push_back(ini_point);
    
    if (memory == false){
    	mode = check;
    } else {
    	mode = check_list;
    }
    
    for(int i = 0; i < iterations; i++){
        
        ini_point = random_lineal_add(ini_point, mode, step, side);
        
        
        check[0] = check[1];
        check[1] = ini_point;
        
        if (check[0] != check[1]) {
		check_list.push_back(ini_point);
    		for (int j = 1; j < 9; j++) { 
				
        		v1 = check[0][0] + (check[1][0] - check[0][0]) * (j / 10.0); 
        		v2 = check[0][1] + (check[1][1] - check[0][1]) * (j / 10.0); 
        		v3 = check[0][2] + (check[1][2] - check[0][2]) * (j / 10.0); 
        		
        		caminante_aleatorio_cubo.push_back({v1, v2, v3});
    		}
    		if (memory == false){
    			mode = check;
    		} else {
    			mode = check_list;
    		}
		} else {
		  cout << progressBar(100) << endl;
	          cout << "\033[A" << "\033[K";
	        break;
		}
	cout << progressBar(round(100*i/iterations)) << endl;
	cout << "\033[A" << "\033[K";
    }
    
    return caminante_aleatorio_cubo;    
}



};

#endif
