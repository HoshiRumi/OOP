#include <iostream>
#include <stack>
#include <queue>
#include <vector>

class Node {
    int x;
    Node *left;
    Node *right;
public:

    Node* createNode(int x) {
        Node* new_n = new Node();
        new_n->x = x;
        new_n->left = nullptr;
        new_n->right = nullptr;
        return new_n;
    }

    Node* insert(Node* n, int x) {
        Node* new_n = createNode(x);
        if(n == nullptr) {
            return new_n;
        }
        Node* curr = n;
        Node* parent = nullptr;
        while(curr != nullptr) {
            parent = curr;
            if(curr->x > x) {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }
        }
        if (x < parent->x) {
            parent->left = new_n;
        }
        else {
            parent->right = new_n;
        }
        return n;
    }

    void print(Node* n) {
        if(n == nullptr) {
            return;
        }
        std::stack<Node*> s;
        Node* curr = n;
        while(curr != nullptr || !s.empty()) {
            while(curr != nullptr) {
                s.push(curr);
                curr = curr->left;
            }
            curr = s.top();
            s.pop();
            std::cout << curr->x << " ";
            curr = curr->right;
        }
        std::cout << std::endl;
    }

    void clear(Node*& n) {
        if(n == nullptr) {
            return;
        }
        std::stack<Node*> s;
        s.push(n);
        while(!s.empty()) {
            Node* curr = s.top();
            s.pop();
            if(curr->left != nullptr) {
                s.push(curr->left);
            }
            if(curr->right != nullptr) {
                s.push(curr->right);
            }
            delete curr;
        }
        n = nullptr;
    }

    int height(Node* n) {
        if(n == nullptr) {
            return 0;
        }
        std::queue<Node*> q;
        q.push(n);
        int height = 0;
        while(!q.empty()) {
            int level_size = q.size();
            height++;
            for(int i = 0; i < level_size; i++) {
                Node* curr = q.front();
                q.pop();
                if(curr->left != nullptr) {
                    q.push(curr->left);
                }
                if(curr->right != nullptr) {
                    q.push(curr->right);
                }
            }
        }
        return height;
    }

    Node* find(Node* n, int x) {
        Node* curr = n;
        while(curr != nullptr) {
            if(curr->x == x) {
                return curr;
            }
            else if(x < curr->x) {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }
        }
        return nullptr;
    }

    std::vector<int> sort(Node* root) {
        std::vector<int> result;
        if (root == nullptr) return result;
        std::stack<Node*> stack;
        Node* current = root;
        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push(current);
                current = current->left;
            }
            current = stack.top();
            stack.pop();
            result.push_back(current->x);
            current = current->right;
        }
        return result;
    }
};

int main() {
    Node node;
    Node* n = nullptr;

    n = node.insert(n, 5);
    n = node.insert(n, 7);
    n = node.insert(n, 9);
    n = node.insert(n, 3);
    n = node.insert(n, 4);
    n = node.insert(n, 6);
    n = node.insert(n, 2);
    n = node.insert(n, 8);

    node.print(n);
    std::cout << node.height(n) << std::endl;
    n = node.find(n,3);


    node.clear(n);

    return 0;
}