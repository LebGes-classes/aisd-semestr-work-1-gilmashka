#include <vector>
#include <climits>
#include <iostream>
#include <algorithm>

class SegmentTree {
private:
    struct Node { //структура, с хранением данных
        int sum; //сумма на отрезке
        int min; //минимум на отрезке
        int max; // максимум на отрезке

        Node(int val = 0) : sum(val), min(val), max(val) {}
    };

    int n;                      // к-во элементов в массиве
    std::vector<int> data;       // копирует входящие данные
    std::vector<Node> tree;      // массив узлов => дерево
    int tree_size;               // раззмер дерева (2*N) - ближайшая степень 2 >= n

    // внутренняя операция по объединению 2х отрезков в один
    Node combine(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum; // сумма отрезка - сумма левого + сумма правого
        res.min = std::min(a.min, b.min); // минимум нового отрезка - минимум минимума детей
        res.max = std::max(a.max, b.max); // аналогично для максимума
        return res;
    }

public:
    // kонструктор дерева отрезков
    SegmentTree(const std::vector<int>& nums) {
        n = nums.size();
        data = nums;

        // вычисление размера деревa по формуле
        tree_size = 1;
        while (tree_size < n) {
            tree_size <<= 1;
        }
        tree.resize(2 * tree_size - 1);

        // построение дерева
        build(0, 0, n - 1);
    }

private:
    // построение через рекурсию
    void build(int node, int left, int right) {
        if (left == right) {
            // структора такова, что лист дерева - отдельный его элемент
            if (left < n) {
                tree[node] = Node(data[left]);
            }
            else {
                // если лист - реальный элемент => сохранить, иначе => заполнить 0
                tree[node] = Node(0);
            }
            return;
        }
        // рекурсивное построение поддерревьев
        int mid = left + (right - left) / 2;
        build(2 * node + 1, left, mid);      // левый
        build(2 * node + 2, mid + 1, right); // правный 

        // узел - combine от детей 
        tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
    }

public:
    // МЕТОД: сумма на отрезке [l, r]
    int querySum(int l, int r) {
        return query(0, 0, n - 1, l, r).sum;
    }

    // МЕТОД: минимум на отрезке [l, r]
    int queryMin(int l, int r) {
        return query(0, 0, n - 1, l, r).min;
    }

    // МЕТОД: максимум на отрезке [l, r]
    int queryMax(int l, int r) {
        return query(0, 0, n - 1, l, r).max;
    }

private:
    // ВСПОМОГАТЕЛЬНЫЙ МЕТОД: запрос
    Node query(int node, int node_left, int node_right, int query_left, int query_right) {
        // отрезок вообще вне запроса => 0
        if (node_right < query_left || node_left > query_right) {
            return Node(0); 
        }

        // отрезок текущего узла полностью содержится в запрашиваемом
        if (query_left <= node_left && node_right <= query_right) {
            return tree[node];
        }

        // содержится только частично => рекурсивно проверить оба поддерева
        int mid = node_left + (node_right - node_left) / 2;
        Node left_res = query(2 * node + 1, node_left, mid, query_left, query_right);
        Node right_res = query(2 * node + 2, mid + 1, node_right, query_left, query_right);

        return combine(left_res, right_res);
    }

public:
    // МЕТОД: обновление конкретного элемента по индексу
    void update(int index, int value) {
        if (index < 0 || index >= n) return; // проверка на правильность введенного индекса

        data[index] = value;
        update(0, 0, n - 1, index, value);
    }

private:
    // внутренняя логика метода
    void update(int node, int node_left, int node_right, int index, int value) {
        if (node_left == node_right) {
            tree[node] = Node(value);
            return;
        }

        int mid = node_left + (node_right - node_left) / 2;
        if (index <= mid) {
            update(2 * node + 1, node_left, mid, index, value);
        }
        else {
            update(2 * node + 2, mid + 1, node_right, index, value);
        }

        tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
    }

public:
    // МЕТОД: вывод дерева
    void printTree() {
        std::cout << "Segment Tree Structure:\n";
        std::cout << "Index\tRange\tSum\tMin\tMax\n";
        printTree(0, 0, n - 1, 0);
    }

private:
    void printTree(int node, int left, int right, int level) {
        if (node >= tree.size()) return;

        // вывод текущего узла
        std::cout << node << "\t[" << left << "," << right << "]\t"
            << tree[node].sum << "\t"
            << tree[node].min << "\t"
            << tree[node].max << "\n";

        if (left != right) {
            int mid = left + (right - left) / 2;
            printTree(2 * node + 1, left, mid, level + 1);
            printTree(2 * node + 2, mid + 1, right, level + 1);
        }
    }
};

int main() {
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;
    
    std::vector<int> nums(size);
    std::cout << "Enter " << size << " numbers separated by spaces:\n";
    for (int i = 0; i < size; i++) {
        std::cin >> nums[i];
    }

    
    SegmentTree st(nums);

    
    std::cout << "Sum [1..4]: " << st.querySum(1, 4) << "\n";      
    std::cout << "Min [1..4]: " << st.queryMin(1, 4) << "\n";      
    std::cout << "Max [1..4]: " << st.queryMax(1, 4) << "\n";     

  
    st.update(2, 10);
    std::cout << "\nAfter update:\n";
    std::cout << "Sum [1..4]: " << st.querySum(1, 4) << "\n";      
    std::cout << "Max [1..4]: " << st.queryMax(1, 4) << "\n";      

    std::cout << "\nTree structure:\n"; //вывод происходит таким образом, что сначала выводится одна ветвь дерева сверху вниз, а потом вторая
    st.printTree();                     //для каждой части дерева выводится сразу сумма, минимум и максимум. 
                                        // у листьев, соответственно, мин, макс, сумм равны самому элементу
    return 0;
}