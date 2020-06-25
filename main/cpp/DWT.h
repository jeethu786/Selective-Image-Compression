//
// Created by jeethender on 17-06-2020.
//

#ifndef IMAGE_COMPRESSOR_DWT_H
#define IMAGE_COMPRESSOR_DWT_H


#include "Bitmap.h"

#include <string>
using namespace std;

class DWT {
private:
    static const char magic[2];

    // Matrix of coefficients
    float *coeff;

    // Quantization coefficient
    unsigned int bpp;

    // Padded width and height
    unsigned int width;
    unsigned int height;

    // Real width and height of the image
    unsigned int realWidth;
    unsigned int realHeight;

    unsigned char nrm(int val);
    unsigned int range(float val);

    void transform1d(float *src, unsigned int length, unsigned int step, float *tmp);
    void untransform1d(float *src, unsigned int length, unsigned int step, float *tmp);
public:
    DWT(unsigned int bpp);
    DWT(Bitmap *input, unsigned int bpp);
    ~DWT();

    void transform();
    void untrasform();
    Bitmap *toBitmap();
    void save(const string &fileName);
    void load(const string &fileName);
};

#endif //IMAGE_COMPRESSOR_DWT_H
