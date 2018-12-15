//
//

#ifndef CARDREADER_OCRPROCESS_H
#define CARDREADER_OCRPROCESS_H

//External Files & Libraries
    //Standard Libraries
    #include <iostream>
    #include <string>
    #include <math.h>
    #include <vector>


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
        bool dateOk;
        int mainY;
        int panNumber[16]={10};
        char expDate[5]="";
    } cardData;


//Functionsk
    //Inits
    void initializeOCR();

    //OCR Main Functions
    cardData getData( cv::Mat resizedCardAreaImage,cardData& cardData1 );


#endif //CARDREADER_OCRPROCESS_H
