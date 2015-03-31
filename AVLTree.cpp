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

#include <iostream>
#include <algorithm>
#include <stack>

#include "AVLTree.h"
using namespace std;

/**************************************************************
 *                                                            *
 *                struct AVLNode implementation               *
 *                                                            *
 **************************************************************/

AVLNode::AVLNode() {
  element = 0;
  lChild = NULL;
  rChild = NULL;
  lHeight = 0;
  rHeight = 0;
  nodesInLeftSubtree = 0;
}

AVLNode::AVLNode(int e) {
  element = e;
  lChild = NULL;
  rChild = NULL;
  lHeight = 0;
  rHeight = 0;
  nodesInLeftSubtree = 0;
}

/**************************************************************
 *                                                            *
 *                class AVLTree implementation                *
 *                                                            *
 **************************************************************/

AVLTree::AVLTree() {
  root = NULL;
  totalAVLNodes = 0;
}

AVLTree::~AVLTree() {
  makeEmpty(root);
}

void AVLTree::makeEmpty(AVLptr &root) {
  if (root != NULL) {
    makeEmpty(root->lChild);
    makeEmpty(root->rChild);
    delete root;
  }
  root = NULL;
  totalAVLNodes = 0;
}

int AVLTree::getHeight(AVLptr current) {                          // Returns max of lChild & rChild + 1
  int h = 0;
  
  if (current != NULL) {
    current->lHeight = getHeight(current->lChild);
    current->rHeight = getHeight(current->rChild);
    int maxHeight = max(current->lHeight, current->rHeight);
    h = maxHeight + 1;
  }
  return h;
}

int AVLTree::difference(AVLptr current) {                         // Computes height difference
  int lHeight = getHeight(current->lChild);
  int rHeight = getHeight(current->rChild);
  int diff = lHeight - rHeight;
  return diff;
}

void AVLTree::rrRotation(AVLptr parent, AVLptr violation) {
  AVLptr temp = violation->rChild;
  
  violation->rChild = temp->lChild;
  
  if (parent == violation) {
    root = temp;
    temp->lChild = violation;
  }
  else {
    temp->lChild = violation;
    if (parent->lChild == violation)
      parent->lChild = temp;
    else if (parent->rChild == violation)
      parent->rChild = temp;
  }
  
  violation->lHeight = getHeight(violation->lChild);            // Update heights & nodesInLeftSubtree
  violation->rHeight = getHeight(violation->rChild);
  temp->lHeight = getHeight(violation);
  temp->nodesInLeftSubtree += violation->nodesInLeftSubtree + 1;
}

void AVLTree::llRotation(AVLptr parent, AVLptr violation) {
  AVLptr temp = violation->lChild;

  violation->lChild = temp->rChild;
  
  if (parent == violation) {
    root = temp;
    temp->rChild = violation;
  }
  else {
    temp->rChild = violation;
    if (parent->rChild == violation)
      parent->rChild = temp;
    else if (parent->lChild == violation)
      parent->lChild = temp;
  }

  violation->lHeight = getHeight(violation->lChild);            // Update heights & nodesInLeftSubtree
  violation->rHeight = getHeight(violation->rChild);
  temp->rHeight = getHeight(violation);
  violation->nodesInLeftSubtree -= (temp->nodesInLeftSubtree + 1);
}

void AVLTree::rlRotation(AVLptr parent, AVLptr violation) {
  AVLptr temp = violation->rChild;
  AVLptr tempL = temp->lChild;
  AVLptr tempLL = tempL->lChild;
  AVLptr tempLR = tempL->rChild;
  int tempLLST = tempL->nodesInLeftSubtree;

  parent->lChild = tempL;
  tempL->rChild = temp;
  violation->rChild = tempLL;
  tempL->lChild = violation;
  temp->lChild = tempLR;
  
  if (parent == violation)
    root = tempL;
  
  temp->lHeight = getHeight(tempLR);                            // Update heights
  violation->rHeight = getHeight(tempLL);
  tempL->lHeight = getHeight(violation);
  tempL->rHeight = getHeight(temp);

  if (tempLL != NULL)                                           // Update nodesInLeftSubtree
    tempL->nodesInLeftSubtree += violation->nodesInLeftSubtree + 1;
  else
    tempL->nodesInLeftSubtree = violation->nodesInLeftSubtree + 1;
  if (tempLR != NULL)
    temp->nodesInLeftSubtree -= (tempLLST + 1);
  else
    temp->nodesInLeftSubtree = 0;
}

void AVLTree::lrRotation(AVLptr parent, AVLptr violation) {
  AVLptr temp = violation->lChild;
  AVLptr tempR = temp->rChild;
  AVLptr tempRL = tempR->lChild;
  AVLptr tempRR = tempR->rChild;
  int tempRLST = tempR->nodesInLeftSubtree;
  
  parent->rChild = tempR;
  tempR->lChild = temp;
  violation->lChild = tempRR;
  tempR->rChild = violation;
  temp->rChild = tempRL;
  
  if (parent == violation)
    root = tempR;
  
  temp->rHeight = getHeight(tempRL);                            // Update heights
  violation->lHeight = getHeight(tempRR);
  tempR->lHeight = getHeight(temp);
  tempR->rHeight = getHeight(violation);
  
  if (tempRR != NULL)                                           // Update nodesInLeftSubtree
    violation->nodesInLeftSubtree -= (temp->nodesInLeftSubtree + 1 + tempR->nodesInLeftSubtree + 1);
  else
    violation->nodesInLeftSubtree = 0;
  if (tempRL != NULL)
    tempR->nodesInLeftSubtree = tempRLST + (temp->nodesInLeftSubtree + 1);
  else
    tempR->nodesInLeftSubtree = temp->nodesInLeftSubtree + 1;
}

void AVLTree::balance(AVLptr parent, AVLptr violation) {
  int balanced = violation->lHeight - violation->rHeight;
  
  if (balanced > 1) {
    if (difference(violation->lChild) >= 0) {                   // LL violation
      cout << "LL VIOLATION" << endl;
      llRotation(parent, violation);
    }
    else {                                                      // LR violation
      cout << "LR VIOLATION" << endl;
      lrRotation(parent, violation);
    }
  }
  else if (balanced < -1) {                                     // RL violation
    if (difference(violation->rChild) > 0) {
      cout << "RL VIOLATION" << endl;
      rlRotation(parent, violation);
    }
    else {                                                      // RR violation
      cout << "RR VIOLATION" << endl;
      rrRotation(parent, violation);
    }
  }
}

void AVLTree::traverseTo(AVLptr &current, int r) {
  int nodeCount = 0;                                            // Current index of pointer
  int ptrCount = 0;                                             // Current count of nodes in left subtree
  
  if ((current == NULL) || (r <= 0) || (r > totalAVLNodes))     // Empty vector
    return;
  else {                                                        // Traversing to node of rank r
    while (nodeCount != r) {
      ptrCount = current->nodesInLeftSubtree + 1;
      if (r == ptrCount + nodeCount)                            // Found the node of rank r
        nodeCount += ptrCount;
      else if (r < ptrCount + nodeCount) {                      // Node of rank r is to the left
        if (current->lChild != NULL)
          current = current->lChild;
      }
      else if (r > ptrCount + nodeCount) {                      // Node of rank r is to the right
        nodeCount += ptrCount;
        if (current->rChild != NULL)
          current = current->rChild;
      }
    } // END while
  } // END main else
}

int AVLTree::elementAtRank(int r) {
  AVLptr current = root;
  
  if (current == NULL) {
    cout << "ERROR: Attempting retrieval of element from empty vector." << endl << endl;
    return -1;
  }
  if ((r <= 0) || (r > totalAVLNodes)) {                        // Retrieval at rank out of bounds
    cout << "ERROR: Attempting retrieval of element of out of bounds" << endl
        << "rank. Current number of elements in vector is ";
    if (totalAVLNodes == 1)
      cout << "1. Only valid" << endl << "retrieval rank is 1." << endl << endl;
    else
      cout << totalAVLNodes << ". Valid" << endl << "ranks to retrieve elements range from 1 - "
          << totalAVLNodes << "." << endl << endl;
    return -1;
  }
  else {                                                        // Legitimate retrieval
    traverseTo(current, r);
    return current->element;
  }
}

void AVLTree::replaceAtRank(int r, int e) {
  AVLptr current = root;
  
  if (current == NULL) {
    cout << "ERROR: Attempting replacement in empty vector." << endl << endl;
    return;
  }
  if (e <= 0) {
    cout << "ERROR: Out of bounds. Element must be a positive integer." << endl << endl;
    return;
  }
  if ((r <= 0) || (r > totalAVLNodes)) {                        // Replacement at rank out of bounds
    if (totalAVLNodes == 0)
      cout << "ERROR: Attempting replacement in empty vector." << endl << endl;
    else if (totalAVLNodes == 1)
      cout << "ERROR: Attempting replacement of out of bounds rank." << endl
           << "Current number of elements in vector is " << totalAVLNodes << ". Only valid" << endl
           << "replacement rank is 1." << endl;
    else
      cout << "ERROR: Attempting replacement of out of bounds rank." << endl
           << "Current number of elements in vector is " << totalAVLNodes << ". Valid" << endl
           << "replacement ranks to replace range from 1 - " << totalAVLNodes << "." << endl;
    return;
  } // END main if
  else {                                                        // Legitimate replacement
    traverseTo(current, r);
    int previous = current->element;
    current->element = e;
    cout << "Replaced " << previous << " at rank " << r << " with " << e << "." << endl;
  }
}

void AVLTree::insert(AVLptr current, int r, int e) {
  int nodeCount = 0;                                            // Current index of pointer
  int ptrCount = 0;                                             // Current count of nodes in left subtree
  stack<AVLptr> pointers;
  stack<char> direction;
  
  if (r == 1) {                                                 // Inserting into rank 1
    while (current->lChild != NULL) {
      current->nodesInLeftSubtree++;
      pointers.push(current);
      direction.push('L');
      current = current->lChild;
    }
    pointers.push(current);
    direction.push('L');
    current->nodesInLeftSubtree++;
    current->lChild = new AVLNode(e);
    totalAVLNodes++;
    while (!pointers.empty()) {                                 // Update heights & balance
      pointers.top()->lHeight = getHeight(pointers.top()->lChild);
      AVLptr violation = pointers.top();
      pointers.pop();
      direction.pop();
      AVLptr parent;
      if (pointers.empty())
        parent = root;
      else
        parent = pointers.top();
      balance(parent, violation);
    }
  } // END main if
  else if (r == (totalAVLNodes + 1)) {                          // Inserting into last rank
    while (current->rChild != NULL) {
      pointers.push(current);
      direction.push('R');
      current = current->rChild;
    }
    pointers.push(current);
    direction.push('R');
    current->rChild = new AVLNode(e);
    totalAVLNodes++;
    while (!pointers.empty()) {                                 // Update heights & balance
      pointers.top()->rHeight = getHeight(pointers.top()->rChild);
      AVLptr violation = pointers.top();
      pointers.pop();
      direction.pop();
      AVLptr parent;
      if (pointers.empty())
        parent = root;
      else
        parent = pointers.top();
      balance(parent, violation);
    }
  } // END else if
  else {                                                        // All other insertions
    while (nodeCount != r) {
      ptrCount = current->nodesInLeftSubtree + 1;
      if (r == ptrCount + nodeCount) {                          // Insert into root rank
        current->nodesInLeftSubtree++;
        pointers.push(current);
        direction.push('L');
        if (current->lChild == NULL) {
          nodeCount += ptrCount;
          pointers.push(current);
          direction.push('L');
          current->lChild = new AVLNode(e);
          totalAVLNodes++;
          while (!pointers.empty()) {                           // Update heights
            if (direction.top() == 'L')
              pointers.top()->lHeight = getHeight(pointers.top()->lChild);
            else
              pointers.top()->rHeight = getHeight(pointers.top()->rChild);
            AVLptr violation = pointers.top();
            pointers.pop();
            direction.pop();
            AVLptr parent;
            if (pointers.empty())
              parent = root;
            else
              parent = pointers.top();
            balance(parent, violation);
          }
          return;
        }
        else if (current->lChild != NULL) {
          current = current->lChild;
          while (current->rChild != NULL) {
            pointers.push(current);
            direction.push('R');
            current = current->rChild;
          }
          pointers.push(current);
          direction.push('R');
          current->rChild = new AVLNode(e);
          totalAVLNodes++;
          while (!pointers.empty()) {                           // Update heights
            if (direction.top() == 'L')
              pointers.top()->lHeight = getHeight(pointers.top()->lChild);
            else
              pointers.top()->rHeight = getHeight(pointers.top()->rChild);
            AVLptr violation = pointers.top();
            pointers.pop();
            direction.pop();
            AVLptr parent;
            if (pointers.empty())
              parent = root;
            else
              parent = pointers.top();
            balance(parent, violation);
          }
          return;
        }
      } // END if
      else if (r < ptrCount + nodeCount) {                      // Node of rank r is to the left
        current->nodesInLeftSubtree++;
        pointers.push(current);
        direction.push('L');
        if (current->lChild != NULL)
          current = current->lChild;
      } // END else if
      else if (r > ptrCount + nodeCount) {                      // Node of rank r is to the right
        nodeCount += ptrCount;
        pointers.push(current);
        direction.push('R');
        if (current->rChild != NULL)
          current = current->rChild;
      } // END else if
    } // END inside while
  } // END main else
}

void AVLTree::insertAtRank(int r, int e) {
  if (e <= 0) {                                                 // Inserting element out of bounds
    cout << "ERROR: All elements must be positive integers (1, 2, 3, ...)" << endl << endl;
    return;
  }
  if ((r <= 0) || (r > totalAVLNodes + 1)) {                    // Inserting node with rank out of bounds
    cout << "ERROR: Out of bounds. Insertion rank must be greater" << endl
         << "than or equal to 1 or less than or equal to total number" << endl
         << "of elements plus 1. Current number of elements in vector" << endl
         << "is " << totalAVLNodes << ". Valid insertion rank";
    if (totalAVLNodes == 0)
      cout << " is 1." << endl << endl;
    else
      cout << "s are from 1 - " << totalAVLNodes + 1 << "." << endl << endl;
    return;
  }
  if (root == NULL) {                                           // Inserting first node into empty tree
    if (r == 1) {                                               // Check to see if inserting at rank 1
      root = new AVLNode(e);
      totalAVLNodes++;
      cout << endl << "Inserted " << e << " at rank 1." << endl;
      cout << "Total number of nodes in tree is " << totalAVLNodes << "." << endl << endl;
    }
    else                                                        // Inserting r != 1 into empty tree
      cout << "ERROR: Out of bounds. First insertion must occur at" << endl << "rank 1." << endl << endl;
    return;
  }
  else {
    insert(root, r, e);
    cout << "Inserted element " << e << " at rank " << r << "." << endl;
    cout << "Total number of nodes in tree is " << totalAVLNodes << "." << endl << endl;
  }
}

int AVLTree::remove(AVLptr current, int r) {
  int nodeCount = 0;                                            // Current index of pointer
  int ptrCount = 0;                                             // Current count of nodes in left subtree
  stack<AVLptr> pointers;
  stack<char> direction;
  AVLptr temp;
  int elementToReturn = 0;
  
  if (r == 1) {                                                 // Deleting from rank 1
    while (current->lChild != NULL) {
      current->nodesInLeftSubtree--;
      pointers.push(current);
      direction.push('L');
      current = current->lChild;
    }
    pointers.top()->lChild = current->rChild;
    delete current;
    totalAVLNodes--;
    while (!pointers.empty()) {                                 // Update heights & balance
      pointers.top()->lHeight = getHeight(pointers.top()->lChild);
      AVLptr violation = pointers.top();
      pointers.pop();
      direction.pop();
      AVLptr parent;
      if (pointers.empty())
        parent = root;
      else
        parent = pointers.top();
      balance(parent, violation);
    }
  } // END main if
  else if (r == totalAVLNodes) {                                // Deleting from last rank
    while (current->rChild != NULL) {
      pointers.push(current);
      direction.push('R');
      current = current->rChild;
    }
    pointers.top()->rChild = current->lChild;
    delete current;
    totalAVLNodes--;
    while (!pointers.empty()) {                                 // Update heights & balance
      pointers.top()->rHeight = getHeight(pointers.top()->rChild);
      AVLptr violation = pointers.top();
      pointers.pop();
      direction.pop();
      AVLptr parent;
      if (pointers.empty())
        parent = root;
      else
        parent = pointers.top();
      balance(parent, violation);
    }
  } // END else if
  else {                                                        // All other deletions
    while (nodeCount != r) {
      ptrCount = current->nodesInLeftSubtree + 1;
      if (r == ptrCount + nodeCount) {                          // Found the node of rank r
        nodeCount += ptrCount;
        elementToReturn = current->element;
        if ((current->lChild == NULL) && (current->rChild == NULL)) { // Leaf node deletion
          if (direction.top() == 'L')
            pointers.top()->lChild = NULL;
          else
            pointers.top()->rChild = NULL;
          delete current;
        }
        else if ((current->lChild != NULL) && (current->rChild == NULL)) {  // rChild NULL deletion
          temp = current;
          temp->nodesInLeftSubtree--;
          pointers.push(temp);
          direction.push('L');
          temp = temp->lChild;
          while (temp->rChild != NULL) {
            pointers.push(temp);
            direction.push('R');
            temp = temp->rChild;
          }
          if (temp->lChild != NULL)
            pointers.top()->rChild = temp->lChild;
          current->element = temp->element;
          pointers.top()->lChild = NULL;
          delete temp;
        }
        else if ((current->lChild == NULL) && (current->rChild != NULL)) {  // lChild NULL node deletion
          temp = current;
          pointers.push(temp);
          direction.push('R');
          temp = temp->rChild;
          while (temp->lChild != NULL) {
            pointers.push(temp);
            direction.push('L');
            temp = temp->lChild;
          }
          if (temp->rChild != NULL)
            pointers.top()->lChild = temp->rChild;
          current->element = temp->element;
          pointers.top()->rChild = NULL;
          delete temp;
        }
        else {                                                  // lChild & rChild != NULL deletion
          temp = current;
          temp->nodesInLeftSubtree--;
          pointers.push(temp);
          direction.push('L');
          temp = temp->lChild;
          while (temp->rChild != NULL) {
            pointers.push(temp);
            direction.push('R');
            temp = temp->rChild;
          }
          if (temp->rChild == NULL) {
            pointers.top()->lChild = temp->lChild;
          }
          else if (temp->lChild != NULL && temp->rChild != NULL)
            pointers.top()->rChild = temp->lChild;
          current->element = temp->element;
          delete temp;
        }
        
        temp = NULL;
        current = NULL;
        totalAVLNodes--;
        
        while (!pointers.empty()) {                             // Update heights & balance
          if (direction.top() == 'L')
            pointers.top()->lHeight = getHeight(pointers.top()->lChild);
          else
            pointers.top()->rHeight = getHeight(pointers.top()->rChild);
          AVLptr violation = pointers.top();
          pointers.pop();
          direction.pop();
          AVLptr parent;
          if (pointers.empty())
            parent = root;
          else
            parent = pointers.top();
          balance(parent, violation);
        }
      } // END main if
      else if (r < ptrCount + nodeCount) {                      // Node of rank r is to the left
        pointers.push(current);
        direction.push('L');
        if (current->lChild != NULL) {
          current->nodesInLeftSubtree--;
          current = current->lChild;
        }
      }
      else if (r > ptrCount + nodeCount) {                      // Node of rank r is to the right
        pointers.push(current);
        direction.push('R');
        nodeCount += ptrCount;
        if (current->rChild != NULL)
          current = current->rChild;
      }
    } // END while
  }
  return elementToReturn;
}

void AVLTree::removeAtRank(int r) {
  int e = 0;
  
  if (root == NULL) {                                           // Deletion attempt on empty vector
    cout << "ERROR: Out of bounds. Deletion attempt from empty vector." << endl << endl;
  }
  else if ((r <= 0) || (r > totalAVLNodes)) {                   // Deletion attempt out of bounds
    cout << "ERROR: Out of bounds. Deletion rank must be greater" << endl
         << "than or equal to 1 or less than or equal to total number" << endl
         << "of elements. Current number of elements in vector is " << totalAVLNodes
         << ". Valid deletion rank";
    if (totalAVLNodes == 1)
      cout << " is 1." << endl << endl;
    else
      cout << "s are from 1 - " << totalAVLNodes << "." << endl << endl;
  }
  else {
    if ((totalAVLNodes == 1) && (r == 1)) {                     // Deleting only element
      e = root->element;
      delete root;
      root = NULL;
      totalAVLNodes = 0;
    }
    else                                                        // All other deletion attempts
      e = remove(root, r);
    cout << "Deleted element " << e << " at rank " << r << "." << endl
         << "Total number of nodes in tree is " << totalAVLNodes << "." << endl << endl;
  }
}

void AVLTree::print(AVLptr current) {
  if (current != NULL) {                                        // In order traversal
    print(current->lChild);
    cout << current->element << " ";
//    if (current == root)
//      cout << "ROOT" << endl;
//    cout << current->element << endl << "lHeight: " << current->lHeight << endl << "rHeight: "
//         << current->rHeight << endl << "nodesInLeftSubtree: " << current->nodesInLeftSubtree << endl;
    print(current->rChild);
  }
}

void AVLTree::printAll() {
  AVLptr current = root;
  if (current == NULL) {                                        // Empty AVL tree
    cout << "ERROR: There are no elements in the vector." << endl << endl;
    return;
  }
  cout << "Current vector contains the following elements:" << endl;
  print(current);                                               // Print non-empty AVL tree
}

void AVLTree::clearTree() {
  makeEmpty(root);
}