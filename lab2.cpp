#include <iostream>

class Avl_Tree {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;
        Node(int k): key(k), left(nullptr), right(nullptr), height(1) {}
    };
    Node* root;
    size_t tree_size;

    int get_height(Node* node) {
        return node ? node->height : 0;
    }
    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + max(get_height(node->left), get_height(node->right));
        }
    }

    int getBalance(Node* node) {
        return node ? get_height(node->left) - get_height(node->right) : 0;
    }

    Node* rotateR(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateL(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* balance(Node* node) {
        updateHeight(node);
        int bal = getBalance(node);

        if (bal > 1 && getBalance(node->left) >= 0) {
            return rotateR(node);
        }
        if (bal > 1 && getBalance(node->left) < 0) {
            node->left = rotateL(node->left);
            return rotateR(node);
        }
        if (bal < -1 && getBalance(node->right) <= 0) {
            return rotateL(node);
        }
        if (bal < -1 && getBalance(node->right) > 0) {
            node->right = rotateR(node->right);
            return rotateL(node);
        }
        return node;
    }

    Node* insertNode(Node* node, int key, bool& inserted)  {
        if (!node) {
            inserted = true;
            tree_size++;
            return new Node(key);
        }
        if (key < node->key) {
            node->left = insertNode(node->left, key, inserted);
        }
        else if (key > node->key) {
            node->right = insertNode(node->right, key, inserted);
        }
        else {
            inserted = false;
            return node;
        }
        return balance(node);
    }

    Node* findMin(Node* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    Node* deleteNode(Node* node, int key, bool& deleted, bool decrease_size = true) {
        if (!node) {
            deleted = false;
            return nullptr;
        }
        if (key < node->key) {
            node->left = deleteNode(node->left, key, deleted, decrease_size);
        }
        else if (key > node->key) {
            node->right = deleteNode(node->right, key, deleted, decrease_size);
        }
        else {
            deleted = true;
            if (decrease_size) {
                tree_size--;
            }

            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            Node* temp = findMin(node->right);
            node->key = temp->key;
            // ВАЖНО: передаем false, чтобы не уменьшать size дважды
            node->right = deleteNode(node->right, temp->key, deleted, false);
        }
        return balance(node);
    }

    bool findNode(Node* node, int key) const {
        if (!node) {
            return false;
        }
        if (key < node->key) {
            return findNode(node->left, key);
        }
        else if (key > node->key) {
            return findNode(node->right, key);
        }
        else {
            return true;
        }
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void printTree(Node* node) const {
        if (node) {
            printTree(node->left);
            std::cout << node->key << " ";
            printTree(node->right);
        }
    }
    int findIndex(Node* node, int key, int& index) const {
        if (!node) {
            return -1;
        }
        if (key < node->key) {
            return findIndex(node->left, key, index);
        }
        else if (key > node->key) {

            index += 1 + (node->left ? countNodes(node->left) : 0);
            return findIndex(node->right, key, index);
        }
        else {

            index += (node->left ? countNodes(node->left) : 0);
            return index;
        }

    }
    int countNodes(Node* node) const {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

public:
    Avl_Tree() : root(nullptr), tree_size(0) {}
    ~Avl_Tree() { clear(root); }

    bool insert(int key) {
        bool inserted = false;
        root = insertNode(root, key, inserted);
        return inserted;
    }

    bool remove(int key) {
        bool deleted = false;
        root = deleteNode(root, key, deleted);
        return deleted;
    }

    bool find(int key) const {
        return findNode(root, key);
    }

    size_t size() const {
        return tree_size;
    }

    bool empty() const {
        return tree_size == 0;
    }

    void print() const {
        printTree(root);
        std::cout << std::endl;
    }

    void clear_tree() {
        clear(root);
        root = nullptr;
        tree_size = 0;
    }
    int find_index(int key) const {
        int index = 0;
        return findIndex(root, key, index);
    }
};

class set {
private:
    Avl_Tree tree;

public:
    set() {}

    bool insert(int key) {
        return tree.insert(key);
    }

    size_t erase(int key) {
        return tree.remove(key) ? 1 : 0;
    }

    bool contains(int key) const {
        return tree.find(key);
    }

    size_t count(int key) const {
        return tree.find(key) ? 1 : 0;
    }

    size_t size() const {
        return tree.size();
    }

    void print_size() const {
        std::cout << "Size: " << tree.size() << std::endl;
    }

    bool empty() const {
        return tree.empty();
    }

    void print() const {
        tree.print();
    }

    void clear() {
        tree.clear_tree();
    }

    int find(int key) const {
        int result = tree.find_index(key);
        std::cout << result << std::endl;
        return result;
    }
};

int main() {
    set s;
    s.insert(7);   //          9
    s.insert(10);  //       7     10
    s.insert(9);   //         8        15
    s.insert(8);   //
    s.insert(15);
    s.insert(9);

    s.print();
    s.print_size();

    s.find(15);

    s.erase(9);
    s.print();
    s.print_size();

    s.find(15);


    s.clear();
    s.print();
    s.print_size();

    return 0;
}
