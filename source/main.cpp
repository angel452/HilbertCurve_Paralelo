#include <iostream>
#include <bits/stdc++.h>
#include <omp.h>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

#define STEP_SZ 10 // es 5 normalmente
#define H_WINDOWS 800
#define W_WINDOWS 800
#define SPEED 1000  // check https://docs.python.org/3/library/turtle.html#turtle.speed

FILE *out;

vector<string> data[4];

void hilbert_curve(int n, int angle, int idthread) { // comienza mirando hacia la derecha   
    if( n == 0 ){
        return;
    }

    // 1. Gira derecha
    data[idthread].push_back("turtle.right(" + to_string(angle) + ")");
    //fprintf(out,"turtle.right(%d)\n", angle);

    // --> Giramos 90 grados (antihorario) para el primer cuadrante
    hilbert_curve(n-1, -angle, idthread);
    
    // 2. Avanza
    data[idthread].push_back("turtle.forward(" + to_string(STEP_SZ) + ")");
    //fprintf(out,"turtle.forward(%d)\n", STEP_SZ);
    // 3. Gira izquierda
    data[idthread].push_back("turtle.left(" + to_string(angle) + ")");
    //fprintf(out,"turtle.left(%d)\n", angle);
    
    // --> Giramos 90 grados (horario) para el primer cuadrante
    hilbert_curve(n-1, angle, idthread);

    // 4. Avanza
    data[idthread].push_back("turtle.forward(" + to_string(STEP_SZ) + ")");
    //fprintf(out,"turtle.forward(%d)\n", STEP_SZ);
    
    // --> Giramos 90 grados (horario) para el primer cuadrante
    hilbert_curve(n-1, angle, idthread);

    // 5. Gira izquierda
    data[idthread].push_back("turtle.left(" + to_string(angle) + ")");
    //fprintf(out,"turtle.left(%d)\n", angle);
    // 6. Avanza
    data[idthread].push_back("turtle.forward(" + to_string(STEP_SZ) + ")");
    //fprintf(out,"turtle.forward(%d)\n", STEP_SZ);

    // --> Giramos 90 grados (antihorario) para el primer cuadrante
    hilbert_curve(n-1, -angle, idthread);
    
    // 7. Gira derecha
    data[idthread].push_back("turtle.right(" + to_string(angle) + ")");
    //fprintf(out,"turtle.right(%d)\n", angle);
}

int main(int argc, char const *argv[]) {
    
    // ############### FOR .PY ########################
    // Print Head python file
    ofstream archivo("hilbert_turtle.py");
    archivo << "import turtle" << endl;
    archivo << "turtle.speed(" + to_string(SPEED) + ")" << endl;
    archivo << "turtle.setup(" + to_string(H_WINDOWS) + "," + to_string(W_WINDOWS) + ")" << endl;
    archivo << "turtle.penup()" << endl;
    archivo << "turtle.goto(-300,300)" << endl;
    archivo << "turtle.pendown()" << endl;

    // ############# CONFIG ###########################
    int order = 4;
    int angle = 90;

    // ############### FOR PARALEL ########################
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        
        #pragma omp for
        for(int i = 0; i < 4; i++){
            if( id == 0 ){
                //cout << "T1";
                data[id].push_back("turtle.right(" + to_string(90) + ") #id0");
                //fprintf(out,"turtle.right(%d)\n", 90);

                hilbert_curve(order-1, -90, id);
            }
            else if( id == 1 ){
                //cout << "T2";
                data[id].push_back("turtle.forward(" + to_string(STEP_SZ) + ") #id1");
                //fprintf(out,"turtle.forward(%d)\n", STEP_SZ);

                data[1].push_back("turtle.right(-" + to_string(90) + ") #id1");
                //fprintf(out,"turtle.right(%d)\n", -90);

                hilbert_curve(order-1, 90, id);
            }

            else if( id == 2 ){
                //cout << "T3";
                data[id].push_back("turtle.forward(" + to_string(STEP_SZ) + ") #id2");
                //fprintf(out,"turtle.forward(%d)\n", STEP_SZ);

                hilbert_curve(order-1, 90, id);
            }

            else if( id == 3 ){
                //cout << "T4";
                data[id].push_back("turtle.right(-" + to_string(90) + ") #id3");
                //fprintf(out,"turtle.right(%d)\n", -90);

                data[id].push_back("turtle.forward(" + to_string(STEP_SZ) + ") #id3");
                //fprintf(out,"turtle.forward(%d)\n", STEP_SZ);

                hilbert_curve(order-1, -90, id);
            }
        } 
    }

    // ################### PASAR RESULTADO AL .PY ########################
    if (archivo.is_open()) {
        for(int i = 0; i < 4; i++ ){
            cout << "Cuadrante 1 " << i+1 << endl;
            for(int j = 0; j < data[i].size(); j++){
                //cout << data[i][j] << " ";
                archivo << data[i][j] << endl;
            }
            cout << endl;
        }

        // Print END python file
        archivo << "turtle.right(90)" << endl;
        archivo << "turtle.update()" << endl;
        archivo << "screen = turtle.Screen()" << endl;
        archivo << "screen.exitonclick()" << endl;

        archivo.close();
        std::cout << "El contenido se ha copiado correctamente en hilbert_turtle.py" << std::endl;
    } else {
        std::cerr << "No se pudo abrir el archivo" << std::endl;
    }

    return 0;
}
