#include <vector>
#include <climits>
#include <iostream>
#include <algorithm>

class SegmentTree {
private:
    struct Node { //���������, � ��������� ������
        int sum; //����� �� �������
        int min; //������� �� �������
        int max; // �������� �� �������

        Node(int val = 0) : sum(val), min(val), max(val) {}
    };

    int n;                      // �-�� ��������� � �������
    std::vector<int> data;       // �������� �������� ������
    std::vector<Node> tree;      // ������ ����� => ������
    int tree_size;               // ������� ������ (2*N) - ��������� ������� 2 >= n

    // ���������� �������� �� ����������� 2� �������� � ����
    Node combine(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum; // ����� ������� - ����� ������ + ����� �������
        res.min = std::min(a.min, b.min); // ������� ������ ������� - ������� �������� �����
        res.max = std::max(a.max, b.max); // ���������� ��� ���������
        return res;
    }

public:
    // k���������� ������ ��������
    SegmentTree(const std::vector<int>& nums) {
        n = nums.size();
        data = nums;

        // ���������� ������� �����a �� �������
        tree_size = 1;
        while (tree_size < n) {
            tree_size <<= 1;
        }
        tree.resize(2 * tree_size - 1);

        // ���������� ������
        build(0, 0, n - 1);
    }

private:
    // ���������� ����� ��������
    void build(int node, int left, int right) {
        if (left == right) {
            // ��������� ������, ��� ���� ������ - ��������� ��� �������
            if (left < n) {
                tree[node] = Node(data[left]);
            }
            else {
                // ���� ���� - �������� ������� => ���������, ����� => ��������� 0
                tree[node] = Node(0);
            }
            return;
        }
        // ����������� ���������� ������������
        int mid = left + (right - left) / 2;
        build(2 * node + 1, left, mid);      // �����
        build(2 * node + 2, mid + 1, right); // ������� 

        // ���� - combine �� ����� 
        tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
    }

public:
    // �����: ����� �� ������� [l, r]
    int querySum(int l, int r) {
        return query(0, 0, n - 1, l, r).sum;
    }

    // �����: ������� �� ������� [l, r]
    int queryMin(int l, int r) {
        return query(0, 0, n - 1, l, r).min;
    }

    // �����: �������� �� ������� [l, r]
    int queryMax(int l, int r) {
        return query(0, 0, n - 1, l, r).max;
    }

private:
    // ��������������� �����: ������
    Node query(int node, int node_left, int node_right, int query_left, int query_right) {
        // ������� ������ ��� ������� => 0
        if (node_right < query_left || node_left > query_right) {
            return Node(0); 
        }

        // ������� �������� ���� ��������� ���������� � �������������
        if (query_left <= node_left && node_right <= query_right) {
            return tree[node];
        }

        // ���������� ������ �������� => ���������� ��������� ��� ���������
        int mid = node_left + (node_right - node_left) / 2;
        Node left_res = query(2 * node + 1, node_left, mid, query_left, query_right);
        Node right_res = query(2 * node + 2, mid + 1, node_right, query_left, query_right);

        return combine(left_res, right_res);
    }

public:
    // �����: ���������� ����������� �������� �� �������
    void update(int index, int value) {
        if (index < 0 || index >= n) return; // �������� �� ������������ ���������� �������

        data[index] = value;
        update(0, 0, n - 1, index, value);
    }

private:
    // ���������� ������ ������
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
    // �����: ����� ������
    void printTree() {
        std::cout << "Segment Tree Structure:\n";
        std::cout << "Index\tRange\tSum\tMin\tMax\n";
        printTree(0, 0, n - 1, 0);
    }

private:
    void printTree(int node, int left, int right, int level) {
        if (node >= tree.size()) return;

        // ����� �������� ����
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

    std::cout << "\nTree structure:\n"; //����� ���������� ����� �������, ��� ������� ��������� ���� ����� ������ ������ ����, � ����� ������
    st.printTree();                     //��� ������ ����� ������ ��������� ����� �����, ������� � ��������. 
                                        // � �������, ��������������, ���, ����, ���� ����� ������ ��������
    return 0;
}