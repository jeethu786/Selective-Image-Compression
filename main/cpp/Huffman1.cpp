#include "Huffman1.h"
#include "bitfile.cpp"
#include <queue>
#include <vector>

Huffman1::Node::Node(unsigned char t, unsigned int occ) : root(0), inner(t), occ(occ) {}

Huffman1::Node::~Node() {}

Huffman1::InnerNode::InnerNode(Huffman1::Node *left, Huffman1::Node *right) : Huffman1::Node(1, left->occ + right->occ) {
    children[0] = left;
    children[1] = right;
    left->root = this;
    right->root = this;
}

Huffman1::InnerNode::~InnerNode() {
    delete children[0];
    delete children[1];
}

Huffman1::Leaf::Leaf(unsigned int occ, unsigned short info) : Node(0, occ), info(info) {}

Huffman1::Leaf::~Leaf() {}

Huffman1::Huffman1(unsigned int bpp) : bitFile(0), tree(0), bpp(bpp) {}

Huffman1::~Huffman1()
{
    delete tree;
}

void Huffman1::buildTree(unsigned int *occurrences, int n)
{
    priority_queue<Node *, vector<Node *>, gt> q;
    for (int i = 0; i < n; i++) {
        if (occurrences[i] > 0) {
            Leaf *n = new Leaf(occurrences[i], i);
            leafs[i] = n;
            q.push(n);
        }
    }
    while (!q.empty()) {
        Node *n1 = q.top();
        q.pop();
        if (q.empty()) {
            tree = n1;
        } else {
            Node *n2 = q.top();
            q.pop();
            q.push(new InnerNode(n1, n2));
        }
    }
}

void Huffman1::setFile(bit_file_t *file)
{
    bitFile = file;
}

void Huffman1::writeNode(Huffman1::Node *n)
{
    BitFilePutBit(n->inner, bitFile);
    if (n->inner) {
        writeNode(((InnerNode*)n)->children[0]);
        writeNode(((InnerNode*)n)->children[1]);
    } else {
        BitFilePutBitsInt(bitFile, &((Leaf*)n)->info, bpp, sizeof(unsigned short));
    }
}

void Huffman1::writeTree()
{
    writeNode(tree);
}

Huffman1::Node *Huffman1::readNode(bool mapLeafs)
{
    unsigned char t = BitFileGetBit(bitFile);
    if (t == 1) {
        Node *l = readNode(mapLeafs);
        Node *r = readNode(mapLeafs);
        return new InnerNode(l, r);
    } else {
        unsigned short info = 0;
        BitFileGetBitsInt(bitFile, &info, bpp, sizeof(unsigned short));
        Leaf *n = new Leaf(0, info);
        if (mapLeafs) leafs[info] = n;
        return n;
    }
}

void Huffman1::readTree(bool mapLeafs)
{
    tree = readNode(mapLeafs);
}

void Huffman1::readTree()
{
    tree = readNode(false);
}

void Huffman1::writeSymRec(Huffman1::Node *n)
{
    if (n == tree) return;
    if (n->root != tree) writeSymRec(n->root);
    BitFilePutBit(((InnerNode*)n->root)->children[1]==n, bitFile);
}

void Huffman1::writeSymbol(unsigned short sym)
{
    writeSymRec(leafs[sym]);
}

unsigned short Huffman1::readSymRec(Huffman1::Node *n)
{
    if (!n->inner)
        return ((Leaf*)n)->info;
    int child = BitFileGetBit(bitFile)?1:0;
    return readSymRec(((InnerNode*)n)->children[child]);
}

unsigned short Huffman1::readSymbol()
{
    return readSymRec(tree);
}
