#include <functional>
#include <iostream>

#ifndef RED_BLACK_TREE
#define RED_BLACK_TREE

/*   
TIME COMPLEXITY

            Average         Worst case
Space       O(n)            O(n)        
Search      O(log n)       O(log n)    
Insert      O(log n)       O(log n)    
Delete      O(log n)       O(log n) 



OPERATIONS

Join
    Given two trees S and T such that all elements of S are smaller than the elements of T, combine S & T 
    in a way that the resultant tree is balanced.

Split
    Given a tree and an element x, return two new trees: one containing all elements less than or equal 
    to x and the other containing all elements greater than x.

Search
    Find node in tree.

Insert
    Place node into tree.

Remove
    Delete a node in the  tree.
*/

template<typename T, typename Comp = std::less<T>>
class rb {

private:

    Comp comp;
    unsigned long p_size;

    struct node {
        node *left, *right, *parent;
        T key;
        bool color;     // red = true, black = false
        node(const T& init = T()) : left(nullptr), right(nullptr), parent(nullptr), key(init), color(true) {}
        ~node() {}
    } *root;
  
    node* rotate_left(node *x) {
        node *y = x->right;
        if (y) {
            x->right = y->left;

            if (y->left) {
                y->left->parent = x;
            }
            y->parent = x->parent;
        }
        
        if (!x->parent) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left  = y;
        }
        else {
            x->parent->right = y;
        }

        if (y) {
            y->left = x;
        }
        x->parent = y;
        return x;
    }
  
    node* rotate_right(node *x) {
        node *y = x->left;
        if (y) {
            x->left = y->right;
            if (y->right) {
                y->right->parent = x;
            }
            y->parent = x->parent;
        }
        if (!x->parent) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left  = y;
        }
        else {
            x->parent->right = y;
        }
        if (y) {
            y->right = x;
        }
        x->parent = y;
        return x;
    }
  
    void replace(node *u, node *v) {
        if (!u->parent) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left  = v;
        }
        else {
            u->parent->right = v;
        }
        if (v) {
            v->parent = u->parent;
        }
    }

    node* subtree_maximum(node *u) {
        while (u->right) {
            u = u->right;
        }
        return u;
    }
  
    node* subtree_minimum(node *u) {
        while (u->left) {
            u = u->left;
        }
        return u;
    }

    void traverse(node *u) {
        if (u->left) {
            traverse(u->left);
        }
        std::cout << u->key << " ";
        if (u->right) {
            traverse(u->right);
        }
    }

public:

    rb() : root(nullptr), p_size(0) {}
  
    void insert(const T &key) {
        node *z = root;
        node *p = nullptr;
        
        while (z) {
            p = z;
            if (comp(z->key, key)) {
                z = z->right;
            }
            else {
                z = z->left;
            }
        }
        
        z = new node(key);
        z->parent = p;
        // Case 1.
        if (!p) {
            root = z;
            // Paint root black.
            root->color = false;
        }
        else if (comp(p->key, z->key)) {
            p->right = z;
        }
        else {
            p->left  = z;
        }

        p_size++;

        node *a = z;
        while (a->parent) {
            a = a->parent;
            // Need to check 4 cases to maintain properties.
            if (a->parent) {
                // Case 2: parent node is black.
                if (!a->parent->color) {
                    // Tree is valid so do nothing.
                }
                // Parent has to be red after above statement.
                if (a->parent->parent) {
                    // Case 4: parent is a red, right child and uncle is black.
                    if (!a->parent->parent->left || !a->parent->parent->left->color) {
                        // Check if a is on the 'inside' of the subtree.
                        if (a == a->parent->left) {
                            a = rotate_right(a->parent);
                        }
                        a = a->parent;
                        a->color = false;
                        a->parent->color = true;
                        a = rotate_left(a->parent);
                    }
                    // Case 4: parent is a red, left child and uncle is black.
                    else if (!a->parent->parent->right || !a->parent->parent->right->color) {
                        // Check if a is on the 'inside' of the subtree.
                        if (a == a->parent->right) {
                            a = rotate_left(a->parent);
                        }
                        a = a->parent;
                        a->color = false;
                        a->parent->color = true;
                        a = rotate_right(a->parent);
                    }
                    // Case 3: both parent node and uncle node are red.
                    else {
                        // Paint parent node and uncle node black.
                        a->parent->parent->left->color  = false;
                        a->parent->parent->right->color = false;

                        // Paint grandparent node red.
                        a->parent->parent->color = true;
                    }
                }
            }
            // Must be root if node does not have parent.
            else {
                // Case 1: node is red and is the root node.
                if (a->color) {
                    // Change color to black.
                    a->color = false;
                    // Double check that this node will be root.
                    root = a;
                }
            }   
        }
    }
  
    node* search(const T &key) {
        node *z = root;
        while (z) {
            if (comp(z->key, key)) {
                z = z->right;
            }
            else if (comp(key, z->key)) {
                z = z->left;
            }
            else {
                return z;
            }
        }
        return nullptr;
    }
        
    void remove(const T &key) {
        node *z = search(key);
        if (!z) {
            return;
        }
        if (!z->left) {
            replace(z, z->right);
        }
        else if (!z->right) {
            replace(z, z->left);
        }
        else {
            node *y = subtree_minimum(z->right);
            if (y->parent != z) {
                replace(y, y->right);
                y->right         = z->right;
                y->right->parent = y;
            }
            replace(z, y);
            y->left         = z->left;
            y->left->parent = y;
        }
        delete z;
        p_size--;
    }
 
    void traverse(void) {
        traverse(root); 
        std::cout << std::endl;
    }

    const T& maximum(void) {
        return subtree_maximum(root)->key;
    }

    const T& minimum(void) {
        return subtree_minimum(root)->key;
    }

    bool empty(void) const {
        return root == nullptr;
    }

    unsigned long size(void) const {
        return p_size;
    }

};
#endif // SPLAY_TREE