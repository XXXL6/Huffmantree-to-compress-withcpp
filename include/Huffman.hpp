/**
 * @file Huffman.hpp
 * @brief 定义Huffmantree节点(HuffmanNode类型)，定义Huffmantree数据结构(Huffman类型)
 * @version 0.1
 *
 *
 */
#ifndef HUFF
#define HUFF
#include <vector>
#include <string>
#include <map>
using namespace std;
using ushort = unsigned short;

// struct TreeNode
// {
// int left;
// int right;
// int parent;
// char c;
// };

struct HuffmanNode
{
    HuffmanNode(int left, int right, int parent, char c, ushort weight) : left{left}, right{right}, parent{parent}, c{c}, weight{weight}
    {
    }
    int left;
    int right;
    int parent;
    char c;
    ushort weight; // 权重
};

// 定义Huffmantree数据类型，提供createHuffman接口与searchHuffman接口
// creatrHuffman: 建立哈夫曼树
// searchHuffuman: 深度优先遍历哈夫曼树，为每一个节点编码，以建立对应的哈夫曼编码表
class Huffman
{
public:
    void createHuffman(const vector<char> &c, const vector<ushort> &w);
    vector<HuffmanNode> huffmantree;
    void searchHuffman(map<char, string> &encode)
    {
        dfs(huffmantree[huffmantree.size() - 1], encode);
    }

private:
    string path;
    void initHuffman(const vector<char> &c, const vector<ushort> &w);
    void searchMin(int &min1, int &min2) const;
    void dfs(const HuffmanNode &node, map<char, string> &encode);
};
#endif