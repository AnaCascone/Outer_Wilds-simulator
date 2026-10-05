/*	Prácticas de Informática Gráfica

	Grupos C y D				Curso 2023-24
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: 

=======================================================
	G. Arroyo, J.C. Torres 
	Dpto. Lenguajes y Sistemas Informticos
	(Univ. de Granada)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details 
 http://www.gnu.org/copyleft/gpl.html

=======================================================

	 modelo.h
*/

/**
	Funcion de redibujado. Se ejecuta con los eventos postRedisplay
**/
void Dibuja (void);

/**
	Funcion de fondo
**/
void idle (int v);

/**
	Funcion de inicializacion del modelo y de las variables globales
**/
void initModel ();


#ifndef MODELO_H
#define MODELO_H

#include <vector>
#include <stdexcept>
using namespace std;

//********************************************************************************
// Definición de estructuras
// Struct de vértices
struct Vertice {
    float x, y, z;

	// Constructor por defecto
	Vertice() : x(0.0f), y(0.0f), z(0.0f) {}

    // Constructor para inicializar el vértice
    Vertice(float x, float y, float z) : x(x), y(y), z(z) {}

	// Suma
    Vertice operator+(const Vertice& v) const {
        return Vertice(x + v.x, y + v.y, z + v.z);
    }

	// Resta
    Vertice operator-(const Vertice& v) const {
    	// Si el valor es muy pequeño, lo redondeamos a 0.0
    	auto ajustarCero = [](float val) {
        	return (fabs(val) < 1e-6) ? 0.0f : val; 
    	};

    	return Vertice(
        	ajustarCero(x - v.x),
        	ajustarCero(y - v.y),
        	ajustarCero(z - v.z)
    	);
	}

	// Producto cruzado
    Vertice operator*(const Vertice& v) const {
		// Si el valor es muy pequeño, lo redondeamos a 0.0
    	auto ajustarCero = [](float val) {
        	return (fabs(val) < 1e-6) ? 0.0f : val;
    	};

    	return Vertice(
        	ajustarCero(y * v.z - z * v.y),
        	ajustarCero(z * v.x - x * v.z),
        	ajustarCero(x * v.y - y * v.x)
    	);
	}

	// División
	Vertice operator/(const int n) const {
		// Si el valor es muy pequeño, lo redondeamos a 0.0
		auto ajustarCero = [](float val) {
			return (fabs(val) < 1e-6) ? 0.0f : val;
		};

		return Vertice(
			ajustarCero(x / n),
			ajustarCero(y / n),
			ajustarCero(z / n)
		);
	}

	// Asignación
	Vertice& operator=(const Vertice& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	// Comparación: map
	bool operator<(const Vertice& v) const {
		return x < v.x && y < v.y && z < v.z;
	}

	// Operador []
	float& operator[](size_t index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Índice fuera de rango (0, 1, 2 permitidos)");
        }
    }
	const float& operator[](size_t index) const{
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Índice fuera de rango (0, 1, 2 permitidos)");
        }
    }

	// Normalizar (útil para las normales)
    Vertice normalizar() const {
    	float magnitud = sqrt(x * x + y * y + z * z);
    
    	// Evitar división por un valor muy pequeño
    	if (magnitud > 1e-6) {
    	    return Vertice(x / magnitud, y / magnitud, z / magnitud);
    	} else {
    	    return Vertice(0.0f, 0.0f, 0.0f);  // Devuelve un vector nulo si la magnitud es muy pequeña
    	}
	}

	void imprimir() {
		printf("(%f, %f, %f)\n", x, y, z);
	}

};

// Struct de caras
struct Cara {
    int v1, v2, v3; // Índices del vector de vértices

    // Constructor para inicializar la cara
    Cara(int v1, int v2, int v3) : v1(v1), v2(v2), v3(v3) {}

	int& operator[](size_t index) {
        switch (index) {
            case 0: return v1;
            case 1: return v2;
            case 2: return v3;
            default: throw std::out_of_range("Índice fuera de rango (0, 1, 2 permitidos)");
        }
    }

	const int& operator[](size_t index) const {
        switch (index) {
            case 0: return v1;
            case 1: return v2;
            case 2: return v3;
            default: throw std::out_of_range("Índice fuera de rango (0, 1, 2 permitidos)");
        }
    }

};

//********************************************************************************
// Métodos del alumno

void copiarColor(const float color1[], float color2[]);

/**
	Cambia el modo de visualización
**/
void cambiarModoVisualización(int M);

/**
	Activa y desactiva la luz
**/
void interruptorLuz();

/**
	Dibuja un vector. La dirección debe estar normalizada
**/
void drawVector(Vertice v, Vertice direccion, float k, float color[3]);

//=========================== INTERRUPTORES ===========================

/*
	Métodos para imprimir las normales de los objetos
*/
void interruptorNormales();

/*
	Métodos para modificar los modos de visualización
*/
void interruptorObservacion();
void interruptorNavegacion();
void interruptorZoom();

/*
	Métodos para modificar las animaciones
*/
void interruptorAcelerar();
void interruptorDesacelerar();
void interruptorReiniciarVelocidad();

/*
	Método para activar/desactivar la animación
*/

void interruptorActivarDesactivar();

//============================= SELECCIÓN =============================

// Seleccionar color para el elemento
int pick(int x, int y, int *i);

//============================= BOOLEANOS =============================
extern bool 
	navegacion_on,  // Modo navegación (necesario para mouse.c)
	zoom_on;		// Modo zoom (necesario para mouse.c)

//********************************************************************************
// Clase Objeto3D
class Objeto3D {
protected:
    float color[4];
    vector<Vertice> vertices, normales_c, normales_v;
    vector<Cara> caras;
    int forma, tipo;
	bool normales_on;
	GLuint textura_id;
	unsigned char* textura;
    vector<pair<float, float>> coordenadas_textura;

	// Método para dibujar las normales
	virtual void dibujarNormales();

public:
	// Constructor por defecto
	Objeto3D();

	// Constructor de la clase
    Objeto3D(const float color[], vector<Vertice> verts, vector<Cara> car, int forma, int tipo);
    
	// Método para establecer el vector de vértices
	virtual void setVertices(vector<Vertice> verts) {
		vertices = verts;
	}

	// Método para establecer el vector de caras
	virtual void setCaras(vector<Cara> car) {
		caras = car;
	}

	// Método para dibujar el objeto 
	virtual void draw();  

	// Método para dibujar en modo SMOOTH
	virtual void drawSMOOTH();

	// Método para calcular las normales de las caras
	virtual void calcularNormalesCaras();

	// Método para calcular las normales de los vértices
	virtual void calcularNormalesVertices();

	// Método para alternar la visualización de las normales
	virtual void activarNormales();

	// Cambiar el color del cubo
    void cambiarColor(const float color[]){
      copiarColor(color, this->color);
    }

	// Método para establecer la textura
	void asignarTextura(const char *archivo);
};


#endif


