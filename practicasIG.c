/*	Prácticas de Informática Gráfica

    Grupos C y D				Curso 2024-25
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: Ana Cascone Hernández

	Programa principal
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

	practicasIG.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include <string.h>
#include "practicasIG.h"

// Variables globales
int MODO_EJECUCION = 0; // Modo de ejecución

// Método auxiliar para mostrar ayuda
void mostrarAyuda(){
  printf("Uso: ./practicasIG\n");
  printf("Opciones de modos:\n");
  printf("\t-h, --help\t\t\tMuestra esta ayuda\n");
};

void mostrarInicio(){
  printf("\x1B[33m"); // Texto en amarillo
  printf("                                        \n");
  printf("                              NXXN      \n");
  printf("                             XkxxkX     \n");
  printf("                             KxddxK     \n");
  printf("            NXKKKKKXXN        XKKX      \n");
  printf("         X0kxddddddddxk0KN              \n");
  printf("       XOxddkOKKKK00OkxddkK             \n");
  printf("     NKxdxk0XNNNN    NX0xdx0N           \n");
  printf("     XxddxkkxxxxkOKN    KxoxK           \n");
  printf("     0dddddddddddddk0X   0ddON          \n");
  printf("    NOdddddddddddddddkX  0ddkX          \n");
  printf("     0xdddddddddddddodK NOddON          \n");
  printf("     XkddddddddddddddxKX0doxX           \n");
  printf("      XOddddddddddddxkOkddkX            \n");
  printf("       NKkxdddddddddddddk0N             \n");
  printf("         NX0OkkxxxxkkO0XN               \n");
  printf("              NNNNN                     \n");
  printf("                                        \n");
  printf("\033[0m"); // Restablece el color
  printf("            OUTER WILDS                 \n");
  printf("        EXPLORE THE UNKNOWN             \n");
  printf("                                        \n");
  printf("\x1B[32m"); // Texto en verde
  printf("        Pulsa A para iniciar            \n");
  printf("\033[0m"); // Restablece el color
};

/*=================================================*/

/**	int main( int argc, char *argv[] )	
	Programa principal.
	
	Inicializa glut y openGL
	Crea la ventana X
	Inicializa las variables globales
	lanza los gestores de eventos	
**/

int main (int argc, char *argv[]){
/******************************************************/
// Comprobamos que los argumentos se hayan pasado correctamente
  if(argc > 1){
    printf("Error: Demasiados argumentos\n");
    mostrarAyuda();
    return 1;
  }
/******************************************************/

// Inicializa glu y openGL
  glutInit (&argc, argv);

// Crea una ventana X para la salida grafica en la posicion 0,0 con tamaño 800x800, con colores RGB-alfa, con doble buffer, y 
// buffer de profundidad
  glutInitWindowPosition (0, 0);
  glutInitWindowSize (800, 800);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("IG. Curso 2024/25. Autor: ANA");


/******************************************************/

// Iniciamos el modelo
initModel();
// Mostramos la pantalla de inicio
  mostrarInicio();

/******************************************************/


// Inicializa las funciones de dibujo y cambio de tamanyo de la ventana X
  glutDisplayFunc (Dibuja);
  glutReshapeFunc (inicializaVentana);

// FUNCIONES DE INTERACCION
  glutKeyboardFunc (letra);
  glutSpecialFunc (especial);

  glutMouseFunc (clickRaton);
  glutMotionFunc (RatonMovido);

// Funcion de fondo
  glutTimerFunc (30, idle, 0);

// Inicializa parametros de openGL
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_CULL_FACE);
  glEnable (GL_LIGHTING);

// Lanza el gestor de eventos de glut
  glutMainLoop ();
  return 0;
}











// VERSION ANTIGUA QUE BORRAR:
/*

  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <GL/glut.h>		// Libreria de utilidades de OpenGL
  #include <string.h>
  #include "practicasIG.h"
  //#include "hiply.h"

  // Variables globales
  int MODO_EJECUCION = 0; // Modo de ejecución

  // Método auxiliar para mostrar ayuda
  void mostrarAyuda(){
    printf("Uso: ./practicasIG\n");
    printf("Opciones de modos:\n");
    printf("\t-h, --help\t\t\tMuestra esta ayuda\n");
    printf("\t0: \t\t\tProyecto completo\n");
    printf("\t1-x: \t\t\tEn la memoria se pueden encontrar todas las opciones redactadas\n"); 
  }

  void mostrarFunciones(){
    printf("\n****************************************************************\n");
    printf("FUNCIONES BÁSICAS DEL ALUMNO\n");
    printf("****************************************************************\n");
    printf("A: Activar/Desactivar animación\n");
    printf("N: alternar la visualización de las normales\n");
    printf("\nModos de interacción:\n");
    printf("\to,O: Modo de observación\n");
    printf("\tz,Z: Modo de zoom\n");
    printf("\td,D: Modo de navegación\n");
    printf("\nVelocidad de la animación:\n");
    printf("\tV: Acelerar\n");
    printf("\tv: Desacelerar\n");
    printf("\tr,R: Reiniciar velocidad\n");
    printf("****************************************************************\n");
    //((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((()))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
    // Añadir las nuevas funciones y ayudas
  }

 //=================================================

  	// int main( int argc, char *argv[] )	
    // Programa principal.
    
    // Inicializa glut y openGL
    // Crea la ventana X
    // Inicializa las variables globales
    // lanza los gestores de eventos	
  

  int main (int argc, char *argv[]){
  /******************************************************
  // Comprobamos que los argumentos se hayan pasado correctamente
    if(argc < 2){
      printf("Error: Faltan argumentos\n");
      mostrarAyuda();
      return 1;
    }

    // Obtenemos el modo de operación
    MODO_EJECUCION = atoi(argv[1]);
    printf("Se ha seleccionado el modo: %d\n", MODO_EJECUCION);
  /******************************************************

  // Inicializa glu y openGL
    glutInit (&argc, argv);

  // Crea una ventana X para la salida grafica en la posicion 0,0 con tamaño 800x800, con colores RGB-alfa, con doble buffer, y 
  // buffer de profundidad
    glutInitWindowPosition (0, 0);
    glutInitWindowSize (800, 800);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow ("IG. Curso 2024/25. Autor: ANA");


  /******************************************************

  // Iniciamos el modelo
  initModel();
  // Explicamos las posibles opciones de visualización
    mostrarFunciones();

  /******************************************************


  // Inicializa las funciones de dibujo y cambio de tamanyo de la ventana X
    glutDisplayFunc (Dibuja);
    glutReshapeFunc (inicializaVentana);

  // FUNCIONES DE INTERACCION
    glutKeyboardFunc (letra);
    glutSpecialFunc (especial);

    glutMouseFunc (clickRaton);
    glutMotionFunc (RatonMovido);

  // Funcion de fondo
    glutTimerFunc (30, idle, 0);

  // Inicializa parametros de openGL
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE);
    glEnable (GL_LIGHTING);

  // Lanza el gestor de eventos de glut
    glutMainLoop ();
    return 0;
  }

*/