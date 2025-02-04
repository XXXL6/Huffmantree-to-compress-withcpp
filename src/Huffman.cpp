#include "Huffman.hpp"

void Huffman::initHuffman(const vector<char> &c, const vector<ushort> &w)
{
    for (int i = 0; i < c.size(); ++i)
    {
        HuffmanNode a(-1, -1, -1, c[i], w[i]);
        this->huffmantree.push_back(a);
    }
}
void Huffman::searchMin(int &min1, int &min2) const
{
    for (int i = 0; i < huffmantree.size(); ++i)
    {
        if (huffmantree[i].parent == -1)
        {
            min1 = i;
            break;
        }
    }
    for (int i = 0; i < huffmantree.size(); ++i)
    {
        if (huffmantree[i].parent == -1 && huffmantree[i].weight < huffmantree[min1].weight)
        {
            min1 = i;
        }
    }
    for (int i = 0; i < huffmantree.size(); ++i)
    {
        if (huffmantree[i].parent == -1 && i != min1)
        {
            min2 = i;
            break;
        }
    }
    for (int i = 0; i < huffmantree.size(); ++i)
    {
        if (huffmantree[i].parent == -1 && huffmantree[i].weight < huffmantree[min2].weight && i != min1)
        {
            min2 = i;
        }
    }
}
void Huffman::createHuffman(const vector<char> &c, const vector<ushort> &w)
{
    initHuffman(c, w);
    int minPos1{};
    int minPos2{};
    for (int i = 0; i < c.size() - 1; ++i)
    {
        searchMin(minPos1, minPos2);
        HuffmanNode newnode(minPos1, minPos2, -1, '\0', huffmantree[minPos1].weight + huffmantree[minPos2].weight);
        huffmantree.push_back(newnode);
        huffmantree[minPos1].parent = huffmantree.size() - 1;
        huffmantree[minPos2].parent = huffmantree.size() - 1;
    }
}

void Huffman::dfs(const HuffmanNode &node, map<char, string> &encode)
{
    if(node.left==-1&&node.right==-1)
    {
        encode[node.c] = path;
        return;
    }
    if(node.left!=-1)
    {
        path.push_back('0');
        dfs(huffmantree[node.left], encode);
        path.pop_back();
    }
    if (node.right != -1)
    {
        path.push_back('1');
        dfs(huffmantree[node.right], encode);
        path.pop_back();
    }
}