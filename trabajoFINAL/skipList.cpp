// SkipListPablito.cpp : Este archivo contiene la función "main". La ejecución
// del programa comienza y termina ahí.
//

#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;
using namespace std::chrono;

const int max_level = 17;

// Node
class Node {
private:
  int valor;
  Node **next;

public:
  // getters
  int getValor() const { return valor; }
  Node **getNext() const { return next; }
  // setters
  void setValor(int vvalor) { Node::valor = vvalor; }
  void setNext(Node **nnext) { Node::next = nnext; }

  // constructor
  Node(int nnivel, int &vvalor) {
    // asignamos memoria para avanzar
    next = new Node *[nnivel + 1];

    // Luego llena el nivel de arriba con 0;
    memset(next, 0, sizeof(Node *) * (nnivel + 1));
    // poner el valor
    this->valor = vvalor;
  }

  virtual ~Node() {
    delete[] next; // Elimina el vector de punteros.
  }
};

class skipList {
private:
  Node *head;
  int value;
  int nivel;

public:
  // constructor
  skipList() {
    head = new Node(max_level, value);
    nivel = 0;
  }
  // destructor
  virtual ~skipList() { delete head; }

  float floatRandom() { return (float)rand() / RAND_MAX; }

  int randomLevel() {
    static bool prim = true;
    if (prim) {
      srand((unsigned)time(NULL));
      prim = false;
    }
    int level = (int)(log(floatRandom()) / (log(1. - 0.5)));
    return (level < max_level) ? level : max_level;
  }

  Node *createNode(int valor, int nivel) {
    Node *n = new Node(valor, nivel);
    return n;
  }

  // insertar elemento en skiplist
  void insertElement(int value) {
    Node *nodeCurrent = head;
    Node *update[max_level + 1];
    memset(update, 0, sizeof(Node *) + (max_level + 1));

    // comienza desde el nivel más alto en la lista de saltos. Siempre que el
    // valor sea mayor que el del nodo contiguo actual, mueve el puntero hacia
    // adelante. De lo contrario, insertamos el nodo actual en el vector de
    // actualización y bajamos un nivel seguimos buscando.
    for (int i = nivel; i >= 0; i--) {
      while (nodeCurrent->getNext()[i] != NULL &&
             nodeCurrent->getNext()[i]->getValor() < value) {
        nodeCurrent = nodeCurrent->getNext()[i];
      }
      update[i] = nodeCurrent;
    }
    nodeCurrent = nodeCurrent->getNext()[0];

    // si el nodo Actual es nulo, entonces hemos llegado al final del nivel O
    // debemos insertar el nodo entre update[0] y el nodo Actual
    if (nodeCurrent == NULL || nodeCurrent->getValor() != value) {
      // genera un nivel aleatorio para poner;
      int nivelRandomGeneral = randomLevel();
      // si el nivel aleatorio es mayor que el nivel SkipList (será el nodo con
      // el nivel más alto insertado en la lista) inicializa el valor del vector
      // de actualización con el puntero a la cabeza
      if (nivelRandomGeneral > nivel) {
        for (int i = nivel + 1; i <= nivelRandomGeneral; i++) {
          update[i] = head;
        }
        nivel = nivelRandomGeneral;
      }
      nodeCurrent = new Node(nivelRandomGeneral, value);
      for (int i = 0; i <= nivelRandomGeneral; i++) {
        nodeCurrent->getNext()[i] = update[i]->getNext()[i];
        update[i]->getNext()[i] = nodeCurrent;
      }
    }
  }
  bool find(int valor) {
    Node *nodeCurrent = head;
    // pasar por cada nivel de SkipList.
    for (int i = nivel; i >= 0; i--) {
      // Hago esto hasta que el nodo->siguiente->valor sea menor que el valor,
      // luego voy al siguiente nivel.
      while (nodeCurrent->getNext()[i] != nullptr &&
             nodeCurrent->getNext()[i]->getValor() < value) {
        nodeCurrent = nodeCurrent->getNext()[i];
      }
    }
    nodeCurrent = nodeCurrent->getNext()[0];
    return (nodeCurrent != nullptr && nodeCurrent->getValor() == value);
  }

  void eleminarElemento(int value) {
    Node *nodeCurrent = head;
    Node *update[max_level + 1];

    memset(update, 0, sizeof(Node *) * (max_level + 1));
    for (int i = nivel; i >= 0; i--) {
      while (nodeCurrent->getNext()[i] != nullptr &&
             nodeCurrent->getNext()[i]->getValor() < value) {
        nodeCurrent = nodeCurrent->getNext()[i];
      }
      update[i] = nodeCurrent;
    }
    nodeCurrent = nodeCurrent->getNext()[0];
    if (nodeCurrent->getValor() == value) {
      for (int i = 0; i < nivel; i++) {
        if (update[i]->getNext()[i] != nodeCurrent) {
          break;
        }
        update[i]->getNext()[i] =
            nodeCurrent
                ->getNext()[i]; // de lo contrario, eliminamos los enlaces
      }
      delete nodeCurrent;
      while (nivel > 0 && head->getNext()[nivel] == nullptr) {
        nivel--;
      }
    }
  }
  void printSkipList() {
    for (int i = 0; i < nivel; i++) {
      Node *node = head->getNext()[i];
      cout << "Nivel : " << i << " : ";
      while (node != nullptr) {
        cout << node->getValor() << " ";
        node = node->getNext()[i];
      }
      cout << '\n';
    }
  }
  // sucesor and predecesor
};

int main() {

  //std::cout << "Vamos Pablito\n";
  skipList lista;
  for (int i = 1; i <= 35; i++) {
    lista.insertElement(i);
  }
  lista.insertElement(1);
  lista.printSkipList();

  cout << endl;
  cout << "Eliminando elementos : " << endl;

  lista.eleminarElemento(9);
  lista.eleminarElemento(5);
  lista.eleminarElemento(25);
  lista.eleminarElemento(35);
  lista.printSkipList();

  skipList listaPruebas;

  // 100 elementos
  /*
  auto start = high_resolution_clock::now();
  for (int i = 1; i <= 100; ++i)
  {
      listaPruebas.insertElement(i);
  }
  auto stop = high_resolution_clock::now();
  listaPruebas.printSkipList();
  auto duration = duration_cast<microseconds>(stop - start);
  cout <<"Tiempo en microsegundos con "<<100<<" inserciones :  " <<
  duration.count() << endl;
  */

  // 1000 elementos

  /*
  auto start = high_resolution_clock::now();
  for (int i = 1; i <= 1000; ++i)
  {
      listaPruebas.insertElement(i);
  }
  auto stop = high_resolution_clock::now();
  listaPruebas.printSkipList();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Tiempo en microsegundos con " << 1000 << " inserciones :  " <<
  duration.count() << endl;
  */

  // 10 000
  /*
  auto start = high_resolution_clock::now();
  for (int i = 1; i <= 10000; ++i)
  {
      listaPruebas.insertElement(i);
  }
  auto stop = high_resolution_clock::now();
  listaPruebas.printSkipList();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Tiempo en microsegundos con " << 10000 << " inserciones :  " <<
  duration.count() << endl;
  */

  // 100 000
  /*
  auto start = high_resolution_clock::now();
  for (int i = 1; i <= 100000; ++i)
  {
      listaPruebas.insertElement(i);
  }
  auto stop = high_resolution_clock::now();
  listaPruebas.printSkipList();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Tiempo en microsegundos con " << 100000 << " inserciones :  " <<
  duration.count() << endl;
  */

  return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de
// soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código
//   fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros
//   mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de
//   código, o a Proyecto > Agregar elemento existente para agregar archivos de
//   código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir
//   > Proyecto y seleccione el archivo .sln
