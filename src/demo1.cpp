#include "Huffman.h"
using namespace Huffman;
// test 压缩功能测试
int main()
{
    const c_wMap count("..//data//inputFile", 'c');
    HuffmanTree huffman(count);
    map<char, string> encode = encodeHuffmanTree(huffman);
    compress("..//data//inputFile", "..//data//outputFile", encode, count);
    return 0;
}