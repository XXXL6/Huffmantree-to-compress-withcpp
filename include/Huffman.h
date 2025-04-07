/**
 * @file Huffman.hpp
 * @brief
 * 定义Huffmantree节点(HuffmanNode类型)，定义Huffmantree数据结构(Huffman类型)
 * @version 0.1
 *
 *
 */
#ifndef HUFF
#define HUFF
#include <map>
#include <string>
#include <vector>
// using namespace std;
// using ushort = unsigned short;

namespace Huffman
{
using namespace std;
using ushort = unsigned short;

struct c_wMap {
  private:
    vector<char> c;
    vector<ushort> w;

  public:
    c_wMap(char *fileName, char flag);
    c_wMap(ifstream &in);
    // const int &c_size() const { return c.size(); }
    const int returnSize() const { return c.size(); }
    const char &c_ele(int i) const { return c[i]; }
    const ushort &w_ele(int i) const { return w[i]; }
    friend void countToc_wMap(c_wMap &cw, char *fileName);
    friend void readToc_wMap(c_wMap &cw, char *fileName);
};

struct HuffmanNode {
    HuffmanNode(int left, int right, int parent, char c, ushort weight)
        : left{left}, right{right}, parent{parent}, c{c}, weight{weight}
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
// searchHuffuman:
// 深度优先遍历哈夫曼树，为每一个节点编码，以建立对应的哈夫曼编码表
class HuffmanTree
{
  public:
    HuffmanTree(const c_wMap cw); // 构造函数
    const HuffmanNode &returnNode(int index) const
    {
        return root[index];
    } // 选择器:返回index号元素
    int returnSize() const { return root.size(); } // 选择器:返回叶子个数
    // vector<HuffmanNode> huffmantree;
    // void encodeHuffmanTree(map<char, string> &encode);
    // {
    //     dfs(huffmantree[huffmantree.size() - 1], encode);
    // }

  private:
    // HuffmanNode *root;
    vector<HuffmanNode> root; // 使用vector作为叶子的容器
    // string path;
    // void initHuffman(const vector<char> &c, const vector<ushort> &w);
    // void searchMin(int &min1, int &min2) const;
    // void dfs(const HuffmanNode &node, map<char, string> &encode);
};

map<char, string> encodeHuffmanTree(const HuffmanTree &Tree);

void compress(char *inFileName, char *outFileName,
              const map<char, string> &encode, const c_wMap &cw);
void decompress(char *inFileName, char *outFileName);

} // namespace Huffman
#endif