//
//

#ifndef CARDREADER_ALGORITHMS_H
#define CARDREADER_ALGORITHMS_H

//External Libraries
#include "./preprocess.h"
#include "./ocrprocess.h"

//Functions
cardData rawProcess(cv::Mat &resizedCardAreaImage);

cardData processBlack(cv::Mat &resizedCardAreaImage, cardData &cardData1);

cardData processRed(cv::Mat &resizedCardAreaImage, cardData &cardData1);

cardData processKmeans(cv::Mat &resizedCardAreaImage, cardData &cardData1, int sliderValue, int level);

cardData invertColors(cv::Mat &resizedCardAreaImage, cardData &cardData1);

#endif //CARDREADER_ALGORITHMS_H
