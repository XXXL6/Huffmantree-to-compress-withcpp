/***
 * @Author: whwhwhw whHit1825@163.com
 * @Date: 2025-04-03 15:46:17
 * @LastEditors: whwhwhw whHit1825@163.com
 * @LastEditTime: 2025-04-08 12:34:39
 * @FilePath: \Huffmantree-to-compress-withcpp\include\Huffman.h
 * @Description:此文件存放namespace Huffman的声明
 * @
 * @Copyright (c) 2025 by Hit, All Rights Reserved.
 */

#ifndef HUFF
#define HUFF
#include <map>
#include <string>
#include <vector>

namespace Huffman
{
using namespace std;
using ushort = unsigned short;

/***
 * @description: 记录文件中字符与对应的出现频数的编码映射
 */
struct c_wMap {
  private:
    vector<char> c;
    vector<ushort> w;

  public:
    c_wMap(char *fileName, char flag);
    c_wMap(ifstream &in);
    const int returnSize() const { return c.size(); }
    const char &c_ele(int i) const { return c[i]; }
    const ushort &w_ele(int i) const { return w[i]; }
    friend void countToc_wMap(c_wMap &cw, char *fileName);
    friend void readToc_wMap(c_wMap &cw, char *fileName);
};

/***
 * @description: 哈夫曼树节点
 */
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

/***
 * @description: 哈夫曼树
 */
class HuffmanTree
{
  public:
    HuffmanTree(const c_wMap cw);                  // 构造函数
    const HuffmanNode &returnNode(int index) const // 选择器:返回index号元素
    {
        return root[index];
    }
    int returnSize() const { return root.size(); } // 选择器:返回叶子个数

  private:
    vector<HuffmanNode> root; // 使用vector作为叶子的容器
};

/***
 * @description: 对哈夫曼树进行编码,获得每个字符对应的哈弗曼编码
 * @param {HuffmanTree} &Tree
 * @return {map<char,string>}
 */
map<char, string> encodeHuffmanTree(const HuffmanTree &Tree);

/***
 * @description: 利用字符编码表,对文件进行哈夫曼压缩
 * @return {*}
 */
void compress(char *inFileName, char *outFileName,
              const map<char, string> &encode, const c_wMap &cw);

/***
 * @description: 对压缩后文件解压
 * @param {char} *inFileName:压缩文件路径
 * @param {char} *outFileName:解压后文件放置路径
 * @return {*}
 */
void decompress(char *inFileName, char *outFileName);

} // namespace Huffman
#endif