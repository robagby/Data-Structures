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
// Properties:
// 1. Each node is either red or black.
// 2. The root is black.
// 3. All leaves (nullptr) are black.
// 4. If a node is red, then both children are black.
// 5. Every path from a node to a leaf has the same # of black nodes.
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
        //return x;
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
        //return x;
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
        std::cout << u->key << " level " << i << std::endl;
        if (u->right) {
            traverse(u->right, i+1);
        }
    }

    void rebalance_insert(node *u) {
        // Need to check 4 cases to maintain properties.
        if (u->parent) {
            // Case 2: parent node is black.
            if (!u->parent->color) {
                // Tree is valid so do nothing.
            }
            // Parent has to be red after above statement.
            else if (u->parent->parent) {
                // Case 4: parent is a red, right child and uncle is black.
                if (!u->parent->parent->left || !u->parent->parent->left->color) {
                    // Grandparent must be black.
                    u->parent->color         = false;
                    u->parent->parent->color = true;

                    // Check if u is on the 'inside' of the subtree.
                    if (u == u->parent->left) {
                        rotate_right(u->parent);
                        rotate_left(u->parent);
                    }
                    else {
                        rotate_left(u->parent->parent);
                    }
                }
                // Case 4: parent is u red, left child and uncle is black.
                else if (!u->parent->parent->right || !u->parent->parent->right->color) {
                    // Grandparent must be black.
                    u->parent->color         = false;
                    u->parent->parent->color = true;

                    // Check if u is on the 'inside' of the subtree.
                    if (u == u->parent->right) {
                        rotate_left(u);
                        rotate_right(u->parent);
                    }
                    else {
                        rotate_right(u->parent->parent);
                    }
                }
                // Case 3: both parent node and uncle node are red.
                else {
                    // Paint parent node and uncle node black.
                    u->parent->parent->left->color  = false;
                    u->parent->parent->right->color = false;

                    // Paint grandparent node red.
                    u->parent->parent->color = true;
                }
            }
        }
        // Must be root if node does not have parent.
        else {
            // Case 1: node is red and is the root node.
            if (u->color) {
                // Change color to black.
                u->color = false;
            }
            root = u;
        }  
    }

    void rebalance_delete(node *u) {
        if (!u) {
            return;
        }
        // Case 1: u new root.
        if (!u->parent) {
            root     = u;
            u->color = false;
            return;
        }
     
        node *sibling;
        // Case 2: sibling is red.
        if (u == u->parent->left) {

            sibling = u->parent->right;
            // Case 2: sibling is red.
            if (sibling && sibling->color) {
                sibling->color         = false;
                sibling->parent->color = true;
                rotate_left(sibling->parent);
            }
            // Case 5: sibling is black right child, sibling's left child is red and sibling's right child is black.
            else if (!sibling->color && sibling->left && sibling->left->color \
                                                && (!sibling->right || sibling->right->color)) {
                sibling->left->color = false;
                sibling->color       = true;
                rotate_right(sibling);
            }
            // Case 6: sibling is black right child, sibling's right child is red.
            else if (!sibling->color && sibling->right && sibling->right->color) {
                // Parent and sibling swap colors so parent has to become black.
                sibling->right->color  = !sibling->parent->color;
                sibling->parent->color = false;
                rotate_left(sibling->parent);
            }
        }
        // Case 2 cont.
        else if (u == u->parent->right) {

            sibling = u->parent->left;
            // Case 2: sibling is red.
            if (sibling && sibling->color) {
                sibling->color         = false;
                sibling->parent->color = true;
                rotate_right(sibling->parent);
            }
        }
        // Case 3 and 4.
        // Sibling and all sibling's children are black.
        if (sibling && !sibling->color && (!sibling->left || !sibling->left->color) \
                                        && (!sibling->right || !sibling->right->color)) {

            // Case 3: parent is also black.
            if (sibling->parent && sibling->parent->color) {
                // Paint sibling red and rebalance the parent node.
                sibling->color = true;
            }
            // Case 4: parent is red.
            else if (sibling->parent && !sibling->parent->color) {
                // Switch colors for both parent and sibling.
                sibling->color         = true;
                sibling->parent->color = false;
            }
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

        while (z->parent) {
            rebalance_insert(z);
            z = z->parent;
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

        node *y = nullptr;
        if (!z->left) {
            y = z->right;
            replace(z, y);
        }
        else if (!z->right) {
            y = z->left;
            replace(z, y);
        }
        else {
            y = subtree_minimum(z->right);
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

        while (y) {
            rebalance_delete(y);
            y = y->parent;
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
#endif
