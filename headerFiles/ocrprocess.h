//
// Created by Vahid Zee on 9/23/18.
//

#ifndef CARDREADER_OCRPROCESS_H
#define CARDREADER_OCRPROCESS_H

//External Files & Libraries
    //Standard Libraries
    #include <iostream>
    #include <string>

    //OpenCv
    #include <opencv2/opencv.hpp>
    #include <opencv2/imgproc.hpp>

    //Project Libraries
    #include "./preprocess.h"
//Constant Values
    #define TEXT_AREA_HEIGHT 0.2

//Structures And Type Definitions
    typedef struct {
        bool ok;
        char panNumber[16];
        char* expDate;
        char* cvv2;
    } cardData;


//Functions
    //Inits
    void initializeOCR();

    //OCR Main Functions
    cardData getData( cv::Mat resizedCardAreaImage );


#endif //CARDREADER_OCRPROCESS_H
