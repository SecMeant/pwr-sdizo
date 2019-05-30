#include <stdio.h>
#include "array.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "tree.hpp"
#include "redblacktree.hpp"
#include "test.hpp"
#include "mst.hpp"
#include "timeutils.hpp"
#include <fmt/format.h>
#include <iostream>
#include <random>

#define GET_OPTION(buffer) while(!scanf(" %c", &buffer))\
                           while(getchar() != '\n');

void run_tests()
{
  using namespace sdizo::tests;
  TEST("Array test", run_array_tests);
  TEST("List test", run_list_tests);
  TEST("Heap test", run_heap_tests);
  TEST("BST test", run_bst_tests);
  TEST("RBT test", run_rbt_tests);
  TEST("Disjoint sets test", run_disjoint_set_tests);
  TEST("Templatize tests", run_templatize_tests);
}

namespace sdizo{
void menu_array(sdizo::Array &array)
{
  using namespace std;
  char option;
  string fileName;
  int index, value;

  do
  {
    puts("--- TABLICA ---");
    puts("1.Wczytaj z pliku");
    puts("2.Usun");
    puts("3.Wstaw");
    puts("4.Wstaw na koniec");
    puts("5.Wstaw na poczatek");
    puts("6.Czy zawiera");
    puts("7.Generuj");
    puts("8.Pokaz");
    puts("0.Wyjdz");

    printf("Podaj opcje: ");
    GET_OPTION(option);

    switch (option)
    {
      case '1':
        printf(" Podaj nazwê zbioru:");
        cin >> fileName;
        array.loadFromFile(fileName.c_str());
        break;

      case '2':
        printf(" podaj index:");
        cin >> index;
        array.removeAt(index);
        break;

      case '3':
        printf(" podaj index:");
        cin >> index;
        printf(" podaj waertoæ:");
        cin >> value;

        array.insert(value,index);
        break;

      case '4':
        printf(" podaj waertoæ:");
        cin >> value;

        array.append(value);
        break;

      case '5':
        printf(" podaj waertoæ:");
        cin >> value;

        array.prepend(value);
        break;

      case '6':
        printf(" podaj waertoæ:");
        cin >> value;
        if (array.contains(value))
          puts("poadana wartoc jest w tablicy");
        else
          puts("poadanej wartoci NIE ma w tablicy");
        break;

      case '7':
        printf("Podaj iloæ elementów tablicy:");
        cin >> value;
        array.generate(0,100,value);
        break;

      case '8':
        array.display();
        break;

    }

    array.display();
  } while (option != '0');
}

void menu_list(sdizo::List<sdizo::ListNode<int32_t>> &list)
{
  using namespace std;
  char option;
  string fileName;
  int index, value;

  do
  {
    puts("--- LISTA ---");
    puts("1.Wczytaj z pliku");
    puts("2.Usun z pozycji");
    puts("3.Usun");
    puts("4.Wstaw");
    puts("5.Wstaw na koniec");
    puts("6.Wstaw na poczatek");
    puts("7.Czy zawiera");
    puts("8.Generuj");
    puts("9.Pokaz");
    puts("0.Wyjdz");

    printf("Podaj opcje: ");
    GET_OPTION(option);

    switch (option)
    {
      case '1':
        printf(" Podaj nazwê zbioru:");
        cin >> fileName;
        list.loadFromFile(fileName.c_str());
        break;

      case '2':
        printf(" podaj index:");
        cin >> index;
        list.removeAt(index);
        break;

      case '3':
        printf(" podaj wartosc:");
        cin >> value;
        list.remove(value);
        break;

      case '4':
        printf(" podaj index:");
        cin >> index;
        printf(" podaj waertoæ:");
        cin >> value;

        list.insert(value,index);
        break;

      case '5':
        printf(" podaj waertoæ:");
        cin >> value;

        list.append(value);
        break;

      case '6':
        printf(" podaj waertoæ:");
        cin >> value;

        list.prepend(value);
        break;

      case '7':
        printf(" podaj waertoæ:");
        cin >> value;
        if (list.contains(value))
          puts("poadana wartoc jest w tablicy");
        else
          puts("poadanej wartoci NIE ma w tablicy");
        break;

      case '8':
        printf("Podaj iloæ elementów tablicy:");
        cin >> value;
        list.generate(0,100,value);
        break;

      case '9':
        list.display();
        break;

    }
    list.display();
  } while (option != '0');
}

void menu_heap(sdizo::Heap<int32_t> &heap)
{
  using namespace std;
  char option;
  string fileName;
  int32_t value;

  do
  {
    puts("--- KOPIEC ---");
    puts("1.Wczytaj z pliku");
    puts("2.Usun element");
    puts("3.Wstaw");
    puts("4.Czy zawiera");
    puts("5.Generuj");
    puts("6.Pokaz");
    puts("0.Wyjdz");

    printf("Podaj opcje: ");
    GET_OPTION(option);

    switch (option)
    {
      case '1':
        printf("Podaj nazwê zbioru:");
        cin >> fileName;
        heap.loadFromFile(fileName.c_str());
        break;

      case '2':
        printf("podaj element:");
        cin >> value;
        heap.remove(value);
        break;

      case '3':
        printf("podaj waertoæ:");
        cin >> value;

        heap.insert(value);
        break;

      case '4':
        printf(" podaj waertoæ:");
        cin >> value;
        if (heap.contains(value))
          puts("poadana wartoc jest w tablicy");
        else
          puts("poadanej wartoci NIE ma w tablicy");
        break;

      case '5':
        printf("Podaj iloæ elementów tablicy:");
        cin >> value;
        heap.generate(0,100,value);
        break;

      case '6':
        heap.display();
        break;
    }
    heap.display();
  } while (option != '0');
}

void menu_redblacktree(sdizo::RedBlackTree &tree)
{
  using namespace std;
  char option;
  string fileName;
  int32_t value;

  do
  {
    puts("--- Drzewo czerwono czarne ---");
    puts("1.Wczytaj z pliku");
    puts("2.Usun");
    puts("3.Wstaw");
    puts("4.Czy zawiera");
    puts("5.Generuj");
    puts("6.Pokaz");
    puts("0.Wyjdz");

    printf("Podaj opcje: ");
    GET_OPTION(option);

    switch (option)
    {
      case '1':
        printf(" Podaj nazwê zbioru:");
        cin >> fileName;
        tree.loadFromFile(fileName.c_str());
        break;

      case '2':
        printf(" podaj element:");
        cin >> value;
        tree.remove(value);
        break;

      case '3':
        printf(" podaj waertoæ:");
        cin >> value;

        tree.insert(value);
        break;

      case '4':
        printf(" podaj waertoæ:");
        cin >> value;
        if (tree.contains(value))
          puts("poadana wartoc jest w tablicy");
        else
          puts("poadanej wartoci NIE ma w tablicy");
        break;

      case '5':
        printf("Podaj iloæ elementów tablicy:");
        cin >> value;
        tree.generate(0,100,value);
        break;

      case '8':
        tree.display();
        break;
    }
    tree.display();
  } while (option != '0');
}

void menu()
{
  using namespace std;

  sdizo::Array array;
  sdizo::List<sdizo::ListNode<int32_t>> list;
  sdizo::Heap<int32_t> heap;
  sdizo::RedBlackTree tree;

  char option;
  do
  {
    cout << endl;
    puts("==== MENU GLOWNE ===");
    puts("1.Tablica");
    puts("2.Lista");
    puts("3.Kopiec");
    puts("4.Drzewo czerwono czarne");
    puts("0.Wyjscie");
    puts("Podaj opcje:");
    GET_OPTION(option);

    switch (option)
    {
      case '1':
        menu_array(array);
        break;

      case '2':
        menu_list(list);
        break;

      case '3':
        menu_heap(heap);
        break;

      case '4':
        menu_redblacktree(tree);
        break;
    }

  } while (option != '0');
}
}; // namespace sdizo

namespace sdizo2{
void menu_kruskal(sdizo2::KruskalSolver& ksolver)
{
  using namespace std;
  char option;
  string fileName;

  do
  {
    puts("--- Algorytm Kruskala MST ---");
    puts("1.Wczytaj z pliku");
    puts("2.Rozwiaż");
    puts("3.Wyświetl wynik w postaci listy");
    puts("4.Wyświetl wynik w postaci macierzy");
    puts("5.Wyświetl wczytane dane (macierz)");
    puts("6.Wyświetl wczytane dane (lista)");
    puts("7.Generuj");
    puts("0.Wyjdz");

    printf("Podaj opcje: ");
    GET_OPTION(option);

    switch (option)
    {
      case '1':
        printf(" Podaj nazwe pliku:");
        cin >> fileName;

        try{
          ksolver.loadFromFile(fileName.c_str());
        }catch(std::runtime_error& err){
          err.what();
        }

        break;

      case '2':
        ksolver.solve();
        break;

      case '3':
        ksolver.display_list();
        break;

      case '4':
        ksolver.display_matrix();
        break;

      case '5':
        ksolver.display_buffer_matrix();
        break;

      case '6':
        ksolver.display_buffer_list();
        break;

      case '7':
      {
        int32_t node_cnt;
        double density;

        fmt::print("Podaj ilość wierzchołkóœ: ");
        cin >> node_cnt;

        fmt::print("Podaj gęstość: ");
        cin >> density;

        ksolver.generate(node_cnt, density);
        break;
      }
    }
  } while (option != '0');
}

void menu_prim(sdizo2::PrimSolver& psolver)
{
  using namespace std;
  char option;
  string fileName;

  do
  {
    puts("--- Algorytm Prima MST ---");
    puts("1.Wczytaj z pliku");
    puts("2.Rozwiaż");
    puts("3.Wyświetl wynik w postaci listy");
    puts("4.Wyświetl wynik w postaci macierzy");
    puts("5.Wyświetl wczytane dane (macierz)");
    puts("6.Wyświetl wczytane dane (lista)");
    puts("7.Generuj");
    puts("0.Wyjdz");

    printf("Podaj opcje: ");
    GET_OPTION(option);

    switch (option)
    {
      case '1':
        printf(" Podaj nazwe pliku:");
        cin >> fileName;

        try{
          psolver.loadFromFile(fileName.c_str());
        }catch(std::runtime_error& err){
          err.what();
        }

        break;

      case '2':
        psolver.solve();
        break;

      case '3':
        psolver.display_list();
        break;

      case '4':
        psolver.display_matrix();
        break;

      case '5':
        psolver.display_buffer_matrix();
        break;

      case '6':
        psolver.display_buffer_list();
        break;

      case '7':
      {
        int32_t node_cnt;
        double density;

        fmt::print("Podaj ilość wierzchołkóœ: ");
        cin >> node_cnt;

        fmt::print("Podaj gęstość: ");
        cin >> density;

        psolver.generate(node_cnt, density);
        break;
      }
    }
  } while (option != '0');
}

void menu()
{
  using namespace std;

  sdizo2::KruskalSolver ksolver;
  sdizo2::PrimSolver psolver;

  char option;
  do
  {
    cout << endl;
    puts("==== MENU GLOWNE ===");
    puts("1.Kruskal solver");
    puts("2.Prim solver");
    puts("0.Wyjscie");
    printf("Podaj opcje: ");
    GET_OPTION(option);

    switch (option)
    {
      case '1':
        menu_kruskal(ksolver);
        break;

      case '2':
        menu_prim(psolver);
        break;

      default:
        break;
    }
  } while (option != '0');
}
}; // namespace sdizo2

int main()
{
  sdizo2::menu();

  return 0;
}
