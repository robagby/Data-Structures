#include <functional>
#include <iostream>

#ifndef AVL_TREE
#define AVL_TREE

/*   
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
        int height;
        node(const T& init = T(), int h = 0) : left(nullptr), right(nullptr), parent(nullptr), key(init), height(h) {}
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

        // Make sure to update height of x (child of y) 1st (because height is a function of child height).
        x->height = get_height(x);
        y->height = get_height(y);

        return x;
    }

    node* rotate_right(node *x) {
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

        x->parent = y;

        // Make sure to update height of x (child of y) 1st (because height is a function of child height).
        x->height = get_height(x);
        y->height = get_height(y);

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

    int get_height(node *u) {
        // Height is max length from leaf node to node u.
        if (u->left && u->right) {
            return 1 + std::max(u->left->height, u->right->height);
        }
        else if (!u->right && u->left) {
            return 1 + u->left->height;
        }
        else if (!u->left && u->right) {
            return 1 + u->right->height;
        }
        else {
            return 0;
        }
    }

    int get_balance(node *u) {
        int l = 0;
        int r = 0;

        if (u->left) {
            l = u->left->height;
        }
        if (u->right) {
            r = u->right->height;
        }
        return r-l;
    }

    node* rebalance(node *a) {
        // Rebalancing with tree rotations.
        int balance;
        
        a->height = get_height(a);
        balance   = get_balance(a);

        // Right heavy.
        if (balance == 2) {
            // Right subtree is right heavy.
            if (a->right && get_balance(a->right) == 1) {
                a = rotate_left(a);
            }
            // Right subtree is left heavy.
            else if (a->right && a->right->left && get_balance(a->right) == -1) {
                //rotate_right_left(a);
                a->right = rotate_right(a->right);
                a = rotate_left(a);
            }
        }
        // Left heavy.
        else if (balance == -2) {
            // Left subtree is left heavy.
            if (a->left && get_balance(a->left) == -1) {
                a = rotate_right(a);
            }
            // Left subtree is right heavy.
            else if (a->left && a->left->right && get_balance(a->left) == 1) {
                //rotate_left_right(a);
                a->left = rotate_left(a->left);
                a = rotate_right(a);
            }
        }
        return a;
    }

public:

    avl() : root(nullptr), p_size(0) {}

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

        node *a = z;
        while (a->parent) {
            a = a->parent;
            a = rebalance(a);
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

        while (p) {
            p = rebalance(p);
            p = p->parent;
        }
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