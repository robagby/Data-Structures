#include <functional>
#include <iostream>

#ifndef SPLAY_TREE
#define SPLAY_TREE

/*   
TIME COMPLEXITY

            Average         Worst case
Space       O(n)            O(n)        
Search      O(log n)*       O(log n)*    
Insert      O(log n)*       O(log n)*    
Delete      O(log n)*       O(log n)* 

(*) Amortized


For performing a sequence S of m accesses in a splay tree containing n elements:
            O(m log n + n log n)      


OPERATIONS

Splay
    When a node x is accessed, a splay operation is performed on x to move it to the root. To perform a 
    splay operation we carry out a sequence of splay steps, each of which moves x closer to the root. By 
    performing a splay operation on the node of interest after every access, the recently accessed nodes 
    are kept near the root and the tree remains roughly balanced, so that we achieve the desired amortized 
    time bounds. 

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
class splay {
private:
    Comp comp;
    unsigned long p_size;

    struct node {
        T key;
        node *left;
        node *right;
        node *parent;
        node(const T& init = T()) : key(init), left(nullptr), right(nullptr), parent(nullptr) { }
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

    void rotate_left_left(node *x) {
        node *y = x->right;
        node *z = y->right;

        if (!x->parent) {
            root = z;
        }
        // x is left child.
        else if (x == x->parent->left) {
            x->parent->left  = z;
        }
        // x is right child.
        else {
            x->parent->right = z;
        }

        z->parent = x->parent;
        x->parent = y;
        y->parent = z;

        x->right  = y->left;
        if (y->left) {
            y->left->parent = x;
        }
        y->left   = x;

        y->right  = z->left;
        if (z->left) {
            z->left->parent = y;
        }
        z->left   = y;
    }

    void rotate_right_right(node *x) {
        node *y = x->left;
        node *z = y->left;

        if (!x->parent) {
            root = z;
        }
        // x is left child.
        else if (x == x->parent->left) {
            x->parent->left  = z;
        }
        // x is right child.
        else {
            x->parent->right = z;
        }

        z->parent = x->parent;
        x->parent = y;
        y->parent = z;

        x->left   = y->right;
        if (y->right) {
            y->right->parent = x;
        }
        y->right  = x;

        y->left   = z->right;
        if (z->right) {
            z->right->parent = y;
        }
        z->right  = y;
    }
  
    void splay_node(node *x) {
        while (x->parent) {
            if (!x->parent->parent) {
                // x is left child.
                if (x->parent->left == x) {
                    rotate_right(x->parent);
                }
                // x is right child.
                else {
                    rotate_left(x->parent);
                }
            } 
            // Both x and parent are left children.
            else if (x->parent->left  == x && x->parent->parent->left == x->parent) {
                rotate_right_right(x->parent->parent);

            } 
            // Both x and parent are right children.
            else if (x->parent->right == x && x->parent->parent->right == x->parent) {
                rotate_left_left(x->parent->parent);

            } 
            // x is left child and parent is right child.
            else if (x->parent->left  == x && x->parent->parent->right == x->parent) {
                rotate_right_left(x->parent->parent);
            } 
            // x is right child and parent is left child.
            else {
                rotate_left_right(x->parent->parent);
            }
        }
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

    void traverse(node *u, int i) {
        if (u->left) {
            traverse(u->left, i+1);
        }
        std::cout << u->key << " level " << i << std::endl;
        if (u->right) {
            traverse(u->right, i+1);
        }
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
    splay() : p_size(0), root(nullptr) { }
  
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
        
        if (!p) {
            root  = z;
        }
        else if (comp(p->key, z->key)) {
            p->right = z;
        }
        else {
            p->left  = z;
        }
        
        splay_node(z);
        p_size++;
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
                splay_node(z);
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
                y->right         = z->right;
                y->right->parent = y;
            }
            replace(z, y);
            y->left         = z->left;
            y->left->parent = y;
        }
        delete z;
        p_size--;

        if (p) {
            splay_node(p);
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

    bool empty(void) const {
        return root == nullptr;
    }

    unsigned long size(void) const {
        return p_size;
    }
};

#endif // SPLAY_TREE
