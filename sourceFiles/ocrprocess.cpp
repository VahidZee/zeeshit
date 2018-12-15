//
//

#include <cstdio>
#include "../headerFiles/ocrprocess.h"


//Global Variables
tesseract::TessBaseAPI *ocr;

//OCR Main Functions
std::map<std::string, int> myMap;

std::multimap<std::string, std::pair<int, int>> mymultimap;
std::multimap<std::string, std::pair<int, int>>::iterator it;


//Inits

void addCardNum(int num) {
    for (int i = 0; i < 5; ++i) {
        mymultimap.insert(std::make_pair(
                std::to_string(num).substr(static_cast<unsigned long>(i), 2),
                std::make_pair(num, i)));
    }
    myMap[std::to_string(num).substr(0, 4)] = num / 100;
}

void initializeOCR() {
    ocr = new tesseract::TessBaseAPI();
    ocr->Init(NULL, "eng", tesseract::OEM_DEFAULT);

    ocr->SetPageSegMode(tesseract::PSM_AUTO_ONLY);
    ocr->SetVariable("tessedit_char_whitelist", "0123456789/");
    addCardNum(502229);
    addCardNum(502806);
    addCardNum(502908);
    addCardNum(502938);
    addCardNum(504172);
    addCardNum(504706);
    addCardNum(589210);
    addCardNum(589463);
    addCardNum(603799);
    addCardNum(606256);
    addCardNum(606373);
    addCardNum(606737);
    addCardNum(610433);
    addCardNum(621986);
    addCardNum(622106);
    addCardNum(627353);
    addCardNum(627381);
    addCardNum(627488);
    addCardNum(627412);
    addCardNum(627648);
    addCardNum(627760);
    addCardNum(627961);
    addCardNum(628023);
    addCardNum(628157);
    addCardNum(636214);
    addCardNum(636949);
    addCardNum(639347);
    addCardNum(639346);
    addCardNum(639599);
    addCardNum(639607);

}

//DataReading


void addNum(const char *data, cardData &cardData1) {
    std::string string(data);
    for (int i = 15; i >= 0; --i) {
        if (cardData1.panNumber[i] == 10) {
            for (int j = (static_cast<int>(string.size()) - 1 >= i ? i : (static_cast<int>(string.size()) - 1));
                 j >= 0 && i - j >= 0; --j) {
                if (string.at(static_cast<unsigned long>(j)) >= '0' && string.at(static_cast<unsigned long>(j)) <= '9')
                    cardData1.panNumber[i - j] = string.at(static_cast<unsigned long>(j)) - '0';
            }
            break;
        }
    }
}

bool processFirst(const char *data, cardData &cardData1) {

    std::string string(data);
    int temp = myMap[string.substr(0, 4)];
    if (temp) {
        for (int j = string.length() - 1; j >= 0; --j) {

            cardData1.panNumber[15 - j] = (int) (string.at(j) - '0');
        }
        for (int k = 0; k < 12; ++k) {

            cardData1.panNumber[k] = 10;
        }
        return true;
    }

    return false;
}


int checkPanNumberValidation(cardData &processedData, int mode = 0) {
    int sum = 0;
    for (int i = 0; i < 16; i += 2) {
        if (processedData.panNumber[i] == 10) {
            if (!mode)
                return 10;
            else {
                continue;
            }
        }
        sum += processedData.panNumber[i];
    }

    for (int i = 1; i < 16; i += 2) {
        if (processedData.panNumber[i] == 10) {
            if (!mode)
                return 10;
            else {
                continue;
            }
        }
        sum += (processedData.panNumber[i] * 2 > 9 ? (processedData.panNumber[i] * 2 - 9) : (
                processedData.panNumber[i] * 2));
    }
    return sum % 10;
}

cardData movePan(cardData &data, int move) {
    cardData tempData;
    tempData.ok = false;
    tempData.ok = data.dateOk;
    for (int i = 15; i >= move; --i) {
        tempData.panNumber[i - move] = data.panNumber[i];
    }
    return tempData;
}

cardData guessPanNumber(cardData &rawData) {
    int empties = 0;
    for (int i = 0; i < 16; ++i) {
        if (rawData.panNumber[i] > 9) {
            empties++;
        }
    }
    if (empties > 6) {
        return rawData;
    }

    std::ostringstream oss("");
    for (int temp = 15; temp > 11; temp--)
        oss << rawData.panNumber[temp];

    for (int j = 15; j >= 0; --j) {

        std::cout << rawData.panNumber[j];
    }
    std::cout << std::endl;
    if (myMap[oss.str()]) {
        if (empties == 1) {
            for (int i = 0; i < 15; ++i) {
                if (rawData.panNumber[i] == 10) {
                    if (i % 2 == 0) {
                        rawData.panNumber[i] = 10 - checkPanNumberValidation(rawData, 1);
                    } else {
                        int remain = checkPanNumberValidation(rawData, 1);
                        if ((10 - remain) % 2 == 0) {
                            rawData.panNumber[i] = (10 - remain) / 2;
                        } else {
                            rawData.panNumber[i] = (10 - remain + 9) / 2;
                        }
                    }
                    rawData.ok = true;
                    return rawData;
                }
            }
        }
    } else if (empties <= 4) {
        for (int i = 0; i < 5; ++i) {
            std::ostringstream os("");
            for (int temp = 15 - i; temp > 13 - i; temp--)
                os << rawData.panNumber[temp];

            for (it = mymultimap.lower_bound(os.str()); it != mymultimap.upper_bound(os.str()); ++it) {

                cardData tempData = movePan(rawData, it->second.second - i);


                int temp = it->second.first;
                for (int i = 5; i >= 0; --i) {

                    tempData.panNumber[10 + i] = static_cast<int>(temp / (pow(10, i)));
                    temp = temp % (int) (pow(10, i));
                }

                for (int j = 15; j >= 0; --j) {

                    std::cout << tempData.panNumber[j];
                }
                std::cout << std::endl;
                if (!checkPanNumberValidation(tempData)) {
                    tempData.ok = true;
                    return tempData;
                }

            }


        }
    } else if (empties == 6) {
        if (checkPanNumberValidation(rawData, 1) == 6) {
            cardData tempData = movePan(rawData, 6);

            tempData.panNumber[15] = 6;
            tempData.panNumber[14] = 0;
            tempData.panNumber[13] = 3;
            tempData.panNumber[12] = 7;
            tempData.panNumber[11] = 9;
            tempData.panNumber[10] = 9;
            tempData.ok = true;
            return tempData;
        }
    }
    return rawData;


}

bool checkIfItsDate(char *data, cardData &cardData1) {
    std::string string(data);

    if (string.find('/') != std::string::npos) {
        if (string.size() == 5 && string.at(2) == '/' && string.find(' ') == std::string::npos) {
            cardData1.expDate[0] = data[0];
            cardData1.expDate[1] = data[1];
            cardData1.expDate[2] = data[2];
            cardData1.expDate[3] = data[3];
            cardData1.expDate[4] = data[4];
            std::cout << cardData1.expDate << std::endl << "umade ha";
            cardData1.dateOk = true;
        }
        if (string.size() == 7 && string.at(4) == '/' && string.find(' ') == std::string::npos) {
            cardData1.expDate[0] = data[2];
            cardData1.expDate[1] = data[3];
            cardData1.expDate[2] = data[4];
            cardData1.expDate[3] = data[5];
            cardData1.expDate[4] = data[6];
            std::cout << cardData1.expDate << std::endl << "umade ha";
            cardData1.dateOk = true;
        }
        return true;
    }
    return false;
}

cardData getData(cv::Mat resizedCardAreaImage,
                 cardData &cardData1) /* we call this function to process output of a preprocessing */{
    cardData tempData = cardData1;

    tempData.ok = false;
    tempData.dateOk = false;
    cv::imshow("resultImage", resizedCardAreaImage);
    cvWaitKey(0);
    //Getting Probable  Text Area
    for (int i = 0; i <= 8; i++) {
        cv::Rect reigonOfIntrest(0, int(i * (TEXT_AREA_HEIGHT / 2.0) * resizedCardAreaImage.rows),
                                 resizedCardAreaImage.cols, (int) (resizedCardAreaImage.rows * TEXT_AREA_HEIGHT));
        cv::Mat subImage = resizedCardAreaImage(reigonOfIntrest).clone();
        ocr->SetImage(subImage.data, subImage.cols, subImage.rows, subImage.channels(),
                      static_cast<int>(subImage.step));
        ocr->Recognize(0);
        tesseract::ResultIterator *ri = ocr->GetIterator();
        tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
        bool flag = true;
        if (ri != 0) {

            do {
                char *word = ri->GetUTF8Text(level);
                float conf = ri->Confidence(level);
                if (conf > 30 && word != NULL && std::string(word).size() > 1) {

                    printf("word: '%s';  \tconf: %.2f;", word, conf);
                    if (checkIfItsDate(word, tempData)) {
                    } else if (!tempData.ok && flag && processFirst(word, tempData)) {
                        flag = false;
                        int x1, y1, x2, y2;
                        ri->BoundingBox(level, &x1, &y1, &x2, &y2);
                        tempData.mainY = y1;
                    } else if (!tempData.ok) {
                        addNum(word, tempData);
                    }
//                    if (std::string(word).size() > 1) {
//                        int x1, y1, x2, y2;
//                        ri->BoundingBox(level, &x1, &y1, &x2, &y2);
//                        addJunk(x1, word, tempData, y1);
//                    }
                }
                delete[] word;
            } while (ri->Next(level));
        }
        if (!checkPanNumberValidation(tempData)) {
            tempData.ok = true;
        } else {
            tempData = guessPanNumber(tempData);
        }
        if (tempData.ok && tempData.dateOk)
            return tempData;
        if (!tempData.ok) {
            for (int &k : tempData.panNumber) {

                k = 10;

            }
        }
        std::cout << std::endl << std::endl << "----------------" << std::endl;
    }
    std::cout << "*********************\n";
    return tempData;
}
