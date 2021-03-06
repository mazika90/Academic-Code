#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stack>
#include "main.h"

using namespace std;

vector< vector<int> > botes;
vector<int> instancia;

// solo hosts puede recibir visitas (hosts no pueden salir tampoco)
bool restriccion_1(int host, int visita)
{
    if(host>=0) // si es que efectivamente tenemos un host
    {
        if (instancia[visita] == 1) // un host se fue de visita
        {
            return false;
        }
        else if (instancia[host] == 0) // el host en realidad no es host
        {
            return false;
        }
    }

    return true;
}

// la tripulacion + invitados no pueden superar la capacidad del bote
bool restriccion_2(Array2D solucion, int host, int visita, int tiempo, int n)
{
    if(host>=0) // si es que efectivamente tenemos un host
    {
        if ( botes[visita][TRIPULANTES] > botes[host][LIBRE]) // verificar que visita + tripulacion < capacidad
            return false;
        else
        {
            int suma = 0;
            for (int i=0 ; i<n ; i++)
            {
                if (solucion(i,tiempo) == host) // el host ya esta siendo usado en el instante de tiempo por otra visita
                {
                    suma += botes[i][TRIPULANTES];
                    if (suma > botes[host][LIBRE])
                        return false;
                }
            }
        }
    }

    return true;
}

// cada tripulacion invitada debe siempre tener un anfitrion asociado
bool restriccion_3(int host, int visita)
{
    if (instancia[visita]==0 && host < 0) // si una visita no tiene host asociado
        return false;
    return true;
}


// una tripulacion invitada no puede visitar el mismo bote anfitrion mas de una vez
bool restriccion_4(Array2D solucion, int host,int visita, int instante, int T)
{
    if (host >= 0)
    {
        for (int tiempo=0 ; tiempo<T ; tiempo++)
        {
            if (solucion(visita,tiempo) == host && tiempo < instante)
            {
                return false;
            }
        }
    }
    return true;
}

// cualquier par de invitados debe encontrarse a lo mas una vez
bool restriccion_5(Array2D solucion, int host, int visita, int instante, int n)
{
    if (host >= 0 && instante > 0)
    {
        for (int i=0; i<n ; i++)
        {
            if (solucion(i,instante) == host && i != visita)
            {
                for (int t=0 ; t<instante ; t++)
                {
                    if (solucion(i,t) == solucion(visita,t))
                    {
                        return false;
                    }
                }
            }
        }

    }
    return true;
}


bool consistente(Array2D solucion, Array3D mismo_bote, int visita, int tiempo, int T, int n)
{
    int host = solucion(visita,tiempo);

    if (!restriccion_1(host,visita))
        return false;

    if (!restriccion_2(solucion,host,visita,tiempo,n))
        return false;

    if (!restriccion_3(host,visita))
        return false;

    if (!restriccion_4(solucion,host,visita,tiempo,T))
        return false;

    if (!restriccion_5(solucion,host,visita,tiempo,n))
        return false;

    return true;

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
    int T = 6; // intervalos de tiempo

    /****************************************************************/
    /******* Obtencion de datos y acciones previas al problema ******/
    /****************************************************************/

    // Si el usuario no define la instancia por parametros, se le pregunta directamente.
    if (argc == 1)
    {
        cout << "Por favor ingrese el numero de la instancia que desea utilizar: ";
        cin >> n_instancia;
        cout << "Por favor ingrese el numero de periodos a utilizar (recomendado entre 6 y 8): ";
        cin >> T;
    }
    else if (argc == 5 && argv[1] == string("-instancia") && argv[3] == string("-periodos"))
    {
        n_instancia = atoi(string(argv[2]).c_str());
        T = atoi(string(argv[4]).c_str());
    }
    else
    {
        cout << "Uso:\t./app \t-instancia [1..n] -periodos [6..8]" << endl;
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
    int n = botes.size();           // cantidad de botes en la fiesta
    Array2D solucion(n,T,-1);       // solucion[i][j] = {1..T} la tripulacion j visita el bote i en el instante t
    Array3D mismo_bote(n,n,T,0);    // mismo_bote[i][j][t]: 1 si la tripulacion i y j se encuentran en el mismo bote en el instante t
    stack<node> auxiliar;           // stack para el backtracking

    int i = 0;
    int j = 0;
    int b = -1;
    long soluciones = 0;
    long iteraciones = 0;
    long backtracks = 0;
    long fails = 0;
    long success = 0;

    cout << endl << "Iteraciones\tConsistentes\tNo Consistentes\tBacktracks\tSoluciones" << endl;

    while (i < n)
    {
        while (j < T)
        {
            cout << endl << "Iteraciones\tConsistentes\tNo Consistentes\tBacktracks\tSoluciones" << endl;
            cout << iteraciones << "\t\t" << success << "\t\t" << fails << "\t\t" << backtracks << "\t\t" << soluciones << endl << endl;

            solucion(i,j) = b;
            //utils.print_to_screen(solucion,n,T);
            //cout << i << " " << j << " " << b << endl;

            if(i==n-1 && j==T-1)
            {
                // estamos en la ultima hoja
                //cout << "ultima hoja" << endl;
                if (consistente(solucion,mismo_bote,i,j,T,n))
                {
                    // si es consistente, encontramos una solucion posible
                    //cout << "solucion posible" << endl;
                    ++soluciones;
                    ++success;
                    utils.print_solution_to_file(solucion,n,T,n_instancia);

                    if (solucion(i,j)==n-1)
                    {
                        // si es el ultimo valor del dominio de la ultima variable, hacemos BT si es posible
                        //cout << "ultimo valor del dominio" << endl;
                        if (!auxiliar.empty())
                        {
                            solucion(i,j) = -1; //unset variable
                            if(b >= 0) mismo_bote(i,b,j) = 0;
                            node jump = auxiliar.top();
                            auxiliar.pop();
                            i = jump.i;
                            j = jump.j;
                            b = jump.b + 1;

                            while (b>=n)
                            {
                                solucion(i,j) = -1; //unset variable
                                node jump = auxiliar.top();
                                auxiliar.pop();
                                i = jump.i;
                                j = jump.j;
                                b = jump.b + 1;
                            }

                            ++backtracks;

                            //cout << "backtrack a " << i << " " << j << " " << b << endl;
                        }
                        else
                        {
                            cout << endl << "Espacio de busqueda recorido completamente. No hay mas instanciaciones posibles." << endl;
                            return 0;
                        }
                    }
                    else
                    {
                        // aun faltan valores del dominio por instanciar, continuamos iterando...
                        //cout << "quedan valores del dominio" << endl;
                        ++b;
                    }
                }
                else
                {
                    // si no es consistente
                    //cout << "ultima variables no consistente" << endl;
                    ++fails;
                    if (solucion(i,j)==n-1)
                    {
                        // si es el ultimo valor del dominio de la variable, hacemos BT si es posible
                        if (!auxiliar.empty())
                        {
                            solucion(i,j) = -1; //unset variable
                            if(b >= 0) mismo_bote(i,b,j) = 0;
                            node jump = auxiliar.top();
                            auxiliar.pop();
                            i = jump.i;
                            j = jump.j;
                            b = jump.b + 1;


                            while (b>=n)
                            {
                                solucion(i,j) = -1; //unset variable
                                node jump = auxiliar.top();
                                auxiliar.pop();
                                i = jump.i;
                                j = jump.j;
                                b = jump.b + 1;
                            }

                            //cout << "backtrack a " << i << " " << j << " " << b << endl;
                            ++backtracks;
                        }
                        else
                        {
                            cout << endl << "Espacio de busqueda recorido completamente. No hay mas instanciaciones posibles." << endl;
                            return 0;
                        }
                    }
                    else
                    {
                        // aun faltan valores del dominio por instanciar, continuamos iterando...
                        //cout << "aumentamos el valor del dominio" << endl;
                        ++b;
                    }
                }
            }
            else
            {
                // estamos en un nodo intermedio
                //cout << "nodo intermedio" << endl;
                if (consistente(solucion,mismo_bote,i,j,T,n))
                {
                    //si es consistente, debemos avanzar a la siguiente variable y recordar este paso!
                    ++success;
                    node step(i,j,b);
                    auxiliar.push(step);
                    if(b >= 0) mismo_bote(i,b,j) = 1;

                    ++j;
                    if (j==T){j=0; ++i;}
                    b=-1;
                    //cout << "consistente y avanzamos a " << i << " " << j << " " << b << endl;

                }
                else
                {
                    // si no es consistente
                    //cout << "no consistente" << endl;
                    ++fails;
                    if (solucion(i,j)==n-1)
                    {
                        // si es el ultimo valor del dominio de la variable, hacemos BT si es posible
                        //cout << "ultimo valor del dominio" << endl;
                        if (!auxiliar.empty())
                        {
                            solucion(i,j) = -1; // unset variable
                            if(b >= 0) mismo_bote(i,b,j) = 0;
                            node jump = auxiliar.top();
                            auxiliar.pop();
                            i = jump.i;
                            j = jump.j;
                            b = jump.b + 1;

                            while (b>=n)
                            {
                                solucion(i,j) = -1; //unset variable
                                node jump = auxiliar.top();
                                auxiliar.pop();
                                i = jump.i;
                                j = jump.j;
                                b = jump.b + 1;
                            }

                            //cout << "backtrack a " << i << " " << j << " " << b << endl;
                            ++backtracks;
                        }
                        else
                        {
                            cout << endl << "Espacio de busqueda recorido completamente. No hay mas instanciaciones posibles." << endl;
                            return 0;
                        }
                    }
                    else
                    {
                        // aun faltan valores del dominio por instanciar, continuamos iterando...
                        //cout << "quedan valores por instanciar" << endl;
                        ++b;
                    }
                }
            }

            //cout << endl;
            ++iteraciones;
        }
    }

    return 0;
}


