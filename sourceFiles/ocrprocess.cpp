//
// Created by Vahid Zee on 9/23/18.
//

#include "../headerFiles/ocrprocess.h"
//Global Variables
    tesseract::TessBaseAPI * ocr;

//OCR Main Functions
//Inits
void initializeOCR() {
    ocr = new tesseract::TessBaseAPI();

    ocr->Init(NULL, "eng", tesseract::OEM_DEFAULT);

    ocr->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    ocr->SetVariable("tessedit_char_whitelist", "0123456789/");
}

//DataReading
cardData getData( cv::Mat resizedCardAreaImage ) /* we call this function to process output of a preprocessing */{
    //TODO
    cardData tempData;
    tempData.ok  = false;
    cv::imshow("resultImage" , resizedCardAreaImage );
    cv::waitKey( 0 );
    //Getting Probable  Text Area
    for( int i  = 0 ; i <= 8 ; i++ ) {
        std::string text;
        cv::Rect reigonOfIntrest(  0 , int (i * (TEXT_AREA_HEIGHT /  2.0 ) * resizedCardAreaImage.rows) , resizedCardAreaImage.cols , (int)( resizedCardAreaImage.rows * TEXT_AREA_HEIGHT) );
        cv::Mat subImage = resizedCardAreaImage(reigonOfIntrest).clone();

        ocr->SetImage( subImage.data , subImage.cols , subImage.rows , subImage.channels() , static_cast<int>(subImage.step)  );
        ocr->Recognize(0);
        tesseract::ResultIterator* ri = ocr->GetIterator();
        tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
        if (ri != 0) {
            do {
                const char* word = ri->GetUTF8Text(level);
                float conf = ri->Confidence(level);
                if( conf > 20 )
                    printf("word: '%s';  \tconf: %.2f;\n", word, conf);
                delete[] word;
            } while (ri->Next(level));
        }
        std::cout<<std::endl<<std::endl<<"----------------"<<std::endl;
    }
    std::cout<<"*********************\n";
    return tempData;
}

cardData processTesseractOutput( char* data  ) {

}

cardData guessPanNumber( cardData& rawData ) {

}

bool checkPanNumberValidation( cardData& processedData ) {
    //TODO
    return true;
}
