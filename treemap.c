#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap *auxArbol = (TreeMap *)malloc(sizeof(TreeMap));
  if(auxArbol == NULL) return NULL;
  auxArbol->root = NULL;
  auxArbol->current = NULL;
  auxArbol->lower_than = lower_than;
  //new->lower_than = lower_than;
  return auxArbol;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  if(searchTreeMap(tree,key) != NULL)
  {
    return;
  }
  TreeNode *auxTree = createTreeNode(key,value);
  
  if(tree->root == NULL)
  {
    tree->root = auxTree;
    tree->current = auxTree;
    return;
  }

  TreeNode *current = tree->root;
  TreeNode *parentNode = NULL;
  
  while(current != NULL)
    {
      if(tree->lower_than(key,current->pair->key))
      {
        parentNode = current;
        current = current->left;
      }
      else{
        parentNode = current;
        current = current->right;
      }
    }
  auxTree->parent = parentNode;

  if(tree->lower_than(key,parentNode->pair->key))
  {
    parentNode->left = auxTree;
  }
  else
  {
    parentNode->right = auxTree;
  }

  tree->current = auxTree;
}

TreeNode * minimum(TreeNode * x){
  while(x->left != NULL) x = x->left;
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
  /*
    if (node->left == NULL && node->right == NULL) // Nodo sin hijos
    { 
      node->parent->left = NULL;
      node->parent->right = NULL;
      free(node);
    }   
      // Nodo con un hijo
    if(node == node->parent->left)
    {
      node->parent->left = node->right;
      node->parent->right = NULL;
      free(node);
    }
    if (node == node->parent->right)
    {
      node->parent->right = node->left;
      node->parent->left = NULL;
      free(node);
    }
    else  // Nodo con dos hijos
    {
      
    }
  */
      // Caso 1: Nodo sin hijos
    if (node->left == NULL && node->right == NULL) {
        if (node == tree->root) { // Si es la raíz
            tree->root = NULL;
        } else if (node == node->parent->left) { // Si es un hijo izquierdo
            node->parent->left = NULL;
        } else { // Si es un hijo derecho
            node->parent->right = NULL;
        }
        free(node); // Liberar la memoria del nodo a eliminar
    }
    // Caso 2: Nodo con un hijo
    else if (node->left == NULL || node->right == NULL) {
        TreeNode *child = node->left ? node->left : node->right; // Obtener el hijo no nulo
        child->parent = node->parent; // Establecer el padre del hijo
        if (node == tree->root) { // Si es la raíz
            tree->root = child;
        } else if (node == node->parent->left) { // Si es un hijo izquierdo
            node->parent->left = child;
        } else { // Si es un hijo derecho
            node->parent->right = child;
        }
        free(node); // Liberar la memoria del nodo a eliminar
    }
    // Caso 3: Nodo con dos hijos
    else {
        TreeNode *minNode = minimum(node->right); // Encontrar el nodo con valor mínimo en el subárbol derecho
        node->pair->key = minNode->pair->key; // Copiar la llave del nodo con valor mínimo al nodo a eliminar
        node->pair->value = minNode->pair->value; // Copiar el valor del nodo con valor mínimo al nodo a eliminar
        removeNode(tree, minNode); // Eliminar el nodo con valor mínimo
    }
}
  
}
void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) 
{
  TreeNode * auxTree = tree->root;

  while(auxTree != NULL)
    {
      if(is_equal(tree,key,auxTree->pair->key) == 1)
      {
        tree->current = auxTree;
        return auxTree->pair;
      }
      
      if(tree -> lower_than(key,auxTree->pair->key) == 0)
      {
        auxTree = auxTree->right;
      }  
      else{
        if(tree -> lower_than(key,auxTree->pair->key) == 1)
        {
          auxTree = auxTree->left;
        }
      }
    }
  return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) 
{
  TreeNode *auxTree = tree->root;
  TreeNode *result = NULL;
  
  while(auxTree != NULL)
    {
      if(is_equal(tree,key,auxTree->pair->key) == 1)
      {
        return auxTree->pair;
      }
      if(tree -> lower_than(key,auxTree->pair->key) == 0)
      {
        auxTree = auxTree->right;
      }
      else
      {
        result = auxTree;
        auxTree = auxTree->left;
      }
    }

  if(result == NULL)
  {
    return NULL;
  }
  
  return result->pair;
}

Pair * firstTreeMap(TreeMap * tree) 
{
  TreeNode * auxTree = tree->root;
  auxTree = minimum(auxTree);
  tree->current = auxTree;
  return auxTree->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
  if(tree->current == NULL)
  {
    return NULL;
  }
  
  if(tree->current->right != NULL)
  {
    tree->current = tree->current->right;
    tree->current = minimum(tree->current);
    return tree->current->pair;
  }
  else
  {
    TreeNode *parent = tree->current->parent;
    
    while(parent != NULL && tree->current == parent->right)
    {
      tree->current = parent;
      parent = parent->parent;
    }
    if(parent != NULL)
    {
      tree->current = parent;
      return tree->current->pair;
    }
  }
  return NULL;
}
