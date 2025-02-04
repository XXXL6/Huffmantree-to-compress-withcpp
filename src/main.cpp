/**
 * @file main.cpp
 * @author HIT-计算学部-2024秋数据结构与算法lab
 * @brief 利用Huffmantree实现文本文件的压缩与解压缩
 * @version 0.1
 * @date 2024-11-04
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <fstream>
#include "Huffman.hpp"

void count(vector<char> &c, vector<ushort> &w);                                     // 遍历待压缩文件中的字符并统计每个字符对应频数，建立哈希表c-w
void compress(const map<char, string> &encode, vector<char> &c, vector<ushort> &w); // 实现压缩功能
void decompress(vector<char> &c, vector<ushort> &w);                                // 实现解压缩

/**
 * @brief: 遍历待压缩文件中的字符并统计每个字符对应频数，建立哈希表c-w
 *
 * @param: c记录出现字符
 * @param: w记录对应字符的频数
 */
void count(vector<char> &c, vector<ushort> &w)
{
    ifstream in;
    in.open("../data/inputFile", ios::in);
    char p;
    p = in.get();
    c.push_back(p);
    w.push_back(1);
    int i = 0;
    while ((p = in.get()) != EOF)
    {
        for (i = 0; i < c.size(); ++i)
        {
            if (p == c[i])
            {
                w[i] += 1;
                break;
            }
        }
        if (i == c.size())
        {
            c.push_back(p);
            w.push_back(1);
        }
    }
    return;
}

/**
 * @brief: 实现压缩功能
 *
 * @param: encode 利用哈夫曼树建立的哈夫曼编码表
 * @param: c 出现字符
 * @param: w 对应字符的频数
 */
void compress(const map<char, string> &encode, vector<char> &c, vector<ushort> &w)
{
    ifstream in;
    ofstream out;
    unsigned char num = '\0';
    char p;
    string s;
    string com;
    out.open("../data/outputFile", ios::binary | ios::out);
    in.open("../data/inputFile", ios::in);
    // 在压缩后文件中的头部记录下哈希表c-w
    for (int i = 0; i < c.size(); ++i)
    {
        out.write(reinterpret_cast<char *>(&(c[i])), sizeof(char));
        out.write(reinterpret_cast<char *>(&w[i]), sizeof(ushort));
    }
    char ccc = '\a';
    out.write(reinterpret_cast<char *>(&(ccc)), sizeof(char));
    // 借助哈夫曼编码表encode,开始压缩
    while ((p = in.get()) != EOF)
    {
        s = s + encode.find(p)->second;
        while (s.size() > 8)
        {
            com = s.substr(0, 8);
            for (int i = 0; i < 8; ++i)
            {
                if (com[i] == '1')
                {
                    num = (num << 1) | 1;
                }
                else
                {
                    num = num << 1;
                }
            }
            out.write(reinterpret_cast<char *>(&num), sizeof(num));
            num = '\0';
            s = s.substr(8, s.size() - 8);
        }
    }
    // 文件末处理,不足8位补足8位后再写入
    for (int i = 0; i < 8 - s.size(); ++i)
    {
        s = s + '1';
    }
    for (int i = 0; i < 8; ++i)
    {
        if (s[i] == '1')
        {
            num = (num << 1) | 1;
        }
        else
        {
            num = num << 1;
        }
    }
    out.write(reinterpret_cast<char *>(&num), sizeof(num));
    in.close();
    out.close();
    return;
}

/**
 * @brief: 实现解压缩
 *
 * @param: c 出现字符
 * @param: w 对应字符的频数
 */
void decompress(vector<char> &c, vector<ushort> &w)
{
    ifstream in;
    ofstream out;
    in.open("../data/outputFile", ios::binary | ios::in);
    out.open("../data/inputFile2", ios::out);
    char p;
    ushort weight;
    in.read(reinterpret_cast<char *>(&p), sizeof(char));
    // 读取压缩文件头部的哈希表c-w,找回原文件的字符及对应频数
    while (p != '\a')
    {
        c.push_back(p);
        in.read(reinterpret_cast<char *>(&weight), sizeof(ushort));
        w.push_back(weight);
        in.read(reinterpret_cast<char *>(&p), sizeof(char));
    }
    Huffman a;
    a.createHuffman(c, w); // 利用c-w表信息建立相同的哈夫曼树，以建立相同的哈夫曼编码表，实现编码还原
    unsigned char num = '\0';
    int root = a.huffmantree.size() - 1;
    // 读取压缩文件中的bit流，利用哈夫曼编码的非前缀特性，还原字符，实现解压缩
    while (true)
    {
        if (!in.read(reinterpret_cast<char *>(&num), sizeof(unsigned char)))
        {
            break;
        };
        for (int i = 0; i < 8; ++i)
        {
            if (num & (128 >> i))
            {
                root = a.huffmantree[root].right;
            }
            else
            {
                root = a.huffmantree[root].left;
            }
            if (a.huffmantree[root].left == -1 && a.huffmantree[root].right == -1)
            {
                out << a.huffmantree[root].c;
                root = a.huffmantree.size() - 1;
            }
        }
    }
    in.close();
    out.close();
    return;
}

/**
 * @brief: 测试压缩功能，构建哈夫曼编码表并输出->压缩->计算压缩率
 *
 * @return: int 测试
 */
int main()
{
    // 统计
    vector<char> c;
    vector<ushort> w;
    count(c, w);
    // 构建哈夫曼树
    Huffman a;
    a.createHuffman(c, w);
    // 建立哈弗曼编码表
    map<char, string> mp;
    a.searchHuffman(mp);

    // 输出构建的哈夫曼表
    for (auto it = mp.begin(); it != mp.end(); it++)
    {
        cout << it->first;
        cout << " ";
        cout << it->second;
        cout << endl;
    }

    // 压缩
    compress(mp, c, w);

    // 计算压缩率
    ifstream in1("../data/outputFile");
    ifstream in2("../data/inputFile");
    in1.seekg(0, ios::end);
    in2.seekg(0, ios::end);
    double size1 = in1.tellg();
    double size2 = in2.tellg();
    cout << "Compression_ratio is: " << size1 / size2 * 100.0 << '%' << endl;
    return 0;
}

/**
 * @brief: 测试解压缩
 *
 * @return: int 测试
 */
// int main()
// {
//     vector<char> c;
//     vector<ushort> w;
//     decompress(c, w);
//     // 输出统计的字符及对应频数，比对检验
//     for (int i = 0; i < c.size(); ++i)
//     {
//         cout << c[i] << ' ' << w[i] << endl;
//     }
//     return 0;
// }
