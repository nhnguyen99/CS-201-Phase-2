/*Nguyen Hung Nguyen CS 201 Phase 2*/

#include <iostream>
using namespace std;

#ifndef _RBTree_cpp
#define _RBTree_cpp

enum Color
{
        RED,
        BLACK
};

template <class keytype, class valuetype>
class RBTree
{

private:
        struct Node
        {
                keytype key;
                valuetype value;
                Node *left;
                Node *right;
                Node *parent;
                Color color;
                int n = 1;

                Node()
                {
                        this->color = BLACK;
                        parent = left = right = nullptr;
                }

                Node(keytype key, valuetype value)
                {
                        this->key = key;
                        this->value = value;
                        this->color = RED;
                        parent = left = right = nullptr;
                }

                Node(keytype key, valuetype value, Color color, int n)
                {
                        this->key = key;
                        this->value = value;
                        this->n = n;
                        this->color = color;
                        parent = left = right = nullptr;
                }
        };

        Node *root;
        int s;

public:
        RBTree()
        {
                s = 0;
                root = nullptr;
        }

        RBTree(keytype k[], valuetype v[], int s)
        {
                this->s = 0;
                root = nullptr;
                for (int i = 0; i < s; i++)
                {
                        insert(k[i], v[i]);
                }
        }

        ~RBTree()
        {
                treeDelete(root);
        }

        void treeDelete(Node *t)
        {
                if (t == nullptr)
                {
                        return;
                }
                if (t->left != nullptr)
                        treeDelete(t->left);
                if (t->right != nullptr)
                        treeDelete(t->right);
                delete t;
        }

        RBTree(const RBTree &t)
        {
                root = parentCopy(t.root);
                childCopy(root, t.root);
        }

        RBTree &operator=(const RBTree &obj)
        {
                if (this != &obj)
                {
                        treeDelete(root);
                        root = parentCopy(obj.root);
                        childCopy(root, obj.root);
                        s = obj.s;
                }
                return *this;
        }

        Node *parentCopy(Node *temp)
        {
                Node *t = new Node(temp->key, temp->value, temp->color, temp->n);
                return t;
        }

        void childCopy(Node *t, Node *temp)
        {
                if (temp->left != nullptr)
                {
                        t->left = parentCopy(temp->left);
                        childCopy(t->left, temp->left);
                }
                if (temp->right != nullptr)
                {
                        t->right = parentCopy(temp->right);
                        childCopy(t->right, temp->right);
                }
        }

        valuetype *search(keytype k)
        {
                return treeSearch(root, k);
        }

        valuetype *treeSearch(Node *node, keytype k)
        {
                if (node == nullptr)
                        return (valuetype *)(nullptr);
                if (k == node->key)
                        return &(node->value);
                if (k > node->key)
                        return treeSearch(node->right, k);
                else
                        return treeSearch(node->left, k);
        }

        void insert(keytype k, valuetype v)
        {
                if (traverse(root, k) == nullptr)
                {
                        s++;
                }
                Node *newNode = new Node(k, v);
                treeInsert(root, newNode);
        }

        void treeInsert(Node *&root, Node *node)
        {
                Node *x = root;
                Node *y = nullptr;
                while (x != nullptr)
                {
                        y = x;
                        if (node->key > x->key)
                                x = x->right;
                        else
                                x = x->left;
                }
                node->parent = y;
                if (y != nullptr)
                {
                        if (node->key > y->key)
                                y->right = node;
                        else
                                y->left = node;
                }
                else
                        root = node;
                node->color = RED;
                insertFixUp(root, node);
        }

        void insertFixUp(Node *&root, Node *node)
        {
                Node *parent;
                parent = node->parent;
                while (node != RBTree::root && parent->color == RED)
                {
                        Node *gparent = parent->parent;
                        if (gparent->left == parent)
                        {
                                Node *uncle = gparent->right;
                                if (uncle != nullptr && uncle->color == RED)
                                {
                                        parent->color = BLACK;
                                        uncle->color = BLACK;
                                        gparent->color = RED;
                                        node = gparent;
                                        parent = node->parent;
                                }
                                else
                                {
                                        if (parent->right == node)
                                        {
                                                leftRotate(parent);
                                                swap(node, parent);
                                        }
                                        rightRotate(gparent);
                                        gparent->color = RED;
                                        parent->color = BLACK;
                                        break;
                                }
                        }
                        else
                        {
                                Node *uncle = gparent->left;
                                if (uncle != NULL && uncle->color == RED)
                                {
                                        gparent->color = RED;
                                        parent->color = BLACK;
                                        uncle->color = BLACK;

                                        node = gparent;
                                        parent = node->parent;
                                }
                                else
                                {
                                        if (parent->left == node)
                                        {
                                                rightRotate(parent);
                                                swap(parent, node);
                                        }
                                        leftRotate(gparent);
                                        parent->color = BLACK;
                                        gparent->color = RED;
                                        break;
                                }
                        }
                }
                root->color = BLACK;
        }

        void transplant(Node *u, Node *v)
        {
                if (u->parent == nullptr)
                {
                        root = v;
                }
                else if (u == u->parent->left)
                {
                        u->parent->left = v;
                }
                else
                {
                        u->parent->right = v;
                }
                v->parent = u->parent;
        }

        int remove(keytype k)
        {
                Node *node = traverse(root, k);
                if (node != nullptr)
                {
                        //treeRemove(node);
                        s--;
                        return 1;
                }
                else
                        return 0;
        }

        void treeRemove(Node *node)
        {
                Node *y = node;
                Node *x = nullptr;
                Color org = y->color;
                if (node->left == nullptr)
                {
                        x = node->right;
                        transplant(node, node->right);
                }

                else if (node->right == nullptr)
                {
                        x = node->left;
                        transplant(node, node->left);
                }
                else
                {
                        y = treeMin(node->right);
                        org = y->color;
                        x = y->right;
                        if (y->parent == node)
                        {
                                x->parent = y;
                        }

                        else
                        {
                                transplant(y, y->right);
                                y->right = node->right;
                                y->right->parent = y;
                        }
                        transplant(node, y);
                        y->left = node->left;
                        y->left->parent = y;
                        y->color = node->color;
                }
                if (org == BLACK)
                {
                        removeFixUp(x);
                }
        }

        Node *removeFixUp(Node *x)
        {
                Node *w = nullptr;
                while (x != root && x->color == BLACK)
                {
                        if (x == x->parent->left)
                        {
                                w = x->parent->right;
                                if (w->color == RED)
                                {
                                        w->color = BLACK;
                                        x->parent->color = RED;
                                        leftRotate(x->parent);
                                        w = x->parent->right;
                                }

                                if ((w->left->color) == BLACK && (w->right->color = BLACK))
                                {
                                        w->color = RED;
                                        x = x->parent;
                                }

                                else
                                {
                                        if (w->right->color == BLACK)
                                        {
                                                w->left->color = BLACK;
                                                w->color = RED;
                                                rightRotate(w);
                                                w = x->parent->right;
                                        }

                                        w->color = x->parent->color;
                                        x->parent->color = BLACK;
                                        w->right->color = BLACK;
                                        leftRotate(x->parent);
                                        x = root;
                                }
                        }

                        else
                        {
                                w = x->parent->left;
                                if (w->color == RED)
                                {
                                        w->color = BLACK;
                                        x->parent->color = RED;
                                        rightRotate(x->parent);
                                        w = x->parent->left;
                                }

                                if ((w->right->color == BLACK) && (w->left->color = BLACK))
                                {
                                        w->color = RED;
                                        x = x->parent;
                                }

                                else
                                {
                                        if (w->left->color == BLACK)
                                        {
                                                w->right->color = BLACK;
                                                w->color = RED;
                                                leftRotate(w);
                                                w = x->parent->left;
                                        }

                                        w->color = x->parent->color;
                                        x->parent->color = BLACK;
                                        w->left->color = BLACK;
                                        rightRotate(x->parent);
                                        x = root;
                                }
                        }
                }
                x->color = BLACK;
                return setNumNodes(x);
        }

        int rank(keytype k)
        {
                if (traverse(root, k) == nullptr)
                        return 0;
                //return 1;
                return treeRank(root, k);
        }

        int treeRank(Node *h, keytype k)
        {
                if (h->key == k)
                {
                        if (h->left != nullptr)
                                return numNodes(h->left) + 1;
                }

                else if (k < h->key)
                {
                        return treeRank(h->left, k);
                }

                else
                {
                        return treeRank(h->right, k) + numNodes(h->left) + 1;
                }
        }

        keytype select(int pos)
        {
                if (root != nullptr)
                        return root->key;
        }

        keytype treeSelect(Node *h, int pos)
        {
                if (rank(h->key) == pos)
                {
                        return h->key;
                }

                else if (rank(h->key) < pos)
                {
                        return treeSelect(h->right, pos);
                }

                else
                {
                        return treeSelect(h->left, pos);
                }
        }

        Node *setNumNodes(Node *t)
        {
                t->n = numNodes(t->left) + numNodes(t->right) + 1;
                return t;
        }

        int numNodes(Node *t)
        {
                if (t == nullptr)
                        return 0;
                else
                        return t->n;
        }

        Node *treeMin(Node *temp)
        {
                if (temp != nullptr)
                {
                        while (temp->left != nullptr)
                                temp = temp->left;
                        return temp;
                }
        }

        keytype *findSuccessor(Node *node, Node *&suc, keytype key)
        {
                if (node == nullptr)
                {
                        return (keytype *)nullptr;
                }
                if (node->key != key && node->left == nullptr && node->right == nullptr)
                        return (keytype *)nullptr;
                if (node->key == key)
                {
                        if (node->right != nullptr)
                        {
                                Node *temp = node->right;
                                while (temp->left != nullptr)
                                        temp = temp->left;
                                suc = temp;
                                return &suc->key;
                        }
                        else if (suc == nullptr)
                        {
                                return (keytype *)nullptr;
                        }
                        else
                        {
                                return &suc->key;
                        }
                }
                else if (node->key > key)
                {
                        suc = node;
                        return findSuccessor(node->left, suc, key);
                }
                else
                        return findSuccessor(node->right, suc, key);
        }

        keytype *findPredecessor(Node *node, Node *&pre, keytype key)
        {
                if (node == nullptr)
                {
                        return (keytype *)nullptr;
                }
                if (node->key != key && node->left == nullptr && node->right == nullptr)
                {
                        return (keytype *)nullptr;
                }
                if (node->key == key)
                {
                        if (node->left != nullptr)
                        {
                                Node *temp = node->left;
                                while (temp->right != nullptr)
                                        temp = temp->right;
                                pre = temp;
                                return &pre->key;
                        }
                        else if (pre == nullptr)
                        {
                                return (keytype *)nullptr;
                        }
                        else
                        {
                                return &pre->key;
                        }
                }
                else if (node->key < key)
                {
                        pre = node;
                        return findPredecessor(node->right, pre, key);
                }
                else
                        return findPredecessor(node->left, pre, key);
        }

        keytype *successor(keytype k)
        {
                Node *suc = nullptr;
                return findSuccessor(root, suc, k);
        }

        keytype *predecessor(keytype k)
        {
                Node *pre = nullptr;
                return findPredecessor(root, pre, k);
        }

        int size()
        {
                return s;
        }

        void printPreorder(Node *node)
        {
                if (node == nullptr)
                        return;
                cout << node->key << " ";
                printPreorder(node->left);
                printPreorder(node->right);
        }

        void printInorder(Node *node)
        {
                if (node == nullptr)
                        return;
                printInorder(node->left);
                cout << node->key << " ";
                printInorder(node->right);
        }

        void printPostorder(Node *node)
        {
                if (node == nullptr)
                        return;
                printPostorder(node->left);
                printPostorder(node->right);
                cout << node->key << " ";
        }

        void preorder()
        {
                printPreorder(root);
                cout << endl;
        }

        void inorder()
        {
                printInorder(root);
                cout << endl;
        }

        void postorder()
        {
                printPostorder(root);
                cout << endl;
        }

        Node *traverse(Node *t, keytype k)
        {
                if (t == nullptr || t->key == k)
                        return t;
                if (t->key > k)
                        return traverse(t->left, k);
                else
                        return traverse(t->right, k);
        }

        Node *leftRotate(Node *x)
        {
                Node *y = x->right;
                x->right = y->left;
                if (y->left != nullptr)
                        y->left->parent = x;
                y->parent = x->parent;
                if (x->parent == nullptr)
                        root = y;
                else if (x == x->parent->left)
                        x->parent->left = y;
                else
                        x->parent->right = y;
                y->left = setNumNodes(x);
                x->parent = y;
                //        x->rightNodes = y->leftNodes;
                //        y->leftNodes = x->leftNodes + x->rightNodes + 1;
                return setNumNodes(y);
        }

        Node *rightRotate(Node *x)
        {
                Node *y = x->left;
                x->left = y->right;
                if (y->right != nullptr)
                        y->right->parent = x;
                y->parent = x->parent;
                if (x->parent == nullptr)
                        root = y;
                else if (x == x->parent->right)
                        x->parent->right = y;
                else
                        x->parent->left = y;
                y->right = setNumNodes(x);
                x->parent = y;
                //        y->leftNodes = x->rightNodes;
                //        x->rightNodes = y->leftNodes + y->rightNodes + 1;
                return setNumNodes(y);
        }
};
#endif
