#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <climits>
#include <algorithm>

class SegmentTree {
private:
    struct Node {
        int sum;
        int min;
        int max;

        Node(int val = 0) : sum(val), min(val), max(val) {}
    };

    int n;
    std::vector<int> data;
    std::vector<Node> tree;
    int tree_size;

    Node combine(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum;
        res.min = std::min(a.min, b.min);
        res.max = std::max(a.max, b.max);
        return res;
    }

    void build(int node, int left, int right);
    Node query(int node, int node_left, int node_right, int query_left, int query_right);
    void update(int node, int node_left, int node_right, int index, int value);
    void printTree(int node, int left, int right, int level);

public:
    SegmentTree(const std::vector<int>& nums);

    int querySum(int l, int r);
    int queryMin(int l, int r);
    int queryMax(int l, int r);

    void update(int index, int value);
    void printTree();
};

#endif // SEGMENT_TREE_H
