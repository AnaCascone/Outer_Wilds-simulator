# 🪐 Simulación Interactiva del Sistema Planetario de Outer Wilds

Simulación 3D e interactiva del sistema solar del videojuego **Outer Wilds**, hecha con OpenGL. Puedes mover la cámara libremente, explorar cada astro, acercarte a él o enviar una nave a visitarlo.

Proyecto individual, desarrollado como práctica final de Informática Gráfica en la Universidad de Granada.

[!WARNING] Alerta de spoilers. Este proyecto contiene detalles del sistema solar de Outer Wilds (cómo funcionan algunos planetas y qué hay en ellos). Si todavía no has jugado, es mejor que primero lo hagas

---

## ✨ Qué puedes hacer

Existen tres modos de ejecución:

- **🔭 Observación:** haz clic en un astro para ver su nombre y su descripción.
- **🔍 Zoom:** observa los astros más de cerca.
- **🚀 Navegación:** aparece una nave en Lumbre. Elige un planeta con el ratón y viajará hasta él de forma automática, esquivando los objetos que encuentre por el camino.

También hay interruptores para modificar la velocidad de la simulación.

## 🌌 Qué contiene

- **El Sol**, con rotación continua y textura.
- **Los planetas:** Gemelos Reloj de Arena (Gemelo Cenizas y Gemelo Brasas), Lumbre, Hondonada Frágil (una versión simplificada) y Abismo del Gigante, con sus órbitas dibujadas, rotaciones y traslaciones.
- **Los satélites:** Rocaterra (la luna de Lumbre) y la lanzadera de Abismo del Gigante, que se dispara en una dirección distinta en cada ciclo.
- **El intruso:** un cuerpo con órbita elíptica cuyo foco es el núcleo del Sol.
- **La nave:** con cinco estados y trayectoria calculada en tiempo real.

### Algunos detalles técnicos

- **Gemelos Reloj de Arena:** la arena pasa de un planeta a otro con dos esferas de crecimiento inversamente proporcional y una cascada (un cilindro) que se estira y se encoge entre ambos.
- **Hondonada Frágil:** estación espacial anillada que se descompone y cae lentamente al agujero negro, con trozos que reaparecen en el agujero blanco.
- **Animaciones fluidas:** el movimiento usa un `delta` de tiempo en la función `idle`, para que no dependa del rendimiento del equipo.
- **Navegación de la nave:** máquina de estados (`EN_CENTRO_PLANETA`, `SALIENDO_DEL_PLANETA`, `ESPERANDO`, `VIAJANDO`, `ATERRIZANDO`). Al viajar, en cada iteración calcula el vector hacia el destino, orienta la nave y comprueba si habrá colisión cuatro ciclos más adelante para corregir el rumbo.
- **Modelos y texturas:** esfera base creada en Blender y exportada a `.PLY`, reutilizada para todos los objetos esféricos. Algunos modelos se simplificaron para no penalizar el rendimiento.

## 🛠️ Tecnologías

- [LENGUAJE: C o C++] y OpenGL
- Blender (modelado de la esfera base)

## ▶️ Cómo ejecutarlo

```bash
git clone https://github.com/AnaCascone/Outer_Wilds-simulator
cd Outer_Wilds-simulator
make
./practicaIG
```

### Requisitos

- **Compilador de C++** (`g++`)
- **Make**
- **OpenGL**
- **GLUT / FreeGLUT**
- **SDL2**
- **SDL2_mixer**
- **libjpeg**

En sistemas Linux basados en Debian/Ubuntu, puedes instalar las dependencias con:

```bash
sudo apt install g++ make freeglut3-dev libsdl2-dev libsdl2-mixer-dev libjpeg-dev
```

Al ejecutarlo, la terminal muestra el menú de controles y espera a que pulses **A** para empezar.
 
### ⌨️ Controles
 
| Tecla | Acción |
|---|---|
| `A` | Activar / desactivar la animación |
| `N` | Alternar la visualización de las normales |
| `o` / `O` | Modo de observación |
| `z` / `Z` | Modo de zoom |
| `d` / `D` | Modo de navegación |
| `V` (mayúscula) | Acelerar |
| `v` (minúscula) | Desacelerar |
| `r` / `R` | Reiniciar la velocidad |
 
Cámara libre: utiliza las flechas del teclado y RePág y AvPág para acercar o alejar la cámara. 
 
## 🚧 Limitaciones conocidas

- Por falta de tiempo no están incluidos la luna cuántica ni el planeta más alejado del Sol.
- La nave no usa el modelo original del juego, sino uno más simple, para no afectar al rendimiento.
- La asignación de coordenadas UV de la textura del Sol es más compleja de lo que debería. El error está entendido, pero se priorizó el resto del proyecto.

## 🙏 Créditos

- Basado en el sistema solar de **Outer Wilds**, de Mobius Digital. Proyecto de fans sin ánimo de lucro y sin relación con sus creadores.
- Modelo de la nave: [Sketchfab](https://sketchfab.com/3d-models/spaceship-70e786969e70447c86bc4168df8ccbcd)
- Modelo de la lanzadera de Abismo del Gigante:  [Sketchfab](https://sketchfab.com/3d-models/eclipse-class-star-destroyer-aee7185dc9224aa0b424c657f7b9b093) 

---

## 😅 Nota final

Lo sé, lo sé. No existe módulo alguno. Todo está encontrado en una masa llamada "modelo.c", un archivo de más de 2700 líneas.

Durante todo el curso se trabajó con el mismo proyecto, al que se le iban añadiendo poco a poco más opciones hasta finalmente ser el caos que es ahora.

En aquel momento no esperaba que el proyecto se convirtiera en algo tan grande. Y mucho menos que fuera a echarle alrededor de 100 horas. Así que cuando pensé en modularizar... ya era demasiado tarde.

Existe un archivo llamado "Mejoras posibles.txt" al que le añadiré "modularización... por favor".

Es un proyecto al que le tengo realmente cariño, pero no deja de ser una práctica de una versión mía que no terminaba de entender ni cómo hacer un Makefile. Mi intención es, en un futuro próximo, darle un correcto lavado de cara y continuar su desarrollo, con nuevas mejoras, arreglos, añadidos...

Tiempo al tiempo.
