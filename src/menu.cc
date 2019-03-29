#include <stdio.h>
#include "array.hpp"
#include "list.hpp"
#include "heap.hpp"
#include "tree.hpp"
#include "test.hpp"
#include <iostream>

void run_tests()
{
  using namespace sdizo::tests;
  TEST("Array test", run_array_tests);
  TEST("List test", run_list_tests);
  TEST("Heap test", run_heap_tests);
  TEST("BST test", run_bst_tests); 
}

void menu_array(sdizo::Array &array)
{
  using namespace std;
  char option;
  string fileName;
  int index, value;

  do{
    puts("--- TABLICA ---");
    puts("1.Wczytaj z pliku");
    puts("2.Usun");
    puts("3.Wstaw");
    puts("4.Czy zawiera");
    puts("5.Generuj");
    puts("0.Wyjdz");

    printf("Podaj opcje: ");
    scanf(" %c", &option);

    switch (option){
    case '1': 
      printf(" Podaj nazwê zbioru:");
      cin >> fileName;
      array.loadFromFile(fileName.c_str());
      array.display();
      break;

    case '2':
      printf(" podaj index:");
      cin >> index;
      array.removeAt(index);
      array.display();
      break;

    case '3':
      printf(" podaj index:");
      cin >> index;
      printf(" podaj waertoæ:");
      cin >> value;

      array.insert(value,index);
      array.display();      
      break;

    case '4':
      printf(" podaj waertoæ:");
      cin >> value;
      if (array.contains(value))
        puts("poadana wartoc jest w tablicy");
      else
        puts("poadanej wartoci NIE ma w tablicy");
      break;

    case '5':
      printf("Podaj iloæ elementów tablicy:");
      cin >> value;
      array.generate(0,100,value);
      array.display();
      break;

    case '6':
      array.display();
      break;
    }

  } while (option != '0');
}

void menu_list()
{

}

void menu_heap()
{

}

int main()
{
  using namespace std;

  sdizo::Array array;

  char option;
  do
  {
    cout << endl;
    puts("==== MENU GLOWNE ===");
    puts("1.Tablica");
    puts("2.Lista");
    puts("3.Kopiec");
    puts("0.Wyjscie");
    puts("Podaj opcje:");
    scanf(" %c", &option);

    switch (option)
    {
      case '1':
        menu_array(array);
        break;

      case '2':
        menu_list();
        break;

      case '3':
        menu_heap();
        break;    
    }

  } while (option != '0');

  return 0;
}
