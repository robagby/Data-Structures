#include <functional>
#include <iostream>

#ifndef RAVL_TREE_H
#define RAVL_TREE_H

/*   

Relaxed AVL Tree


Implementation from: "Deletion Without Rebalancing in Binary Search Trees" 
    by Siddartha Sen, Robert E. Tarjan, David Hong Kyun Kim




Properties:

1. Each node has height no greater than its rank.

For an i,j node u:
    i = rank(u) - rank(leftchild(u))
    j = rank(u) - rank(rightchild(u))

If a child is nullptr its rank is considered -1. 
This makes every leaf node 1,1 (and a 0-child by property 1).


TIME COMPLEXITY

            Average         Worst case
Space       O(n)            O(n)        
Search      O(log n)        O(h + 1)    
Insert      O(log n)        O(log n)    
Delete      O(log n)        O(h + 1)    

Rebalance   O(1)*

(*) Amortized.



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
    Delete a node in the tree.
*/

template<typename T, typename Comp = std::less<T>>
class ravl {
private:
    Comp comp;
    int p_size;

    struct node {
        node *left, *right, *parent;
        T key;
        std::uint8_t rank;
        node(const T& init = T()) : left(nullptr), right(nullptr), parent(nullptr), key(init), rank(0) { }
        ~node() { }
    } *root;

    void rotate_left(node *x) {
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
    }

    void rotate_right(node *x) {
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
    }

    void rotate_left_right(node *x) {
        node *z = x->left;
        node *y = z->right;

        if (!x->parent) {
            root = y;
        }
        // x is left child.
        else if (x == x->parent->left) {
            x->parent->left  = y;
        }
        // x is right child.
        else {
            x->parent->right = y;
        }

        x->left = y->right;
        if (y->right) {
            y->right->parent = x;
        }
        y->right = x;

        z->right = y->left;
        if (y->left) {
            y->left->parent = z;
        }
        y->left   = z;

        y->parent = x->parent;
        x->parent = y;
        z->parent = y;
    }

    void rotate_right_left(node *x) {
        node *z = x->right;
        node *y = z->left;

        if (!x->parent) {
            root = y;
        }
        // x is left child.
        else if (x == x->parent->left) {
            x->parent->left  = y;
        }
        // x is right child.
        else {
            x->parent->right = y;
        }

        x->right = y->left;
        if (y->left) {
            y->left->parent = x;
        }
        y->left = x;

        z->left = y->right;
        if (y->right) {
            y->right->parent = z;
        }
        y->right  = z;

        y->parent = x->parent;
        x->parent = y;
        z->parent = y;
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

    void traverse(node *u, int i) {
        if (u->left) {
            traverse(u->left, i+1);
        }
        std::cout << u->key << " rank " << unsigned(u->rank) << " level " << i << std::endl;
        if (u->right) {
            traverse(u->right, i+1);
        }
    }

    void rebalance_insert(node *u) {
        // Rebalancing with tree rotations.
        if (!u->parent) {
            root = u;
        }
        else {
            // Everything after the above case must have a parent.
            int pldiff;
            int prdiff;

            node* p = u->parent;
            if (p->left && (u == p->left)) {
                pldiff = p->rank - u->rank;
                if (p->right) {
                    prdiff = p->rank - p->right->rank;
                }
                else {
                    prdiff = p->rank + 1;
                }
            }
            else {
                prdiff = p->rank - u->rank;
                if (p->left) {
                    pldiff = p->rank - p->left->rank;
                }
                else {
                    pldiff = p->rank + 1;
                }
            }

            // Parent is 1,0 or 0,1.
            if ((pldiff == 1 && prdiff == 0) || (pldiff == 0 && prdiff == 1)) {
                p->rank++;
            }
            // Parent is 2,0 or 0,2.
            else if ((pldiff == 2 && prdiff == 0) || (pldiff == 0 && prdiff == 2)) {
                // Node u is right child.
                if (p->right && (u == p->right)) {
                    if ((u->left && (u->rank - u->left->rank == 2)) || (!u->left && u->rank == 1)) {
                        p->rank--;
                        rotate_left(p);
                    }
                    else if (u->left && u->rank - u->left->rank == 1) {
                        u->left->rank++;
                        u->rank--;
                        p->rank--;
                        rotate_right_left(p);
                    }
                }
                // Node u is left child.
                else if (p->left && (u == p->left)) {
                    if ((u->right && (u->rank - u->right->rank == 2)) || (!u->right && (u->rank == 1))) {
                        p->rank--;
                        rotate_right(p);
                    }
                    else if (u->right && u->rank - u->right->rank == 1) {
                        u->right->rank++;
                        u->rank--;
                        p->rank--;
                        rotate_left_right(p);
                    }
                }
            }
        }
    }

public:
    ravl() : p_size(0), root(nullptr) { }

    void insert(const T &key) {
        node *z = root;
        node *p = nullptr;

        while (z) {
            p = z;
            // New node greater than z.
            if (comp(z->key, key)) {
                z = z->right;
            }
            // New node less than or equal to z.
            else {
                z = z->left;
            }
        }
        z = new node(key);
        z->parent = p;

        if (!p) {
            root = z;
        }
        else if (comp(p->key, z->key)) {
            p->right = z;
        }
        else {
            p->left = z;
        }
        
        p_size++;
        for (node *a = z ; a ; a = a->parent) {
            rebalance_insert(a);
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
        node *z = root;
        while (z && z->key != key) {
            if (comp(z->key, key)) {
                z = z->right;
            }
            else {
                z = z->left;
            }
        }
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
                y->right = z->right;
                y->right->parent = y;
            }
            replace(z, y);
            y->left = z->left;
            y->left->parent = y;
        }
        delete z;
        p_size--;
    }

    void traverse(void) {
        traverse(root, 0); 
        traverse(root);
        std::cout << std::endl;
    }

    const T& maximum(void) {
        return subtree_maximum(root)->key;
    }

    const T& minimum(void) {
        return subtree_minimum(root)->key;
    }

    int rank(void) {
        return root->rank;
    }

    bool empty(void) const {
        return root == nullptr;
    }

    unsigned long size(void) const {
        return p_size;
    }
};

#endif