#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "licence.h"

using namespace std;
using namespace cv;

string file, img;

void init(){
    file = R"(D:\workspace\CPP\resources\licence_file.txt)";
    img = R"(D:\workspace\CPP\resources\rdr2_4k.png)";
}

void embed(LICENCE l){
    //Load text from text file
    ifstream textFile(l.getFile());
    string text((istreambuf_iterator<char>(textFile)), istreambuf_iterator<char>());
    textFile.close();

    //Read the image
    Mat image = imread(l.getImg());

    int imageCapacity = image.rows * image.cols * 3; // Assuming 3 channels (RGB)

    // Check if the image has enough capacity to embed the text
    if (imageCapacity < text.size() * 8) {
        cout << "Error: Text size exceeds image capacity.\n";
        return;
    }

    // Embed the text into the image
    l.embedLicence(image, text);

    // Save the steganography image
    imwrite("output.png", image);

    //Finally record the text file length
    LICENCE(text.length());
}

void extract(LICENCE l){
    // Extract the text from the steganography image
    Mat steganographyImage = imread("output.png");
    string extractedText = l.extractLicence(steganographyImage, l.getTextLen());

    // Print the extracted text
    cout << "Extracted Text:\n" << extractedText << endl;
}

int main() {
    init();
    LICENCE L(file, img);
    //cout << L.getTextLen() << endl;
    //cout << L.getFile() << endl;
    //cout << L.getImg() << endl;
    embed(L);
    extract(L);

    return 0;
}
