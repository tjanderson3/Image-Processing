#ifndef IMAGEPROCESSING_HEADER_H
#define IMAGEPROCESSING_HEADER_H
struct Header{
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};


#endif //IMAGEPROCESSING_HEADER_H
