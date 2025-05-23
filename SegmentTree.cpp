#include <vector>
#include <climits>
#include <iostream>
#include <algorithm>

class SegmentTree {
private:
    struct Node { //ñòðóêòóðà, ñ õðàíåíèåì äàííûõ
        int sum; //ñóììà íà îòðåçêå
        int min; //ìèíèìóì íà îòðåçêå
        int max; // ìàêñèìóì íà îòðåçêå

        Node(int val = 0) : sum(val), min(val), max(val) {}
    };

    int n;                      // ê-âî ýëåìåíòîâ â ìàññèâå
    std::vector<int> data;       // êîïèðóåò âõîäÿùèå äàííûå
    std::vector<Node> tree;      // ìàññèâ óçëîâ => äåðåâî
    int tree_size;               // ðàççìåð äåðåâà (2*N) - áëèæàéøàÿ ñòåïåíü 2 >= n

    // âíóòðåííÿÿ îïåðàöèÿ ïî îáúåäèíåíèþ 2õ îòðåçêîâ â îäèí
    Node combine(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum; // ñóììà îòðåçêà - ñóììà ëåâîãî + ñóììà ïðàâîãî
        res.min = std::min(a.min, b.min); // ìèíèìóì íîâîãî îòðåçêà - ìèíèìóì ìèíèìóìà äåòåé
        res.max = std::max(a.max, b.max); // àíàëîãè÷íî äëÿ ìàêñèìóìà
        return res;
    }

public:
    // Êîíñòðóêòîð äåðåâà îòðåçêîâ
    SegmentTree(const std::vector<int>& nums) {
        n = nums.size();
        data = nums;

        // âû÷èñëåíèå ðàçìåðà äåðåâî ïî ôîðìóëå
        tree_size = 1;
        while (tree_size < n) {
            tree_size <<= 1;
        }
        tree.resize(2 * tree_size - 1);

        // ïîñòðîåíèå äåðåâà
        build(0, 0, n - 1);
    }

private:
    // ïîñòðîåíèå ÷åðåç ðåêóðñèþ
    void build(int node, int left, int right) {
        if (left == right) {
            // ñòðóêòîðà òàêîâà, ÷òî ëèñò äåðåâà - îòäåëüíûé åãî ýëåìåíò
            if (left < n) {
                tree[node] = Node(data[left]);
            }
            else {
                // åñëè ëèñò - ðåàëüíûé ýëåìåíò => ñîõðàíèòü, èíà÷å => çàïîëíèòü 0
                tree[node] = Node(0);
            }
            return;
        }
        // ðåêóðñèâíîå ïîñòðîåíèå ïîääåððåâüåâ
        int mid = left + (right - left) / 2;
        build(2 * node + 1, left, mid);      // ëåâûé
        build(2 * node + 2, mid + 1, right); // ïðàâíûé 

        // óçåë - combine îò äåòåé 
        tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
    }

public:
    // ÌÅÒÎÄ: ñóììà íà îòðåçêå [l, r]
    int querySum(int l, int r) {
        return query(0, 0, n - 1, l, r).sum;
    }

    // ÌÅÒÎÄ: ìèíèìóì íà îòðåçêå [l, r]
    int queryMin(int l, int r) {
        return query(0, 0, n - 1, l, r).min;
    }

    // ÌÅÒÎÄ: ìàêñèìóì íà îòðåçêå [l, r]
    int queryMax(int l, int r) {
        return query(0, 0, n - 1, l, r).max;
    }

private:
    // ÂÑÏÎÌÎÃÀÒÅËÜÍÛÉ ÌÅÒÎÄ: çàïðîñ
    Node query(int node, int node_left, int node_right, int query_left, int query_right) {
        // îòðåçîê âîîáùå âíå çàïðîñà => 0
        if (node_right < query_left || node_left > query_right) {
            return Node(0); 
        }

        // îòðåçîê òåêóùåãî óçëà ïîëíîñòüþ ñîäåðæèòñÿ â çàïðàøèâàåìîì
        if (query_left <= node_left && node_right <= query_right) {
            return tree[node];
        }

        // ñîäåðæèòñÿ òîëüêî ÷àñòè÷íî => ðåêóðñèâíî ïðîâåðèòü îáà ïîääåðåâà
        int mid = node_left + (node_right - node_left) / 2;
        Node left_res = query(2 * node + 1, node_left, mid, query_left, query_right);
        Node right_res = query(2 * node + 2, mid + 1, node_right, query_left, query_right);

        return combine(left_res, right_res);
    }

public:
    // ÌÅÒÎÄ: îáíîâëåíèå êîíêðåòíîãî ýëåìåíòà ïî èíäåêñó
    void update(int index, int value) {
        if (index < 0 || index >= n) return; // ïðîâåðêà íà ïðàâèëüíîñòü ââåäåííîãî èíäåêñà

        data[index] = value;
        update(0, 0, n - 1, index, value);
    }

private:
    // âíóòðåííÿÿ ëîãèêà ìåòîäà
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
    // ÌÅÒÎÄ: âûâîä äåðåâà
    void printTree() {
        std::cout << "Segment Tree Structure:\n";
        std::cout << "Index\tRange\tSum\tMin\tMax\n";
        printTree(0, 0, n - 1, 0);
    }

private:
    void printTree(int node, int left, int right, int level) {
        if (node >= tree.size()) return;

        // âûâîä òåêóùåãî óçëà
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

    std::cout << "\nTree structure:\n";
    st.printTree();

    return 0;
}
