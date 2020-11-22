#include <functional>
#include <iostream>

#ifndef AVL_TREE_H
#define AVL_TREE_H

/*   

AVL Tree


TIME COMPLEXITY

            Average         Worst case
Space       O(n)            O(n)        
Search      O(log n)        O(log n)    
Insert      O(log n)        O(log n)    
Delete      O(log n)        O(log n)    



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
class avl {
private:

    Comp comp;
    unsigned long p_size;

    struct node {
        node *left, *right, *parent;
        T key;
        int balance;
        node(const T& init = T()) : left(nullptr), right(nullptr), parent(nullptr), key(init), balance(0) { }
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
        x->parent  = y;

        x->balance = 0;
        y->balance = 0;
    }

    void rotate_right(node *x) {
        // Assumes x & x->left are both left heavy.
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
        x->parent  = y;

        x->balance = 0;
        y->balance = 0;
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
        y->left    = z;

        y->parent  = x->parent;
        x->parent  = y;
        z->parent  = y;

        z->balance = z->balance - 1 - std::max(y->balance, 0);
        y->balance = y->balance - 1 + std::min(z->balance, 0);

        x->balance = x->balance + 1 - std::min(z->balance, 0);
        z->balance = z->balance + 1 + std::max(x->balance, 0);
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
        y->right   = z;

        y->parent  = x->parent;
        x->parent  = y;
        z->parent  = y;

        z->balance = z->balance + 1 - std::min(y->balance, 0);
        y->balance = y->balance + 1 + std::max(z->balance, 0);

        x->balance = x->balance - 1 - std::max(z->balance, 0);
        z->balance = z->balance - 1 + std::min(x->balance, 0);
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
        std::cout << u->key << " balance " << u->balance <<" level " << i << std::endl;
        if (u->right) {
            traverse(u->right, i+1);
        }
    }

public:

    avl() : p_size(0), root(nullptr) { }

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
        bool max_path;
        if (p) {
            max_path = (p->balance == 0);
        }
        else {
            max_path = false;
        }
        
        for (node *u = z ; u ; u = u->parent) {
    
            if (!u->parent) {
                root = u;
            }

            // Right heavy.
            if (u->balance == 2) {

                // Right subtree is right heavy.
                if (u->right && (u->right->balance == 1)) {
                    rotate_left(u);
                }
                // Right subtree is left heavy.
                else if (u->right && u->right->left && (u->right->balance == -1)) {                    
                    rotate_right_left(u);
                }
            }
            // Left heavy.
            else if (u->balance == -2) {

                // Left subtree is left heavy.
                if (u->left && (u->left->balance == -1)) {                    
                    rotate_right(u);
                }
                // Left subtree is right heavy.
                else if (u->left && u->left->right && (u->left->balance == 1)) {
                    rotate_left_right(u);
                }
            }
            // Update the parent node's balance factor as long as it is part of the max length path.
            if (u->parent && max_path) {

                if (u->parent->left && (u == u->parent->left)) {
                    max_path &= (u->parent->balance <= 0);
                    u->parent->balance--;
                }
                else if (u->parent->right && (u == u->parent->right)) {
                    max_path &= (u->parent->balance >= 0);
                    u->parent->balance++;
                }
                else {
                    max_path = false;
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
        node *z = root;
        node *p = nullptr;

        while (z && z->key != key) {
            p = z;
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

        for (node *u = p ; u ; u = u->parent) {
            if (!u->parent) {
                root = u;
            }

            // Right heavy.
            if (u->balance == 2) {

                // Right subtree is right heavy.
                if (u->right && (u->right->balance == 1)) {
                    rotate_left(u);
                }
                // Right subtree is left heavy.
                else if (u->right && u->right->left && (u->right->balance == -1)) {                    
                    rotate_right_left(u);
                }
            }
            // Left heavy.
            else if (u->balance == -2) {

                // Left subtree is left heavy.
                if (u->left && (u->left->balance == -1)) {                    
                    rotate_right(u);
                }
                // Left subtree is right heavy.
                else if (u->left && u->left->right && (u->left->balance == 1)) {
                    rotate_left_right(u);
                }
            }
        }
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

    int height(void) {
        return root->height;
    }

    bool empty(void) const {
        return root == nullptr;
    }

    unsigned long size(void) const {
        return p_size;
    }
};

#endif
