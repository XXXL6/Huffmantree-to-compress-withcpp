#include "Huffman.h"
using namespace Huffman;
int main()
{
    const c_wMap count("..//data//inputFile", 'c');
    HuffmanTree huffman(count);
    map<char, string> encode = encodeHuffmanTree(huffman);
    compress("..//data//inputFile", "..//data//outputFile", encode, count);
    return 0;
}