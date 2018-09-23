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
    tempData.ok  = true;

    //Getting Probable  Text Area
    for( int i  = 0 ; i < 10 ; i++ ) {
        cv::Rect reigonOfIntrest(  0 , int (i * (TEXT_AREA_HEIGHT /  2.0 ) * resizedCardAreaImage.rows) , resizedCardAreaImage.cols , (int)( resizedCardAreaImage.rows * TEXT_AREA_HEIGHT) );
        cv::Mat subImage = resizedCardAreaImage(reigonOfIntrest).clone();
        cv::imshow("resultImage" , subImage );
        cv::waitKey( 0 );
        ocr->SetImage( subImage.data , subImage.cols , subImage.rows , subImage.channels() , static_cast<int>(subImage.step)  );
        ocr->Recognize(0);
        tesseract::ResultIterator* ri = ocr->GetIterator();
        tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
        if (ri != 0) {
            do {
                const char* word = ri->GetUTF8Text(level);

                float conf = ri->Confidence(level);
                int x1, y1, x2, y2;
                ri->BoundingBox(level, &x1, &y1, &x2, &y2);
                if( conf > 40 )
                printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n",
                       word, conf, x1, y1, x2, y2);
                delete[] word;
            } while (ri->Next(level));
        }
    }

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
