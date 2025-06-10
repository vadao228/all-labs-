#include <iostream>
#include <queue>
#include <vector>
#include <sstream>
#include <string>

template <class T>
class BinarySearchTree {
public:
    struct Node {
        Node* Left;
        Node* Right;
        T Value;
        Node(T value) : Left(nullptr), Right(nullptr), Value(value) {}
    };

    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { clear(); }
    //добавление элемента
    void Add(const T& value) {
        Node* newNode = new Node(value);
        if (!root) {
            root = newNode;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;
        while (current) {
            parent = current;
            if (value < current->Value) {
                current = current->Left;
            }
            else {
                current = current->Right;
            }
        }

        if (value < parent->Value) {
            parent->Left = newNode;
        }
        else {
            parent->Right = newNode;
        }
    }
    //поиск 
    Node* Find(const T& value) {
        Node* current = root;
        while (current) {
            if (value == current->Value) {
                return current;
            }
            else if (value < current->Value) {
                current = current->Left;
            }
            else {
                current = current->Right;
            }
        }
        return nullptr;
    }
    //удаление
    void Remove(Node* node) {
        if (!node) return;

        Node* parent = findParent(node);

        // первый случай(нет детей)
        if (!node->Left && !node->Right) {
            updateParent(parent, node, nullptr);
            delete node;
        }
        // второй случай(один ребенок)
        else if (!node->Left || !node->Right) {
            Node* child = node->Left ? node->Left : node->Right;
            updateParent(parent, node, child);
            delete node;
        }
        // третий случай(два ребенка)
        else {
            Node* successor = node->Right;
            Node* succParent = node;
            while (successor->Left) {
                succParent = successor;
                successor = successor->Left;
            }

            node->Value = successor->Value;
            if (succParent->Left == successor) {
                succParent->Left = successor->Right;
            }
            else {
                succParent->Right = successor->Right;
            }
            delete successor;
        }
    }
    //обратный обход
    void Postorder(Node* root) {
        if (root) {
            Postorder(root->Left);
            Postorder(root->Right);
            std::cout << root->Value << " ";
        }
    }

    std::string serialize() {
        if (!root) return "N";

        std::queue<Node*> q;
        q.push(root);
        std::vector<std::string> tokens;
        bool nonNullInQueue = true;

        while (!q.empty() && nonNullInQueue) {
            nonNullInQueue = false;
            int levelSize = q.size();
            for (int i = 0; i < levelSize; i++) {
                Node* node = q.front();
                q.pop();
                if (node) {
                    tokens.push_back(std::to_string(node->Value));
                    q.push(node->Left);
                    q.push(node->Right);
                    if (node->Left || node->Right) {
                        nonNullInQueue = true;
                    }
                }
                else {
                    tokens.push_back("N");
                }
            }
        }

        // объединение в строку
        std::string result;
        for (size_t i = 0; i < tokens.size(); i++) {
            if (i > 0) result += ',';
            result += tokens[i];
        }
        return result;
    }

    void deserialize(const std::string& data) {
        clear();
        if (data == "N" || data.empty()) return;

        std::vector<std::string> tokens = split(data, ',');
        if (tokens[0] == "N") return;

        root = new Node(static_cast<T>(std::stoi(tokens[0])));
        std::queue<Node*> q;
        q.push(root);
        size_t index = 1;

        while (!q.empty() && index < tokens.size()) {
            Node* node = q.front();
            q.pop();

            // левый потомок
            if (index < tokens.size() && tokens[index] != "N") {
                node->Left = new Node(static_cast<T>(std::stoi(tokens[index])));
                q.push(node->Left);
            }
            index++;

            // правый потомок
            if (index < tokens.size() && tokens[index] != "N") {
                node->Right = new Node(static_cast<T>(std::stoi(tokens[index])));
                q.push(node->Right);
            }
            index++;
        }
    }

    Node* getRoot() const { return root; }

private:
    Node* root;

    void clear() {
        if (!root) return;
        std::queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            if (node->Left) q.push(node->Left);
            if (node->Right) q.push(node->Right);
            delete node;
        }
        root = nullptr;
    }

    Node* findParent(Node* child) {
        if (child == root) return nullptr;
        Node* current = root;
        while (current) {
            if (current->Left == child || current->Right == child) {
                return current;
            }
            if (child->Value < current->Value) {
                current = current->Left;
            }
            else {
                current = current->Right;
            }
        }
        return nullptr;
    }

    void updateParent(Node* parent, Node* node, Node* newChild) {
        if (!parent) {
            root = newChild;
        }
        else if (parent->Left == node) {
            parent->Left = newChild;
        }
        else {
            parent->Right = newChild;
        }
    }

    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

int main() {
    // Пример использования
    BinarySearchTree<int> bst;

    // Добавление элементов
    bst.Add(5);
    bst.Add(3);
    bst.Add(7);
    bst.Add(6);
    bst.Add(9);
    bst.Add(11);
    // Обратный обход
    std::cout << "Postorder Traversal: ";
    bst.Postorder(bst.getRoot());
    std::cout << "\n";

    // Сериализация
    std::string serialized = bst.serialize();
    std::cout << "Serialized: " << serialized << "\n";

    // Десериализация
    BinarySearchTree<int> bst2;
    bst2.deserialize(serialized);
    std::cout << "Postorder after deserialization: ";
    bst2.Postorder(bst2.getRoot());
    std::cout << "\n";

    return 0;
}
