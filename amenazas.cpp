#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <ctime>

using namespace std;
int countID = 0;

struct Amenaza{
    string ID;
    string descripcion;
    string clasificacion;
    vector<string> hora;
};

void menuPrincipal(vector<Amenaza> &amenazasAltas, vector<Amenaza> &amenazasBajas, vector<Amenaza> &amenazasFalsas, vector<Amenaza> &historial);
void hacerHoraActual(vector<string> &horaCompleta);
void validarIngresoNumerico(int &opc, int tam);
void registrarAmenaza(vector<Amenaza> &amenazasAltas, vector<Amenaza> &amenazasBajas, vector<Amenaza> &amenazasFalsas);
void resolverAmenazas(vector<Amenaza> &altas, vector<Amenaza> &bajas, vector<Amenaza> &falsas, vector<Amenaza> &historial);
void mostrarListadoAmenazas(const vector<Amenaza> &altas, const vector<Amenaza> &bajas, const vector<Amenaza> &historial);

int main() {
    vector<Amenaza> listaAmenazasAltas;
    vector<Amenaza> listaAmenazasBajas;
    vector<Amenaza> listaAmenazasFalsas;
    vector<Amenaza> historialAmenazas;

    menuPrincipal(listaAmenazasAltas, listaAmenazasBajas, listaAmenazasFalsas, historialAmenazas);

    return 0;
}

void hacerHoraActual(vector<string> &horaCompleta) {

    time_t tiempo = time(0);
    tm* tiempoLocal = localtime(&tiempo);
    string hora = tiempoLocal->tm_hour < 10 ? "0" + to_string(tiempoLocal->tm_hour) : to_string(tiempoLocal->tm_hour);
    string minuto = tiempoLocal->tm_min < 10 ? "0" + to_string(tiempoLocal->tm_min) : to_string(tiempoLocal->tm_min);
    string segundo = tiempoLocal->tm_sec < 10 ? "0" + to_string(tiempoLocal->tm_sec) : to_string(tiempoLocal->tm_sec);

    horaCompleta.push_back(hora);
    horaCompleta.push_back(minuto);
    horaCompleta.push_back(segundo);
    horaCompleta.push_back(to_string(tiempo));
}

void validarIngresoNumerico(int &opc, int tam){ 
    while (true){
        if (!(cin>>opc) || opc > tam || opc < 1){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Opcion no valida, intente de nuevo: ";
            continue;
        }
        break;
    }
}

void registrarAmenaza(vector<Amenaza> &amenazasAltas, vector<Amenaza> &amenazasBajas, vector<Amenaza> &amenazasFalsas) {
    Amenaza nuevaAmenaza;
    int opcionClasificacion;

    countID++;
    nuevaAmenaza.ID = "M" + to_string(countID);
    
    cout << "\nIngrese la descripcion de la amenaza: ";
    cin.ignore();
    getline(cin, nuevaAmenaza.descripcion);
    
    cout << "\nClasificacion de la amenaza: \n";
    cout << "1. Alta\n2. Baja\n3. Falsa\nSeleccione una opcion: ";
    validarIngresoNumerico(opcionClasificacion, 3);
    switch(opcionClasificacion) {
        case 1:
            nuevaAmenaza.clasificacion = "Alta";
            break;
        case 2:
            nuevaAmenaza.clasificacion = "Baja";
            break;
        case 3:
            nuevaAmenaza.clasificacion = "Falsa";
            break;
        default:
            cout << "Ha ocurrido un error.\n";
            break;
    }

    hacerHoraActual(nuevaAmenaza.hora);

    if(opcionClasificacion == 1) {
        amenazasAltas.push_back(nuevaAmenaza);
    } else if(opcionClasificacion == 2) {
        amenazasBajas.push_back(nuevaAmenaza);
    } else if(opcionClasificacion == 3) {
        amenazasFalsas.push_back(nuevaAmenaza);
    }

    cout << "\nHora de registro: " << nuevaAmenaza.hora[0] << ":" << nuevaAmenaza.hora[1] << ":" << nuevaAmenaza.hora[2] << "\n";

    cout << "Amenaza registrada con ID: " << nuevaAmenaza.ID << "\n";
}

void resolverAmenazas(vector<Amenaza> &altas, vector<Amenaza> &bajas, vector<Amenaza> &falsas, vector<Amenaza> &historial) {
    time_t ahora = time(0);

    // Resolver amenazas altas (1 minuto)
    for (int i = altas.size() - 1; i >= 0; --i) {
        time_t tRegistro = static_cast<time_t>(stoll(altas[i].hora[3]));
        double segundos = difftime(ahora, tRegistro);
        if (segundos >= 60) {
            historial.push_back(altas[i]);
            altas.erase(altas.begin() + i);
        }
    }

    // Resolver amenazas bajas (3 minutos)
    for (int i = bajas.size() - 1; i >= 0; --i) {
        time_t tRegistro = static_cast<time_t>(stoll(bajas[i].hora[3]));
        double segundos = difftime(ahora, tRegistro);
        if (segundos >= 180) {
            historial.push_back(bajas[i]);
            bajas.erase(bajas.begin() + i);
        }
    }

    // Amenazas falsas se archivan de inmediato
    for (const auto &a : falsas) {
        historial.push_back(a);
    }
    falsas.clear();
}

void mostrarListadoAmenazas(const vector<Amenaza> &altas, const vector<Amenaza> &bajas, const vector<Amenaza> &historial) {

    if (altas.empty() && bajas.empty() && historial.empty()){
        cout << "\nNo hay amenazas registradas.\n\n";
    } else {
        if (!altas.empty()) {
            cout << "\nAmenazas altas:\n";
            for (const auto &a : altas) {
                cout << "ID: " << a.ID << " | Descripcion: " << a.descripcion << " | Hora: " << a.hora[0] << ":" << a.hora[1] << ":" << a.hora[2] << "\n";
            }
        } 
        if(!bajas.empty()){
            cout << "\nAmenazas bajas:\n";
            for (const auto &a : bajas) {
                cout << "ID: " << a.ID << " | Descripcion: " << a.descripcion << " | Hora: " << a.hora[0] << ":" << a.hora[1] << ":" << a.hora[2] << "\n";
            }
        }
        if(!historial.empty()){
            cout << "\nHistorial de amenazas:\n";
            for (const auto &h : historial) {
                cout << "ID: " << h.ID << " | Descripcion: " << h.descripcion << " | Clasificacion: " << h.clasificacion
                     << " | Hora: " << h.hora[0] << ":" << h.hora[1] << ":" << h.hora[2] << "\n";
            }
        }
    }
}

void menuPrincipal(vector<Amenaza> &amenazasAltas, vector<Amenaza> &amenazasBajas, vector<Amenaza> &amenazasFalsas, vector<Amenaza> &historial) {
    int opcion;
    do{
        cout << "\nMenu Principal:\n";
        cout << "1. Ingresar una amenaza\n";
        cout << "2. Ver informe\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        validarIngresoNumerico(opcion, 3); 
        
        if (opcion == 1){
            registrarAmenaza(amenazasAltas, amenazasBajas, amenazasFalsas);
        } else if (opcion == 2) {
            resolverAmenazas(amenazasAltas, amenazasBajas, amenazasFalsas, historial);
            mostrarListadoAmenazas(amenazasAltas, amenazasBajas, historial);    
        } else {
            cout << "Saliendo del programa.\n";
        }
    } while (opcion != 3);
    

}
