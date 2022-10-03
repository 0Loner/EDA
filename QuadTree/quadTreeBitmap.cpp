#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>

#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

typedef unsigned int int32;
typedef short int16;
typedef unsigned char byte;
using namespace std;

// aser una structura punto para guardar el color y cordenadas para el cua tree
struct punto {
  pair<int, int> cordenadas;
  int color;

  punto(int x, int y, int c) {
    cordenadas.first = x;
    cordenadas.second = y;
    color = c;
  }
  void optener_cordenadas_y_color(int &x, int &y, int &c) {
    x = cordenadas.first;
    y = cordenadas.second;
    c = color;
    cout << "x: " << x << " y: " << y << " color: " << c << endl;
  }
};
// aser una structura nodo para crear los rectangulos a partir de los puntos
struct nodo {
  punto *rectangulo[2]; // Creamos el rectangulo a partir de dos puntos
  vector<punto> puntos; // Vector que guarda los puntos de un rectangulo
  nodo *hijos[4];       // Rectangulos hijos
  int color;            // Color del rectangulo
  // Creamos los rectangulos hijos y los igualamos a null
  nodo(punto *p1, punto *p2) {
    rectangulo[0] = p1;
    rectangulo[1] = p2;
    hijos[0] = hijos[1] = hijos[2] = hijos[3] = NULL;
  }
  nodo() { hijos[0] = hijos[1] = hijos[2] = hijos[3] = NULL; }
  nodo(vector<punto> p) {
    puntos = p;
    hijos[0] = hijos[1] = hijos[2] = hijos[3] = NULL;
  }
  nodo(const nodo &n) {
    rectangulo[0] = n.rectangulo[0];
    rectangulo[1] = n.rectangulo[1];
    hijos[0] = n.hijos[0];
    hijos[1] = n.hijos[1];
    hijos[2] = n.hijos[2];
    hijos[3] = n.hijos[3];
    color = n.color;
  }
  // aser el constructor nodo para dividir
  nodo(nodo *n, int x, int y) {
    rectangulo[0] = n->rectangulo[0];
    rectangulo[1] = n->rectangulo[1];
    hijos[0] = n->hijos[0];
    hijos[1] = n->hijos[1];
    hijos[2] = n->hijos[2];
    hijos[3] = n->hijos[3];
    color = n->color;
  }
  // Funcion que divide el rectangulo en 4 rectangulos hijos
  void dividir() {
    int x1, y1, x2, y2;
    rectangulo[0]->optener_cordenadas_y_color(x1, y1, color);
    rectangulo[1]->optener_cordenadas_y_color(x2, y2, color);
    int x = (x1 + x2) / 2;
    int y = (y1 + y2) / 2;
    hijos[0] = new nodo(new punto(x1, y1, color), new punto(x, y, color));
    hijos[1] = new nodo(new punto(x, y1, color), new punto(x2, y, color));
    hijos[2] = new nodo(new punto(x1, y, color), new punto(x, y2, color));
    hijos[3] = new nodo(new punto(x, y, color), new punto(x2, y2, color));
  }
  // Funcion que verifica si el rectangulo es hoja
  bool es_hoja() { return hijos[0] == NULL; }
  // Funcion que verifica si el rectangulo es de un solo color y si no es
  // mostrar el color mas repetido
  void verificar_color() {
    if (es_hoja()) {
      if (puntos.size() == 1) {
        color = puntos[0].color;
      } else {
        int color1 = 0, color2 = 0, color3 = 0, color4 = 0;
        for (int i = 0; i < puntos.size(); i++) {
          if (puntos[i].color == 1) {
            color1++;
          } else if (puntos[i].color == 2) {
            color2++;
          } else if (puntos[i].color == 3) {
            color3++;
          } else if (puntos[i].color == 4) {
            color4++;
          }
        }
        if (color1 > color2 && color1 > color3 && color1 > color4) {
          color = 1;
        } else if (color2 > color1 && color2 > color3 && color2 > color4) {
          color = 2;
        } else if (color3 > color1 && color3 > color2 && color3 > color4) {
          color = 3;
        } else if (color4 > color1 && color4 > color2 && color4 > color3) {
          color = 4;
        } else {
          color = 0;
        }
      }
    } else {
      for (int i = 0; i < 4; i++) {
        hijos[i]->verificar_color();
      }
    }
  }
};
// Funcion que lee el archivo bmp y lo guarda en un vector de puntos
void leer_archivo(vector<punto> &puntos, byte **pixels, int32 *width,
                  int32 *height, int32 *bytesPerPixel) {

  FILE *imageFile = (fopen_s(&imageFile, "img.bmp", "rb"));
  if (imageFile == NULL) {
    cout << "No se pudo abrir el archivo" << endl;
  } else {
    // Leemos el encabezado del archivo
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, imageFile);

    // Leemos la imagen
    *pixels = new unsigned char[*width * *height * *bytesPerPixel];
    fread(*pixels, sizeof(unsigned char), *width * *height * *bytesPerPixel,
          imageFile);
    fclose(imageFile);
    // Guardamos los puntos en el vector
    for (int i = 0; i < *height; i++) {
      for (int j = 0; j < *width; j++) {
        int color = 0;
        if ((*pixels)[(i * *width + j) * *bytesPerPixel] == 255 &&
            (*pixels)[(i * *width + j) * *bytesPerPixel + 1] == 255 &&
            (*pixels)[(i * *width + j) * *bytesPerPixel + 2] == 255) {
          color = 1;
        } else if ((*pixels)[(i * *width + j) * *bytesPerPixel] == 0 &&
                   (*pixels)[(i * *width + j) * *bytesPerPixel + 1] == 0 &&
                   (*pixels)[(i * *width + j) * *bytesPerPixel + 2] == 0) {
          color = 2;
        } else if ((*pixels)[(i * *width + j) * *bytesPerPixel] == 255 &&
                   (*pixels)[(i * *width + j) * *bytesPerPixel + 1] == 0 &&
                   (*pixels)[(i * *width + j) * *bytesPerPixel + 2] == 0) {
          color = 3;
        } else if ((*pixels)[(i * *width + j) * *bytesPerPixel] == 0 &&
                   (*pixels)[(i * *width + j) * *bytesPerPixel + 1] == 0 &&
                   (*pixels)[(i * *width + j) * *bytesPerPixel + 2] == 255) {
          color = 4;
        }
        puntos.push_back(punto(j, i, color));
      }
    }
  }
}
// Funcion que crea el arbol
void crear_arbol(nodo *raiz, vector<punto> &puntos) {
  if (raiz->es_hoja()) {
    for (int i = 0; i < puntos.size(); i++) {
      int x1, y1, x2, y2, color;
      raiz->rectangulo[0]->optener_cordenadas_y_color(x1, y1, color);
      raiz->rectangulo[1]->optener_cordenadas_y_color(x2, y2, color);
      int x = puntos[i].cordenadas.first;
      int y = puntos[i].cordenadas.second;
      if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
        raiz->puntos.push_back(puntos[i]);
      }
    }
    if (raiz->puntos.size() > 1) {
      raiz->dividir();
      for (int i = 0; i < 4; i++) {
        crear_arbol(raiz->hijos[i], raiz->puntos);
      }
    }
  }
}
// Funcion que muestra el arbol
void mostrar_arbol(nodo *raiz) {
  if (raiz->es_hoja()) {
    int x1, y1, x2, y2, color;
    raiz->rectangulo[0]->optener_cordenadas_y_color(x1, y1, color);
    raiz->rectangulo[1]->optener_cordenadas_y_color(x2, y2, color);
    cout << "Rectangulo: (" << x1 << "," << y1 << ") (" << x2 << "," << y2
         << ")" << endl;
    cout << "Puntos: " << endl;
    for (int i = 0; i < raiz->puntos.size(); i++) {
      cout << raiz->puntos[i].cordenadas.first << " "
           << raiz->puntos[i].cordenadas.second << " " << raiz->puntos[i].color
           << endl;
    }
    cout << endl;
  } else {
    for (int i = 0; i < 4; i++) {
      mostrar_arbol(raiz->hijos[i]);
    }
  }
}
// Funcion que muestra el arbol con los colores
void mostrar_arbol_colores(nodo *raiz) {
  if (raiz->es_hoja()) {
    int x1, y1, x2, y2, color;
    raiz->rectangulo[0]->optener_cordenadas_y_color(x1, y1, color);
    raiz->rectangulo[1]->optener_cordenadas_y_color(x2, y2, color);
    cout << "Rectangulo: (" << x1 << "," << y1 << ") (" << x2 << "," << y2
         << ")" << endl;
    cout << "Color: " << raiz->color << endl;
    cout << endl;
  } else {
    for (int i = 0; i < 4; i++) {
      mostrar_arbol_colores(raiz->hijos[i]);
    }
  }
}

int main() {
  byte *pixels;
  int32 width;
  int32 height;
  int32 bytesPerPixel;
  vector<punto> puntos;
  leer_archivo(puntos, &pixels, &width, &height, &bytesPerPixel);
  nodo *raiz = new nodo(puntos);
  crear_arbol(raiz, puntos);
  mostrar_arbol(raiz);
  mostrar_arbol_colores(raiz);
}
// Quadtree segmentar la imagen , particionar y ver la desviacion standar 0, la
// desviacio estandar es 0 si es del mismo color. Hacer el quadtree con una
// matriz 8x8 El proximo viernes explicar nuestra insercion del r-tree
