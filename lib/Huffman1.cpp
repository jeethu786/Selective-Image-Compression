// Copyright (C) 2010 Alessandro Bruni
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
// 02110-1301, USA.

#include "bitfile.cpp"
#include <queue>
#include <vector>

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
