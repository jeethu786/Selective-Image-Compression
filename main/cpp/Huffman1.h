//
// Created by jeethender on 17-06-2020.
//

#ifndef IMAGE_COMPRESSOR_HUFFMAN1_H
#define IMAGE_COMPRESSOR_HUFFMAN1_H


#include "bitfile.h"
#include <iostream>
#include <map>
using namespace std;

/**
  * Huffman encoder/decoder
  */
class Huffman1 {
private:
    /**
      * Structures used to represent the Huffman tree
      */
    class Node {
    public:
        Node *root;
        unsigned char inner;
        unsigned int occ;
        Node(unsigned char t, unsigned int occ);
        ~Node();
    };
    struct gt
    {
        bool operator()(Node *a, Node *b) const
        {
            return (*a).occ > (*b).occ;
        }
    };

    class InnerNode : public Node {
    public:
        InnerNode(Node *left, Node *right);
        ~InnerNode();
        Node *children[2];
    };

    class Leaf : public Node {
    public:
        Leaf(unsigned int occ, unsigned short info);
        ~Leaf();
        unsigned short info;
    };

    // Local fields
    bit_file_t *bitFile;
    Node *tree;
    map<unsigned short, Leaf *>leafs;
    unsigned int bpp;

    void writeNode(Node *n);
    Node *readNode(bool mapLeafs);
    void writeSymRec(Node *n);
    unsigned short readSymRec(Node *n);
public:
    Huffman1(unsigned int bpp);
    ~Huffman1();

    void setFile(bit_file_t *file);

    // Given an array of occurrences of samplings, builds the optimal encoding tree
    void buildTree(unsigned int *occurrences, int n);

    // Writes the generated tree to the bitfile
    void writeTree();

    // Reads a tree from the bitfile
    void readTree(bool mapLeafs);
    void readTree();

    // Encodes a symbol and writes it to the bitfile
    void writeSymbol(unsigned short sym);

    // Reads and decodes a symbol from the bitfile
    unsigned short readSymbol();
};



#endif //IMAGE_COMPRESSOR_HUFFMAN1_H
