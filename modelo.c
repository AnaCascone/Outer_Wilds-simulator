/*	Prácticas de Informática Gráfica

	Grupos C y D				Curso 2023-24

	Práctica final

	Estudiante: Ana Cascone Hernández

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

=======================================================*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>        // Esfera
#include <map>          // Esfera
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include <algorithm>    // Esfera
#include <cfloat>       // Esfera
#include <map>          // map de nombres
#include <ctime>        // Para la animación
#include <random>       // Para la lanzadera
#include <vector>
#include <list>         // Cálculo de normales
#include <utility>
#include "practicasIG.h"
#include "file_ply_stl.h"
#include "lector-jpg.h"

//===================== VARIABLES GLOBALES ======================

// Colores (sí, lo sé son muchos)
  float lila[4] = { 0.8f, 0.0f, 1.0f, 1.0f };
  float morado[4] = { 0.5f, 0.0f, 0.5f, 1.0f };
  float azul[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
  float blanco[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  float blanco_suave[4] = {0.98f, 0.98f, 0.98f, 1.0f};
  float rojo[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  float marron[4] = {0.5f, 0.2f, 0.0f, 1.0f};
  float marron_claro[4] = {0.8f, 0.4f, 0.0f, 1.0f};
  float negro[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // Totalmente negro
  float amarillo[4] = {1.0f, 1.0f, 0.0f, 1.0f};
  float naranja[4] = {1.0f, 0.4f, 0.0f, 1.0f};
  float gris[4] = {0.3f, 0.3f, 0.3f, 1.0f};
  float burdeos_pastel[4] = {0.9f, 0.4f, 0.5f, 1.0f};
  float azul_claro_pastel[4] = {0.6f, 0.8f, 1.0f, 1.0f};
  float morado_pastel[4] = {0.7f, 0.6f, 0.9f, 1.0f};
  float azul_oscuro_pastel[4] = {0.4f, 0.6f, 0.8f, 1.0f};

// Vectores vacíos
vector<Vertice> vertices_vacios;
vector<Cara> caras_vacias;
vector<Vertice> normales_vacias;

// Textura
unsigned ancho, alto;

// CONTROL GENERAL DE LA ANIMACIÓN
// Cronómetro
clock_t start = clock(); // Inicializamos el reloj
float delta;

// Inicio de la animación
bool animacion_on = false;
bool primer_inicio = true; // Para controlar la primera vez que se pulsa A

// Escala para la velocidad de las animaciones
const float ESCALA_VELOCIDAD_INICIAL = 30.0;
float escala_velocidad = ESCALA_VELOCIDAD_INICIAL;

// NIVEL A: SOL
  // Radio del Sol
  const float radio_sol = 1;

  float 
      angulo_sol = 0.0,
      velocidad_rot_sol = 0.7 * escala_velocidad;

// NIVEL B: PLANETAS
  // Radios planetas
  const float
      radio_gemelos = radio_sol * 0.15,
      radio_lumbre = radio_sol * 0.25,
      radio_hondonada = radio_sol * 0.15,
      radio_abismo = radio_sol * 0.5,
      radio_agujero = radio_sol * 0.05; 

  // Radios órbitas
  const float
      RADIO_ORBITA_GEMELOS = 2.5,
      RADIO_ORBITA_LUMBRE = 4.5,
      RADIO_ORBITA_HONDONADA = 7.0,
      RADIO_ORBITA_ABISMO = 10.0;

  // Ángulos de los planetas respecto al eje x (traslación)
  const float
      ANGULO_INI_GEMELOS = 0.0,
      ANGULO_INI_LUMBRE = 240.0,
      ANGULO_INI_HONDONADA = 327.0,
      ANGULO_INI_ABISMO = 42.0;
  float
      angulo_gemelos = ANGULO_INI_GEMELOS,
      angulo_lumbre = ANGULO_INI_LUMBRE,
      angulo_hondonada = ANGULO_INI_HONDONADA,
      angulo_abismo = ANGULO_INI_ABISMO;

  // Ángulos respecto al eje y (rotación)
  float
      rotacion_gemelos = 0.0,
      rotacion_gemelos_reloj = 0.0, // Rotación en z entre ambos planetas
      rotacion_lumbre = 0.0,
      rotacion_hondonada = 0.0,
      rotacion_abismo = 0.0;


  // Animaciones
    // Velocidad de movimiento
    float
        velocidad_gemelos = 0.5 * escala_velocidad,
        velocidad_lumbre = 0.22 * escala_velocidad,
        velocidad_hondonada = 0.17 * escala_velocidad,
        velocidad_abismo = 0.1 * escala_velocidad;

    // Velocidad de rotación
    float
        velocidad_rot_gemelos = 0.5 * escala_velocidad,           // Rotación individual
        velocidad_rot_gemelos_reloj = 0.75 * escala_velocidad,    // Rotación en z entre ambos planetas
        velocidad_rot_lumbre = 0.3 * escala_velocidad,
        velocidad_rot_hondonada = 0.2 * escala_velocidad,
        velocidad_rot_abismo = 0.2 * escala_velocidad;

  // SUBNIVEL B.1: ANIMACIÓN GEMELOS RA
    bool
        aumenta_brasas = true,       // La animación inicia de Cenizas a Brasas
        mov_arena = false,           // La arena no se mueve al inicio
        mov_cascada = true,          // La cascada empieza moviéndose
        fin_cascada = false,         // Inicia la animación de regresión de la cascada
        pausa_arena = false;         // Periodo de pausa entre un ciclo y otro

    const float
        DIST_CENTROS_GEMELOS = 0.6,                // Distancia entre los planetas
        RADIO_ARENA_MAX = radio_sol * 0.25,        // Radio máximo de las arenas (rodea el planeta)
        RADIO_ARENA_MIN = radio_sol * 0.14,        // Radio mínimo de las arenas (interior del planeta)
        LONGITUD_INI_CASCADA = 0.1,                // Longitud de la cascada
        CENTRO_INI_CASCADA = (DIST_CENTROS_GEMELOS / 2) - LONGITUD_INI_CASCADA / 2;   // Altura de cenizas - l0/2

    float
        rotacion_arena = 0.0,                                // Rotación de la arena
        radio_arena1 = RADIO_ARENA_MAX,                      // Arena 1 inicia al máximo
        radio_arena2 = RADIO_ARENA_MIN,                      // Arena 2 inicia al mínimo
        escala_arena_crece = 0.00015 * escala_velocidad,     // Velocidad de crecimiento de la arena
        escala_arena_decrece = 0.0003 * escala_velocidad,    // Velocidad de decrecimiento de la arena. Más rápida para fluidez
        longitud_cascada = LONGITUD_INI_CASCADA,
        centro_cascada = CENTRO_INI_CASCADA,                 // Centro del cilindro
        escala_cascada = 0.002 * escala_velocidad;           // Velocidad crecimiento/decrecimiento del cilindro

    long long contador_pausa = 0;


  // SUBNIVEL B.2: HONDONADA FŔAGIL
    float 
        mov_x_1 = 0.0,        // Movimiento del trozo 1
        mov_y_2 = 0.0,
        mov_x_2 = 0.0,
        velocidad_trozo1 = 0.001 * escala_velocidad,
        velocidad_trozo2 = 0.001 * escala_velocidad;

    bool casa1 = true, casa2 = true;


// NIVEL C: SATÉLITES
  // Rocaterra: Luna de lumbre
  const float RADIO_ORBITA_ROCATERRA = 0.5, radio_rocaterra = 0.1;

  float 
      angulo_rocaterra = 0.0,
      velocidad_rocaterra = 5.0 * escala_velocidad,
      rotacion_rocaterra = 0.0,
      velocidad_rot_rocaterra = 10.0 * escala_velocidad;

  // Lanzadera
  const float RADIO_ORBITA_LANZADERA = 1.0;
  float
      angulo_lanzadera,  // El ángulo inicial de la lanzadera es aleatorio
      angulo_lanzadera_nave, // Para guardar el último ángulo antes de dispararse
      velocidad_lanzadera = 0.0, // Velocidad de la lanzadera
      velocidad_lanzadera_maxima = 0.38 * escala_velocidad, // No es constante por el interruptor V
      rotacionX_lanzadera, rotacionY_lanzadera, // Ángulos de rotación de la lanzadera
      aceleracion_lanzadera = 12.0 * escala_velocidad, // Aceleración de la lanzadera
      movx = 0.0, movy = 0.0, movz = 0.0; // Movimiento de la lanzadera

  bool disparada_lanzadera = false; // Para que la lanzadera se dispare solo una vez

  Vertice direccion_lanzadera; // Dirección de la lanzadera (aleatoria cada ciclo)
// NIVEL D: EL INTRUSO
  float 
      n_intruso = 177.0,                                      // Para modificar el ángulo del intruso
      angulo_intruso = -n_intruso * M_PI / 180.0,             // Ángulo de movimiento
      intruso_x = 7.0 * cos(angulo_intruso),                  // Posicion x del intruso
      intruso_z = 4.5 * sin(angulo_intruso),                  // Posición z del intruso
      velocidad_intruso = 0.5 * escala_velocidad,             // Velocidad de rotación del intruso
      velocidad_rotacion_intruso = 10.0 * escala_velocidad,   // Velocidad de rotación del intruso
      rotacion_intruso = 0.0,                                 // Rotación del intruso
      rotacion_intruso_x = 0.0, rotacion_intruso_y = 0.0;     // Dirección del intruso
    

// NIVEL E: NAVE
  // Movimiento de la nave
  float
      velocidad_nave = 0.2 * escala_velocidad,              // Velocidad de movimiento de la nave
      velocidad_despegue_nave = 15.0 * escala_velocidad;     // Velocidad de movimiento de salida y entrada de los planetas

  // Estados de la nave
  enum EstadoNave{
    EN_CENTRO_PLANETA,
    SALIENDO_DEL_PLANETA,
    ESPERANDO,
    VIAJANDO,
    ATERRIZANDO
  };

  EstadoNave estado_nave;

  int indice_planeta_navegacion;

  bool corrigiendo = false; // Para corregir la trayectoria de la nave

  // Nota: el nodo de la nave se dibuja con el método dibujarNave. El centro de la nave coincide con el centro del nodo
    // cuando la nave está en el centro del planeta. Cuando despega, deja de coincidir. Es importante entender la diferencia

  Vertice
      posicion_nave,            // Posición de la nave dentro de su nodo
      posicion_centro_nodo,     // Posición del centro del NODO
      posicion_real_nave,       // Posición del centro de la NAVE
      posicion_planeta,         // Posición del planeta al que se dirige la nave o donde se encuentra
      direccion_nave,           // Dirección de la nave respecto al planeta al que se dirige
      direccion_nave_temp,      // Para correciones de la trayectoria
      posicion_ceniza,
      posicion_brasas,
      posicion_lumbre,
      posicion_hondonada,
      posicion_abismo,
      posicion_agujero;
    
  // Orientación de la nave
  float rotacionX_nave = 0.0, rotacionY_nave = 0.0;

// NIVEL EXTRA
  // SUBNIVEL: SELECCIÓN
  int n_seleccion = 1;

  bool
    seleccionActivada = false,
    navegacion_on = false,
    zoom_on = false;

  map<int,string> nombres;
  map<int,string> descripciones;
  map<int,string> navegacion;

//*********************************************************************

//============== MÉTODOS AUXILIARES Y CLASES PRINCIPALES ==============

//*********************************************************************

// IMPRIMIR POR PANTALLA

void mostrarFunciones(){
 printf("\n╔══════════════════════════════════════════════════════════╗\n");
  printf("║                        CONTROLES                         ║\n");
  printf("╠══════════════════════════════════════════════════════════╣\n");
  printf("║  A: Activar/Desactivar animación                         ║\n");
  printf("║  N: Alternar la visualización de las normales            ║\n");
  printf("║                                                          ║\n");
  printf("║  Modos de interacción:                                   ║\n");
  printf("║    o, O: Modo de observación                             ║\n");
  printf("║    z, Z: Modo de zoom                                    ║\n");
  printf("║    d, D: Modo de navegación                              ║\n");
  printf("║                                                          ║\n");
  printf("║  Velocidad de la animación:                              ║\n");
  printf("║    V: Acelerar                                           ║\n");
  printf("║    v: Desacelerar                                        ║\n");
  printf("║    r, R: Reiniciar velocidad                             ║\n");
  printf("╚══════════════════════════════════════════════════════════╝\n");
};

void mostrarNave(){
    printf("                     `. ___\n");
    printf("                    __,' __`.                _..----....____\n");
    printf("        __...--.'``;.   ,.   ;``--..__     .'    ,-._    _.-'\n");
    printf("  _..-''-------'   `'   `'   `'     O ``-''._   (,;') _,'\n");
    printf(",'________________                          \\`-._`-',\n");
    printf(" `._              ```````````------...___   '-.._'-:\n");
    printf("    ```--.._      ,.                     ````--...__\\-.\n");
    printf("            `.--. `-`                       ____    |  |`\n");
    printf("              `. `.                       ,'`````.  ;  ;`\n");
    printf("                `._`.        __________   `.      \\'__/'`\n");
    printf("                   `-:._____/______/___/____`.     \\  `\n");
    printf("                               |       `._    `.    \\\n");
    printf("                               `._________`-.   `.   `.___\n");
    printf("                                                  `------'\n");
}

void mostrarObservacion(){
  printf("┌──────────────────────────────────────────────────────────┐\n");
  printf("│                    (Modo: Observación)                   │\n");
  printf("└──────────────────────────────────────────────────────────┘\n");

  cout << "------------------------------------------------------------\n"
    << "    HAGA CLIC EN UN ASTRO PARA CONOCER SU DESCRIPCIÓN\n"
    << "------------------------------------------------------------\n" << endl;
}

void mostrarNavegacion(){
  printf("┌──────────────────────────────────────────────────────────┐\n");
  printf("|                    (Modo: Navegación)                    |\n");
  printf("└──────────────────────────────────────────────────────────┘\n");

  cout << "------------------------------------------------------------\n"
    << "         HAGA CLIC EN UN ASTRO PARA VIAJAR A ÉL\n"
    << "------------------------------------------------------------\n" << endl;

  printf("\033[1;36m");
  cout << "REGISTRO DE LA NAVE \n";
  printf("\033[0m");
}

void mostrarZoom(){       
  printf("┌──────────────────────────────────────────────────────────┐\n");
  printf("|                       (Modo: Zoom)                       |\n");
  printf("└──────────────────────────────────────────────────────────┘\n");
  
  cout << "------------------------------------------------------------\n"
      << "     HAGA CLIC EN UN ASTRO PARA OBSERVARLO DE CERCA\n"
      << "------------------------------------------------------------\n" << endl;
}

// --------------------------------------------------------------------

float clamp(float val, float minVal, float maxVal) {
    return max(minVal, min(maxVal, val));
}

// Copia las propiedades de un color (tamaño 4)
void copiarColor(const float color1[], float color2[]){
  for(int i = 0; i < 4; i++){
    color2[i] = color1[i];
  }
}

// --------------------------------------------------------------------

// Dibuja un vector. Dirección debe estar normalizado
void drawVector(Vertice v, Vertice direccion, float k, float color[3]){
  glDisable (GL_LIGHTING);

  glBegin (GL_LINES);
  {
  glColor3f (color[0], color[1], color[2]);
  glVertex3f (v.x, v.y, v.z);
  Vertice destino = v + direccion; 
  glVertex3f (destino.x, destino.y, destino.z);
  }
  glEnd ();

  glEnable (GL_LIGHTING);

}

void drawPoint(Vertice v, float color[3], int size){
  glDisable (GL_LIGHTING);

  glPointSize(size);
  glBegin(GL_POINTS);
  glColor3f(color[0], color[1], color[2]);
  glVertex3f(v.x, v.y, v.z);
  glEnd();

  glEnable (GL_LIGHTING);
}

// --------------------------------------------------------------------

// Método para ajustar la velocidad de las animaciones 
void ajustarVelocidades(float escala){
  // NIVEL A: SOL
    velocidad_rot_sol = 0.7 * escala;

  // NIVEL B: PLANETAS
    // Velocidad de movimiento
      velocidad_gemelos = 0.5 * escala;
      velocidad_lumbre = 0.22 * escala;
      velocidad_hondonada = 0.17 * escala;
      velocidad_abismo = 0.1 * escala;

    // Velocidad de rotación
      velocidad_rot_gemelos = 0.5 * escala;           // Rotación individual
      velocidad_rot_gemelos_reloj = 0.75 * escala;    // Rotación en z entre ambos planetas
      velocidad_rot_lumbre = 0.3 * escala;
      velocidad_rot_hondonada = 0.2 * escala;
      velocidad_rot_abismo = 0.2 * escala;

    // SUBNIVEL B.1: ANIMACIÓN GEMELOS RELOJ DE ARENA
      escala_arena_crece = 0.00015 * escala;     // Velocidad de crecimiento de la arena
      escala_arena_decrece = 0.0003 * escala;    // Velocidad de decrecimiento de la arena. Más rápida para fluidez     
      escala_cascada = 0.002 * escala;           // Velocidad crecimiento/decrecimiento del cilindro


    // SUBNIVEL B.2: HONDONADA FŔAGIL 
        velocidad_trozo1 = 0.001 * escala_velocidad;
        velocidad_trozo2 = 0.001 * escala_velocidad;

  // NIVEL C: SATÉLITES
    // Rocaterra: Luna de lumbre
      velocidad_rocaterra = 5.0 * escala;
      velocidad_rot_rocaterra = 10.0 * escala;

    // Lanzadera
      velocidad_lanzadera_maxima = 0.38 * escala; // No es constante por el interruptor V
      aceleracion_lanzadera = 12.0 * escala; // Aceleración de la lanzadera

  // NIVEL D: EL INTRUSO 
    velocidad_intruso = 0.5 * escala_velocidad, // Velocidad de rotación del intruso
    velocidad_rotacion_intruso = 10.0 * escala_velocidad; // Velocidad de rotación del intruso


  // NIVEL E: NAVE
    // Movimiento de la nave
      velocidad_nave = 0.2 * escala;              // Velocidad de movimiento de la nave
      velocidad_despegue_nave = 15.0 * escala;     // Velocidad de movimiento de salida y entrada de los planetas

}

// --------------------------------------------------------------------

// CLASE: MATERIALES
// Valores por defecto del material
GLfloat initial_diffuse[4] = {0.8, 0.8, 0.8, 1.0};
GLfloat initial_specular[4] = {0.0, 0.0, 0.0, 1.0};
GLfloat initial_ambient[4] = {0.2, 0.2, 0.2, 1.0};
GLfloat initial_emission[4] = {0.0, 0.0, 0.0, 0.0};

class Material{
  private:
    GLfloat mat_diffuse[4];
    GLfloat mat_specular[4];
    GLfloat mat_ambient[4];
    GLfloat mat_shininess[1];
    GLfloat mat_emission[4];

  public:
    Material(){
      copiarColor(initial_diffuse, mat_diffuse);
      copiarColor(initial_specular, mat_specular);
      copiarColor(initial_ambient, mat_ambient);
      copiarColor(initial_emission, mat_emission);

      mat_shininess[0] = 0.0;
    }

    void setDiffuse(float color[]){
      copiarColor(color, mat_diffuse);
    }

    void setSpecular(float color[]){
      copiarColor(color, mat_specular);
    }

    void setAmbient(float color[]){
      copiarColor(color, mat_ambient);
    }

    void setShininess(float shininess){
      mat_shininess[0] = shininess;
    }

    void setEmission(float color[]){
      copiarColor(color, mat_emission);
    }

    void establecerMaterial(){
      glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
      glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
};

Material 
    materialPorDefecto, material1, material2, material3, materialazul, materialrojo, materiallila,
    sunMaterial, agujero_negro, material_abismo, 
    material_dorado, material_bronce, material_plata, 
    material_morado_brillante, material_azul_brillante, material_amarillo_brillante, material_blanco_brillante;

// Método para establecer los materiales
void crearMateriales(){
  {
    // Material 1: Azul con brillo intenso
    material1.setDiffuse(azul);       // Color difuso azul
    material1.setSpecular(blanco);    // Color especular blanco
    material1.setAmbient(azul);       // Color ambiente azul
    material1.setShininess(100.0);    // Brillo intenso

    // Material 2: Amarillo con brillo medio
    material2.setDiffuse(rojo);   // Color difuso amarillo
    material2.setSpecular(rojo);      // Color especular rojo
    material2.setAmbient(rojo);   // Color ambiente amarillo
    material2.setShininess(100.0);     // Brillo medio

    // Material 3: Morado con brillo suave
    material3.setDiffuse(morado);     // Color difuso morado
    material3.setSpecular(blanco);    // Color especular marrón
    material3.setAmbient(rojo);       // Color ambiente rojo
    material3.setShininess(20.0);     // Brillo suave

    // Material azul
    materialazul.setDiffuse(azul);     // Color difuso azul
    materialazul.setSpecular(azul);  // Color especular blanco
    materialazul.setAmbient(azul);     // Color ambiente azul
    materialazul.setShininess(50.0);  // Brillo intenso

    // Material rojo
    materialrojo.setDiffuse(rojo);     // Color difuso rojo
    materialrojo.setSpecular(rojo);  // Color especular rojo
    materialrojo.setAmbient(rojo);     // Color ambiente rojo
    materialrojo.setShininess(50.0);  // Brillo intenso

    // Material lila
    materiallila.setDiffuse(lila);     // Color difuso lila
    materiallila.setSpecular(lila);  // Color especular lila
    materiallila.setAmbient(lila);     // Color ambiente lila
    materiallila.setShininess(50.0);  // Brillo intenso
  }

  // SOL
    float diffuseSun[] = {1.0f, 0.8f, 0.0f, 1.0f}; // Amarillo cálido, completamente opaco
    float specularSun[] = {1.0f, 1.0f, 0.5f, 1.0f}; // Brillo blanco-amarillo
    float ambientSun[] = {0.8f, 0.5f, 0.0f, 1.0f}; // Luz ambiental tenue y cálida
    float shininessSun = 100.0f; // Brillantez alta para un efecto especular definido

    sunMaterial.setDiffuse(diffuseSun);
    sunMaterial.setSpecular(specularSun);
    sunMaterial.setAmbient(ambientSun);
    sunMaterial.setShininess(shininessSun);

  // ABISMO
    // Colores para el material
    float abismo_diffuse[] = {0.2f, 0.8f, 0.2f, 1.0f};
    float abismo_specular[] = {0.1f, 0.3f, 0.1f, 1.0f};
    float abismo_shininess = 10.0f;

    material_abismo.setDiffuse(abismo_diffuse);
    material_abismo.setSpecular(abismo_specular);
    material_abismo.setShininess(abismo_shininess);

    // Lanzadera
    //0.2125 	0.1275 	0.054 	0.714 	0.4284 	0.18144 	0.393548 	0.271906 	0.166721 	0.2

    float bronce_ambient[] = {0.2125f, 0.1275f, 0.054f};
    float bronce_diffuse[] = {0.714f, 0.4284f, 0.18144f};
    float bronce_specular[] = {0.393548f, 0.271906f, 0.166721f};
    float bronce_shininess = 0.2f;

    material_bronce.setAmbient(bronce_ambient);
    material_bronce.setDiffuse(bronce_diffuse);
    material_bronce.setSpecular(bronce_specular);
    material_bronce.setShininess(bronce_shininess);



  // Material dorado
    float dorado_ambient[] = {0.24725f, 0.1995f, 0.0745f};
    float dorado_diffuse[] = {0.75164f, 0.60648f, 0.22648f};
    float dorado_specular[] = {0.628281f, 0.555802f, 0.366065f};
    float dorado_shininess = 100.0f;

    material_dorado.setAmbient(dorado_ambient);
    material_dorado.setDiffuse(dorado_diffuse);
    material_dorado.setSpecular(dorado_specular);
    material_dorado.setShininess(dorado_shininess);

  // Material morado brillante
    GLfloat morado_ambiental[] = {0.1f, 0.0f, 0.1f, 1.0f};    // Morado base tenue
    GLfloat morado_difuso[] = {0.5f, 0.0f, 0.5f, 1.0f};       // Morado pastel
    GLfloat morado_especular[] = {0.7f, 0.0f, 0.7f, 1.0f};    // Brillo tenue morado
    GLfloat morado_emission[] = {1.0f, 0.0f, 1.0f, 1.0f};     // Emisión morada tenue
    GLfloat morado_brillo = 10.0f;                             // Brillo medio-alto

    material_morado_brillante.setAmbient(morado_ambiental);
    material_morado_brillante.setDiffuse(morado_difuso);
    material_morado_brillante.setSpecular(morado_especular);
    material_morado_brillante.setShininess(morado_brillo);
    material_morado_brillante.setEmission(morado_emission);

  // Material azul brillante
    GLfloat azul_ambiental[] = {0.1f, 0.15f, 0.2f, 1.0f};    // Azul base tenue
    GLfloat azul_difuso[] = {0.3f, 0.4f, 0.6f, 1.0f};        // Azul pastel suave
    GLfloat azul_especular[] = {0.5f, 0.5f, 0.7f, 1.0f};     // Brillo tenue azul
    GLfloat azul_emission[] = {0.5f, 0.9f, 1.0f, 1.0f};      // Emisión azul intensa
    GLfloat azul_brillo = 100.0f;                             // Brillo medio-alto

    material_azul_brillante.setAmbient(azul_ambiental);
    material_azul_brillante.setDiffuse(azul_difuso);
    material_azul_brillante.setSpecular(azul_especular);
    material_azul_brillante.setShininess(azul_brillo);
    material_azul_brillante.setEmission(azul_emission);

  // Material amarillo brillante
    GLfloat amarillo_ambiental[] = {0.1f, 0.1f, 0.0f, 1.0f};    // Amarillo base tenue
    GLfloat amarillo_difuso[] = {0.5f, 0.5f, 0.0f, 1.0f};       // Amarillo pastel
    GLfloat amarillo_especular[] = {0.7f, 0.7f, 0.0f, 1.0f};    // Brillo tenue amarillo
    GLfloat amarillo_emission[] = {1.0f, 1.0f, 0.0f, 1.0f};     // Emisión amarilla tenue
    GLfloat amarillo_brillo = 100.0f;                           // Brillo medio-alto

    material_amarillo_brillante.setAmbient(amarillo_ambiental);
    material_amarillo_brillante.setDiffuse(amarillo_difuso);
    material_amarillo_brillante.setSpecular(amarillo_especular);
    material_amarillo_brillante.setShininess(amarillo_brillo);
    material_amarillo_brillante.setEmission(amarillo_emission);

  // Material blanco brillante
    GLfloat blanco_ambiental[] = {0.1f, 0.1f, 0.1f, 1.0f};    // Blanco base tenue
    GLfloat blanco_difuso[] = {0.5f, 0.5f, 0.5f, 1.0f};       // Blanco pastel
    GLfloat blanco_especular[] = {0.7f, 0.7f, 0.7f, 1.0f};    // Brillo tenue blanco
    GLfloat blanco_emission[] = {1.0f, 1.0f, 1.0f, 1.0f};     // Emisión blanca tenue

    material_blanco_brillante.setAmbient(blanco_ambiental);
    material_blanco_brillante.setDiffuse(blanco_difuso);
    material_blanco_brillante.setSpecular(blanco_especular);
    material_blanco_brillante.setShininess(100.0f);
    material_blanco_brillante.setEmission(blanco_emission);

  // Material plateado
    GLfloat plateado_ambiental[] = {0.19225f, 0.19225f, 0.19225f, 1.0f};    // Color base tenue
    GLfloat plateado_difuso[] = {0.50754f, 0.50754f, 0.50754f, 1.0f};       // Plateado
    GLfloat plateado_especular[] = {0.508273f, 0.508273f, 0.508273f, 1.0f};    // Brillo intenso plateado
    GLfloat plateado_brillo = 0.4f;     

    material_plata.setAmbient(plateado_ambiental);
    material_plata.setDiffuse(plateado_difuso);
    material_plata.setSpecular(plateado_especular);
    material_plata.setShininess(plateado_brillo);
                        
}

// --------------------------------------------------------------------

// NIVEL B: PLANETAS
  void drawOrbitaCircular(float radio){
    float radianes= 0.0;

    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_LOOP);
      glColor3f(0.5, 0.5, 0.5);

      glPushMatrix();
      // Calculamos el vértice de los 360 ángulos
      for(int i = 0; i < 360; i++){
        radianes= i * M_PI / 180.0;
        glVertex3f(radio * cos(radianes), 0, radio * sin(radianes));
      }
      glPopMatrix();
    glEnd();

    glEnable(GL_LIGHTING);
  }

  void drawOrbitaElipse(float a, float b) {
    float radianes = 0.0;
    glDisable(GL_LIGHTING);

    glBegin(GL_LINE_LOOP);
      glColor3f(0.5, 0.5, 0.5);
      glPushMatrix();
        // Generamos los vértices
        for (int i = 0; i < 360; i++) {
          radianes = i * M_PI / 180.0;
          float x = a * cos(radianes); // Eje mayor en X
          float z = b * sin(radianes); // Eje menor en Z
          glVertex3f(x, 0, z);
        }
      glPopMatrix();
    glEnd();

    glEnable(GL_LIGHTING);
  }

// --------------------------------------------------------------------

// NIVEL C: SATÉLITES
  // LANZADERA
    // Devuelve una dirección aleatoria desde el origen hasta el exterior de una semiesfera( x <= 0.0 )
    Vertice generarDireccionAleatoria() {
      random_device rd;
      mt19937 gen(rd());
      uniform_real_distribution<float> dist(-1.0f, 1.0f);

      Vertice out;
      do {
          out.x = dist(gen);
          out.y = dist(gen);
          out.z = dist(gen);
      } while ((out.x * out.x + out.y * out.y + out.z * out.z > 1.0f) || (out.x < 0.0f)); // Asegurarse de que esté dentro de la semiesfera y x <= 0

      out.normalizar();

      return out;
    }

    float generarAnguloAleatorio() {
      random_device rd;
      mt19937 gen(rd());
      uniform_real_distribution<float> dist(0.0f, 360.0f);
      return dist(gen);
    }

// --------------------------------------------------------------------

// NIVEL E: NAVE

// POSICIONAMIENTO DE LOS ASTROS
  // Devuelve la posición de los astros rastreados
  Vertice obtenerPosicionPlaneta(int indice){
    switch (indice){
      case 0: return Vertice(0.0f, 0.0f, 0.0f); // El sol
      case 1: return posicion_ceniza;
      case 2: return posicion_brasas;
      case 3: return posicion_lumbre;
      case 4: return posicion_hondonada;
      case 5: return posicion_abismo;
      case 6: return posicion_agujero;
      default: return Vertice(0.0f, -99.9f, 0.0f);
    }
  }

  Vertice calcularPosicionOrbita(float radio, float angulo){
    return Vertice(radio * cos(2* M_PI - angulo * M_PI / 180.0), 0.0f, radio * sin(2* M_PI - angulo * M_PI / 180.0));
  }


// Calcula si el módulo del vector que los une es inferior a una distancia
bool boolColision(Vertice punto1, Vertice punto2, float distancia){
  Vertice separacion = punto1 - punto2;
  float modulo = sqrt(pow(separacion.x, 2) + pow(separacion.y, 2) + pow(separacion.z, 2));
  return modulo < distancia;
}

// Comprueba que la nave no colisione con algún astro en un instante determinado
bool boolColisionNaveInstante(Vertice dir, float n){
  int colisiones = 0;
  float radio_nave = 0.2; // Radio de la nave
  // n = 0: si está chocando ahora mismo
  // n > 0: si chocará en n ciclos
  
  // Calculamos posiciones en el instante n
  Vertice posicion_nave_temp = posicion_real_nave;
  if(n != 0){
    posicion_nave_temp.x += dir.x * velocidad_nave * delta * n;
    posicion_nave_temp.y += dir.y * velocidad_nave * delta * n;
    posicion_nave_temp.z += dir.z * velocidad_nave * delta * n;
  }

  // Si choca con el Sol, no hay posibilidad de salvarse
  return boolColision(posicion_nave_temp, Vertice(0.0f, 0.0f, 0.0f), radio_sol + radio_nave);

  Vertice 
      posicion_ceniza_temp = posicion_ceniza,
      posicion_brasas_temp = posicion_brasas,
      posicion_lumbre_temp = posicion_lumbre,
      posicion_hondonada_temp = posicion_hondonada,
      posicion_abismo_temp = posicion_abismo;

  // Gemelos reloj de arena
    float rotacion_gemelos_reloj_temp = rotacion_gemelos_reloj + velocidad_rot_gemelos_reloj * delta * n;
    posicion_ceniza_temp.x = (DIST_CENTROS_GEMELOS/2) * sin(2 * M_PI - (rotacion_gemelos_reloj_temp * M_PI / 180.0));
    posicion_ceniza_temp.y = (DIST_CENTROS_GEMELOS/2) * cos(2 * M_PI - (rotacion_gemelos_reloj_temp * M_PI / 180.0));
    posicion_brasas_temp.x = (DIST_CENTROS_GEMELOS/2) * sin(M_PI - (rotacion_gemelos_reloj_temp * M_PI / 180.0));
    posicion_brasas_temp.y = (DIST_CENTROS_GEMELOS/2) * cos(M_PI - (rotacion_gemelos_reloj_temp * M_PI / 180.0));

    float radio_ceniza, radio_brasas;
    radio_ceniza = RADIO_ORBITA_GEMELOS + posicion_ceniza.x;
    posicion_ceniza_temp.x = calcularPosicionOrbita(radio_ceniza,angulo_gemelos).x;
    posicion_ceniza_temp.z = calcularPosicionOrbita(radio_ceniza,angulo_gemelos).z;

    radio_brasas = RADIO_ORBITA_GEMELOS + posicion_brasas.x;
    posicion_brasas_temp.x = calcularPosicionOrbita(radio_brasas,angulo_gemelos).x;
    posicion_brasas_temp.z = calcularPosicionOrbita(radio_brasas,angulo_gemelos).z;

  // Lumbre
    posicion_lumbre_temp = calcularPosicionOrbita(RADIO_ORBITA_LUMBRE, angulo_lumbre * delta * n);

  // Hondonada
    posicion_hondonada_temp = calcularPosicionOrbita(RADIO_ORBITA_HONDONADA, angulo_hondonada * delta * n);

  // Abismo
    posicion_abismo_temp = calcularPosicionOrbita(RADIO_ORBITA_ABISMO, angulo_abismo * delta * n);

  // El único planeta con el que no se chocaría sería el planeta al que se dirige
  if(indice_planeta_navegacion != 1 && boolColision(posicion_nave_temp, posicion_ceniza_temp, radio_ceniza + radio_nave)){
    colisiones++;
  }

  if(indice_planeta_navegacion != 2 && boolColision(posicion_nave_temp, posicion_brasas_temp, radio_brasas + radio_nave)){
    colisiones++;
  }

  if(indice_planeta_navegacion != 3 && boolColision(posicion_nave_temp, posicion_lumbre_temp, radio_lumbre + radio_nave)){
    colisiones++;
  }

  if(indice_planeta_navegacion != 4 && boolColision(posicion_nave_temp, posicion_hondonada_temp, radio_hondonada + radio_nave)){
    colisiones++;
  }

  if(indice_planeta_navegacion != 5 && boolColision(posicion_nave_temp, posicion_abismo_temp, radio_abismo + radio_nave)){
    colisiones++;
  }

  return colisiones > 0;
}

// --------------------------------------------------------------------

// Método para establecer un color de selección
void ColorSeleccion( int i, int componente){
	unsigned char r = (i & 0xFF);
	unsigned char g = (componente & 0xFF);
	glColor3ub(r,g,0);
}

// ********************************************************************
// CLASE OBJETO3D: Implementación de la clase Objeto3D
// --------------------------------------------------------------------

  // Constructor por defecto
  Objeto3D::Objeto3D(){
      // Valores por defecto
      copiarColor(blanco, color);
      normales_on = false;
      forma = GL_FRONT;
      tipo = GL_AMBIENT_AND_DIFFUSE;

      // Inicializamos los vectores a vacío
      vertices = vector<Vertice>();
      normales_c = vector<Vertice>();
      normales_v = vector<Vertice>();
      caras = vector<Cara>();

      textura_id = 0; // solo tiene valor cuando se llama a la función asignarTextura
      coordenadas_textura = vector<pair<float,float>>();
  }

  // Constructor: recibe el color, los vértices, las caras, la forma y el tipo. Crea las normales
  Objeto3D::Objeto3D(const float color[], vector<Vertice> verts, vector<Cara> car, int forma, int tipo){
    copiarColor(color, this->color);
    vertices = verts;
    caras = car;
    this->forma = forma;
    this->tipo = tipo;
    normales_on = false;

    // Calculamos las normales
    calcularNormalesCaras();
    calcularNormalesVertices();

    textura_id = 0; // solo tiene valor cuando se llama a la función asignarTextura
  }
  
  // Dibuja el objeto3D
  void Objeto3D::draw(){
    // Comprobamos si está activada la selección y luego la luz
    if(!seleccionActivada){
      materialPorDefecto.establecerMaterial(); // Establecemos el material por defecto
      glMaterialfv(forma, tipo, color); // Usar el material si la iluminación está activada
    }
    else{
      ColorSeleccion(n_seleccion, 0);
      n_seleccion++;
    }

    // Dibujamos el objeto
    glBegin(GL_TRIANGLES);

    for(long long i = 0; i < caras.size(); i++){
    // Establecemos la normal correspondiente a la cara i
      glNormal3f(normales_c[i].x, normales_c[i].y, normales_c[i].z);
    // Enviamos los vértices de la cara i
      glVertex3f(vertices[caras[i].v1].x, vertices[caras[i].v1].y, vertices[caras[i].v1].z);
      glVertex3f(vertices[caras[i].v2].x, vertices[caras[i].v2].y, vertices[caras[i].v2].z);
      glVertex3f(vertices[caras[i].v3].x, vertices[caras[i].v3].y, vertices[caras[i].v3].z);
    }

    glEnd();

    // Dibujar las normales si está activada su visualización
    if(normales_on){
      this->dibujarNormales();
    }
  }

  // Dibuja el objeto3D en modo SMOOTH
  void Objeto3D::drawSMOOTH(){
    // Comprobamos si está activada la selección y luego la luz
    if(!seleccionActivada){
      glColor3f(color[0], color[1], color[2]); // Usar el color si la iluminación está desactivada
    }
    else{
      ColorSeleccion(n_seleccion, 0);
      n_seleccion++;
    }

    glShadeModel(GL_SMOOTH);

    // Dibujamos el objeto
    glBegin(GL_TRIANGLES);

    for(long long i = 0; i < caras.size(); i++){
      // Establecemos las normales de cada vértice antes de enviarlo
      glNormal3f(normales_v[caras[i].v1].x, normales_v[caras[i].v1].y, normales_v[caras[i].v1].z);
      glVertex3f(vertices[caras[i].v1].x, vertices[caras[i].v1].y, vertices[caras[i].v1].z);

      glNormal3f(normales_v[caras[i].v2].x, normales_v[caras[i].v2].y, normales_v[caras[i].v2].z);
      glVertex3f(vertices[caras[i].v2].x, vertices[caras[i].v2].y, vertices[caras[i].v2].z);

      glNormal3f(normales_v[caras[i].v3].x, normales_v[caras[i].v3].y, normales_v[caras[i].v3].z);
      glVertex3f(vertices[caras[i].v3].x, vertices[caras[i].v3].y, vertices[caras[i].v3].z);
    }

    glEnd();

    // Dibujar las normales si está activada su visualización
    if(normales_on){
      this->dibujarNormales();
    }

  }

  // Modifica la visualización de las normales (variable normales_on)
  void Objeto3D::activarNormales(){
    if(normales_on){
      normales_on = false;
    }
    else{
      normales_on = true;
    }
  }

  // Calcula las normales de las caras
  void Objeto3D::calcularNormalesCaras(){
    // Nos aseguramos de que el vector de vértices y de caras no estén vacíos
    if(vertices.empty() == false && caras.empty() == false){

      for(const Cara& cara : caras){
        // Obtener los vértices de la cara
        Vertice v1 = vertices[cara.v1];
        Vertice v2 = vertices[cara.v2];
        Vertice v3 = vertices[cara.v3];

        // Calcular los vectores de los lados
        Vertice lado1 = v2 - v1;
        Vertice lado2 = v3 - v1;

        // Calcular la normal y la normalizamos
        Vertice normal = lado1 * lado2;
        normales_c.push_back(normal.normalizar());
      }
    }
    // Si están vacíos, añadimos normales vacías
    else{
      normales_c = normales_vacias;
    }
  }

  // Calcula las normales de los vértices
  void Objeto3D::calcularNormalesVertices(){
    Vertice norm_v(0.0,0.0,0.0);

    // Nos aseguramos de que el vector de vértices y de caras no estén vacíos
    if(vertices.empty() == false && caras.empty() == false){
      // Paso 1: Crear lista de adyacencias
      vector<list<long long>> adyacencias(vertices.size());
      for (long long j = 0; j < caras.size(); j++) {
          adyacencias[caras[j].v1].push_back(j);
          adyacencias[caras[j].v2].push_back(j);
          adyacencias[caras[j].v3].push_back(j);
      }

      // Paso 2: Calcular las normales de los vértices
      normales_v.clear();
      normales_v.reserve(vertices.size());
      for (long long i = 0; i < vertices.size(); i++) {
          Vertice norm_v(0.0, 0.0, 0.0);
          for (long long caraIdx : adyacencias[i]) {
              norm_v = norm_v + normales_c[caraIdx];
          }
          normales_v.push_back(norm_v.normalizar());
      }

    }
    // Si están vacíos, añadimos normales vacías
    else{
      normales_v = normales_vacias;
    }
  }

  // Dibuja las normales
  void Objeto3D::dibujarNormales(){

    // Dibujamos las normales de las caras
    for(long long i=0; i < caras.size(); i++){
      Vertice centro = (vertices[caras[i].v1] + vertices[caras[i].v2] + vertices[caras[i].v3]) / 3;
      drawVector(centro, normales_c[i], 3, blanco);
    }

    // Dibujamos las normales de los vértices
    for(long long i=0; i < vertices.size(); i++){
      drawVector(vertices[i], normales_v[i], 3, rojo);
    }

  }

  // Lee una textura
  void Objeto3D::asignarTextura(const char *archivo){
    textura = LeerArchivoJPEG(archivo, ancho, alto);

    glGenTextures(1, &textura_id);
    glBindTexture(GL_TEXTURE_2D, textura_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, textura);

    // Nota: minimaps no disponibles. Optar por usar otra función

    delete[] textura;
  }

// --------------------------------------------------------------------
// ********************************************************************

// CLASE MALLA DE TRIÁNGULOS - HEREDA DE OBJETO3D
class Malla : public Objeto3D {
  private:
    Material material;
    string nombre, descripcion;
    int i_seleccion;

  public:
    // Constructor por defecto
    Malla() : Objeto3D(){
      material = materialPorDefecto;
      nombre = "sin definir";
      descripcion = "no hay información disponible";
      i_seleccion = n_seleccion;
      nombres[n_seleccion] = nombre;
      descripciones[n_seleccion] = descripcion;
      n_seleccion++;
    }

    Malla(const float color[], int forma, int tipo, vector<Vertice> verts, vector<Cara> c, Material mat,vector<pair<float,float>> coord_text, string nom, int sel)
      : Objeto3D(color, verts, c, forma, tipo){
        material = mat;
        nombre = nom;
        i_seleccion = sel;
        if(coord_text.empty()){
          coordenadas_textura = vector<pair<float,float>>();
        }
        else{
          coordenadas_textura = coord_text;
        }
    }

    // Aplica el archivo PLY modificando el vector de vértices y de caras
    void generarPLY(char *archivoPLY){
      vector<float> vertices_ply; // coordenadas de vértices
      vector<int> caras_ply; // índices de los vértices de las caras

      // Leemos el archivo PLY
      ply::read(archivoPLY, vertices_ply, caras_ply);

      // Nos aseguramos de que los vectores estén vacíos
      vertices.clear();
      caras.clear();

      // Transformamos los datos para que sean compatibles con los vectores vertices y caras
      for(long long i = 0; i < vertices_ply.size(); i+=3){
        vertices.push_back(Vertice(vertices_ply[i], vertices_ply[i+1], vertices_ply[i+2]));
      }
      for(long long i = 0; i < caras_ply.size(); i+=3){
        caras.push_back(Cara(caras_ply[i], caras_ply[i+1], caras_ply[i+2]));
      }

      // Actualizamos las normales
      calcularNormalesCaras();
      calcularNormalesVertices();
    }

    void setMaterial(Material mat){
      material = mat;
    }

    void setNombre(string nom){
      nombre.clear();
      nombre = nom;
      nombres[i_seleccion] = nombre;
    }

    void setDescripcion(string desc){
      descripcion.clear();
      descripcion = desc;
      descripciones[i_seleccion] = descripcion;
    }

    string getNombre(){
     return nombre;
    }

    void draw() override{
      if(seleccionActivada){
        glDisable(GL_LIGHTING);
        ColorSeleccion(i_seleccion, 0);
      }
      else{
        glEnable(GL_LIGHTING);
        material.establecerMaterial();
      }

      // Si el método de selección está activado
      if(seleccionActivada || textura_id == 0){

        // Dibujamos el objeto
        glBegin(GL_TRIANGLES);

        for(long long i = 0; i < caras.size(); i++){
        // Establecemos la normal correspondiente a la cara i
          glNormal3f(normales_c[i].x, normales_c[i].y, normales_c[i].z);
        // Enviamos los vértices de la cara i
          glVertex3f(vertices[caras[i].v1].x, vertices[caras[i].v1].y, vertices[caras[i].v1].z);
          glVertex3f(vertices[caras[i].v2].x, vertices[caras[i].v2].y, vertices[caras[i].v2].z);
          glVertex3f(vertices[caras[i].v3].x, vertices[caras[i].v3].y, vertices[caras[i].v3].z);
        }

        glEnd();
      }
      else{
        // Activamos las texturas y dibujamos el objeto
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura_id);

        glBegin(GL_TRIANGLES);

        for(long long i = 0; i < caras.size(); i++){
        // Establecemos la normal correspondiente a la cara i
          glNormal3f(normales_c[i].x, normales_c[i].y, normales_c[i].z);
        // Primeor su coordenada correspondiente (x: i*3, y: i*3+1, z: i*3+2) y luego los vértices
          glTexCoord2f(coordenadas_textura[i*3].first, coordenadas_textura[i*3].second);
            glVertex3f(vertices[caras[i].v1].x, vertices[caras[i].v1].y, vertices[caras[i].v1].z);
          glTexCoord2f(coordenadas_textura[i*3+1].first, coordenadas_textura[i*3+1].second);
            glVertex3f(vertices[caras[i].v2].x, vertices[caras[i].v2].y, vertices[caras[i].v2].z);
          glTexCoord2f(coordenadas_textura[i*3+2].first, coordenadas_textura[i*3+2].second);
            glVertex3f(vertices[caras[i].v3].x, vertices[caras[i].v3].y, vertices[caras[i].v3].z);
        }

        glEnd();

        // Desactivamos las texturas
        glDisable(GL_TEXTURE_2D);
      }

      // Dibujar las normales si está activada su visualización
      if(normales_on){
        this->dibujarNormales();
      }

    }

    // Dibuja el objeto3D en modo SMOOTH
    void drawSMOOTH() override{
      if(seleccionActivada){
        glDisable(GL_LIGHTING);
        ColorSeleccion(i_seleccion, 0);
      }
      else{
        glEnable(GL_LIGHTING);
        material.establecerMaterial();
      }

      // Si el método de selección está activado
      if(seleccionActivada){
        // Dibujamos el objeto
        glBegin(GL_TRIANGLES);

        for(long long i = 0; i < caras.size(); i++){
        // Establecemos la normal correspondiente a la cara i
          glNormal3f(normales_c[i].x, normales_c[i].y, normales_c[i].z);
        // Enviamos los vértices de la cara i
          glVertex3f(vertices[caras[i].v1].x, vertices[caras[i].v1].y, vertices[caras[i].v1].z);
          glVertex3f(vertices[caras[i].v2].x, vertices[caras[i].v2].y, vertices[caras[i].v2].z);
          glVertex3f(vertices[caras[i].v3].x, vertices[caras[i].v3].y, vertices[caras[i].v3].z);
        }

        glEnd();
      }
      else{
        if(textura_id != 0){
          // Activamos las texturas y dibujamos el objeto
          glEnable(GL_TEXTURE_2D);
          glBindTexture(GL_TEXTURE_2D, textura_id);

          // Cambiamos el modo de sombreado a SMOOTH
          glShadeModel(GL_SMOOTH);

          glBegin(GL_TRIANGLES);

          for(long long i = 0; i < caras.size(); i++){
          // Primero su coordenada correspondiente (x: i*3, y: i*3+1, z: i*3+2) y luego los vértices
            glTexCoord2f(coordenadas_textura[i*3].first, coordenadas_textura[i*3].second);
              glNormal3f(normales_v[caras[i].v1].x, normales_v[caras[i].v1].y, normales_v[caras[i].v1].z); // para el Smooth
              glVertex3f(vertices[caras[i].v1].x, vertices[caras[i].v1].y, vertices[caras[i].v1].z);
            glTexCoord2f(coordenadas_textura[i*3+1].first, coordenadas_textura[i*3+1].second);
              glNormal3f(normales_v[caras[i].v2].x, normales_v[caras[i].v2].y, normales_v[caras[i].v2].z);
              glVertex3f(vertices[caras[i].v2].x, vertices[caras[i].v2].y, vertices[caras[i].v2].z);
            glTexCoord2f(coordenadas_textura[i*3+2].first, coordenadas_textura[i*3+2].second);
              glNormal3f(normales_v[caras[i].v3].x, normales_v[caras[i].v3].y, normales_v[caras[i].v3].z);
              glVertex3f(vertices[caras[i].v3].x, vertices[caras[i].v3].y, vertices[caras[i].v3].z);
          }

          glEnd();

          // Desactivamos las texturas
          glDisable(GL_TEXTURE_2D);
        }
        else{
          // Cambiamos el modo de sombreado a SMOOTH
          glShadeModel(GL_SMOOTH);

          glBegin(GL_TRIANGLES);

          for(long long i = 0; i < caras.size(); i++){
            glNormal3f(normales_v[caras[i].v1].x, normales_v[caras[i].v1].y, normales_v[caras[i].v1].z); // para el Smooth
            glVertex3f(vertices[caras[i].v1].x, vertices[caras[i].v1].y, vertices[caras[i].v1].z);
            glNormal3f(normales_v[caras[i].v2].x, normales_v[caras[i].v2].y, normales_v[caras[i].v2].z);
            glVertex3f(vertices[caras[i].v2].x, vertices[caras[i].v2].y, vertices[caras[i].v2].z);
            glNormal3f(normales_v[caras[i].v3].x, normales_v[caras[i].v3].y, normales_v[caras[i].v3].z);
            glVertex3f(vertices[caras[i].v3].x, vertices[caras[i].v3].y, vertices[caras[i].v3].z);
          }

          glEnd();

        }
      }
    }

    void calcularCoordenadasPlano(){
      if(textura !=0){
        // Calcular las coordenadas de textura (método sencillo)
        for(long long i = 0; i < caras.size(); i++){
          // Todas las caras tienen las mismas coordenadas
          coordenadas_textura.push_back(make_pair(0.0, 0.0));
          coordenadas_textura.push_back(make_pair(1.0, 0.0));
          coordenadas_textura.push_back(make_pair(0.0, 1.0));
        }

      }
      else{
        cout << "No se ha asignado textura" << endl;
      }
    }

    void calcularCoordenadasCirculares(){
      // Esta función tiene errores. Se resuelven de forma manual
        // en caso de una esfera con más divisiones, se tendría que hacer una adaptación
      float u, v, u_min, u_max;
      int n_cara = 0;
      bool modificar = false;

      for(Cara& cara : caras){
        for(int i = 0; i < 3; i++){
          Vertice vertex = vertices[cara[i]];

          // Normalizar el vértice
            float length = sqrt(vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z);
            float nx = vertex.x / length;
            float ny = vertex.y / length;
            float nz = vertex.z / length;

            // Convertir a coordenadas esféricas
            float theta = atan2(nx, nz);
            float phi = acos(ny);

            if (phi < 1e-6 || fabs(phi - M_PI) < 1e-6) {
              u = 0.5;
            } else {
              // Mapear a coordenadas de textura
              u = (theta + M_PI) / (2 * M_PI);
            }
            v = phi / M_PI;

          // Buscamos las caras que abarcan la imagen entera
          if(u < u_min)
            u_min = u;
          if(u > u_max)
            u_max = u;

          if(vertex.x < -2.16067e-07)
          // Nos sirve para identificar los vértices que fallan
              modificar = true;

          coordenadas_textura.push_back(make_pair(u, v));
        }

        if(modificar && (u_max - u_min) > 0.9999999999){
          for(int i = 0; i < 3; i++){
            Vertice vertex = vertices[cara[i]];

            // Modificamos el vértice erróneo
            if(vertex.x < 2.16067e-07 && vertex.x > -2.16067e-07)
              coordenadas_textura[n_cara * 3 + i].first = 0.0;
          }
          modificar = false;
      
        }
        u_min = 0;
        u_max = 0;
        n_cara++;
      }
    }

} ;

// --------------------------------------------------------------------
//======================================================================

//============================== OBJETOS ===============================
Malla
    sol,
    gemelo_ceniza,
    gemelo_brasas,
    lumbre,
    hondonada_fragil,
    abismo_gigante,
    agujero_blanco,
    nave,
    intruso, 
    cascada, arena1, arena2,                            // Gemelos reloj de arena
    rocaterra,                                          // Lumbre
    anillo1, anillo2, trozo1, trozo2, trozo3, trozo4,   // Hondonada frágil
    lanzadera, base_lanzadera,                          // Abismo del gigante
    intruso_cola,
    nave_cristal;


//===================== INICIO DEL MODELO Y DIBUJO ======================

void initModel(){
  // CARGA DE LOS ARCHIVOS PLY
    char
      archivo_esfera[] = "PLY/modelos/esfera.ply",                // Modelo básico esfera

      archivo_cilindro[] = "PLY/modelos/cilindro.ply",            // Cascada gemelos

      archivo_anillo1[] = "PLY/modelos/hondonada/anillo1.ply",    // Anillo hondonada
      archivo_anillo2[] = "PLY/modelos/hondonada/anillo2.ply",    // Anillo hondonada
      archivo_trozo1[] = "PLY/modelos/hondonada/trozo1.ply",      // Trozo hondonada
      archivo_trozo2[] = "PLY/modelos/hondonada/trozo2.ply",      // Trozo hondonada
      archivo_trozo3[] = "PLY/modelos/hondonada/trozo3.ply",      // Trozo hondonada
      archivo_trozo4[] = "PLY/modelos/hondonada/trozo4.ply",      // Trozo hondonada

      archivo_b_lanzadera[] = "PLY/modelos/base_lanzadera.ply",   // Base lanzadera
      archivo_lanzadera[] = "PLY/cubo.ply",                       // Lanzadera

      archivo_nave[] = "PLY/modelos/nave.ply",                    // Nave
      archivo_nave_cristal[] = "PLY/modelos/nave_cristal.ply",    // Cristal de la nave

      archivo_intruso[] = "PLY/modelos/intruso_cabeza.ply",       // Intruso
      archivo_intruso_cola[] = "PLY/modelos/intruso_cola.ply";    // Cola del intruso


  // --------------------------------------------------------------------
  
  // CREACIÓN DE LOS MATERIALES
    crearMateriales();
  
  // --------------------------------------------------------------------

  // ESTABLECER LOS NOMBRES DE LOS ASTROS
    sol.setNombre("Sol");
    gemelo_ceniza.setNombre("Gemelo Ceniza");
    gemelo_brasas.setNombre("Gemelo Brasas");
      arena1.setNombre("Gemelo Ceniza");
      arena2.setNombre("Gemelo Brasas");
      cascada.setNombre("Cascada");
    lumbre.setNombre("Lumbre");
      rocaterra.setNombre("Rocaterra");
    hondonada_fragil.setNombre("Hondonada Frágil");
      anillo1.setNombre("Estación hondonada");
      anillo2.setNombre("Estación hondonada");
      trozo1.setNombre("Estación hondonada");
      trozo2.setNombre("Estación hondonada");
      trozo3.setNombre("Estación hondonada");
      trozo4.setNombre("Estación hondonada");
    abismo_gigante.setNombre("Abismo del Gigante");
      base_lanzadera.setNombre("Base Lanzadera");
      lanzadera.setNombre("Lanzadera");
    agujero_blanco.setNombre("Agujero Blanco");
    intruso.setNombre("Intruso");
    nave.setNombre("Nave espacial");
    nave_cristal.setNombre("Nave espacial");

  // --------------------------------------------------------------------

  // ESTABLECER LAS DESCRIPCIONES
    sol.setDescripcion("\n\tLa única estrella de sus sistema solar. Se trata de una enana\n\tamarilla, un tipo de estrella común en muchos sistemas solares.\n\tSu tamaño es mucho más pequeño en comparación con el Sol de la\n\tTierra. Mientras que el Sol tiene un diámetro de 696.340\n\tkilómetros este pequeño astro apenas alcanza los 4 kilómetros.\n\t\n\tEste Sol está en una fase avanzada de su vida y se encuentra\n\ten el proceso de colapso hacia una supernova.\n\tSi decides visitarlo, te recomendamos mantenerte a una distan-\n\tcia prudente, ya que su intensa radiación es peligrosa. Aunque\n\tsu resplandor es fascinante, acercarse demasiado podría ser\n\tfatal. ¡Así que disfruta de la vista, pero mantén la distancia!\n");
                                
    gemelo_ceniza.setDescripcion("\n\tGemelo Cenizas es uno de los planetas más intrigantes del\n\tsistema solar de Outer Wilds junto a su hermano, Gemelo Brasas.\n\tEstos dos pequeños mundos forman parte de una órbita binaria\n\tque los mantiene en constante movimiento. Son los planetas más\n\tcercanos al Sol y comparten una peculiaridad única en su siste-\n\tma: la transferencia de arena entre ellos.\n\n\n\tGemelo Cenizas es un planeta rocoso con un terreno árido y de-\n\tsolado pero fascinante para explorar. A medida que el tiempo\n\tavanza, la arena de este planeta se traslada a su hermano, Ge-\n\tmelo Brasas, revelando antiguas estructuras Nomai enterradas.\n\tCada 20 minutos, este ciclo se repite, volviendo a ocultar \n\ttodos sus misterios.\n\n\tSi decides visitar Gemelo Cenizas, asegúrate de mantenerte aler-\n\ta, ¡o acabarás succionado o enterrado por la arena!\n");    
    arena1.setDescripcion("\n\tGemelo Cenizas es uno de los planetas más intrigantes del\n\tsistema solar de Outer Wilds junto a su hermano, Gemelo Brasas.\n\tEstos dos pequeños mundos forman parte de una órbita binaria\n\tque los mantiene en constante movimiento. Son los planetas más\n\tcercanos al Sol y comparten una peculiaridad única en su siste-\n\tma: la transferencia de arena entre ellos.\n\n\n\tGemelo Cenizas es un planeta rocoso con un terreno árido y de-\n\tsolado pero fascinante para explorar. A medida que el tiempo\n\tavanza, la arena de este planeta se traslada a su hermano, Ge-\n\tmelo Brasas, revelando antiguas estructuras Nomai enterradas.\n\tCada 20 minutos, este ciclo se repite, volviendo a ocultar \n\ttodos sus misterios.\n\n\tSi decides visitar Gemelo Cenizas, asegúrate de mantenerte aler-\n\ta, ¡o acabarás succionado o enterrado por la arena!\n");   
    gemelo_brasas.setDescripcion("\n\tGemelo Brasas es uno de los planetas más intrigantes del sis-\n\ttema solar de Outer Wilds junto a su hermano, Gemelo Cenizas.\n\tEstos dos pequeños mundos forman parte de una órbita binaria\n\tque los mantiene en constante movimiento. Son los planetas más\n\tcercanos al Sol y comparten una peculiaridad única en su siste-\n\tma: la transferencia de arena entre ellos.\n\n\tGemelo Brasas es un planeta árido y severo, donde los acantila-\n\tdos se alzan como gigantescos muros de piedra, ofreciendo im-\n\tpresionantes vistas, pero también creando un entorno difícil de\n\ttransitar. La actividad volcánica constante cubre el planeta\n\tcon un aire abrasador y una sensación de peligro latente en\n\ttodo momento. A medida que el tiempo avanza, la arena de su \n\thermano, Gemelo Cenizas, se traslada a este planeta, cubriéndo-\n\tpor completo y ocultando sus cavernas repletas de secretos.\n\tCada 20 minutos, este ciclo se repite, liberando a las pocas\n\tplantas que sobreviven a estas condiciones.\n\n\tSi decides visitar Gemelo Brasas, asegúrate de mantenerte aler-\n\ta, ¡o acabarás succionado o enterrado por la arena!\n");
    arena2.setDescripcion("\n\tGemelo Brasas es uno de los planetas más intrigantes del sis-\n\ttema solar de Outer Wilds junto a su hermano, Gemelo Cenizas.\n\tEstos dos pequeños mundos forman parte de una órbita binaria\n\tque los mantiene en constante movimiento. Son los planetas más\n\tcercanos al Sol y comparten una peculiaridad única en su siste-\n\tma: la transferencia de arena entre ellos.\n\n\tGemelo Brasas es un planeta árido y severo, donde los acantila-\n\tdos se alzan como gigantescos muros de piedra, ofreciendo im-\n\tpresionantes vistas, pero también creando un entorno difícil de\n\ttransitar. La actividad volcánica constante cubre el planeta\n\tcon un aire abrasador y una sensación de peligro latente en\n\ttodo momento. A medida que el tiempo avanza, la arena de su \n\thermano, Gemelo Cenizas, se traslada a este planeta, cubriéndo-\n\tpor completo y ocultando sus cavernas repletas de secretos.\n\tCada 20 minutos, este ciclo se repite, liberando a las pocas\n\tplantas que sobreviven a estas condiciones.\n\n\tSi decides visitar Gemelo Brasas, asegúrate de mantenerte aler-\n\ta, ¡o acabarás succionado o enterrado por la arena!\n");
    cascada.setDescripcion("\n\tArena en movimiento entre los gemelos\n");

    lumbre.setDescripcion("\n\tLumbre es el segundo planeta más cercano al Sol y el más pare-\n\tcido a la Tierra. Su atmósfera es rica en oxígeno y su superficie\n\testá cubierta de bosques y montañas. La vida en Lumbre es abun-\n\tdante y variada, con especies como los lumbreanos. Estos seres\n\tson bípedos, de piel azul, con cuatro ojos y orejas largas.\n\tSon una especie pacífica y curiosa, que ha desarrollado una\n\tsociedad avanzada y tecnológica con un claro y único objetivo:\n\texplorar el espacio y descubrir los secretos del universo.\n\tSon la única especie inteligente conocida en el sistema solar\n\taunque existen vestigios de una antigua civilización, los Nomai.\n\n\tA pesar de ser un planeta acogedor, Lumbre no está exento de\n\tpeligros. Cuenta con una red de cavernas subterráneas, con mu-\n\tchos géiseres que proporcionan aberturas.\n\n\tDesde este planeta inicia la aventura de tu nave.\n");
    rocaterra.setDescripcion("\n\tRocaterra es el satélite de Lumbre. Es un planeta pequeño y\n\trocoso, con una atmósfera fina y una gravedad baja. Su super-\n\tficie está cubierta de cráteres y montañas, con un terreno\n\tirregular y difícil de transitar.\n\n\tA pesar de su aspecto inhóspito, Rocaterra cuenta con un\n\tpuesto de avanzada lumbreano.");

    hondonada_fragil.setDescripcion("\n\tSi antes había un planeta en Hondonada frágil, ya no queda ni\n\trastro de él. Se sospecha que la integridad de la superficie\n\tse vio comprometida por un impacto de un asteroide. Esto frac-\n\tturó la corteza y provocó un colapso en la estructura del pla-\n\tneta, que se desplomó hacia el interior (donde se encontraba\n\tel agujero negro).\n\tActualmente, el agujero lo rodea una estación espacial nomai en \n\truinas, cuya estabilidad amenaza por desaparecer. Si observas\n\tcon atención, podrás ver cómo algunos trozos caen al agujero y\n\tson transportados, quién sabe dónde.\n");
    anillo1.setDescripcion("\n\tEstación nomai completamente en ruinas. Apenas explorado\n.");
    anillo2.setDescripcion("\n\tEstación nomai completamente en ruinas. Apenas explorado\n.");
    trozo1.setDescripcion("\n\tEstación nomai completamente en ruinas. Apenas explorado\n.");
    trozo2.setDescripcion("\n\tEstación nomai completamente en ruinas. Apenas explorado\n.");
    trozo3.setDescripcion("\n\tEstación nomai completamente en ruinas. Apenas explorado\n.");
    trozo4.setDescripcion("\n\tEstación nomai completamente en ruinas. Apenas explorado\n.");


    abismo_gigante.setDescripcion("\n\tAbismo del Gigante es el planeta más grande del sistema y está\n\tcubierto por un inmenso, peligroso y profundo océano. Sus fuer-\n\ttes corrientes hacen imposible una exploración segura, pero \n\texisten rumores de unas ruinas en el fondo del océano.\n\tBuena suerte si decides intentarlo.\n\n\tFlotando sobre el agua encontrarás pequeñas islas en las que\n\tpodrás aterrizar... si es que consigues esquivar los ciclones\n\tque viajan por toda la superficie.\n\n\tAlrededor del planeta orbita una base lanzadera nomai cuyo\n\tobjetivo nos es desconocido. Recientemente se ha presenciado el\n\tdisparo de una lanzadera tras años de inactividad. ¿Cómo es po-\n\tsible que aún funcione? ¿Alguien la ha reactivado? ¿Hacia dónde\n\tse dirige la nave?\n");
    base_lanzadera.setDescripcion("\n\tBase lanzadera nomai destruida tras su último uso.\nApenas explorado.\n");
    lanzadera.setDescripcion("\n\tLanzadera nomai destruida tras el último uso. No explorado.\n\n\tWow. ¿Has conseguido darle a la nave? Enhorabuena. Te has gana-\n\tdo un SPOILER: está buscando el Ojo del Universo. ¿Qué qué es?\n\tBueno, por suerte existe el juego para descubrirlo.\n");

    agujero_blanco.setDescripcion("\n\tEl agujero blanco es otro de los grandes misterios de este sis-\n\ttema. Se trata de un fenómeno poco común: la antítesis de un\n\tagujero negro. Se desconoce su naturaleza.\n\n\tSe están observando movimientos cerca... ¿Eso son...? ¿Los \n\trestos de la estación de Hondonada frágil?\n");

    nave.setDescripcion("\n\tAlguien (no muy inteligente) a confiado en tí para que pilotes\n\testa auténtica obra de ingeniería. No te preocupes, no es tan \n\tcomplicado. Solo tienes que recordar que el botón rojo es el de \n\tautodestrucción. El piloto automático hará el resto\n");
    nave_cristal.setDescripcion("\n\tAlguien (no muy inteligente) a confiado en tí para que pilotes\n\testa auténtica obra de ingeniería. No te preocupes, no es tan \n\tcomplicado. Solo tienes que recordar que el botón rojo es el de \n\tautodestrucción. El piloto automático hará el resto\n");

    intruso.setDescripcion("\n\tEl Intruso es un asteroide de procedencia desconocida. No sabe-\n\tmos cuánto lleva orbitando el Sol, pero no parece ser tan an-\n\ttiguo como este. Está formado por hielo y algunos metales.\n\tApenas se ha explorado.\n");
    intruso_cola.setDescripcion("\n\tEl Intruso es un asteroide de procedencia desconocida. No sabe-\n\tmos cuánto lleva orbitando el Sol, pero no parece ser tan an-\n\ttiguo como este. Está formado por hielo y algunos metales.\n\tApenas se ha explorado.\n");

  // --------------------------------------------------------------------
  
  // CREACIÓN DE LOS MODELOS
  // NIVEL A: EXPLOSIÓN DEL SOL
    // Creación del Sol
      sol.generarPLY(archivo_esfera);
      sol.setMaterial(material_amarillo_brillante);
      sol.asignarTextura("text/astros/sol.jpg");
      sol.calcularCoordenadasCirculares();

  // NIVEL B: PLANETAS

    // Gemelos reloj de arena y sus animaciones
      // Gemelos
      gemelo_ceniza.generarPLY(archivo_esfera);
      gemelo_ceniza.asignarTextura("text/planetas/Ceniza.jpg");
      gemelo_ceniza.calcularCoordenadasCirculares();
      gemelo_brasas.generarPLY(archivo_esfera);
      gemelo_brasas.asignarTextura("text/planetas/Brasas.jpg");
      gemelo_brasas.calcularCoordenadasCirculares();
      // Arenas
      arena1.generarPLY(archivo_esfera);  // Arena que rodea Ceniza
      arena1.asignarTextura("text/planetas/arena.jpg");
      arena1.calcularCoordenadasPlano();
      arena2.generarPLY(archivo_esfera);  // Arena que rodea Brasas
      arena2.asignarTextura("text/planetas/arena.jpg");
      arena2.calcularCoordenadasPlano();

      // Cascada
      cascada.generarPLY(archivo_cilindro);
      cascada.asignarTextura("text/planetas/arena.jpg");
      cascada.calcularCoordenadasPlano();

    // Lumbre
      lumbre.generarPLY(archivo_esfera);
      lumbre.asignarTextura("text/planetas/Lumbre.jpg");
      lumbre.calcularCoordenadasCirculares();

      rocaterra.generarPLY(archivo_esfera);
      rocaterra.asignarTextura("text/planetas/rocaterra.jpg");
      rocaterra.calcularCoordenadasCirculares();

    // Hondonada frágil
      // Estación
      anillo1.generarPLY(archivo_anillo1);
      anillo2.generarPLY(archivo_anillo2);
      trozo1.generarPLY(archivo_trozo1);
      trozo2.generarPLY(archivo_trozo2);
      trozo3.generarPLY(archivo_trozo3);
      trozo4.generarPLY(archivo_trozo4);

      hondonada_fragil.generarPLY(archivo_esfera);
      hondonada_fragil.asignarTextura("text/planetas/agujero-negro.jpg");
      hondonada_fragil.calcularCoordenadasCirculares();

      

    // Abismo del gigante
      abismo_gigante.generarPLY(archivo_esfera);
      abismo_gigante.asignarTextura("text/planetas/Abismo.jpg");
      abismo_gigante.calcularCoordenadasCirculares();

      // Lanzadera
      base_lanzadera.generarPLY(archivo_b_lanzadera);
      base_lanzadera.setMaterial(material_bronce);
      lanzadera.generarPLY(archivo_lanzadera);
      lanzadera.setMaterial(material_morado_brillante);

      angulo_lanzadera = generarAnguloAleatorio(); // ángulo de órbita en el que se encontrará al inicio
      direccion_lanzadera = generarDireccionAleatoria(); // dirección a la que se disparará
      direccion_lanzadera = direccion_lanzadera.normalizar();

      // Cálculo para la direccionar la base de la lanzadera según direccion_lanzadera
      rotacionX_lanzadera = asin(direccion_lanzadera.y) * 180.0 / M_PI;
      rotacionY_lanzadera = atan2(direccion_lanzadera.z,direccion_lanzadera.x) * 180.0 / M_PI + 270.0;

    // Agujero blanco
      agujero_blanco.generarPLY(archivo_esfera);
      agujero_blanco.setMaterial(material_blanco_brillante);

  // NIVEL D: INTRUSO
    intruso.generarPLY(archivo_intruso);
    intruso.asignarTextura("text/astros/intruso.jpg");
    intruso.calcularCoordenadasPlano();
    
    intruso_cola.generarPLY(archivo_intruso_cola);
    intruso_cola.setMaterial(material_azul_brillante);

  // NIVEL E: NAVE
    // Lista de todos los astros posibles para viajar 
      navegacion[0] = "Sol";
      navegacion[1] = "Gemelo Ceniza";
      navegacion[2] = "Gemelo Brasas";
      navegacion[3] = "Lumbre";
      navegacion[4] = "Hondonada Frágil";
      navegacion[5] = "Abismo del Gigante";
      navegacion[6] = "Agujero Blanco";

    // Posiciones iniciales de los planetas
      posicion_ceniza.x = RADIO_ORBITA_GEMELOS;
      posicion_ceniza.y += DIST_CENTROS_GEMELOS / 2;
      posicion_ceniza.z = 0.0;
      posicion_brasas.x = RADIO_ORBITA_GEMELOS;
      posicion_brasas.y -= DIST_CENTROS_GEMELOS / 2;
      posicion_brasas.z = 0.0;

      posicion_lumbre = calcularPosicionOrbita(RADIO_ORBITA_LUMBRE, angulo_lumbre);
      posicion_hondonada = calcularPosicionOrbita(RADIO_ORBITA_HONDONADA, angulo_hondonada);
      posicion_abismo = calcularPosicionOrbita(RADIO_ORBITA_ABISMO, angulo_abismo);
      posicion_agujero.x = -10.0;
      posicion_agujero.y = 0.0;
      posicion_agujero.z = 5.0;


    // Nave
      // Al inicio la nave se encuentra en Lumbre
      indice_planeta_navegacion = 3;
      posicion_planeta = obtenerPosicionPlaneta(indice_planeta_navegacion);
      posicion_centro_nodo = posicion_planeta; // el nodo de la nave se encuentra en Lumbre al inicio
      
      // La nave inicia en el núcleo del planeta
      posicion_nave.x = 0.0;
      posicion_nave.y = 0.0;
      posicion_nave.z = 0.0;

      // Posición del centro de la nave
      posicion_real_nave = posicion_nave + posicion_centro_nodo;

      nave.generarPLY(archivo_nave);
      nave.setMaterial(material_plata);
      nave_cristal.generarPLY(archivo_nave_cristal);
      nave_cristal.setMaterial(material_azul_brillante);
}

// --------------------------------------------------------------------

// MÉTODO DIBUJA SECCIONADO EN NIVELES
// NIVEL B: PLANETAS
// Gemelos reloj de arena
void dibujaGemelos(){
  glRotatef(rotacion_gemelos_reloj, 0.0, 0.0, 1.0); // orbitación entre los gemelos

  // Gemelo Ceniza
  glPushMatrix();
    glTranslatef(0.0, DIST_CENTROS_GEMELOS / 2, 0.0); // separación con el hermano

    // Planeta
    glPushMatrix();
      glRotatef(rotacion_gemelos, 0.0, 1.0, 0.0); // rotación individual
      glScalef(radio_gemelos, radio_gemelos, radio_gemelos);
      gemelo_ceniza.drawSMOOTH();
    glPopMatrix();

    // Arena 1
    glPushMatrix();
      glRotatef(rotacion_gemelos, 0.0, 1.0, 0.0); // rotación individual
      glScalef(radio_arena1, radio_arena1, radio_arena1);
      arena1.drawSMOOTH();
    glPopMatrix();

  glPopMatrix();

  // Gemelo Brasas
  glPushMatrix();
    glTranslatef(0.0, -(DIST_CENTROS_GEMELOS / 2), 0.0); // separación con el hermano

    // Planeta
    glPushMatrix();
      glRotatef(rotacion_gemelos, 0.0, 1.0, 0.0); // rotación individual
      glScalef(radio_gemelos, radio_gemelos, radio_gemelos);
      gemelo_brasas.drawSMOOTH();
    glPopMatrix();

    // Arena 2
    glPushMatrix();
      glRotatef(rotacion_gemelos, 0.0, 1.0, 0.0); // rotación individual
      glScalef(radio_arena2, radio_arena2, radio_arena2);
      arena2.drawSMOOTH();
    glPopMatrix();

  glPopMatrix();

  // Cascada
  glPushMatrix();
    glTranslatef(0.0, centro_cascada, 0.0);
    glScalef(0.05, longitud_cascada, 0.05);
    glRotatef(rotacion_arena, 0.0, 1.0, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    cascada.draw();
  glPopMatrix();
}

// --------------------------------------------------------------------

// Lumbre
void dibujaLumbre(){
  glPushMatrix();
    glRotatef(rotacion_lumbre, 0.0, 1.0, 0.0); // rotación
    glScalef(radio_lumbre, radio_lumbre, radio_lumbre);
    lumbre.drawSMOOTH();
  glPopMatrix();

  glPushMatrix();
    glRotatef(angulo_rocaterra, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.2, RADIO_ORBITA_ROCATERRA);
    glRotatef(rotacion_rocaterra, 0.0, 1.0, 0.0);
    glScalef(radio_rocaterra, radio_rocaterra, radio_rocaterra);
    rocaterra.drawSMOOTH();
  glPopMatrix();
}

// --------------------------------------------------------------------

// Hondonada frágil
void dibujaHondonada(){
  glPushMatrix();
    glScalef(0.75, 0.75, 0.75);
    anillo1.draw();
    anillo2.draw();
    if(casa1){
      glPushMatrix();
        glTranslatef(mov_x_1, 0.0,0.0);
        trozo1.draw();
      glPopMatrix();
    }
    if(casa2){
      glPushMatrix();
        glTranslatef(mov_x_2, mov_y_2,0.0);
        trozo2.draw();
      glPopMatrix();
    }
  glPopMatrix();
  glPushMatrix();
    glScalef(radio_hondonada, radio_hondonada, radio_hondonada);
    hondonada_fragil.draw();
  glPopMatrix();
}

// --------------------------------------------------------------------

// Abismo del gigante
void dibujaAbismo(){
  // Dibujamos el planeta en el centro
  glPushMatrix();
    glScalef(radio_abismo, radio_abismo, radio_abismo);
    abismo_gigante.drawSMOOTH();
  glPopMatrix();

  // Dibujamos la base de la lanzadera
  glPushMatrix();
    glRotatef(angulo_lanzadera, 0.0, 1.0, 0.0); // rotación alrededor de Abismo
    glTranslatef(RADIO_ORBITA_LANZADERA, 0.0, 0.0); // colocar en su órbita
    
    // colocamos la lanzadera en la dirección correspondiente
    glRotatef(-rotacionY_lanzadera, 0.0, 1.0, 0.0);
    glRotatef(-rotacionX_lanzadera, 1.0, 0.0, 0.0);
    glScalef(2.0, 2.0, 2.0);
    base_lanzadera.draw();
  glPopMatrix();

  // Lanzadera
  glPushMatrix();
    // si todavía no se ha disparado, movemos la lanzadera con el resto de la base
    if(!disparada_lanzadera){
      glRotatef(angulo_lanzadera, 0.0, 1.0, 0.0); // rotación alrededor de Abismo
      angulo_lanzadera_nave = angulo_lanzadera; // guardamos la última posición
    }
    else{
      // Si ya se ha disparado, se mantiene la última rotación guardada
      glRotatef(angulo_lanzadera_nave, 0.0, 1.0, 0.0);
    }

    glTranslatef(RADIO_ORBITA_LANZADERA, 0.0, 0.0); // colocar en su órbita
    glTranslatef(movx, movy, movz); // se mueve en la dirección correspondiente

    // colocamos la lanzadera en la dirección correspondiente
    glRotatef(-rotacionY_lanzadera, 0.0f, 1.0f, 0.0f); // Rotación alrededor del eje Y
    glRotatef(-rotacionX_lanzadera, 1.0f, 0.0f, 0.0f); // Rotación alrededor del eje X
    glRotatef(180.0, 0.0, 1.0, 0.0); // orientación de la lanzadera
    glScalef(0.05, 0.03, 0.08);
    lanzadera.draw();
  glPopMatrix();

}

// --------------------------------------------------------------------

// El intruso
void dibujaIntruso(){
  glPushMatrix();
    glRotatef(rotacion_intruso_y, 0.0, 1.0, 0.0);
    glRotatef(rotacion_intruso_x, 1.0, 0.0, 0.0);
    glRotatef(rotacion_intruso, 0.0, 0.0, 1.0);
    intruso.draw();
    intruso_cola.draw();
  glPopMatrix();
}

// --------------------------------------------------------------------

// NIVEL E: NAVE

  void dibujaNave(){
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glScalef(2.0, 2.0, 2.0);
    nave.draw();
    nave_cristal.draw();
  }

  void posicionaNaveNodo(){
    switch(estado_nave){
      case EN_CENTRO_PLANETA:
      case ESPERANDO:
        // La nave permanece quieta en su posición respecto al planeta
        glPushMatrix();
          glTranslatef(posicion_nave.x, posicion_nave.y, posicion_nave.z);
          dibujaNave();
        glPopMatrix();
        break;
      case SALIENDO_DEL_PLANETA:
        if(animacion_on){
          // La nave sale del centro del planeta al exterior
          if(posicion_nave.x < 0.5f || posicion_nave.y > -0.1){
            posicion_nave.x += 0.005f * velocidad_despegue_nave * delta;
            posicion_nave.y -= 0.001f * velocidad_despegue_nave * delta;
          }
          else{
            estado_nave = ESPERANDO;
            printf("\033[1;36m");
            printf("Registro: ");
            printf("\033[0m");
            cout << "La nave está lista para viajar" << endl;
          }
        }

        glPushMatrix();
          glTranslatef(posicion_nave.x, posicion_nave.y, posicion_nave.z);
          dibujaNave();
        glPopMatrix(); 

        break;
      case VIAJANDO:
        // Orienta la nave
        glPushMatrix();
          glTranslatef(posicion_nave.x, posicion_nave.y, posicion_nave.z);
          glRotatef(-rotacionY_nave, 0.0, 1.0, 0.0);
          glRotatef(-rotacionX_nave, 1.0, 0.0, 0.0);
          dibujaNave();
        glPopMatrix();
        break;
      case ATERRIZANDO:
        if(animacion_on){
          // Inverso a SALIENDO_DEL_PLANETA
          if(posicion_nave.x > 0.0f || posicion_nave.y < 0.0){
            posicion_nave.x -= 0.005f * velocidad_despegue_nave * delta;
            posicion_nave.y += 0.001f * velocidad_despegue_nave * delta;
          }
          else{
            estado_nave = EN_CENTRO_PLANETA;
            printf("\033[1;36m");
            printf("Registro: ");
            printf("\033[0m");
            cout << "La nave ha aterrizado exitosamente" << endl;
          }
        }

        glPushMatrix();
          glTranslatef(posicion_nave.x, posicion_nave.y, posicion_nave.z);
          dibujaNave();
        glPopMatrix();
    }
  }

// ********************************************************************

void dibujarModelo(){
  switch(MODO_EJECUCION){
    // MODELO COMPLETO
      case 0:
        // NIVEL A: SOL
          glPushMatrix();
            glRotatef(angulo_sol, 0.0, 1.0, 0.0);
            sol.drawSMOOTH();
          glPopMatrix();

        // NIVEL B: PLANETAS
          // Gemelos reloj de arena
          drawOrbitaCircular(RADIO_ORBITA_GEMELOS);

          glPushMatrix();
            // Movimiento de ambos planetas
            glRotatef(angulo_gemelos, 0.0, 1.0, 0.0); // traslación
            glTranslatef(RADIO_ORBITA_GEMELOS, 0.0, 0.0); // órbita
            dibujaGemelos();

          glPopMatrix();
        
          // Actualización de la posición de los gemelos
            // Primero la rotación entre ellos
            posicion_ceniza.x = (DIST_CENTROS_GEMELOS/2) * sin(2 * M_PI - (rotacion_gemelos_reloj * M_PI / 180.0));
            posicion_ceniza.y = (DIST_CENTROS_GEMELOS/2) * cos(2 * M_PI - (rotacion_gemelos_reloj * M_PI / 180.0));

            posicion_brasas.x = (DIST_CENTROS_GEMELOS/2) * sin(M_PI - (rotacion_gemelos_reloj * M_PI / 180.0));
            posicion_brasas.y = (DIST_CENTROS_GEMELOS/2) * cos(M_PI - (rotacion_gemelos_reloj * M_PI / 180.0));

            // Luego la traslación alrededor del sol
              // Nota: por la rotación entre ellos, en su nodo se alejan del origen de coordenadas
              // por ello debemos calcular primero la posición de x y agregarla al radio de la órbita
            float radio_ceniza, radio_brasas;

            radio_ceniza = RADIO_ORBITA_GEMELOS + posicion_ceniza.x;
            posicion_ceniza.x = calcularPosicionOrbita(radio_ceniza,angulo_gemelos).x;
            posicion_ceniza.z = calcularPosicionOrbita(radio_ceniza,angulo_gemelos).z;

            radio_brasas = RADIO_ORBITA_GEMELOS + posicion_brasas.x;
            posicion_brasas.x = calcularPosicionOrbita(radio_brasas,angulo_gemelos).x;
            posicion_brasas.z = calcularPosicionOrbita(radio_brasas,angulo_gemelos).z;

          // Lumbre
          drawOrbitaCircular(RADIO_ORBITA_LUMBRE);

          glPushMatrix();
            glRotatef(angulo_lumbre, 0.0, 1.0, 0.0); // traslación
            glTranslatef(RADIO_ORBITA_LUMBRE, 0.0, 0.0);
            posicion_lumbre = calcularPosicionOrbita(RADIO_ORBITA_LUMBRE, angulo_lumbre);
            dibujaLumbre();

          glPopMatrix();

          // Honodonada frágil
          drawOrbitaCircular(RADIO_ORBITA_HONDONADA);

          glPushMatrix();
            glRotatef(angulo_hondonada, 0.0, 1.0, 0.0); // traslación
            glTranslatef(RADIO_ORBITA_HONDONADA, 0.0, 0.0);
            posicion_hondonada = calcularPosicionOrbita(RADIO_ORBITA_HONDONADA, angulo_hondonada);
            dibujaHondonada();

          glPopMatrix();

            // Subnivel B2

            if(!casa1){
              glPushMatrix();
                glTranslatef(posicion_agujero.x, posicion_agujero.y, posicion_agujero.z);
              
                glPushMatrix();
                  glTranslatef(mov_x_1, 0.0,0.0);
                  trozo1.draw();
                glPopMatrix();
              glPopMatrix();
            }

            if(!casa2){
              glPushMatrix();
                glTranslatef(posicion_agujero.x, posicion_agujero.y, posicion_agujero.z);
                
                glPushMatrix();
                  glTranslatef(mov_x_2, mov_y_2,0.0);
                  trozo1.draw();
                glPopMatrix();
              glPopMatrix();
            }
            

          // Abismo gigante
          drawOrbitaCircular(RADIO_ORBITA_ABISMO);

          glPushMatrix();
            glRotatef(angulo_abismo, 0.0, 1.0, 0.0); // traslación
            glTranslatef(RADIO_ORBITA_ABISMO, 0.0, 0.0);
            posicion_abismo = calcularPosicionOrbita(RADIO_ORBITA_ABISMO, angulo_abismo);
            dibujaAbismo();
          glPopMatrix();

          // Agujero blanco
          {
            glPushMatrix();
              glTranslatef(-10.0, 0.0, 5.0);
              glPushMatrix();
                glScalef(radio_agujero, radio_agujero, radio_agujero);
                agujero_blanco.drawSMOOTH();
              glPopMatrix();

              glPushMatrix();
                glTranslatef(0.2, 0.3, 0.15);
                trozo3.draw();
              glPopMatrix();

              glPushMatrix();
                glTranslatef(-0.2, 0.4, 0.15);
                trozo4.draw();
              glPopMatrix();
            glPopMatrix();
          }

        // NIVEL D: INTRUSO
          // Dibujamos la órbita
          glPushMatrix();
            glRotatef(30.0, 0.0, 1.0, 0.0);
            glTranslatef(-5.8, 0.0, 0.0);

            drawOrbitaElipse(7.0,4.5);

            glTranslatef(intruso_x, 0.0, intruso_z);
            dibujaIntruso();
          glPopMatrix();

        // NIVEL E: NAVE

          // Si la nave se encuentra anclada a un planeta, esa será la posición del nodo de la nave
          // Si la nave está viajando, en el método idle se actualizan los valores de posicion_centro_nodo

          glPushMatrix();
            glTranslatef(posicion_centro_nodo.x, posicion_centro_nodo.y, posicion_centro_nodo.z);
            posicionaNaveNodo();
          glPopMatrix();

        break;
    // NIVEL A: SOL
      case 1:
        // Sol
        glScalef(3.0, 3.0, 3.0);
        glRotatef(angulo_sol, 0.0, 1.0, 0.0);
        sol.drawSMOOTH();
        break;
    // NIVEL B: PLANETAS
      case 2:
        // Gemelos Reloj de Arena
        glScalef(3.0, 3.0, 3.0);
        dibujaGemelos();
        break;
      case 3:
        // Lumbre
        glScalef(3.0, 3.0, 3.0);
        dibujaLumbre();
        break;
      case 4:
        // Hondonada frágil
        glScalef(3.0, 3.0, 3.0);
        dibujaHondonada();
        break;
      case 5:
        // Abismo gigante
        glScalef(2.0, 2.0, 2.0);
        dibujaAbismo();
        break;
      case 6:
        // Agujero blanco
        glScalef(5.0, 5.0, 5.0);
        glPushMatrix();
              glPushMatrix();
                glScalef(radio_agujero, radio_agujero, radio_agujero);
                agujero_blanco.drawSMOOTH();
              glPopMatrix();

              glPushMatrix();
                glTranslatef(0.2, 0.2, 0.15);
                trozo3.draw();
              glPopMatrix();

              glPushMatrix();
                glTranslatef(-0.2, 0.1, 0.15);
                trozo4.draw();
              glPopMatrix();
            glPopMatrix();

            // Los trozos que queadban en Hondonada
            if(!casa1){
                glPushMatrix();
                  glTranslatef(mov_x_1, 0.0,0.0);
                  trozo1.draw();
                glPopMatrix();
              glPopMatrix();
            }

            if(!casa2){
                glPushMatrix();
                  glTranslatef(mov_x_2, mov_y_2,0.0);
                  trozo1.draw();
                glPopMatrix();
              glPopMatrix();
            }
        break;
    // NIVEL E: NAVE
      case 7:
        glScalef(10.0, 10.0, 10.0);
        dibujaNave();
        break;
    // NIVEL D: INTRUSO
      case 8:
        glScalef(5.0,5.0,5.0);
        dibujaIntruso();
        break;
  
  }
}

// ********************************************************************
//============================ FIN DE DIBUJO ============================

class Ejes{
  public:
      float longitud = 30;
  // Dibuja el objeto
  void draw( )
  {
    glDisable (GL_LIGHTING);
    glBegin (GL_LINES);
    {
      glColor3f (0, 1, 0);
      glVertex3f (0, 0, 0);
      glVertex3f (0, longitud, 0);

      glColor3f (1, 0, 0);
      glVertex3f (0, 0, 0);
      glVertex3f (longitud, 0, 0);

      glColor3f (0, 0, 1);
      glVertex3f (0, 0, 0);
      glVertex3f (0, 0, longitud);
    }
    glEnd ();
    glEnable (GL_LIGHTING);
  }
} ;

Ejes ejesCoordenadas;

//============================= SELECCIÓN =============================


void dibujarEscena() {
  // Posiciones de las fuentes de luz
  static GLfloat pos0[4] = { 0.0, 0.0, 0.0, 1.0 };  // Fuente de luz 1 (posicional)

  static GLfloat pos1[4] = { 5.0, 5.0, 5.0, 0.0 };  // Fuente de luz 0 (direccional)
  static GLfloat pos2[4] = { -5.0, 5.0, -5.0, 0.0 };  // Fuente de luz 1 (posicional)
  static GLfloat pos3[4] = { 0.0, 10.0, 0.0, 0.0 };  // Fuente de luz 2 (posicional)

  // Colores de las fuentes de luz
  static GLfloat color0[4] = { 1.0, 1.0, 1.0, 1.0 }; // Luz blanca

  // Aumentar la luz ambiente
  GLfloat global_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f }; 
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  glPushMatrix(); // Apila la transformación geométrica actual
  glClearColor(0.0, 0.0, 0.0, 1.0); // Fija el color de fondo a negro
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Inicializa el buffer de color y el Z-Buffer
  transformacionVisualizacion(); // Carga transformación de visualización

  // CONFIGURACIÓN DE LAS LUCES
    // Configuración de la luz 0
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, color0);

    // Configuración de la luz 1
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, color0);

    // Configuración de la luz 2
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, color0);
    glLightfv(GL_LIGHT2, GL_SPECULAR, color0);

    // Configuración de la luz 3
    glLightfv(GL_LIGHT3, GL_POSITION, pos3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, color0);
    glLightfv(GL_LIGHT3, GL_SPECULAR, color0);

  // Activar las luces
  glEnable(GL_LIGHTING);  // Activa la iluminación
  
  if(MODO_EJECUCION == 0){
    glEnable(GL_LIGHT0);    // Activa la luz 0
    glDisable(GL_LIGHT1);    // Desactiva la luz 1
    glDisable(GL_LIGHT2);    // Desactiva la luz 2
    glDisable(GL_LIGHT3);    // Desactiva la luz 3
  }
  else{
    glDisable(GL_LIGHT0);    // Desactiva la luz 0
    glEnable(GL_LIGHT1);    // Activa la luz 1
    glEnable(GL_LIGHT2);    // Activa la luz 2
    glEnable(GL_LIGHT3);    // Activa la luz 3
  }

  //==================================================================
  dibujarModelo();
  //==================================================================

  glPopMatrix(); // Desapila la transformación geométrica
}


int pick(int x, int y,int * i){
  GLint viewport[4];
  unsigned char data[4];

  glGetIntegerv (GL_VIEWPORT, viewport);

  glDisable(GL_DITHER);
  glDisable(GL_LIGHTING);

  seleccionActivada = true;
  dibujarEscena();
  seleccionActivada = false;

  glEnable(GL_LIGHTING);
  glEnable(GL_DITHER);

  glFlush();
  glFinish();

  glReadPixels(x,viewport[3]-y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,data);

  *i=data[0];

  // Según el modo, haremos distintas operaciones
  // Ningún modo seleccionado: nombramos el astro seleccionado
  if(!navegacion_on && !zoom_on && animacion_on){
    if(*i == 0){
      printf("\033[1;31m");
      cout << "No se ha seleccionado ningún astro" << endl;
      printf("\033[0m");
    }
    else if(nombres.find(*i) != nombres.end()){
      cout << "Nombre: " << nombres[*i] << endl;
      cout << "Descripción: \n" << descripciones[*i] << endl;
    }
    else{
      printf("\033[1;31m");
      cout << "No se ha encontrado información sobre el objeto" << endl;
      printf("\033[0m");
    }
  }
  // Modo navegación activado: la nave se dirige al astro seleccionado
  else if(navegacion_on && animacion_on){
    if(*i == 0){
      printf("\033[1;31m");
      cout << "No se ha seleccionado ningún lugar para viajar" << endl;
      printf("\033[0m");
    }
    else if(*i == 1){
      printf("\033[1;35m");
      printf("No creo que eso sea buena idea...\n");
      printf("\033[0m");
    }
    else{
      // Solo puede seleccionarse planeta si la nave está esperando
      if(estado_nave != ESPERANDO){
        printf("\033[1;31m");
        printf("La nave no puede viajar en este momento\n");
        printf("\033[0m");
      }
      else{
        int indice_destino = *i;

        // Ajustamos el indice del destino
          // añadimos las arenas de los gemelos para poder seleccionar esos planetas
          if(indice_destino == 11){
            indice_destino = 1; // Cenizas
          }
          else if(indice_destino == 12){
            indice_destino = 2; // Brasas
          }

          // si no es ninguna de esas opciones, coincide con indice - 1
          else{
            indice_destino--;
          }

        Vertice destino = obtenerPosicionPlaneta(indice_destino);

        // Comprobamos que se pueda viajar
        if(destino.y < -50.0){
          printf("\033[1;31m");
          printf("No se puede viajar a esa posición\n");
          printf("\033[0m");
          cout << *i;
        }
        // Comprobamos que no se seleccione el mismo planeta
        else if(indice_destino == indice_planeta_navegacion){
          printf("\033[1;31m");
          printf("La nave ya se encuentra en %s\n", nombres[indice_destino + 1].c_str());
          printf("\033[0m");
        }
        else{
          printf("\033[1;36m");
          printf("Registro: ");
          printf("\033[0m");
          cout << "Viajando a " << nombres[indice_destino + 1] << endl;
          mostrarNave();
          estado_nave = VIAJANDO;
          indice_planeta_navegacion = indice_destino;
        }
      
      }
    }
  }
  // Modo zoom activado: se observará el astro seleccionado
  else if(zoom_on && animacion_on){
    if(*i == 0){
      printf("\033[1;31m");
      printf("No se ha seleccionado ningún astro para observar\n");
      printf("\033[0m");
    }
    else{    
      if(*i <= 9){
        MODO_EJECUCION = *i;

        // Gemelos reloj de arena son dos planetas:
        // i_seleccion a partir de brasas es MODO_EJECUCION + 1
        if(*i > 2)
          MODO_EJECUCION--;
      }

      // Gemelos reloj de arena
      if(*i > 9 && *i <= 12){
        MODO_EJECUCION = 2;
      }

      // Lumbre
      if(*i == 13){
        MODO_EJECUCION = 3;
      }

      // Hondonada frágil
      if(*i > 13 && *i <= 19){
        MODO_EJECUCION = 4;
      }

      // Abismo del gigante
      if(*i == 21){
        MODO_EJECUCION = 5;
      }

      // Intruso
      if(*i == 22){
        MODO_EJECUCION = 8;
      }

      // Nave
      if(*i == 23){
        MODO_EJECUCION = 7;
      }

      cout << "Ha seleccionado: " << nombres[*i] << endl;
      ajustarVelocidades(150.0); // Un ajuste para que se vea mejor
    }
  }

  glutPostRedisplay();
  return *i;
}


// --------------------------------------------------------------------
//========================= FIN DE SELECCIÓN ===========================

//=========================== INTERRUPTORES ============================

void interruptorAcelerar(){
  if(animacion_on){
    escala_velocidad += 10.0;
    ajustarVelocidades(escala_velocidad);
  }
}

void interruptorDesacelerar(){
  if(animacion_on){
    escala_velocidad -= 10.0;

    if(escala_velocidad <= 10.0){
      escala_velocidad = 10.0;
      printf("\033[1;31m");
      printf("La velocidad no puede ser menor a 10.0\n");
      printf("\033[0m");
    }

    ajustarVelocidades(escala_velocidad);
  }
}

void interruptorReiniciarVelocidad(){
  if(animacion_on){
    escala_velocidad = ESCALA_VELOCIDAD_INICIAL;
    ajustarVelocidades(escala_velocidad);
  }
}

// --------------------------------------------------------------------

// Desactivar ambos modos. Activar modo observación
void interruptorObservacion(){
  if(animacion_on){
    if(zoom_on == true || navegacion_on == true){
      zoom_on = false;
      navegacion_on = false;
      MODO_EJECUCION = 0;

      cout << endl;
      mostrarObservacion();
    }
    // Si ya estaba en modo observación, no hace nada
    else{
      printf("\033[1;31m");
      printf("Modo de observación ya activado\n");
      printf("\033[0m");
    }
  }
}

// Activar el modo de navegación
void interruptorNavegacion(){
  if(animacion_on){
    if(MODO_EJECUCION == 0){
      if(animacion_on){
        if(!navegacion_on){
          cout << endl;
          mostrarNavegacion();

          // Si está dentro del planeta, sale
          if(estado_nave == EN_CENTRO_PLANETA){
            estado_nave = SALIENDO_DEL_PLANETA;
            printf("\033[1;36m");
            printf("Registro: ");
            printf("\033[0m");
            printf("Despegando...\n");
          }
          // Inicamos el modo
          navegacion_on = true;
        }
        else{
          // Si ya estaba en modo navegación, aterriza o despega
          if(estado_nave == EN_CENTRO_PLANETA){
            estado_nave = SALIENDO_DEL_PLANETA;
            printf("\033[1;36m");
            printf("Registro: ");
            printf("\033[0m");
            printf("Despegando...\n");
          }
          else if(estado_nave == ESPERANDO){
            estado_nave = ATERRIZANDO;
            printf("\033[1;36m");
            printf("Registro: ");
            printf("\033[0m");
            printf("Aterrizando...\n");
          }
          
        }
      }
    }
    else{
      printf("\033[1;31m");
      cout << "No se puede activar el modo de navegación durante la observación de un elemento" << endl;
      printf("\033[0m");
    }
  }
}

// Activar el modo de zoom
void interruptorZoom(){
  if(animacion_on){
    // si el modo zoom está observando un astro, retrocede al sistema completo
    if(MODO_EJECUCION != 0){
      MODO_EJECUCION = 0;
      interruptorReiniciarVelocidad();
    }
    // si el modo de ejecución es 0, se alterna el modo zoom
    else{
      if(zoom_on){
        printf("\033[1;31m");
        printf("Modo de zoom ya activado\n");
        printf("\033[0m");
      }
      else{
        // Desactivamos el modo navegación en caso de que esté activado
        if(navegacion_on){
          printf("\033[1;31m");
          printf("Modos incompatibles: ");
          printf("\033[0m");
          printf("Aterrizando...\n");
          navegacion_on = false;
          estado_nave = ATERRIZANDO;
        }

        // Inicamos el modo
        zoom_on = true;

        cout << endl;
        mostrarZoom();
      }
    }
  }
}

// --------------------------------------------------------------------

void interruptorActivarDesactivar(){
  animacion_on = !animacion_on;

  if(primer_inicio){
    mostrarFunciones();
    primer_inicio = false;
  }

  if(animacion_on){
    printf("\033[1;32m");
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║                                                          ║\n");
    printf("║  »  »  »  »  »  »  ANIMACIÓN INICIADA  «  «  «  «  «  «  ║\n");
    printf("║                                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝");
    cout << endl;
    printf("\033[0m");

    if(navegacion_on)
      mostrarNavegacion();
    else if(zoom_on)
      mostrarZoom();
    else
      mostrarObservacion();
  }
  else{
    printf("\033[1;31m");
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║  »  »  »  »  »  » ANIMACIÓN DESACTIVADA «  «  «  «  «  « ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\033[0m");
  } 
}

// --------------------------------------------------------------------
//=========================== FIN DE INTERRUPTORES ============================

/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/

void Dibuja (void){
	glEnable(GL_LIGHTING);
	glEnable(GL_DITHER);
	clock_t end = clock();
  delta = float(end - start) / CLOCKS_PER_SEC;
  start = clock();
  dibujarEscena();
	glutSwapBuffers();
}


/**	void idle()

Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.

**/
void idle (int v){
  if(animacion_on){
    // NIVEL A: SOL
    angulo_sol += velocidad_rot_sol * delta;

    // NIVEL B: PLANETAS
      // Rotación de los planetas
      rotacion_gemelos += velocidad_rot_gemelos * delta;
      rotacion_gemelos_reloj += velocidad_rot_gemelos_reloj * delta;
      rotacion_lumbre += velocidad_rot_lumbre * delta;
      rotacion_hondonada += velocidad_rot_hondonada * delta;
      rotacion_abismo += velocidad_rot_abismo * delta;

      // Traslación de los planetas
      angulo_gemelos += velocidad_gemelos * delta;
      angulo_lumbre += velocidad_lumbre * delta;
      angulo_hondonada += velocidad_hondonada * delta;
      angulo_abismo += velocidad_abismo * delta;

      // SUBNIVEL B.1: ARENAS DE GEMELOS
        // La cascada gira para dar efecto de movimiento
        rotacion_arena += 0.01;

        // ANIMACIÓN DE LA ARENA
        // Se activa la cascada (va al planeta vacío)
        if(mov_cascada){
          // primero va de Cenizas a brasas
          if(aumenta_brasas){
            longitud_cascada += escala_cascada * delta;
            centro_cascada -= escala_cascada * delta;

            // Cuando la cascadada avanza lo suficiente, permite que se mueva la arena: mov_arena = true
            if(centro_cascada <= CENTRO_INI_CASCADA * 0.6) 
              mov_arena = true;

            // Detiene extensión de la cascada cuando llega a 0.0
              // el centro está en medio de los dos planetas exactamente
            if(centro_cascada <= 0.0)
              mov_cascada = false;

          }
          // luego de Brasas a Cenizas
          else{
            longitud_cascada += escala_cascada * delta;
            centro_cascada += escala_cascada * delta;

            // Cuando la cascadada avanza lo suficiente, permite que se mueva la arena: mov_arena = true
            if(centro_cascada >= (-CENTRO_INI_CASCADA) * 0.6)
              mov_arena = true;

            // Detiene extensión de la cascada cuando llega a 0.0
              // el centro está en medio de los dos planetas exactamente
            if(centro_cascada >= 0.0)
              mov_cascada = false;
          }
        }

        // Se ativa la arena (se vacía de arena uno y se llena el otro)
        if(mov_arena){
          if(aumenta_brasas){
            // Arena 1 (Ceniza) decrece y Arena 2 (Brasas) crece
            radio_arena1 -= escala_arena_decrece * delta;
            radio_arena2 += escala_arena_crece * delta;

            // Antes de que Brasas esté lleno, se inicia la regresión de la cascada
              // Nota: la regresión acaba antes de la arena. Cuidado con activar fin_cascada
            if(radio_arena2 >= RADIO_ARENA_MAX * 0.85 && centro_cascada >= -CENTRO_INI_CASCADA){
              fin_cascada = true;
            }

            // Cuando Brasas esté lleno, se detiene la arena
            if(radio_arena2 >= RADIO_ARENA_MAX){
              mov_arena = false;
              pausa_arena = true; // inicia el periodo de pausa
            }
          }

          else{
            // Arena 1 (Ceniza) crece y Arena 2 (Brasas) decrece
            radio_arena1 += escala_arena_crece * delta;
            radio_arena2 -= escala_arena_decrece * delta;

            // Antes de que cenizas esté lleno, se inicia la regresión de la cascada
              // Nota: la regresión acaba antes de la arena. Cuidado con activar fin_cascada
            if(radio_arena1 >= RADIO_ARENA_MAX*0.85 && centro_cascada <= CENTRO_INI_CASCADA){
              fin_cascada = true;
            }

            // Cuando Ceniza esté lleno, se detiene la arena
            if(radio_arena1 >= RADIO_ARENA_MAX){
              mov_arena = false;
              pausa_arena = true; // inicia el periodo de pausa
            }
          }
        }

        // Regresión de la cascada (va al planeta lleno)
        if(fin_cascada){
          // primero va de Cenizas a brasas
          if(aumenta_brasas){
            longitud_cascada -= escala_cascada * delta;
            centro_cascada -= escala_cascada * delta;

            // Si el centro de la cascada llega casi al centro de Brasas: para la regresión
            if(centro_cascada <= -CENTRO_INI_CASCADA){
              fin_cascada = false; // detiene la regresión
            }
          }
          // luego de Brasas a Cenizas
          else{
            longitud_cascada -= escala_cascada * delta;
            centro_cascada += escala_cascada * delta;

            // Si el centro de la cascada llega casi al centro de Cenizas: para la regresión
            if(centro_cascada >= CENTRO_INI_CASCADA){
              fin_cascada = false; // detiene la regresión
            }
          }
        }

        if(pausa_arena){
          contador_pausa++;

          if(contador_pausa == 40){
            aumenta_brasas = !aumenta_brasas; // Cambia el sentido de la animación

            // Antes de continuar, se asegura de que los valores estén en su estado inicial
            if(aumenta_brasas){
              centro_cascada = CENTRO_INI_CASCADA;
              radio_arena1 = RADIO_ARENA_MAX;
              radio_arena2 = RADIO_ARENA_MIN;
            }
            else{
              centro_cascada = -CENTRO_INI_CASCADA;
              radio_arena1 = RADIO_ARENA_MIN;
              radio_arena2 = RADIO_ARENA_MAX;
            }

            longitud_cascada = LONGITUD_INI_CASCADA;
            mov_cascada = true;
            pausa_arena = false;
            contador_pausa = 0;
          }
        }

      // SUBNIVEL B.2: ESTACIÓN DE HONDONADA FRÁGIL
        // Movimiento de los trozos
        if(!casa2){
          if(mov_x_1 > -0.6)
            mov_x_1 -= velocidad_trozo1 * delta;
        }

        if(mov_x_2 < 0.2)
          mov_x_2 += velocidad_trozo2 * delta * 0.5;
        
        if(mov_y_2 > -0.6)
          mov_y_2 -= velocidad_trozo2 * delta;

        if(mov_x_1 <= -0.3)
          casa1 = false;

        if(mov_x_2 > 0.2)
          casa2 = false;

    // NIVEL C: SATÉLITES
      // ROCATERRA
      angulo_rocaterra += velocidad_rocaterra * delta;
      rotacion_rocaterra += velocidad_rot_rocaterra * delta;

      // LANZADERA
      // Solo se hace una vez: aceleración de la lanzadera
      if(!disparada_lanzadera){
        velocidad_lanzadera += aceleracion_lanzadera * delta;

        // Una vez se suelta de la base, mantiene constante la velocidad 
        if(velocidad_lanzadera >= velocidad_lanzadera_maxima)
          disparada_lanzadera = true;
      }

      // Rotación de los satélites
      angulo_lanzadera += velocidad_lanzadera * delta;

      // Movimiento de la lanzadera
      movx += direccion_lanzadera.x * velocidad_lanzadera * delta;
      movy += direccion_lanzadera.y * velocidad_lanzadera * delta;
      movz += direccion_lanzadera.z * velocidad_lanzadera * delta;

    // NIVEL D: INTRUSO
      float x_1 = intruso_x, z_2 = intruso_z;

      // Rotación del intruso
        rotacion_intruso += velocidad_rotacion_intruso * delta;
      
      // Calcular traslado del intruso
        n_intruso += velocidad_intruso * delta;
        n_intruso = fmod(n_intruso, 360.0);
        angulo_intruso = -n_intruso * M_PI / 180.0; // convertimos a radianes

        intruso_x = 7.0 * cos(angulo_intruso);
        intruso_z = 4.5 * sin(angulo_intruso); 

      // Calcular la distancia del intruso
        float x_2 = intruso_x, z_1 = intruso_z;
        Vertice direccion_intruso = Vertice(x_2 - x_1, 0.0, z_2 - z_1).normalizar();

      rotacion_intruso_x = asin(direccion_intruso.y) * 180.0 / M_PI;
      rotacion_intruso_y = atan2(direccion_intruso.z,direccion_intruso.x) * 180.0 / M_PI + 270.0;

    // NIVEL E: NAVE
      // La posición del planeta se actualiza constantemente
      posicion_planeta = obtenerPosicionPlaneta(indice_planeta_navegacion);

      // 1. Cambiamos la dirección de la nave hacia el planeta destino
      // 2. Modifica el valor de posicion_centro_nodo para que la nave se dirija al centro del planeta
      // 3. Cuando la posición de la nave esté en el centro del planeta, se coloca la nave en el centro
      // 4. En ese caso, se activará nuevamente SALIR DEL PLANETA
      if(estado_nave == VIAJANDO){
        // Si va chocarse en dos instantes siguientes, esquiva subiendo por el eje y
        if(!corrigiendo){
          direccion_nave_temp = direccion_nave;
        }

        if(boolColisionNaveInstante(direccion_nave_temp, 4.0)){
          direccion_nave = Vertice(direccion_nave_temp.x,5.0,direccion_nave_temp.z).normalizar();
          if(corrigiendo == false){
            printf("\033[1;31m");
            printf("COLISIÓN INMINENTE: Corrigiendo ruta\n");
            printf("\033[0m");
          }
          corrigiendo = true;
        }
        else{
          // Calculamos la dirección hacia el planeta
          direccion_nave = (posicion_planeta - posicion_real_nave).normalizar();
          direccion_nave_temp = direccion_nave;
          corrigiendo = false;
        }

        rotacionX_nave = asin(direccion_nave.y) * 180.0 / M_PI;
        rotacionY_nave = atan2(direccion_nave.z,direccion_nave.x) * 180.0 / M_PI + 270.0;

        // Si la nave no ha llegado al centro del planeta
        if(!boolColision(posicion_real_nave, posicion_planeta, 0.1)){
          posicion_centro_nodo.x += direccion_nave.x * velocidad_nave * delta;
          posicion_centro_nodo.y += direccion_nave.y * velocidad_nave * delta;
          posicion_centro_nodo.z += direccion_nave.z * velocidad_nave * delta;
        }
        else{
          // Si la nave ha llegado al centro del planeta
          estado_nave = SALIENDO_DEL_PLANETA; // le pedimos que vuelva a salir del planeta
          
          // Ponemos la nave en el centro del planeta
          posicion_nave.x = 0.0;
          posicion_nave.y = 0.0;
          posicion_nave.z = 0.0;

          // Ponemos el centro del nodo en la posición del planeta
          posicion_centro_nodo = posicion_planeta;
        }
      }
      else{
        posicion_centro_nodo = posicion_planeta;
      }

      // Actualizamos la posición real de la nave
      posicion_real_nave = posicion_nave + posicion_centro_nodo;
  }

  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
}