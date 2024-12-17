#ifndef IMAGEPROCESSING_IMAGEDATA_H
#define IMAGEPROCESSING_IMAGEDATA_H
#include "Header.h"
#include "Pixels.h"
#include <vector>
using namespace std;
class imageData{
public:
    vector<Pixels> image_dat;
    Header image_head;
    imageData(){}
    imageData(Header& h, vector<Pixels>& p){
        image_dat = p;
        image_head = h;
    }
};

#endif //IMAGEPROCESSING_IMAGEDATA_H
