//
// Created by Vahid Zee on 9/23/18.
//

#include "../headerFiles/algorithms.h"

//Functions
cardData rawProcess( cv::Mat& resizedCardAreaImage ) {
    cardData data;
    //Extracting Black
    cv::Mat mask =  extractColorBlack( resizedCardAreaImage );
//    data = getData( mask  );
//    if( data.ok )
//        return data;
//    mask = extractColorRed(resizedCardAreaImage  );
//    data = getData( mask  );
//    if( data.ok )
//        return data;
    for( int i = 1 ; i <= 6 ; i++ ) {
        mask = extractColorStrip(resizedCardAreaImage , i  , 6 , 0 , true );
        data = getData( mask  );
        if( data.ok )
            return data;
    }

    return data;
}

cardData processColors( cv::Mat& resizedCardAreaImage ) {

}

cardData processKmeans( cv::Mat& resizedCardAreaImage ) {
//    return getData( resizedCardAreaImage );

}

