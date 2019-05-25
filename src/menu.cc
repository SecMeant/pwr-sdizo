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
}

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

int main()
{
  sdizo::Heap<sdizo2::Edge, sdizo::HeapType::max> edge_heap_max;
  edge_heap_max.insert({1,2});
  edge_heap_max.insert({4,5});
  edge_heap_max.insert({12,90});
  edge_heap_max.display();

  sdizo::Heap<sdizo2::Edge, sdizo::HeapType::min> edge_heap_min;
  edge_heap_min.insert({1,2});
  edge_heap_min.insert({4,5});
  edge_heap_min.insert({12,90});
  edge_heap_min.display();
  edge_heap_min.pop();
  edge_heap_min.display();

  run_tests();
  return 0;
}
