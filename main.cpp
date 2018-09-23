//Libraries
//Standard Libraries
#include <iostream>

//OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

//Project Libraries
#include "headerFiles/preprocess.h"

#include "headerFiles/algorithms.h"

//Defines
//Handles
#define TEST true
#define CONSOLE_OUTPUT true

//CV Loop Handles
#define WAIT_KEY 0

//Constant Values
//Paths
#define SIMPLE_INPUT_PATH "../simpleInput/"
#define RAW_IMAGES_PATH "../input/RAW_IMAGES/"
#define PAN_ONLY_PATH "../input/PAN_ONLY/"
#define EXP_ONLY_PATH
#define CVV2_ONLY_PATH
#define INPUT_COUNT 30

int main(int argc, char *argv[]) {

    //Initializes
#if TEST
    //GUI Windows
    cv::namedWindow("imageDisplayer", cv::WINDOW_KEEPRATIO);
    cv::namedWindow("srcDisplayer", cv::WINDOW_KEEPRATIO);
    cv::namedWindow("resultImage", cv::WINDOW_KEEPRATIO);
#endif

    //Initialize
    initializeOCR();

#if TEST
    //Loading Tests
    for (int i = 21; i <= INPUT_COUNT; i++) {
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
        std::cout << "\033[0;32Level 1: \033[0m \nTrimming And Force resizing Image #" << i << " :\n";
#endif

        cv::Mat cardArea = trimCardArea(srcImage);
        cv::Mat cardImageResized = forceResizeCardAreaImage(cardArea);
        srcImage.deallocate();
        cardArea.deallocate();

#if TEST
        cv::imshow("srcDisplayer", cardImageResized);
        cv::waitKey(0);
#endif

        //TODO blur shit
//            cv::blur( cardImageResized , cardImageResized , cv::Size( 3 , 3 )  );

        cardData data;
        data = rawProcess(cardImageResized);

        std::cout << data.ok;
        if (data.ok) {
#if TEST
            //TODO
            std::cout << data.panNumber << "shit\n";
            continue;
#endif
        }
        data = processColors(cardImageResized);
        if (data.ok) {
#if TEST
            //TODO
            std::cout << data.panNumber;
            continue;
#endif
        }

        data = processKmeans(cardImageResized);
        if (data.ok) {
#if TEST
            //TODO
            std::cout << data.panNumber;
            continue;
#endif
        }
#if TEST
    }
#endif
    return EXIT_SUCCESS;

}