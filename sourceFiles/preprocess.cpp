//
//

//Constant Values
//Handles

//Constant Values

//CardRatio
#define CARD_ASPECT_RATIO 1.586f
#define RAW_IMAGE_CARD_WIDTH_MARGIN 0.05f

#define CARD_DEFAULT_WIDTH 640


//External Files & Libraries
//Standard Libraries
#include <iostream>
#include <string>

//OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

//Project Libraries
#include "../headerFiles/preprocess.h"

//Functions
//Trims & Crops
/* Trims Card Area of rawInput Image Blindly */
cv::Mat trimCardArea(cv::Mat &rawImage) {
    //Local Images
    cv::Rect reigonOfIntrest((int) (rawImage.cols * RAW_IMAGE_CARD_WIDTH_MARGIN), \
(int) ((rawImage.rows - (1.0 / CARD_ASPECT_RATIO) * rawImage.cols * (1 - 2 * RAW_IMAGE_CARD_WIDTH_MARGIN)) / 2.0), \
                                   (int) (rawImage.cols * (1 - 2 * RAW_IMAGE_CARD_WIDTH_MARGIN)), \

                             (int) ((1.0 / CARD_ASPECT_RATIO) * rawImage.cols * (1 - 2 * RAW_IMAGE_CARD_WIDTH_MARGIN)) \
);
    return rawImage(reigonOfIntrest).clone();
}


//Resizings
cv::Mat forceResizeCardAreaImage(cv::Mat &cardAreaImage) {
    //Local Variables
    cv::Mat resizedImage;

    cv::resize(cardAreaImage, resizedImage,
               cv::Size(CARD_DEFAULT_WIDTH, (int) (CARD_DEFAULT_WIDTH / CARD_ASPECT_RATIO)), 1, 1);

    return resizedImage;
}

//Clusterings & Adaptive Filters
/* Kmeans Clustering : SubFunctions */
int *
countLables(std::vector<int> labels, cv::Mat data, int wLen, int hLen, int i, int j, int sliderValue, int level = 1) {
    //Local Variables
    auto lableCount = (int *) malloc(sliderValue * sizeof(int));

    for (int x = 0; x < sliderValue; x++)
        lableCount[x] = 0;

    if (i != 0)
        for (int y = 0; y < hLen; y++)
            for (int x = 0; x < level; x++) {
                lableCount[labels[x + y * wLen]]++;
            }

    for (int x = 0; x < sliderValue; x++)
        if (lableCount[x] == wLen)
            lableCount[x] = 0;
    if (j != 0)
        for (int x = 0; x < wLen; x++)
            for (int y = 0; y < level; y++) {
                lableCount[labels[x + y * wLen]]++;
            }

    return lableCount;
}

int *countColors(cv::Mat &clustImage, int wLen, int hLen, int i, int j, int sliderValue, int level = 1) {
    //Local Variables
    auto colorCount = (int *) malloc(sliderValue * sizeof(int));

    for (int x = 0; x < sliderValue; x++)
        colorCount[x] = 0;

    if (i != 0)
        for (int y = 0; y < hLen; y++)
            for (int x = 1; x <= level; x++) {
                colorCount[(int) (clustImage.at<cv::Vec3b>(j * hLen + y, i * wLen - x)[0] / 255.0f *
                                  (sliderValue - 1))]++;
            }

    if (j != 0)
        for (int x = 0; x < wLen; x++)
            for (int y = 1; y <= level; y++) {
                colorCount[(int) (clustImage.at<cv::Vec3b>(j * hLen - y, i * wLen + x)[0] / 255.0f *
                                  (sliderValue - 1))]++;
            }
    return colorCount;
}

void swap(int &a, int &b) {
    //Local Variables
    int temp = a;
    a = b;
    b = temp;
}

int *getColorMap(std::vector<int> labels, cv::Mat &clustImage, cv::Mat data, int wLen, int hLen, int i, int j,
                 int sliderValue, int level = 1) {
    std::cout << "i = " << i << " j = " << j << std::endl;
    auto lablesCount = countLables(labels, data, wLen, hLen, i, j, sliderValue, level);
    auto colorsCount = countColors(clustImage, wLen, hLen, i, j, sliderValue, level);

    auto lablesOnly = (int *) malloc(sliderValue * sizeof(int));
    auto colorsOnly = (int *) malloc(sliderValue * sizeof(int));

    auto colorMap = (int *) malloc(sliderValue * sizeof(int));

    for (int x = 0; x < sliderValue; x++) {
        lablesOnly[x] = x;
        colorsOnly[x] = x;
        colorMap[x] = x;
    }

    for (int x = 0; x < sliderValue; x++)

    std::cout << std::endl;
    for (int x = 0; x < sliderValue - 1; x++)
        for (int y = 0; y < sliderValue - x - 1; y++) {
            if (lablesCount[y] > lablesCount[y + 1]) {
                swap(lablesCount[y], lablesCount[y + 1]);
                swap(lablesOnly[y], lablesOnly[y + 1]);
            }
            if (colorsCount[y] > colorsCount[y + 1]) {
                swap(colorsCount[y], colorsCount[y + 1]);
                swap(colorsOnly[y], colorsOnly[y + 1]);
            }
        }

    for (int x = 0; x < sliderValue; x++)

    std::cout << std::endl;
    std::cout << std::endl;

    for (int x = 0; x < sliderValue; x++)
        colorMap[lablesOnly[x]] = colorsOnly[x];

    return colorMap;
}

int *getColorMap(cv::Mat1f colors, int sliderValue, int level) {

    auto grayScaleColors = (int *) malloc(sliderValue * sizeof(int));
    auto grayLabel = (int *) malloc(sliderValue * sizeof(int));
    auto colorMap = (int *) malloc(sliderValue * sizeof(int));
    for (int x = 0; x < sliderValue; x++) {
        grayScaleColors[x] = (int) ((colors(x, 0) + colors(x, 1) + colors(x, 2)) / 3.0);
        grayLabel[x] = x;
    }
    for (int x = 0; x < sliderValue - 1; x++)
        for (int y = 0; y < sliderValue - x - 1; y++)
            if (grayScaleColors[y] > grayScaleColors[y + 1]) {
                swap(grayScaleColors[y], grayScaleColors[y + 1]);
                swap(grayLabel[y], grayLabel[y + 1]);
            }

    if (level == -1) {
        for (int x = 0; x < sliderValue; x++)
            colorMap[grayLabel[x]] = x;
    } else {
        colorMap[grayLabel[0]] = 0;
        for (int x = 1; x < sliderValue; x++)
            colorMap[grayLabel[x]] = sliderValue - 1;
    }

    return colorMap;
}

cv::Mat kmeansParts(cv::Mat &image, int wPartsCount, int hPartsCount, int sliderValue, int level) {
    //Local Varibles
    int wLen = (int) (image.cols / (double) wPartsCount);
    int hLen = (int) (image.rows / (double) hPartsCount);
    std::cout << "wLen:" << wLen << " hLen:" << hLen << std::endl;
    cv::Mat clusImage(image.rows, image.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    for (int i = 0; i < wPartsCount; i++)
        for (int j = 0; j < hPartsCount; j++) {
            cv::Rect reigonOfIntrest(i * wLen, j * hLen, wLen, hLen);
            cv::Mat part = image(reigonOfIntrest).clone();
            auto singleLineSize = static_cast<const unsigned int>(wLen * hLen);
            cv::Mat data = part.reshape(1, singleLineSize);
            data.convertTo(data, CV_32F);
            std::vector<int> labels;

            cv::Mat1f colors;
            cv::kmeans(data, sliderValue, labels,
                       cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.), 10,
                       cv::KMEANS_RANDOM_CENTERS, colors);
            if (level == 1) {
                if (i == 0 && j == 0) {
                    for (unsigned int i = 0; i < singleLineSize; i++) {
                        data.at<float>(i, 0) = 255.0f / (sliderValue - 1) * labels[i];
                        data.at<float>(i, 1) = 255.0f / (sliderValue - 1) * labels[i];
                        data.at<float>(i, 2) = 255.0f / (sliderValue - 1) * labels[i];
                    }
                } else {
                    auto colorMap = getColorMap(labels, clusImage, data, wLen, hLen, i, j, sliderValue, level);

                    for (unsigned int i = 0; i < singleLineSize; i++) {
                        data.at<float>(i, 0) = 255.0f / (sliderValue - 1) * colorMap[labels[i]];
                        data.at<float>(i, 1) = 255.0f / (sliderValue - 1) * colorMap[labels[i]];
                        data.at<float>(i, 2) = 255.0f / (sliderValue - 1) * colorMap[labels[i]];
                    }

                }
            } else {
                auto colorMap = getColorMap(colors, sliderValue, level);

                for (unsigned int i = 0; i < singleLineSize; i++) {
                    data.at<float>(i, 0) = 255.0f / (sliderValue - 1) * colorMap[labels[i]];
                    data.at<float>(i, 1) = 255.0f / (sliderValue - 1) * colorMap[labels[i]];
                    data.at<float>(i, 2) = 255.0f / (sliderValue - 1) * colorMap[labels[i]];
                }
            }
            cv::Mat outputImage = data.reshape(3, part.rows);
            outputImage.convertTo(outputImage, CV_8U);

            for (int i2 = 0; i2 < outputImage.cols; ++i2)
                for (int j2 = 0; j2 < outputImage.rows; ++j2)
                    for (int z = 0; z < 3; ++z)
                        clusImage.at<cv::Vec3b>(j2 + j * hLen, i2 + i * wLen)[z] = outputImage.at<cv::Vec3b>(j2, i2)[z];
        }
    return clusImage;
}

//Color Extractions
cv::Mat extractColorBlack(cv::Mat &resizedCardArea, int blurSize, bool denoise) {
    cv::Mat mask;
    cv::Mat hsv;
    if (blurSize)
        cv::blur(resizedCardArea, resizedCardArea, cv::Size(blurSize, blurSize));
    cv::cvtColor(resizedCardArea, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0, 0, 255 * 0.20), cv::Scalar(180, 255, 255), mask);
    if (denoise)
        cv::fastNlMeansDenoising(mask, mask, 10, 3, 7);
    return mask;
}

cv::Mat extractColorRed(cv::Mat &resizedCardArea, int blurSize, bool denoise) {
    cv::Mat mask1;
    cv::Mat mask2;
    cv::Mat mask;
    cv::Mat hsv;
    if (blurSize)
        cv::blur(resizedCardArea, resizedCardArea, cv::Size(blurSize, blurSize));
    cv::cvtColor(resizedCardArea, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0, 255 * 0.20, 255 * 0.30), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(hsv, cv::Scalar(170, 255 * 0.20, 255 * 0.30), cv::Scalar(180, 255, 255), mask2);
    mask = mask1 | mask2;
    if (denoise)
        cv::fastNlMeansDenoising(mask, mask, 10, 3, 7);
    return mask;
}
