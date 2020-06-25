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

#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include "bitfile.h"

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

#endif
