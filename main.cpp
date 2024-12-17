#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "ImageData.h"
imageData loadTGA(const string& filename){
    Header file_header;
    vector<Pixels> file_contents;
    ifstream file(filename, ios_base::binary);
    file.read(&file_header.idLength, sizeof(file_header.idLength));
    file.read(&file_header.colorMapType, sizeof(file_header.colorMapType));
    file.read(&file_header.dataTypeCode, sizeof(file_header.dataTypeCode));
    file.read(reinterpret_cast<char*>(&file_header.colorMapOrigin), sizeof(file_header.colorMapOrigin));
    file.read(reinterpret_cast<char*>(&file_header.colorMapLength), sizeof(file_header.colorMapLength));
    file.read(&file_header.colorMapDepth, sizeof(file_header.colorMapDepth));
    file.read(reinterpret_cast<char*>(&file_header.xOrigin), sizeof(file_header.xOrigin));
    file.read(reinterpret_cast<char*>(&file_header.yOrigin), sizeof(file_header.yOrigin));
    file.read(reinterpret_cast<char*>(&file_header.width), sizeof(file_header.width));
    file.read(reinterpret_cast<char*>(&file_header.height), sizeof(file_header.height));
    file.read(&file_header.bitsPerPixel, sizeof(file_header.bitsPerPixel));
    file.read(&file_header.imageDescriptor, sizeof(file_header.imageDescriptor));
    unsigned int pCount = file_header.height * file_header.width;
    for(unsigned int i = 0; i < pCount; i++){
        unsigned char blue;
        unsigned char green;
        unsigned char red;
        file.read(reinterpret_cast<char*>(&blue), sizeof(unsigned char));
        file.read(reinterpret_cast<char*>(&green), sizeof(unsigned char));
        file.read(reinterpret_cast<char*>(&red), sizeof(unsigned char));
        Pixels newPixel(blue, green, red);
        file_contents.push_back(newPixel);
    }
    file.close();
    imageData loadedFile(file_header,file_contents);
    return loadedFile;
}

void writeFile(const string& filename, imageData fileToWrite){
    ofstream file(filename, ios_base::binary);
    file.write(&fileToWrite.image_head.idLength, 1);
    file.write(&fileToWrite.image_head.colorMapType, 1);
    file.write(&fileToWrite.image_head.dataTypeCode, 1);
    file.write(reinterpret_cast<char*>(&fileToWrite.image_head.colorMapOrigin), 2);
    file.write(reinterpret_cast<char*>(&fileToWrite.image_head.colorMapLength), 2);
    file.write(&fileToWrite.image_head.colorMapDepth, 1);
    file.write(reinterpret_cast<char*>(&fileToWrite.image_head.xOrigin), 2);
    file.write(reinterpret_cast<char*>(&fileToWrite.image_head.yOrigin), 2);
    file.write(reinterpret_cast<char*>(&fileToWrite.image_head.width), 2);
    file.write(reinterpret_cast<char*>(&fileToWrite.image_head.height), 2);
    file.write(&fileToWrite.image_head.bitsPerPixel, 1);
    file.write(&fileToWrite.image_head.imageDescriptor, 1);
    for(auto & i : fileToWrite.image_dat){
        file.write(reinterpret_cast<char*>(&i.Blue()), 1);
        file.write(reinterpret_cast<char*>(&i.Green()), 1);
        file.write(reinterpret_cast<char*>(&i.Red()), 1);
    }
    file.close();
}

void AddGreen(vector<Pixels>& A, int amountToAdd){
    for(auto & i : A){
        i.addGreen(amountToAdd);
    }
}
void AddBlue(vector<Pixels>& A, int amountToAdd){
    for(auto & i : A){
        i.addBlue(amountToAdd);
    }
}
void AddRed(vector<Pixels>& A, int amountToAdd){
    for(auto & i : A){
        i.addRed(amountToAdd);
    }
}


void Subtract(vector<Pixels>& vec1, vector<Pixels>& vec2, vector<Pixels>& newImageData){
    for(unsigned int i = 0; i < vec1.size(); i++){
        unsigned char red, blue, green;
        if(vec2.at(i).Green() - vec1.at(i).Green() < 0){
            green = 0;
        }
        else{
            green = vec2.at(i).Green() - vec1.at(i).Green();
        }
        if(vec2.at(i).Blue() - vec1.at(i).Blue() < 0){
            blue = 0;
        }
        else{
            blue = vec2.at(i).Blue() - vec1.at(i).Blue();
        }
        if(vec2.at(i).Red() - vec1.at(i).Red() < 0){
            red = 0;
        }
        else{
            red = vec2.at(i).Red() - vec1.at(i).Red();
        }
        Pixels newValues(blue, green, red);
        newImageData.push_back(newValues);
    }
}
void Multiply(vector<Pixels> image1, vector<Pixels> image2, vector<Pixels>& multImage){
    for(unsigned int i = 0; i < image1.size(); i++){
        float red = (((float)image1.at(i).Red() * (float)image2.at(i).Red()) / 255) +0.5f;
        float green = (((float)image1.at(i).Green() * (float)image2.at(i).Green()) / 255) +0.5f;
        float blue = (((float)image1.at(i).Blue() * (float)image2.at(i).Blue()) / 255) +0.5f;
        Pixels newPix(blue, green, red);
        multImage.push_back(newPix);
    }
}

void Screen(vector<Pixels> image1, vector<Pixels> image2, vector<Pixels>& screenIm){
    vector<Pixels> im1temp;
    vector<Pixels> im2temp;
    vector<Pixels> imtemp3;
    for(unsigned int i = 0; i < image1.size(); i++){
        unsigned char red1 = 255 -image1.at(i).Red();
        unsigned char blue1 = 255 - image1.at(i).Blue();
        unsigned char green1 = 255 - image1.at(i).Green();
        unsigned char red2 = 255 - image2.at(i).Red();
        unsigned char blue2 = 255 - image2.at(i).Blue();
        unsigned char green2 = 255 - image2.at(i).Green();
        Pixels pix1(green1,blue1,red1);
        Pixels pix2(green2,blue2,red2);
        im1temp.push_back(pix1);
        im2temp.push_back(pix2);
    }
    Multiply(im1temp, im2temp, imtemp3);
    for(auto & i : imtemp3){
        unsigned char red = 255 - i.Red();
        unsigned char blue = 255 - i.Blue();
        unsigned char green = 255 - i.Green();
        Pixels pix(green,blue,red);
        screenIm.push_back(pix);
    }
}

void Overlay(vector<Pixels> im1, vector<Pixels> im2, vector<Pixels>& result){
    for(unsigned int i = 0; i < im1.size(); i++){
        unsigned char r, b, g;
        if((float)im2.at(i).Red()/255 <= 0.5) {
            r = 2 * (float) im1.at(i).Red() * (float) im2.at(i).Red() / 255 + 0.5f;
        }
        else{
            r = 255 - 2 * (255 - (float) im1.at(i).Red()) * (255 - (float) im2.at(i).Red()) / 255 + 0.5f;
        }
        if((float)im2.at(i).Blue()/255 <= 0.5) {
            b = 2 * (float) im1.at(i).Blue() * (float) im2.at(i).Blue() / 255 + 0.5f;
        }
        else{
            b = 255 - 2 * (255 - (float) im1.at(i).Blue()) * (255 - (float) im2.at(i).Blue()) / 255 + 0.5f;
        }
        if((float)im2.at(i).Green()/255 <= 0.5) {
            g = 2 * (float) im1.at(i).Green() * (float) im2.at(i).Green() / 255 + 0.5f;
        }
        else{
            g = 255 - 2 * (255 - (float) im1.at(i).Green()) * (255 - (float) im2.at(i).Green()) / 255 + 0.5f;
        }
        Pixels pix(b,g,r);
        result.push_back(pix);
    }
}
void Combine(vector<Pixels> R, vector<Pixels> B, vector<Pixels> G, vector<Pixels>& result){
    for(unsigned int i = 0; i < R.size(); i++){
        unsigned char r,b,g;
        r = R.at(i).Red();
        b = B.at(i).Red();
        g = G.at(i).Green();
        Pixels pix(b,g,r);
        result.push_back(pix);
    }
}
vector<unsigned char> onlyRed(vector<Pixels>& R){
    vector<unsigned char> redChannel;
    for(auto &i : R){
        redChannel.push_back(i.Red());
    }
    return redChannel;
}
vector<unsigned char> onlyBlue(vector<Pixels>& B){
    vector<unsigned char> blueChannel;
    for(auto &i : B){
        blueChannel.push_back(i.Blue());
    }
    return blueChannel;
}
vector<unsigned char> onlyGreen(vector<Pixels>& G){
    vector<unsigned char> greenChannel;
    for(auto &i : G){
        greenChannel.push_back(i.Green());
    }
    return greenChannel;
}
void Flip(vector<Pixels>& A){
    reverse(A.begin(), A.end());
}
void scaleRed(vector<Pixels>& A, int scaleFactor, vector<Pixels>& result){
    int scaledVal;
    for(auto & i : A){
        unsigned char r, b, g;
        b= i.Blue();
        g = i.Green();
        scaledVal = scaleFactor * i.Red();
        if(scaledVal < 0){
            r = 0;
        }
        else if(scaledVal> 255){
            r = 255;
        }
        else{
            r = (unsigned char)scaledVal;
        }
        Pixels pix(b,g,r);
        result.push_back(pix);
    }
}
void scaleGreen(vector<Pixels>& A, int scaleFactor, vector<Pixels>& result){
    int scaledVal;
    for(auto & i : A){
        unsigned char r, b, g;
        b= i.Blue();
        r = i.Red();
        scaledVal = scaleFactor * i.Green();
        if(scaledVal < 0){
            g = 0;
        }
        else if(scaledVal> 255){
            g = 255;
        }
        else{
            g = (unsigned char)scaledVal;
        }
        Pixels pix(b,g,r);
        result.push_back(pix);
    }
}
void scaleBlue(vector<Pixels>& A, int scaleFactor, vector<Pixels>& result){
    int scaledVal;
    for(auto & i : A){
        unsigned char r, b, g;
        r= i.Red();
        g = i.Green();
        scaledVal = scaleFactor * i.Blue();
        if(scaledVal < 0){
            b = 0;
        }
        else if(scaledVal> 255){
            b = 255;
        }
        else{
            b = (unsigned char)scaledVal;
        }
        Pixels pix(b,g,r);
        result.push_back(pix);
    }
}

void task1(){
    imageData t = loadTGA("input/layer1.tga");
    imageData j = loadTGA("input/pattern1.tga");
    vector<Pixels> a;
    Multiply(t.image_dat, j.image_dat, a);
    imageData a1(t.image_head, a);
    writeFile("output/part1.tga", a1);
}

void task2(){
    imageData b = loadTGA("input/layer2.tga");
    imageData w = loadTGA("input/car.tga");
    vector<Pixels> jwa;
    Subtract(b.image_dat,w.image_dat,jwa);
    imageData c(b.image_head, jwa);
    writeFile("output/part2.tga", c);
}
void task3(){
    imageData l = loadTGA("input/layer1.tga");
    imageData p = loadTGA("input/pattern2.tga");
    imageData t = loadTGA("input/text.tga");
    vector<Pixels> lp;
    Multiply(l.image_dat, p.image_dat, lp);
    vector<Pixels> screenIm;
    Screen(lp, t.image_dat, screenIm);
    imageData whole(t.image_head, screenIm);
    writeFile("output/part3.tga", whole);

}

void task4(){
    imageData l = loadTGA("input/layer2.tga");
    imageData c = loadTGA("input/circles.tga");
    imageData p = loadTGA("input/pattern2.tga");
    vector<Pixels> lc;
    Multiply(l.image_dat,c.image_dat,lc);
    vector<Pixels> final;
    Subtract(p.image_dat,lc, final);
    imageData wholeFin(p.image_head, final);
    writeFile("../output/part4.tga", wholeFin);
}
void task5(){
    imageData l = loadTGA("input/layer1.tga");
    imageData p = loadTGA("input/pattern1.tga");
    vector<Pixels> lp;
    Overlay(l.image_dat, p.image_dat, lp);
    imageData lpIM(l.image_head, lp);
    writeFile("output/part5.tga", lpIM);
}
void task6(){
    imageData c = loadTGA("input/car.tga");
    AddGreen(c.image_dat, 200);
    writeFile("output/part6.tga", c);
}
void task7(){
    imageData c = loadTGA("input/car.tga");
    vector<Pixels> temp;
    scaleRed(c.image_dat, 4,temp);
    vector<Pixels> fin;
    scaleBlue(temp, 0, fin);
    imageData finIM(c.image_head, fin);
    writeFile("output/part7.tga", finIM);

}
void task8(){
    imageData c = loadTGA("input/car.tga");
    vector<Pixels> red;
    vector<Pixels> green;
    vector<Pixels> blue;
    vector<unsigned char>  g, b, r;
    g = onlyGreen(c.image_dat);
    b = onlyBlue(c.image_dat);
    r = onlyRed(c.image_dat);
    for(int i =0; i < c.image_dat.size(); i++){
        Pixels R(r.at(i),r.at(i),r.at(i));
        Pixels B(b.at(i),b.at(i), b.at(i));
        Pixels G(g.at(i),g.at(i),g.at(i));
        red.push_back(R);
        blue.push_back(B);
        green.push_back(G);
    }
    imageData Red(c.image_head,red);
    imageData Green(c.image_head,green);
    imageData Blue(c.image_head,blue);
    writeFile("output/part8_r.tga", Red);
    writeFile("output/part8_g.tga", Green);
    writeFile("output/part8_b.tga", Blue);

}
void task9(){
    imageData R = loadTGA("input/layer_red.tga");
    imageData B = loadTGA("input/layer_blue.tga");
    imageData G = loadTGA("input/layer_green.tga");
    vector<Pixels> comb;
    Combine(R.image_dat,B.image_dat,G.image_dat, comb);
    imageData combination(R.image_head, comb);
    writeFile("output/part9.tga", combination);
}
void task10(){
    imageData flip = loadTGA("input/text2.tga");
    Flip(flip.image_dat);
    writeFile("output/part10.tga",flip);

}
void runTasks(){
    task1();
    task2();
    task3();
    task4();
    task5();
    task6();
    task7();
    task8();
    task9();
    task10();
}
bool FileExists(const string& filename){
    ifstream file(filename);
    return file.good();
}

int main(int argc, char* argv[]) {
    if(argc <=1  || strcmp(argv[1], "--help")==0){
        cout << "Project 2: Image Processing, Spring 2023" << endl << endl;
        cout << "Usage:" << endl;
        cout << "    ./project2.out [output] [firstImage] [method] [...]" << endl;
        return 0;
    }
    if(strcmp(argv[1], "runTasks")==0){
        runTasks();
    }
    string outputImage = argv[1];
    if(argc <=2 || outputImage.substr(outputImage.size()-4)!=".tga"){
        cout << "Invalid file name." << endl;
        return 0;
    }
    string sourceImage = argv[2];
    if(sourceImage.substr(sourceImage.size()-4)!=".tga"){
        cout << "Invalid file name." << endl;
        return 0;
    }
    if(!FileExists("input/"+sourceImage)){
        cout << "File does not exist." << endl;
        return 0;
    }
    imageData sourceData = loadTGA("input/"+sourceImage);
    if(argc < 4){
        cout << "Invalid method name." << endl;
        return 0;
    }
    int i = 3;
    while(i < argc){
        if(strcmp(argv[i],"multiply")==0){
            if(i+1 >= argc){
                cout<< "Missing argument."<< endl;
                return 0;
            }
            string multFile = argv[i+1];
            if(multFile.substr(multFile.size()-4)!=".tga"){
                cout<< "Invalid argument, invalid file name." << endl;
                return 0;
            }
            if(!FileExists("input/"+multFile)){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            imageData multIm = loadTGA("input/"+multFile);
            vector<Pixels> manipIM;
            Multiply(multIm.image_dat, sourceData.image_dat, manipIM);
            sourceData.image_dat = manipIM;
            i += 2;
        }
        else if(strcmp(argv[i], "subtract")==0){
            if(i+1 >= argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string subFile = argv[i+1];
            if(subFile.substr(subFile.size()-4)!=".tga"){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            if(!FileExists("input/"+subFile)){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            imageData subIm = loadTGA("input/"+subFile);
            vector<Pixels> manipIM;
            Subtract(subIm.image_dat, sourceData.image_dat,manipIM);
            sourceData.image_dat=manipIM;
            i += 2;
        }
        else if(strcmp(argv[i], "overlay")==0){
            if(i+1 >= argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string subFile = argv[i+1];
            if(subFile.substr(subFile.size()-4)!=".tga"){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            if(!FileExists("input/"+subFile)){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            imageData subIm = loadTGA("input/"+subFile);
            vector<Pixels> manipIM;
            Subtract(subIm.image_dat, sourceData.image_dat,manipIM);
            sourceData.image_dat=manipIM;
            i += 2;

        }
        else if(strcmp(argv[i], "screen")==0){
            if(i+1 >= argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string subFile = argv[i+1];
            if(subFile.substr(subFile.size()-4)!=".tga"){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            if(!FileExists("input/"+subFile)){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            imageData subIm = loadTGA("input/"+subFile);
            vector<Pixels> manipIM;
            Subtract(subIm.image_dat, sourceData.image_dat,manipIM);
            sourceData.image_dat=manipIM;
            i += 2;

        }
        else if(strcmp(argv[i], "combine")==0){
            if(i+1 >= argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string subFile = argv[i+1];
            if(subFile.substr(subFile.size()-4)!=".tga"){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            if(!FileExists("input/"+subFile)){
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            imageData subIm = loadTGA("input/"+subFile);
            vector<Pixels> manipIM;
            Subtract(subIm.image_dat, sourceData.image_dat,manipIM);
            sourceData.image_dat=manipIM;
            i += 2;

        }
        else if(strcmp(argv[i], "flip")==0){
            Flip(sourceData.image_dat);
            i += 1;
        }
        else if(strcmp(argv[i], "onlyred")==0){
            onlyRed(sourceData.image_dat);
            i += 1;
        }
        else if(strcmp(argv[i], "onlygreen")==0){
            onlyGreen(sourceData.image_dat);
            i+= 1;
        }
        else if(strcmp(argv[i], "onlyblue")==0){
            onlyBlue(sourceData.image_dat);
            i += 1;
        }
        else if(strcmp(argv[i], "addred")==0){
            if(i + 1 >= argc){
                cout << "Missing argument."<< endl;
                return 0;
            }
            try {
                int red = stoi(argv[i+1]);
                AddRed(sourceData.image_dat, red);
                i += 2;
            }
            catch(...){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
        }
        else if(strcmp(argv[i], "addgreen")==0){
            if(i + 1 >= argc){
                cout << "Missing argument."<< endl;
                return 0;
            }
            try {
                int green = stoi(argv[i+1]);
                AddGreen(sourceData.image_dat, green);
                i+= 2;
            }
            catch(...){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
        }
        else if(strcmp(argv[i], "addblue")==0){
            if(i + 1 >= argc){
                cout << "Missing argument."<< endl;
                return 0;
            }
            try {
                int blue = stoi(argv[i+1]);
                AddBlue(sourceData.image_dat, blue);
                i += 2;
            }
            catch(...){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
        }
        else if(strcmp(argv[i], "scalered")==0){
            if(i + 1 >= argc){
                cout << "Missing argument."<< endl;
                return 0;
            }
            try {
                int red = stoi(argv[i+1]);
                vector<Pixels> result;
                scaleRed(sourceData.image_dat, red, result);
                sourceData.image_dat = result;
                i += 2;
            }
            catch(...){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
        }
        else if(strcmp(argv[i], "scaleblue")==0){
            if(i + 1 >= argc){
                cout << "Missing argument."<< endl;
                return 0;
            }
            try {
                int blue = stoi(argv[i+1]);
                vector<Pixels> result;
                scaleBlue(sourceData.image_dat, blue, result);
                sourceData.image_dat = result;
                i += 2;
            }
            catch(...){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
        }
        else if(strcmp(argv[i], "scalegreen")==0){
            if(i + 1 >= argc){
                cout << "Missing argument."<< endl;
                return 0;
            }
            try {
                int green = stoi(argv[i+1]);
                vector<Pixels> result;
                scaleGreen(sourceData.image_dat, green, result);
                sourceData.image_dat = result;
                i += 2;
            }
            catch(...){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
        }
        else{
            cout << "Invalid method name."<< endl;
            return 0;
        }
    }
    writeFile("output/"+outputImage,sourceData);
    return 0;
}
