#include <iostream>
#include <string>
#include <cstdlib> 
#include <fstream> // Libreria requerida en M4: Para lectura y escritura de archivos

using namespace std;

// --- PROTOTIPOS DE FUNCIONES ---
void mostrarBienvenida();
void verInventario(int equipos[], int total_equipos);
void registrarUsuario(string bitacora[][3], int &contador, int equipos[], int total_equipos);
void generarReporte(string bitacora[][3], int contador);
void consultarReporte();
// Funciones booleanas para validacion de datos (Tema M4)
bool validarHora(string hora); 
bool validarID(string id); 
bool validarNombre(string nombre); 

int main() {
    // -------------------------------------------------------------
    // ESTRUCTURAS DE DATOS (Tema 4.1 del Contenido Nuclear)
    // -------------------------------------------------------------
    
    // Arreglo Unidimensional: Inventario de 20 computadoras (0 = Libre, 1 = Ocupada)
    int equipos[20] = {0}; 
    
    // Arreglo Bidimensional (Matriz): Bitacora de 10 usuarios con 3 columnas (ID, Nombre, Hora)
    string bitacora[10][3]; 
    
    int contador_usuarios = 0; 
    int opcion = 0;

    // Ciclo repetitivo para mantener el sistema activo
    do {
        mostrarBienvenida();
        // Menu limpio en pantalla, con justificacion en comentarios para evaluacion
        cout << "1. Ver inventario de equipos" << endl;     // (Uso de Arreglo 1D)
        cout << "2. Registrar nuevo usuario" << endl;       // (Uso de Matriz Bidimensional y getline)
        cout << "3. Guardar reporte en archivo" << endl;    // (Uso de ofstream / Persistencia)
        cout << "4. Leer reporte desde archivo" << endl;    // (Uso de ifstream / Archivo de texto)
        cout << "5. Salir del sistema" << endl;
        cout << "Elige una opcion: ";
        
        // Estructura de proteccion por si el usuario teclea un caracter invalido
        if (!(cin >> opcion)) {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            opcion = 0;
        }

        switch(opcion) {
            case 1:
                verInventario(equipos, 20);
                break;
            case 2:
                registrarUsuario(bitacora, contador_usuarios, equipos, 20);
                break;
            case 3:
                generarReporte(bitacora, contador_usuarios);
                break;
            case 4:
                consultarReporte();
                break;
            case 5:
                cout << "\nCerrando el sistema SIG-Conecta. ¡Hasta pronto!" << endl;
                break;
            default:
                cout << "\n-> Error: Opcion no valida." << endl;
        }

    } while(opcion != 5);

    return 0;
}

// --- IMPLEMENTACIÓN DE FUNCIONES ---

void mostrarBienvenida() {
    cout << "\n==========================================" << endl;
    cout << " UNIVERSIDAD NACIONAL ROSARIO CASTELLANOS" << endl;
    cout << "       SISTEMA SIG-CONECTA MODULO 4" << endl;
    cout << "==========================================" << endl;
}

void verInventario(int equipos[], int total_equipos) {
    cout << "\n--- INVENTARIO DE EQUIPOS ---" << endl;
    // Recorrido de arreglo unidimensional mediante un ciclo for
    for(int i = 0; i < total_equipos; i++) {
        if(equipos[i] == 0) {
            cout << "PC " << (i + 1) << ": Disponible" << endl;
        } else {
            cout << "PC " << (i + 1) << ": Ocupada" << endl;
        }
    }
    cout << "-----------------------------" << endl;
}

// Funcion para asegurar que la cadena del ID contenga unicamente caracteres numericos
bool validarID(string id) {
    for(int i = 0; i < id.length(); i++) {
        if(id[i] < '0' || id[i] > '9') {
            return false;
        }
    }
    return true;
}

// Funcion para validar que la cadena del nombre contenga solo letras y espacios
bool validarNombre(string nombre) {
    if(nombre.length() == 0) return false;
    for(int i = 0; i < nombre.length(); i++) {
        char c = nombre[i];
        // Validamos que sea letra mayuscula, minuscula o un espacio en blanco
        if(!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
            return false;
        }
    }
    return true;
}

// Funcion que valida la longitud de la cadena para asegurar un formato correcto de hora
bool validarHora(string hora) {
    if(hora.length() < 4) {
        return false;
    }
    return true;
}

void registrarUsuario(string bitacora[][3], int &contador, int equipos[], int total_equipos) {
    if(contador < 10) {
        cout << "\n--- REGISTRO DE USUARIO ---" << endl;
        
        cout << "Ingresa el ID del usuario (solo numeros): ";
        cin >> bitacora[contador][0]; 
        
        // Aplicacion de validacion booleana para el ID
        if (!validarID(bitacora[contador][0])) {
            cout << "-> Error: El ID debe contener estrictamente numeros. Registro cancelado.\n";
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            return; 
        }
        
        // Uso de getline con libreria string para evitar truncamientos con espacios
        cout << "Ingresa el nombre completo (solo letras): ";
        getline(cin >> ws, bitacora[contador][1]); 
        
        // Aplicacion de validacion booleana para el texto
        if (!validarNombre(bitacora[contador][1])) {
            cout << "-> Error: El nombre solo debe contener letras. Registro cancelado.\n";
            return; 
        }
        
        cout << "Ingresa la hora de entrada (ej. 09:00): ";
        cin >> bitacora[contador][2]; 
        
        // Aplicacion de validacion booleana para la longitud de la hora
        if (!validarHora(bitacora[contador][2])) {
            cout << "-> Error: Hora invalida. Registro cancelado.\n";
            return; 
        }
        
        // Busqueda de disponibilidad en el arreglo unidimensional
        bool asignado = false;
        for(int i = 0; i < total_equipos; i++) {
            if(equipos[i] == 0) {
                equipos[i] = 1; 
                cout << "-> ¡Registro Exitoso! Se asigno la PC numero " << (i + 1) << endl;
                asignado = true;
                break; 
            }
        }
        
        if(!asignado) {
            cout << "-> Error: No hay computadoras disponibles." << endl;
        }
        
        contador++; 
    } else {
        cout << "\n-> Error: La bitacora de hoy esta llena." << endl;
    }
}

void generarReporte(string bitacora[][3], int contador) {
    // Uso de ofstream en modo ios::app para agregar datos sin sobrescribir el archivo
    ofstream archivoGuardar("reporte_dia.txt", ios::app);
    
    if(archivoGuardar.is_open()) {
        // Almacenamiento de datos en formato separado por comas (CSV)
        for(int i = 0; i < contador; i++) {
            archivoGuardar << bitacora[i][0] << "," << bitacora[i][1] << "," << bitacora[i][2] << "\n";
        }
        archivoGuardar.close(); 
        cout << "\n-> ¡Exito! Los datos fueron guardados en el archivo fisico 'reporte_dia.txt'" << endl;
    } else {
        cout << "\n-> Error: No se pudo crear el archivo." << endl;
    }
}

void consultarReporte() {
    // Uso de ifstream para lectura y recuperacion de informacion (Persistencia)
    ifstream archivoLeer("reporte_dia.txt");
    string lineaTexto;
    
    if(archivoLeer.is_open()) {
        cout << "\n--- LEYENDO DATOS DEL ARCHIVO (PERSISTENCIA) ---" << endl;
        // Lectura del archivo linea por linea hasta el final
        while(getline(archivoLeer, lineaTexto)) {
            cout << lineaTexto << endl;
        }
        archivoLeer.close();
        cout << "------------------------------------------------" << endl;
    } else {
        cout << "\n-> Error: No se encontro el archivo 'reporte_dia.txt'." << endl;
    }
}