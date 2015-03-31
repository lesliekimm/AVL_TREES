/**
 *
 * main.cpp
 *
 * Project 2
 * AVL Trees
 * Leslie Kim
 * NetID: LK584
 * COSC 160
 * Section 02
 * In accordance with the class policies and Georgetown's Honor Code,
 * I certify that I have neither given nor received any assistance on
 * this project with the exceptions of lecture notes and those items
 * listed below.
 * Any help items:
 * Description <This program will implement Vector using AVL trees and
 * pointer implementation. All items in each nodes are of type int.>
 *
 **/

#include <iostream>
#include <iomanip>
#include <string>

#include "AVLTree.h"
using namespace std;

// FUNCTION PROTOTYPES
void printInstructions();
char selectMenuOption();
void program();

int main() {
  program();
  
//  AVLTree one;
//  
//  one.insertAtRank(1, 8);
//  one.printAll();
//  one.insertAtRank(1, 2);
//  one.printAll();
//  one.insertAtRank(3, 7);
//  one.printAll();
//  one.insertAtRank(3, 4);
//  one.printAll();
//  one.insertAtRank(4, 5);
//  
//  one.printAll();
//  
//  cout << "Element at 1: " << one.elementAtRank(1) << endl;
//  cout << "Element at 2: " << one.elementAtRank(2) << endl;
//  cout << "Element at 3: " << one.elementAtRank(3) << endl;
//  cout << "Element at 4: " << one.elementAtRank(4) << endl;
//  cout << "Element at 5: " << one.elementAtRank(5) << endl;
  //  cout << "Element at 6: " << one.elementAtRank(6) << endl;
  //  cout << "Element at 7: " << one.elementAtRank(7) << endl;
  //  cout << "Element at 8: " << one.elementAtRank(8) << endl;
  //  cout << "Element at 9: " << one.elementAtRank(9) << endl;
  //  cout << "Element at 10: " << one.elementAtRank(10) << endl;
  
  return 0;
} // END main

// FUNCTION DEFINITIONS
// Prints instructions for program
void printInstructions() {
  cout << "INSTRUCTIONS: This program will allow user to implement" << endl;
  cout << "vectors using AVL trees." << endl << endl;
  cout << "NOTES:" << endl;
  cout << "1) All elements (and ranks) are integers." << endl;
  cout << "2) All elements must be positive integers (1, 2, 3, ...)." << endl;
  cout << "3) Ranks start at 1." << endl;
  cout << "   i.e. First insertion must occur at rank 1." << endl;
  cout << "4) Insertion cannot skip ranks. (Out of bounds)" << endl;
  cout << "   i.e. Cannot insert at rank 8 if highest rank is 3." << endl;
  cout << "5) If prompted for two variables, enter rank followed by" << endl;
  cout << "   element, separated by a space." << endl;
}

// Prints menu options and returns user selection
char selectMenuOption() {
  char selection = ' ';
  string trash;
  
  cout << endl << right << setw(45) << "MENU OPTIONS:" << endl;
  cout << setw(45) << "Return an element at rank r" << setw(10) << "R" << endl;
  cout << setw(45) << "Replace an element at rank r with int e" << setw(10) << "X" << endl;
  cout << setw(45) << "Insert an element at rank r with int e" << setw(10) << "I" << endl;
  cout << setw(45) << "Delete an element at rank r" << setw(10) << "D" << endl;
  cout << setw(45) << "Print all elements in order" << setw(10) << "P" << endl;
  cout << setw(45) << "Clear all elements in vector" << setw(10) << "C" << endl;
  cout << setw(45) << "Quit" << setw(10) << "Q" << endl;
  
  cout << "Please enter your choice: ";
  cin >> selection;
  getline(cin, trash);
  cin.clear();
  cout << endl;
  
  return selection;
}

// Runs program
void program() {
  bool finished = false;
  char userSelection = ' ';
  AVLTree tree;
  int r = 0;
  int e = 0;
  string trash;
  
  printInstructions();
  while(!finished) {
    userSelection = selectMenuOption();
    
    switch(userSelection) {
      case 'R':
      case 'r': {                               // Return element at r
        cout << "Please enter rank int to retrieve element from: ";
        cin >> r;
        getline(cin, trash);
        cin.clear();
        int returned = tree.elementAtRank(r);
        if (returned == -1)
          continue;
        else
          cout << "Element at rank " << r << " is " << returned << "." << endl;
        break;
      }
      case 'X':
      case 'x': {                               // Replace element at r with e
        cout << "Please enter rank int to replace element and" << endl
        << "element int to replace with: ";
        cin >> r >> e;
        getline(cin, trash);
        cin.clear();
        tree.replaceAtRank(r, e);
        break;
      }
      case 'I':
      case 'i': {                               // Insert e at r
        cout << "Please enter rank int and element int to insert," << endl << "separated by a space: ";
        cin >> r >> e;
        getline(cin, trash);
        cin.clear();
        tree.insertAtRank(r, e);
        break;
      }
      case 'D':
      case 'd': {                               // Delete element at r
        cout << "Please enter rank int to delete element from: ";
        cin >> r;
        getline(cin, trash);
        cin.clear();
        tree.removeAtRank(r);
        break;
      }
      case 'P':
      case 'p': {                               // Print all elements
        tree.printAll();
        break;
      }
      case 'Q':
      case 'q': {                               // Gracefully exit program
        cout << "Goodbye!" << endl;
        finished = true;
        break;
      }
      case 'C':
      case 'c': {
        tree.clearTree();
        cout << "Cleared all elements in vector." << endl << endl;
        break;
      }
      default: {                                // Invalid menu selection
        cout << "ERROR: Please select valid menu option." << endl;
        break;
      }
    } // END switch case
  } // END while loop
}