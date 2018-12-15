//
//

#include "../headerFiles/algorithms.h"
#include "../headerFiles/ocrprocess.h"

//Functions
cardData rawProcess(cv::Mat &resizedCardAreaImage) {
    cardData data;
    data = getData(resizedCardAreaImage, data);

    if (data.ok) {
        return data;
    }

    return data;
}

cardData processBlack(cv::Mat &resizedCardAreaImage, cardData &cardData1) {
    cardData1 = getData(extractColorBlack(resizedCardAreaImage, 0), cardData1);

    return cardData1;
}

cardData processKmeans(cv::Mat &resizedCardAreaImage, cardData &cardData1, int sliderValue, int level) {
    return getData(kmeansParts(resizedCardAreaImage, 8, 8, sliderValue, level), cardData1);

}

cardData processRed(cv::Mat &resizedCardAreaImage, cardData &cardData1) {
    for (int i = 0; i <= 2; i++) {
        cardData1 = getData(extractColorRed(resizedCardAreaImage, i), cardData1);

        if (cardData1.ok) {
            return cardData1;
        }

    }

    return cardData1;
}

cardData invertColors(cv::Mat &resizedCardAreaImage, cardData &cardData1) {
    cv::Mat temp;
    cv::bitwise_not(resizedCardAreaImage, temp);
    return processKmeans(temp, cardData1, 3, 0);

}
