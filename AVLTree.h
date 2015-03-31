/**
 *
 * AVLTree.h
 *
 * Project 2
 * AVL Trees
 * Leslie Kim
 * NetID: LK584
 * COSC 160
 * Section 02
 *
 **/

#ifndef __AVLTrees__AVLTree__
#define __AVLTrees__AVLTree__

#include <stdio.h>

/*****************************************************
 *                                                   *
 *              struct AVLNode definition            *
 *                                                   *
 *****************************************************/

struct AVLNode {
  int element;
  AVLNode *lChild;
  AVLNode *rChild;
  int lHeight;
  int rHeight;
  int nodesInLeftSubtree;
  
  AVLNode();
  AVLNode(int);
}; // END AVLNode definition

typedef struct AVLNode* AVLptr;

/*****************************************************
 *                                                   *
 *              struct AVLNode definition            *
 *                                                   *
 *****************************************************/

class AVLTree {
private:
  AVLptr root;
  int totalAVLNodes;
  
  // HELPER FUNCTIONS
  void makeEmpty(AVLptr&);
  
  int getHeight(AVLptr);
  int difference(AVLptr);
  
  // ROTATION FUNCTIONS
  void rrRotation(AVLptr, AVLptr);
  void llRotation(AVLptr, AVLptr);
  void rlRotation(AVLptr, AVLptr);
  void lrRotation(AVLptr, AVLptr);
  void balance(AVLptr, AVLptr);
  
  void traverseTo(AVLptr&, int);
  void insert(AVLptr, int, int);
  int remove(AVLptr, int);
  void print(AVLptr);
public:
  AVLTree();
  ~AVLTree();
  
  int getTotalAVLNodes() { return totalAVLNodes; }
    
  // ASSIGNMENT FUNCTIONS
  int elementAtRank(int);
  void replaceAtRank(int, int);
  void insertAtRank(int, int);
  void removeAtRank(int);
  void printAll();
  
  void clearTree();
}; // END AVLTree definition
#endif