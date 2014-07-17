#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stack>
#include "main.h"

using namespace std;

vector< vector<int> > botes;
vector<int> instancia;


Array2D reset_from(Array2D solucion,int from_i, int from_j, int n)
{
    for (int i=from_i ; i<n ; i++)
        for (int j=from_j ; j<n ; j++)
            solucion(i,j) = 0;

    return solucion;
}


bool consistente(Array2D solucion, int i, int j, int T)
{
    // solo hosts puede recibir visitas
    // tripulacion i visita bote j
    if (solucion(i,j)>0 && instancia[j]==1)
        return true;
    else if (solucion(i,j)>0 && instancia[j]==0)
        return false;
    else if (solucion(i,j)==0)
        return true;


    // la capacidad de un bote no puede ser superada

}

int main(int argc, char **argv)
{
    cout << "================================================================= \n"
            "Entrega 2 - Inteligencia Artificial 2014-1\n"
            "'Progressive Party Problem' - Implementacion en Backtracking + GBJ\n"
            "Autor: Victor Gonzalez (victor.gonzalezro@alumnos.usm.cl)\n"
            "Codigo Fuente y Material extra en https://github.com/XzAeRo/Academic-Code/tree/master/IA/2014/\n"
            "================================================================= \n\n";

    Utilidades utils;
    int n_instancia = 1;

    /****************************************************************/
    /******* Obtencion de datos y acciones previas al problema ******/
    /****************************************************************/

    // Si el usuario no define la instancia por parametros, se le pregunta directamente.
    if (argc == 1)
    {
        cout << "Por favor ingrese el numero de la instancia que desea utilizar: ";
        cin >> n_instancia;
    }
    else if (argc == 3 && argv[1] == string("-instancia"))
    {
        n_instancia = atoi(string(argv[2]).c_str());
    }
    else
    {
        cout << "Uso:\t./app \t[-instancia 1..n]" << endl;
        return 1;
    }

    // cargar datos desde los archivos
    botes = utils.get_boats();
    instancia = utils.get_instance(n_instancia);


    // verificar si la informacion proporcionada por los archivos coincide
    if (botes.size() != instancia.size())
    {
        cout << "La instancia elegida tiene una cantidad distinta de botes a las definidas por ppp.cap" << endl;
        return 1;
    }


    /****************************************************************/
    /******* Busqueda de solucion mediante Backtracking (GBJ) *******/
    /****************************************************************/
    int T = TMIN + 1;               // intervalos de tiempo
    int n = botes.size();       // cantidad de botes en la fiesta
    Array2D solucion(n,n,0);       // solucion[i][j] = {1..T} la tripulacion j visita el bote i en el instante t
    Array3D mismo_bote(n,n,T,0);    // mismo_bote[i][j][t]: 1 si la tripulacion i y j se encuentran en el mismo bote en el instante t
    stack<node> auxiliar;

    int i = 0;
    int j = 0;
    int t = 0;
    int soluciones = 0;

    int input;

    while (i < n)
    {
        while (j < n)
        {
            solucion(i,j) = t;

            if(i==n-1 && j==n-1)
            {
                // estamos en la ultima hoja
                if (consistente(solucion,i,j,T))
                {
                    // si es consistente, encontramos una solucion posible
                    ++soluciones;
                    utils.print_solution_to_file(solucion,n,soluciones);

                    if (solucion(i,j)==T)
                    {
                        // si es el ultimo valor del dominio de la ultima variable, hacemos BT si es posible
                        if (!auxiliar.empty())
                        {
                            solucion(i,j) = 0; //unset variable
                            node jump = auxiliar.top();
                            auxiliar.pop();
                            i = jump.i;
                            j = jump.j;
                            t = jump.t + 1;

                            while (t>=T)
                            {
                                solucion(i,j) = 0; //unset variable
                                node jump = auxiliar.top();
                                auxiliar.pop();
                                i = jump.i;
                                j = jump.j;
                                t = jump.t + 1;
                            }
                        }
                        else
                        {
                            cout << "no hay mas instanciaciones posibles: " << i << " " << j << " " << t << endl;
                            return 0;
                        }
                    }
                    else
                    {
                        // aun faltan valores del dominio por instanciar, continuamos iterando...
                        ++t;
                    }
                }
                else
                {
                    // si no es consistente
                    if (solucion(i,j)==T)
                    {
                        // si es el ultimo valor del dominio de la variable, hacemos BT si es posible
                        if (!auxiliar.empty())
                        {
                            solucion(i,j) = 0; //unset variable
                            node jump = auxiliar.top();
                            auxiliar.pop();
                            i = jump.i;
                            j = jump.j;
                            t = jump.t + 1;


                            while (t>=T)
                            {
                                solucion(i,j) = 0; //unset variable
                                node jump = auxiliar.top();
                                auxiliar.pop();
                                i = jump.i;
                                j = jump.j;
                                t = jump.t + 1;
                            }
                        }
                        else
                        {
                            cout << "no deberia ocurrir esto :P " << i << " " << j << " " << t << endl;
                        }
                    }
                    else
                    {
                        // aun faltan valores del dominio por instanciar, continuamos iterando...
                        ++t;
                    }
                }
            }
            else
            {
                // estamos en un nodo intermedio
                if (consistente(solucion,i,j,T))
                {
                    //si es consistente, debemos avanzar a la siguiente variable y recordar este paso!
                    node step(i,j,t);
                    auxiliar.push(step);

                    ++j;
                    if (j==n){j=0; ++i;}
                    t=0;
                    solucion = reset_from(solucion,i,j,n);

                }
                else
                {
                    // si no es consistente
                    if (solucion(i,j)==T)
                    {
                        // si es el ultimo valor del dominio de la variable, hacemos BT si es posible
                        if (!auxiliar.empty())
                        {
                            solucion(i,j) = 0; // unset variable
                            node jump = auxiliar.top();
                            auxiliar.pop();
                            i = jump.i;
                            j = jump.j;
                            t = jump.t + 1;

                            while (t>=T)
                            {
                                solucion(i,j) = 0; //unset variable
                                node jump = auxiliar.top();
                                auxiliar.pop();
                                i = jump.i;
                                j = jump.j;
                                t = jump.t + 1;
                            }
                        }
                        else
                        {
                            cout << "no deberia ocurrir esto! :P " << i << " " << j << " " << t << endl;
                        }
                    }
                    else
                    {
                        // aun faltan valores del dominio por instanciar, continuamos iterando...
                        ++t;
                    }
                }
            }

        }
    }

    cout << "gola?" << endl;

    return 0;
}


