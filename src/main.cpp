/*
 * program name:main.c
 * program desc:test driver for KmVConverter class
 * program date:2018.12.28
 * program auth:James Lisle
*/

#include "kmvconverter.h"

using namespace std;


const char CMD_LNE_USAGE[] = "Usage: %s -f <filename>";
const char FILE_OPEN_ERROR_MSG[] = "%s file failed to open.";

//test data
const double kelvin[] = {1.23,1.34,1.40,2.1,3.5,4.0125,40.125,401.25,505.13,99.1};
const double mVolts[] = {85.023, 628.302,892.35,1725.0,1600.2,1050.13,100};

int main(int argc, char* argv[])
{
    char errorMsg[64] = {""};

    if ( argc != 3 ) {
        sprintf(errorMsg, CMD_LNE_USAGE, argv[0]);
        cout << errorMsg << endl;
        return -1;
    } else if ( strcmp(argv[1], "-f") != 0 ) {
        sprintf(errorMsg, CMD_LNE_USAGE, argv[0]);
        cout << errorMsg << endl;
        return -1;
    } else  {

        ifstream inFile;
        inFile.open(argv[2],ios::in);

        if (inFile.is_open()) {

            inFile.close();

            //create converter
            KmVConverter kmVConv = KmVConverter(argv[2]);

            //run test cases for Kelvin to mVolt
            int maxTestCnt=sizeof(kelvin)/sizeof(kelvin[0]);
            double result=0.0;
            for (int i=0;i<maxTestCnt;i++) {
               result = kmVConv.convertKtomV(kelvin[i]);
               if ( static_cast<int>(result) != -1 ) {
                  cout << kelvin[i] << " degrees Kelvin correlates to " << result << " mVolts " << endl;
               } else {
                  cout << kelvin[i] << " degree Kelvin to milliVolts is undefined for the dataset given." << endl;
               }
            }

            //run test cases for mVolt to Kelvin
            maxTestCnt=sizeof(mVolts)/sizeof(mVolts[0]);
            result=0.0;
            for (int i=0;i<maxTestCnt;i++) {
               result = kmVConv.convertmVtoK(mVolts[i]);
               if ( static_cast<int>(result) != -1 ) {
                  cout << mVolts[i] << " milliVolts correlates to " << result << " degrees Kelvin " << endl;
               } else {
                  cout << mVolts[i] << " milliVolts to degree Kelvin is undefined for the dataset given." << endl;
               }
            }

        } else {

            sprintf(errorMsg, FILE_OPEN_ERROR_MSG, argv[2]);
            cout << errorMsg << endl;
            return -1;

        }

    }

    return 0;
}

