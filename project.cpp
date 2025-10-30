#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <utility>
#include <cmath>
#include <functional>
#include <algorithm>
#include <iomanip>
#include <set>

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
        if (key > node->key) {
            return findNode(node->right, key);
        }
        {
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

    bool isEqual(Node* a, Node* b) const {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return (a->key == b->key) &&
               isEqual(a->left, b->left) &&
               isEqual(a->right, b->right);
    }
    Node* copyTree(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->key);
        newNode->height = node->height;
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

public:
    ~Avl_Tree() { clear(root); }
    Avl_Tree() : root(nullptr), tree_size(0) {}
    Avl_Tree(const std::initializer_list<int>& a) : root(nullptr), tree_size(0) {
        for (auto e : a) {
            insert(e);
        }
    }
    Avl_Tree(const Avl_Tree& other) : root(nullptr), tree_size(other.tree_size) {
        root = copyTree(other.root);
    }
    Avl_Tree(Avl_Tree&& other) noexcept : root(other.root), tree_size(other.tree_size) {
        other.root = nullptr;
        other.tree_size = 0;
    }
    Avl_Tree& operator=(const Avl_Tree& other) {
        if (this != &other) {
            clear(root);
            root = copyTree(other.root);
            tree_size = other.tree_size;
        }
        return *this;
    }
    Avl_Tree& operator=(Avl_Tree&& other)  {
        if (this != &other) {
            clear(root);
            root = other.root;
            tree_size = other.tree_size;
            other.root = nullptr;
            other.tree_size = 0;
        }
        return *this;
    }
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

    bool contains(int key) const {
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

    void clear() {
        clear(root);
        root = nullptr;
        tree_size = 0;
    }

    int get_index(int key) const {
        int index = 0;
        return findIndex(root, key, index);
    }

    void swap(Avl_Tree& other) {
        std::swap(root, other.root);
        std::swap(tree_size, other.tree_size);
    }

    bool is_equal(const Avl_Tree& other) const {
        if (tree_size != other.tree_size) return false;
        return isEqual(root, other.root);
    }
};

class Maze {
private:
    std::vector<std::vector<int>> grid;
    Avl_Tree visitedCells;
    int rows, cols;

    int coordToKey(int x, int y) const {
        return x * cols + y;
    }

    std::pair<int, int> keyToCoord(int key) const {
        return {key / cols, key % cols};
    }

    bool isValid(int x, int y) const {
        return x >= 0 && x < rows && y >= 0 && y < cols;
    }

    bool isPassable(int x, int y) const {
        return isValid(x, y) && grid[x][y] == 0;
    }

    // Проверка, является ли клетка тупиком
    bool isDeadEnd(int x, int y) const {
        if (grid[x][y] == 1) return false; // Стена не может быть тупиком

        int passageCount = 0;
        const int dx[4] = {-1, 0, 1, 0};
        const int dy[4] = {0, 1, 0, -1};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (isValid(nx, ny) && grid[nx][ny] == 0) {
                passageCount++;
            }
        }

        // Тупик имеет только 1 проход
        return passageCount == 1;
    }

    // Получить количество соседей-проходов
    int countPassageNeighbors(int x, int y) const {
        int count = 0;
        const int dx[4] = {-1, 0, 1, 0};
        const int dy[4] = {0, 1, 0, -1};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (isValid(nx, ny) && grid[nx][ny] == 0) {
                count++;
            }
        }
        return count;
    }

public:
    Maze() : rows(0), cols(0) {}
    Maze(const std::vector<std::vector<int>>& maze) : grid(maze) {
        rows = maze.size();
        cols = maze.empty() ? 0 : maze[0].size();
    }
    Maze(int r, int c) : rows(r), cols(c) {
        grid.resize(r, std::vector<int>(c, 0));
    }

    void setGrid(const std::vector<std::vector<int>>& maze) {
        grid = maze;
        rows = maze.size();
        cols = maze.empty() ? 0 : maze[0].size();
        visitedCells.clear();
    }

    void setCell(int x, int y, int value) {
        if (isValid(x, y)) {
            grid[x][y] = value;
        }
    }

    // BRAIDING ALGORITHM - устранение тупиков
    void braidMaze(double braidFactor = 0.5) {
        std::cout << "\n=== APPLYING BRAIDING ALGORITHM ===" << std::endl;
        std::cout << "Braid factor: " << braidFactor << std::endl;

        std::vector<std::pair<int, int>> deadEnds;

        // Находим все тупики
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (isDeadEnd(i, j)) {
                    deadEnds.push_back({i, j});
                }
            }
        }

        std::cout << "Found " << deadEnds.size() << " dead ends" << std::endl;

        // "Зашиваем" случайные тупики в соответствии с braidFactor
        std::random_shuffle(deadEnds.begin(), deadEnds.end());
        int braidCount = static_cast<int>(deadEnds.size() * braidFactor);

        for (int i = 0; i < braidCount && i < deadEnds.size(); i++) {
            auto [x, y] = deadEnds[i];
            braidDeadEnd(x, y);
        }

        std::cout << "Braided " << braidCount << " dead ends" << std::endl;
    }

private:
    // "Зашивание" конкретного тупика
    void braidDeadEnd(int x, int y) {
        const int dx[4] = {-1, 0, 1, 0};
        const int dy[4] = {0, 1, 0, -1};

        // Находим стену, которую можно убрать чтобы соединить с другим проходом
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (isValid(nx, ny) && grid[nx][ny] == 1) {
                // Проверяем, не создаст ли это слишком много соединений
                int newConnections = 0;
                for (int j = 0; j < 4; j++) {
                    int nnx = nx + dx[j];
                    int nny = ny + dy[j];
                    if (isValid(nnx, nny) && grid[nnx][nny] == 0) {
                        newConnections++;
                    }
                }

                // Если убирание стены создает разумное количество соединений
                if (newConnections >= 1 && newConnections <= 2) {
                    grid[nx][ny] = 0; // Убираем стену
                    std::cout << "  Braided: (" << x << "," << y << ") -> (" << nx << "," << ny << ")" << std::endl;
                    return;
                }
            }
        }
    }

public:
    // Wave Algorithm (Branding Algorithm)
    std::vector<std::pair<int, int>> findPathWave(std::pair<int, int> start,
                                                 std::pair<int, int> end) {
        if (!isPassable(start.first, start.second) ||
            !isPassable(end.first, end.second)) {
            std::cout << "Start or end position is blocked!" << std::endl;
            return {};
        }

        std::vector<std::vector<int>> wave(rows, std::vector<int>(cols, -1));
        std::vector<std::vector<std::pair<int, int>>> parent(rows,
            std::vector<std::pair<int, int>>(cols, {-1, -1}));

        std::queue<std::pair<int, int>> q;

        wave[start.first][start.second] = 0;
        q.push(start);

        const int dx[4] = {-1, 0, 1, 0};
        const int dy[4] = {0, 1, 0, -1};

        bool found = false;

        while (!q.empty() && !found) {
            auto [x, y] = q.front();
            q.pop();

            if (x == end.first && y == end.second) {
                found = true;
                break;
            }

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (isPassable(nx, ny) && wave[nx][ny] == -1) {
                    wave[nx][ny] = wave[x][y] + 1;
                    parent[nx][ny] = {x, y};
                    q.push({nx, ny});
                    visitedCells.insert(coordToKey(nx, ny));
                }
            }
        }

        if (!found) {
            std::cout << "No path found by wave algorithm!" << std::endl;
            return {};
        }

        std::vector<std::pair<int, int>> path;
        auto current = end;

        while (current != start) {
            path.push_back(current);
            current = parent[current.first][current.second];
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());

        std::cout << "Wave algorithm path found! Length: " << path.size() << std::endl;
        return path;
    }

    // BFS Algorithm
    std::vector<std::pair<int, int>> findPathBFS(std::pair<int, int> start,
                                                std::pair<int, int> end) {
        if (!isPassable(start.first, start.second) ||
            !isPassable(end.first, end.second)) {
            return {};
        }

        visitedCells.clear();

        std::queue<std::pair<std::pair<int, int>,
                            std::vector<std::pair<int, int>>>> q;

        const int dx[4] = {-1, 0, 1, 0};
        const int dy[4] = {0, 1, 0, -1};

        std::vector<std::pair<int, int>> initialPath = {start};
        q.push({start, initialPath});
        visitedCells.insert(coordToKey(start.first, start.second));

        while (!q.empty()) {
            auto [current, path] = q.front();
            q.pop();

            int x = current.first;
            int y = current.second;

            if (x == end.first && y == end.second) {
                std::cout << "BFS path found! Length: " << path.size() << std::endl;
                return path;
            }

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                int key = coordToKey(nx, ny);

                if (isPassable(nx, ny) && !visitedCells.contains(key)) {
                    visitedCells.insert(key);
                    std::vector<std::pair<int, int>> newPath = path;
                    newPath.push_back({nx, ny});
                    q.push({{nx, ny}, newPath});
                }
            }
        }

        std::cout << "No BFS path found!" << std::endl;
        return {};
    }

    void printMazeWithPath(const std::vector<std::pair<int, int>>& path = {}) {
        Avl_Tree pathCells;
        for (const auto& p : path) {
            pathCells.insert(coordToKey(p.first, p.second));
        }

        std::cout << "\nMaze (" << rows << "x" << cols << "):\n   ";
        for (int j = 0; j < cols; j++) {
            std::cout << j << " ";
        }
        std::cout << "\n";

        for (int i = 0; i < rows; i++) {
            std::cout << i << "  ";
            for (int j = 0; j < cols; j++) {
                int key = coordToKey(i, j);
                if (!path.empty() && pathCells.contains(key)) {
                    std::cout << "* ";
                } else if (grid[i][j] == 1) {
                    std::cout << "# ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
    }

    void printMaze() {
        std::cout << "\nMaze (" << rows << "x" << cols << "):\n   ";
        for (int j = 0; j < cols; j++) {
            std::cout << j << " ";
        }
        std::cout << "\n";

        for (int i = 0; i < rows; i++) {
            std::cout << i << "  ";
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 1) {
                    std::cout << "# ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
    }

    void printStats() const {
        std::cout << "Maze size: " << rows << "x" << cols << std::endl;
        std::cout << "Visited cells: " << visitedCells.size() << std::endl;
        std::cout << "Total cells: " << rows * cols << std::endl;

        // Подсчет тупиков
        int deadEnds = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (isDeadEnd(i, j)) {
                    deadEnds++;
                }
            }
        }
        std::cout << "Dead ends: " << deadEnds << std::endl;
    }

    void clear() {
        grid.clear();
        visitedCells.clear();
        rows = cols = 0;
    }
};

// Демонстрация работы
int main() {
    // Создаем лабиринт с тупиками
    std::vector<std::vector<int>> mazeGrid = {
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 0}
    };

    Maze maze(mazeGrid);

    std::cout << "=== MAZE SOLVER WITH BRAIDING ALGORITHM ===" << std::endl;

    // Показываем оригинальный лабиринт
    std::cout << "\n--- ORIGINAL MAZE ---" << std::endl;
    maze.printMaze();
    maze.printStats();

    std::pair<int, int> start = {0, 0};
    std::pair<int, int> end = {6, 7};

    // Поиск пути до брайдинга
    std::cout << "\n--- PATH FINDING (BEFORE BRAIDING) ---" << std::endl;
    auto pathBefore = maze.findPathWave(start, end);
    maze.printMazeWithPath(pathBefore);

    // Применяем Braiding Algorithm
    maze.braidMaze(0.7); // Braid 70% тупиков

    // Показываем лабиринт после брайдинга
    std::cout << "\n--- MAZE AFTER BRAIDING ---" << std::endl;
    maze.printMaze();
    maze.printStats();

    // Поиск пути после брайдинга
    std::cout << "\n--- PATH FINDING (AFTER BRAIDING) ---" << std::endl;
    auto pathAfter = maze.findPathWave(start, end);
    maze.printMazeWithPath(pathAfter);

    // Сравнение BFS до и после
    std::cout << "\n--- BFS COMPARISON ---" << std::endl;
    std::cout << "BFS before braiding:" << std::endl;
    auto bfsBefore = maze.findPathBFS(start, end);

    std::cout << "BFS after braiding:" << std::endl;
    auto bfsAfter = maze.findPathBFS(start, end);

    return 0;
}
