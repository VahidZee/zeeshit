//Libraries
//Standard Libraries
#include <iostream>

//OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

//Project Libraries
#include "headerFiles/preprocess.h"
#include "headerFiles/algorithms.h"
#include "headerFiles/ocrprocess.h"

//Handles
#define TEST true

#define RAW_IMAGES_PATH "../input/"
#define INPUT_COUNT 30

void printInfo(cardData data) {
    std::cout << "EXTRACTED!!!!" << std::endl;
    for (int j = 15; j >= 0; --j) {

        std::cout << data.panNumber[j];
    }
    std::cout << std::endl;
    std::cout << data.expDate << std::endl;
}

int main(int argc, char *argv[]) {

    //Initialize
    initializeOCR();

#if TEST
    //Loading Tests
    for (int i = 6; i <= INPUT_COUNT; i++) {
        //Reading Image
        cv::Mat srcImage = cv::imread(RAW_IMAGES_PATH + std::to_string(i) + ".jpg", cv::IMREAD_COLOR);
        if (srcImage.empty()) {
            std::cout << "image : " << i << "couldn't be found\n";
            continue;
        }
#endif
#if !TEST
        //Reading Image
        if (argc < 2) {
            std::cerr << "Path to Image is missing\n";
            return 1;
        }
        cv::Mat srcImage = cv::imread(argv[1], cv::IMREAD_COLOR);

        if (srcImage.empty()) {
#if CONSOLE_OUTPUT
            std::cout << "image : " << argv[1] << "couldn't be found\n";
#endif
            return 1;
        }
#endif

        //Level One : raw Image Trimming And Force Resize

#if TEST
        std::cout << "\nExtracting PAN from Image #" << i << " :\n#####################\n";
#endif
        cv::Mat cardArea = trimCardArea(srcImage);
        cv::Mat cardImageResized = forceResizeCardAreaImage(cardArea);
        srcImage.deallocate();
        cardArea.deallocate();

#if TEST
        cv::imshow("srcDisplayer", cardImageResized);
#endif

        cardData data = rawProcess(cardImageResized);
        if (data.ok && data.dateOk) {
#if TEST
            printInfo(data);
            cv::waitKey(0);
            continue;
#endif
        }
        data = processBlack(cardImageResized, data);
        if (data.ok && data.dateOk) {
#if TEST
            printInfo(data);
            cv::waitKey(0);
            continue;
#endif
        }
        data = processKmeans(cardImageResized, data, 4, 0);
        if (data.ok && data.dateOk) {
#if TEST
            printInfo(data);
            cv::waitKey(0);
            continue;
#endif
        }
        data = invertColors(cardImageResized, data);
        if (data.ok) {
#if TEST
            printInfo(data);
            cv::waitKey(0);
            continue;
#endif
        }
#if TEST
    }
#endif
    return EXIT_SUCCESS;

}