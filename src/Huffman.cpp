#include "Huffman.h"
#include <fstream>
#include <iostream>
using namespace Huffman;
void Huffman::countToc_wMap(c_wMap &cw, char *fileName)
{

    std::ifstream in;
    in.open(fileName, std::ios::in);
    char p;
    p = in.get();
    cw.c.push_back(p);
    cw.w.push_back(1);
    int i = 0;
    while ((p = in.get()) != EOF) {
        for (i = 0; i < cw.c.size(); ++i) {
            if (p == cw.c[i]) {
                cw.w[i] += 1;
                break;
            }
        }
        if (i == cw.c.size()) {
            cw.c.push_back(p);
            cw.w.push_back(1);
        }
    }
    return;
}

void Huffman::readToc_wMap(c_wMap &cw, char *fileName)
{
    ifstream in;
    in.open(fileName, ios::binary | ios::in);
    char p;
    ushort weight;
    in.read(reinterpret_cast<char *>(&p), sizeof(char));
    // 读取压缩文件头部的哈希表c-w,找回原文件的字符及对应频数
    while (p != '\a') {
        cw.c.push_back(p);
        in.read(reinterpret_cast<char *>(&weight), sizeof(ushort));
        cw.w.push_back(weight);
        in.read(reinterpret_cast<char *>(&p), sizeof(char));
    }
}
c_wMap::c_wMap(char *fileName, char flag)
{
    if (flag == 'c')
        countToc_wMap(*this, fileName);
    else if (flag == 'w')
        readToc_wMap(*this, fileName);
    else {
        cout << "c_wMap construct error !!" << endl;
        exit(1);
    }
}

c_wMap::c_wMap(ifstream &in)
{
    char p;
    ushort weight;
    in.read(reinterpret_cast<char *>(&p), sizeof(char));
    // 读取压缩文件头部的哈希表c-w,找回原文件的字符及对应频数
    while (p != '\a') {
        c.push_back(p);
        in.read(reinterpret_cast<char *>(&weight), sizeof(ushort));
        w.push_back(weight);
        in.read(reinterpret_cast<char *>(&p), sizeof(char));
    }
}

HuffmanTree::HuffmanTree(const c_wMap cw)
{
    for (int i = 0; i < cw.returnSize(); ++i) {
        HuffmanNode a(-1, -1, -1, cw.c_ele(i), cw.w_ele(i));
        root.push_back(a);
    }
    int minPos1{};
    int minPos2{};
    auto searchMin = [&minPos1, &minPos2, this]() -> void {
        for (int i = 0; i < root.size(); ++i) {
            if (root[i].parent == -1) {
                minPos1 = i;
                break;
            }
        }
        for (int i = 0; i < root.size(); ++i) {
            if (root[i].parent == -1 && root[i].weight < root[minPos1].weight) {
                minPos1 = i;
            }
        }
        for (int i = 0; i < root.size(); ++i) {
            if (root[i].parent == -1 && i != minPos1) {
                minPos2 = i;
                break;
            }
        }
        for (int i = 0; i < root.size(); ++i) {
            if (root[i].parent == -1 && root[i].weight < root[minPos2].weight &&
                i != minPos1) {
                minPos2 = i;
            }
        }
    };
    for (int i = 0; i < cw.returnSize() - 1; ++i) {
        searchMin();
        HuffmanNode newnode(minPos1, minPos2, -1, '\0',
                            root[minPos1].weight + root[minPos2].weight);
        root.push_back(newnode);
        root[minPos1].parent = root.size() - 1;
        root[minPos2].parent = root.size() - 1;
    }
}

map<char, string> Huffman::encodeHuffmanTree(const HuffmanTree &Tree)
{
    map<char, string> encode;
    string path;
    auto dfs = [&encode, &path, &Tree](auto &&dfs,
                                       const HuffmanNode &node) -> void {
        if (node.left == -1 && node.right == -1) {
            encode[node.c] = path;
            return;
        }
        if (node.left != -1) {
            path.push_back('0');
            dfs(dfs, Tree.returnNode(node.left));
            path.pop_back();
        }
        if (node.right != -1) {
            path.push_back('1');
            dfs(dfs, Tree.returnNode(node.right));
            path.pop_back();
        }
    };
    dfs(dfs, Tree.returnNode(Tree.returnSize() - 1));
    return encode;
}

void writeMetaData(char *fileName, const map<char, string> &encode,
                   const c_wMap &cw)
{
    // ifstream in;
    ofstream out;
    out.open(fileName, ios::binary | ios::out);
    // in.open(fileName, ios::in);
    //  在压缩后文件中的头部记录下哈希表c-w
    for (int i = 0; i < cw.returnSize(); ++i) {
        out.write(reinterpret_cast<const char *>(&(cw.c_ele(i))), sizeof(char));
        out.write(reinterpret_cast<const char *>(&(cw.w_ele(i))),
                  sizeof(ushort));
    }
    const char ccc = '\a';
    out.write(reinterpret_cast<const char *>(&(ccc)), sizeof(char));
    out.close();
}
void Huffman::compress(char *inFileName, char *outFileName,
                       const map<char, string> &encode, const c_wMap &cw)
{
    writeMetaData(outFileName, encode, cw);
    ifstream in;
    ofstream out;
    out.open(outFileName, ios::binary | ios::out | ios::app);
    in.open(inFileName, ios::in);
    unsigned char num = '\0';
    char p;
    string s;
    string com;
    // 借助哈夫曼编码表encode,开始压缩
    while ((p = in.get()) != EOF) {
        s = s + encode.find(p)->second;
        while (s.size() > 8) {
            com = s.substr(0, 8);
            for (int i = 0; i < 8; ++i) {
                if (com[i] == '1') {
                    num = (num << 1) | 1;
                } else {
                    num = num << 1;
                }
            }
            out.write(reinterpret_cast<char *>(&num), sizeof(num));
            num = '\0';
            s = s.substr(8, s.size() - 8);
        }
    }
    // 文件末处理,不足8位补足8位后再写入
    for (int i = 0; i < 8 - s.size(); ++i) {
        s = s + '1';
    }
    for (int i = 0; i < 8; ++i) {
        if (s[i] == '1') {
            num = (num << 1) | 1;
        } else {
            num = num << 1;
        }
    }
    out.write(reinterpret_cast<char *>(&num), sizeof(num));
    in.close();
    out.close();
    return;
}

void Huffman::decompress(char *inFileName, char *outFileName)
{
    ifstream in;
    ofstream out;
    in.open(inFileName, ios::binary | ios::in);
    out.open(outFileName, ios::out);
    // char p;
    // ushort weight;
    c_wMap cw(in);
    // in.read(reinterpret_cast<char *>(&p), sizeof(char));
    // // 读取压缩文件头部的哈希表c-w,找回原文件的字符及对应频数
    // while (p != '\a') {
    //     c.push_back(p);
    //     in.read(reinterpret_cast<char *>(&weight), sizeof(ushort));
    //     w.push_back(weight);
    //     in.read(reinterpret_cast<char *>(&p), sizeof(char));
    // }
    HuffmanTree a(
        cw); // 利用c-w表信息建立相同的哈夫曼树，以建立相同的哈夫曼编码表，实现编码还原
    unsigned char num = '\0';
    int root = a.returnSize() - 1;
    // 读取压缩文件中的bit流，利用哈夫曼编码的非前缀特性，还原字符，实现解压缩
    while (true) {
        if (!in.read(reinterpret_cast<char *>(&num), sizeof(unsigned char))) {
            break;
        };
        for (int i = 0; i < 8; ++i) {
            if (num & (128 >> i)) {
                root = a.returnNode(root).right;
            } else {
                root = a.returnNode(root).left;
            }
            if (a.returnNode(root).left == -1 &&
                a.returnNode(root).right == -1) {
                out << a.returnNode(root).c;
                root = a.returnSize() - 1;
            }
        }
    }
    in.close();
    out.close();
    return;
}