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

=======================================================/
modulo entradaTeclado.c
	Gestion de eventos de teclado
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"


/** 

Imprime en la consola las instrucciones del programa

**/
void printHelp ()
{

  printf ("\n\n     Prácticas de Informática Gráfica	Curso 2024-2025");
  printf ("\n\n Dpto. Lenguajes y Sistemas Informaticos");
  printf ("\n E.T.S.I. Informatica		Univ. de Granada ");
  printf ("\n");
  printf ("\n Opciones: \n\n");
  printf ("h, H: Imprime informacion de ayuda \n");
  printf ("PgUp, PgDn: avanza y retrocede la cámara \n\n");
  printf ("+,-: avanza y retrocede la cámara \n\n");
  printf ("Teclas de movimiento de cursor: giran la camara\n");
  
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
  
  printf ("\n Escape: Salir");
  printf ("\n\n\n");
}




/* @teclado ---------------------------------------------------------------- */

/** 		void letra (unsigned char k, int x, int y)

Este procedimiento es llamado por el sistema cuando se pulsa una tecla normal
El codigo k es el ascii de la letra

Para anyadir nuevas ordenes de teclado coloca el correspondiente case.

Parametros de entrada:

k: codigo del caracter pulsado

x:

y:

**/

float rotxCamara = 30, rotyCamara = 45;
float dCamara = 10;

void letra (unsigned char k, int x, int y)
{
  switch (k){
    case 'h':
    case 'H':
      printHelp ();		// H y h imprimen ayuda
      break;
    case '+':			// acerca la cámara
      dCamara -= 0.5;
      break;
    case '-':			// aleja la cámara
      dCamara += 0.5;
      break;
  // Activar/desactivar animación
    case 'a':
    case 'A':
      interruptorActivarDesactivar();
      break;
  // Modos de visualización
    case 'o':
    case 'O':
      interruptorObservacion();
      break;
    case 'z':
    case 'Z':
      interruptorZoom();
      break;
    case 'd':
    case 'D':
      interruptorNavegacion();
      break;
  // Velocidad de la animación
    case 'v':
      interruptorDesacelerar();
      break;
    case 'V':
      interruptorAcelerar();
      break;
    case 'r':
    case 'R':
      interruptorReiniciarVelocidad();
      printf("\033[1;35m");
      printf("Velocidad reiniciada\n");
      printf("\033[0m");
      break;
  // Activar/desactivar normales
    case 'n':
    case 'N':
      //interruptorNormales();
      break;
  
    case 27:			// Escape  Terminar
      exit (0);
    default:
      return;
  }

  setCamara (rotxCamara, rotyCamara, 0.0, dCamara);
  glutPostRedisplay ();		// Algunas de las opciones cambian paramentros
}				// de la camara. Es necesario actualziar la imagen

/**		void especial(int k, int x, int y)
Este procedimiento es llamado por el sistema cuando se pulsa una tecla
especial. El codigo k esta definido en glut mediante constantes

Parametros de entrada:

k: codigo del caracter pulsado (definido en glut mediante constantes).

x:

y:

**/
void especial (int k, int x, int y)
{

  switch (k)
    {
    case GLUT_KEY_UP:
      rotxCamara += 5.0;	// Cursor arriba + rotacion x
      if (rotxCamara > 360)
	    rotxCamara -= 360;
      break;
    case GLUT_KEY_DOWN:
      rotxCamara -= 5.0;
      if (rotxCamara < 0)
	    rotxCamara += 360;
      break;
    case GLUT_KEY_LEFT:
      rotyCamara += 5.0;
      if (rotyCamara > 360)
	    rotyCamara -= 360;
      break;
    case GLUT_KEY_RIGHT:
      rotyCamara -= 5.0;
      if (rotyCamara < 0)
	    rotyCamara += 360;
      break;
    case GLUT_KEY_PAGE_DOWN:	// acerca la cámara
      dCamara -= 0.2;
      break;
    case GLUT_KEY_PAGE_UP:	// aleja la cámara
      dCamara += 0.2;
      break;
    default:
      return;
    }
  setCamara (rotxCamara, rotyCamara, 0.0, dCamara);
  glutPostRedisplay ();		// Actualiza la imagen (ver proc. letra)
}
