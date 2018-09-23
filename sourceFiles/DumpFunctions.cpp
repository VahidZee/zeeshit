//
// Created by Vahid Zee on 9/3/18.
//


Mat contrast( Mat rawImage , double contrastLevelPercentage ) {
    //Local Variables
    Mat dst(rawImage.rows , rawImage.cols , CV_8UC3 , Scalar( 0 , 0 , 0));
    int temp;
    auto * channelsAverage = ( double * ) malloc( 3 * sizeof( double));

    for( int z = 0 ; z < 3 ; z++ )
        channelsAverage[z] = 0;

    for( int x = 0 ; x < rawImage.cols ; x++ )
        for( int y = 0 ; y < rawImage.rows ; y++ )
            for( int z = 0 ; z < 3 ; z++ )
                channelsAverage[z]+=  ( (int) rawImage.at<Vec3b>(x,y)[z] ) / 255.0 ;
    for( int z = 0 ; z < 3 ; z++ )
        channelsAverage[z] = channelsAverage[z]/(double (rawImage.cols * rawImage.rows))*255.0;

    for( int x = 0 ; x < rawImage.rows ; x++ )
        for( int y = 0 ; y < rawImage.cols ; y++ )
            for( int z = 0 ; z < 3 ; z++ ) {
                temp =   (int) ( ( rawImage.at<Vec3b>(x,y)[z] - channelsAverage[z] ) *( 1 + contrastLevelPercentage / 100 ) + channelsAverage[z] ) ;
                dst.at<Vec3b>(x,y)[z] = static_cast<uchar> ( ( temp < 0 ) ? 0 : ( temp > 255 ) ? 255 : temp );
            }
    return dst;
}

Mat contrastJunk( Mat rawImage , double contrastLevelPercentage ) {
    //Local Variables
    Mat dst(rawImage.rows , rawImage.cols , CV_8UC3 , Scalar( 0 , 0 , 0));
    int temp;
    auto * channelsAverage = ( double * ) malloc( 3 * sizeof( double));
    auto * channelMin = ( double * ) malloc( 3 * sizeof( double));
    auto * channelMax = ( double * ) malloc( 3 * sizeof( double));

    for( int z = 0 ; z < 3 ; z++ ){
        channelsAverage[z] = 0;
        channelMin[z] = 255;
        channelMax[z] = 0;

    }

    for( int x = 0 ; x < rawImage.cols ; x++ )
        for( int y = 0 ; y < rawImage.rows ; y++ )
            for( int z = 0 ; z < 3 ; z++ ) {
                channelMin[z] =  ( ( (int) rawImage.at<Vec3b>(x,y)[z] ) / 255.0 < channelMin[z] ) ?( (int) rawImage.at<Vec3b>(x,y)[z] ) / 255.0 :channelMin[z]    ;
                channelMax[z] =  ( ( (int) rawImage.at<Vec3b>(x,y)[z] ) / 255.0 > channelMin[z] ) ?( (int) rawImage.at<Vec3b>(x,y)[z] ) / 255.0 :channelMax[z]    ;
            }
    for( int z = 0 ; z < 3 ; z++ )
        channelsAverage[z] = ( channelMin[z] + channelMax[z] ) / 2.0 * 255.0;

    for( int z = 0 ; z < 3 ; z++ )
        cout<< channelsAverage[z]  << "  " << std::endl;

    for( int x = 0 ; x < rawImage.rows ; x++ )
        for( int y = 0 ; y < rawImage.cols ; y++ )
            for( int z = 0 ; z < 3 ; z++ ) {
                temp =   (int) ( ( rawImage.at<Vec3b>(x,y)[z] - channelsAverage[z] ) *( 1 + contrastLevelPercentage / 100 ) + channelsAverage[z] ) ;
                dst.at<Vec3b>(x,y)[z] = static_cast<uchar> ( ( temp < 0 ) ? - temp  : ( temp > 255 ) ?temp - 255 : temp );
            }
    return dst;
}

Mat contrastKoshsereSoroosh( Mat rawImage , double contrastLevelPercentage ) {
    //Local Variables
    Mat dst(rawImage.rows , rawImage.cols , CV_8UC3 , Scalar( 0 , 0 , 0));
    int temp;
    auto * channelsAverage = ( double * ) malloc( 3 * sizeof( double));
    auto * channelMin = ( double * ) malloc( 3 * sizeof( double));
    auto * channelMax = ( double * ) malloc( 3 * sizeof( double));

    for( int z = 0 ; z < 3 ; z++ ){
        channelsAverage[z] = 0;
        channelMin[z] = 255;
        channelMax[z] = 0;

    }


    for( int x = 0 ; x < rawImage.rows ; x++ )
        for( int y = 0 ; y < rawImage.cols ; y++ )
            for( int z = 0 ; z < 3 ; z++ ) {
                if(rawImage.at<Vec3b>(x,y)[z] < 64){
                    dst.at<Vec3b>(x,y)[z] = static_cast<uchar>(128-(128-(rawImage.at<Vec3b>(x,y)[z]+64)) * ( 1 + contrastLevelPercentage / 100.0 ));
                }
                else if(rawImage.at<Vec3b>(x,y)[z] > 191){
                    dst.at<Vec3b>(x,y)[z] = static_cast<uchar>(128+(rawImage.at<Vec3b>(x,y)[z]-64) * ( 1 + contrastLevelPercentage / 100.0 ));
                }
                else if(rawImage.at<Vec3b>(x,y)[z] < 191&&rawImage.at<Vec3b>(x,y)[z] >= 128){
                    dst.at<Vec3b>(x,y)[z] = static_cast<uchar>((rawImage.at<Vec3b>(x,y)[z]-128) * ( 1 + contrastLevelPercentage / 100.0 )+128);

                }
                else{
                    dst.at<Vec3b>(x,y)[z] = static_cast<uchar>(128-(128-rawImage.at<Vec3b>(x,y)[z]) * ( 1 + contrastLevelPercentage / 100.0 ));

                }
            }
    return dst;
}
