#include "Bitmap.h"
#include "DWT.h"
#include "lib/getopt_pp.h"

#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;
using namespace GetOpt;

int show_help()
{
	cout	<< "compression using Discrete Wavelet Transform, Huffman+RLE encoding"
			<< endl << endl
			<< "Usage: ./a.out [-b <bpp>] {-c|-d} input output" << endl
			<< "   -b --bpp          Bits per uncompressed pixels (defaults to 10)" << endl
			<< "   -c --compress     Compress Bitmap input into DWT output" << endl
			<< "   -h --help         Show this help message" << endl;
	return 0;
}

int main(int argc, char **argv)
{
	GetOpt_pp opt(argc, argv);
	bool compress = false, decompress = false;
	unsigned int bpp = 8;
	opt >> Option('b', "bpp", bpp);
	opt >> OptionPresent('c', "compress", compress);
        opt >> OptionPresent('d', "decompress", decompress);
	if (argc < 3 || opt >> OptionPresent('h', "help") || (!compress && !decompress))
		return show_help();
	string input(argv[argc-2]), output(argv[argc-1]);

	if (bpp > 26 || bpp < 8) {
		cerr << "Invalid quantization value" << endl;
		return 1;
	}
        
	if (compress) {
		Bitmap *src = new Bitmap();
		src->readImage(input);
		DWT *dwt = new DWT(src, bpp);
		dwt->transform();
		dwt->save(output);
		delete src;
		delete dwt;
	} else if (decompress) {
		DWT *dwt = new DWT(bpp);
		dwt->load(input);
		dwt->untrasform();
		Bitmap *out = dwt->toBitmap();
		out->writeImage(output);
		delete dwt;
		delete out;}
        /*
        if (compress) {
		Bitmap *src = new Bitmap();
		src->readImage(input);
		DWT *dwt = new DWT(src, bpp);
		dwt->transform();
		dwt->save(output);
		delete src;
		delete dwt;
		DWT *dwt1 = new DWT(bpp);
		dwt1->load(output);
		dwt1->untrasform();
		Bitmap *out = dwt1->toBitmap();
		out->writeImage(output);
		delete dwt;
		delete out;
	}
        */
	return 0;
}
