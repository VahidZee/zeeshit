//
// Created by Vahid Zee on 9/23/18.
//

#ifndef CARDREADER_ALGORITHMS_H
#define CARDREADER_ALGORITHMS_H

//External Libraries
    #include "./preprocess.h"
    #include "./ocrprocess.h"

//Functions
    cardData rawProcess( cv::Mat& resizedCardAreaImage );

    cardData processColors( cv::Mat& resizedCardAreaImage );

    cardData processKmeans( cv::Mat& resizedCardAreaImage );

#endif //CARDREADER_ALGORITHMS_H
