//
//

#ifndef CARDREADER_PREPROCESS_H
#define CARDREADER_PREPROCESS_H

//Libraries
    //Standard Libraries
    #include <iostream>
    #include <string>

    //OpenCv
    #include <opencv2/opencv.hpp>
    #include <opencv2/imgproc.hpp>

    //Tesseract
    #include <tesseract/baseapi.h>
    #include <leptonica/allheaders.h>


//Functions
    //Trimming & shit
    cv::Mat trimCardArea( cv::Mat& rawImage );

    cv::Mat forceResizeCardAreaImage( cv::Mat& cardAreaImage );

    //Clustering & shit
    cv::Mat kmeansParts( cv::Mat& image , int wPartsCount , int hPartsCount , int sliderValue  , int level = -1  );

    cv::Mat extractColorBlack( cv::Mat& resizedCardArea , int blurSize = 0  , bool denoise  = true );

    cv::Mat extractColorRed(cv::Mat& resizedCardArea ,  int blurSize = 0 , bool denoise = true );

#endif
