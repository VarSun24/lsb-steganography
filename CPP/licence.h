//
// Created by varun on 20-Jul-23.
//

#ifndef CPP_LICENCE_H
#define CPP_LICENCE_H

#include <utility>
#include <bitset>

#include "string"
#include "iostream"
#include "opencv2/opencv.hpp"
#include "fstream"

using namespace std;
using namespace cv;

class LICENCE{
private:
    string* FILE_PATH;
    string* IMG_PATH;
    int TEXT_LEN;
public:
    explicit LICENCE(int text_len = 0){
        TEXT_LEN = text_len;
    }
    explicit LICENCE(string file_path="", string img_path=""){
        FILE_PATH = new string(std::move(file_path));
        IMG_PATH = new string(std::move(img_path));
    }

    string getFile();
    string getImg();
    int getTextLen();
    void embedLicence(cv::Mat& image, const std::string& text);
    string extractLicence(const cv::Mat& image, int textLength);

};

//Gives (FILE_PATH)
string LICENCE::getFile() {
    return *FILE_PATH;
}

//Gives (IMG_PATH)
string LICENCE::getImg() {
    return *IMG_PATH;
}

int LICENCE::getTextLen() {
    return TEXT_LEN;
}

//Implements Steganography of the licence on the image
void LICENCE::embedLicence(cv::Mat &image, const std::string &text) {
    int strLength = text.size();
    int strIndex = 0;

    int count = 0;
    char toHide = text[strIndex++];
    bitset<8> charBits(toHide);
    for(int i=0; i < image.rows; i++){
        for(int j=0; j < image.cols; j++){
            cv::Vec3b& pixel = image.at<cv::Vec3b>(i, j);
            for(int k=0; k<3; k++){
                //cout << " bfr: " << (bitset<8>)pixel[k];
                if(count == 8){
                    count = 0;
                    j++;
                    toHide = text[strIndex++];
                    if(strIndex >= strLength){
                        if (!cv::imwrite("output.png", image)) {
                            cerr << "Error: Unable to save the modified image!" << endl;
                            return;
                        }
                        return;
                    }
                    bitset<8> ch(toHide);
                    charBits = ch;
                }
                pixel[k] = (pixel[k] & 0xFE) | charBits[count++];
                //cout << " afr: " << (bitset<8>)pixel[k] << endl;
            }
        }

    }
}


//Extracts the licence embedded inside the image
string LICENCE::extractLicence(const cv::Mat &image, int strLength) {
    cout << "EXTRACT!";
    string extractedText = "";
    ofstream f("extracted_licence.txt", std::ios::app);
    if(!f){
        cerr << "Error: Could not open file!";
        exit(0);
    }

    int count = 0;
    char ch = '\0';

    bitset<8> bitsChar('\0');
    for(int i=0; i < image.rows; i++){
        for(int j=0; j < image.cols; j++){
            cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
            for(int k=0; k<3; k++){
                //cout << " bfr: " << (bitset<8>)pixel[k] << " " << bitsChar;
                if(count == 8){
                    count = 0;
                    j++;
                    ch = static_cast<char>(bitsChar.to_ulong());
                    f << ch;
                    extractedText.push_back(ch);
                    if(!ch)
                        return extractedText;
                }
                bitsChar[count++] = (pixel[k] & 0x01);
                //cout << " afr: " << bitsChar << endl;
            }
            j++;
            cout << endl;
        }

    }

    return extractedText;
}

#endif //CPP_LICENCE_H
