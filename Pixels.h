#ifndef IMAGEPROCESSING_PIXELS_H
#define IMAGEPROCESSING_PIXELS_H
class Pixels{
private:
    unsigned char blue, green, red;
public:
    Pixels(){ //constructors
        blue = 0;
        green = 0;
        red = 0;
    }
    Pixels(unsigned char b, unsigned char g, unsigned char r){
        blue = b;
        green = g;
        red = r;
    }
    //getters
    unsigned char& Blue(){
        return blue;
    }
    unsigned char& Green(){
        return green;
    }
    unsigned char& Red(){
        return red;
    }
    void addRed(int amount){
        if(red + amount > 255){
            red = 255;
        }
        else{
            red += amount;
        }
    }
    void addGreen(int amount){
        if(green + amount > 255){
            green = 255;
        }
        else{
            green += amount;
        }
    }
    void addBlue(int amount){
        if(blue + amount > 255){
            blue = 255;
        }
        else{
            blue += amount;
        }
    }
};


#endif //IMAGEPROCESSING_PIXELS_H
