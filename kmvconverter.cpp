/*
 * program name:KmVConverter.h/KmVConverter.cpp
 * program date:2018.12.28
 * program auth:James Lisle
 * program desc:Simple class to: (1) load a dataset that correlates degree Kelvin to milliVolt
 *                               (2) provides 2 public methods that translate between Kelvin and milliVolts in both directions
 *
 *                               - Uses binary search to locate data point or where data point should be located
 *                               - Uses linear interpolation when an exact match is not found [y = y1 + (y2 - y1)((x - x1)/(x2 - x1))]
 *                               - Assumes data is sorted
 *                               - Requires input to exist in dataset
 *
 *                               - currently does not consider significant digits
 *
*/



#include "kmvconverter.h"

KmVConverter::KmVConverter(char* fileName)
{

    //store filename and record count
    size_t len = strlen(fileName) + 1;
    dataFileName = new char[ len ];
    memset(dataFileName,'\0',len);
    strcpy(dataFileName,fileName);

    //load XRef table
    loadKmVXRefTable(dataFileName);

}

KmVConverter::~KmVConverter()
{

    //release dataStore in 2 steps
    for (int i=0; i<recordCnt; i++) {
        delete [] dataStore[i];
    }

    delete [] dataStore;



    //char* string
    delete dataFileName;


}


double KmVConverter::convertmVtoK(double mVolts) {

    int high = recordCnt - 1;
    int low  = 0;
    double mVResult = -1.0;

    //convert to volts
    mVolts /= 1000;

    if ( validateInput(mVolts, MVOLT) == true ) {

       mVResult = KmVLookupDesc(high, low, MVOLT, mVolts );

        if (mVResult < 0) {

            mVResult = mVToKInterpolation(high, low, mVolts);

        } else {

            int row = static_cast<int>(mVResult);
            mVResult = dataStore[row][0];

        }
     }

    return mVResult;
}


double KmVConverter::convertKtomV(double Kelvin){

    int high = recordCnt - 1;
    int low  = 0;
    double mVResult = -1.0;

    if ( validateInput(Kelvin, KELVIN) == true) {

        mVResult = KmVLookup(high, low, KELVIN , Kelvin );

        if (static_cast<int>(mVResult) == -1) {

            mVResult = KTomVInterpolation(high, low, Kelvin);
        } else {

            int row = static_cast<int>(mVResult);
            mVResult = dataStore[row][1];

        }
    }

    return mVResult;

}

int KmVConverter::KmVLookup(int &high, int &low, ConvUnit convUnit, double value) {

    int mid = 0;

    while (high - low > 1) {

        mid = low + (high - low) / 2;

        if (value < dataStore[mid][convUnit]) {

            high = mid;

        }else{

            low = mid;

        }
    }

  return (low < high && isEqual(dataStore[low][convUnit], value)) ? low : -1;
}

int KmVConverter::KmVLookupDesc(int &high, int &low, ConvUnit convUnit, double value) {

    int mid = 0;

    while (high - low > 1) {

        mid = low + (high - low) / 2;

        if (value < dataStore[mid][convUnit]) {

            low = mid;

        }else{

            high = mid;

        }
    }

  return (low < high && isEqual(dataStore[low][convUnit], value)) ? low : -1;
}


double KmVConverter::KTomVInterpolation(int high, int low, double target) {

    double mVResult = dataStore[low][MVOLT] + (dataStore[high][MVOLT] - dataStore[low][MVOLT]) * ((target - dataStore[low][KELVIN])/(dataStore[high][KELVIN] - dataStore[low][KELVIN]));

    return mVResult;
}

double KmVConverter::mVToKInterpolation(int low, int high, double target) {

    double KResult = dataStore[low][KELVIN] + (dataStore[high][KELVIN] - dataStore[low][KELVIN]) * ((target - dataStore[low][MVOLT])/(dataStore[high][MVOLT] - dataStore[low][MVOLT]));

    return KResult;
}


void KmVConverter::loadKmVXRefTable(char* fileName) {

    ifstream inFile;
    string record;

    inFile.open(fileName,ios::in);

    if (inFile.is_open()) {

        int recCntr=0;
        while(getline(inFile,record)) {
            recCntr++;
        }

        //close and reopen
        inFile.close();

        inFile.open(fileName,ios::in);

        if (inFile.is_open()) {

            //save record count to instance variable for use in destructor
            recordCnt = recCntr;

            //allocate storage for xref table

            //rows
            dataStore = new double*[recordCnt];

            //columns
            for(int i=0; i < recordCnt; i++ ) {
                dataStore[i] = new double[2];
            }

            //initialize variables for data input
            double KValue=0.0;
            double mVValue=0.0;
            double dVdTValue=0.0;

            //read file and load data table
            recCntr = 0;
            while(inFile >> KValue >> mVValue >> dVdTValue ) {

                //cout << KValue << " " << mVValue << " " << dVdTValue << endl;

                dataStore[recCntr][KELVIN] = KValue;
                dataStore[recCntr][MVOLT] = mVValue;

                recCntr++;
            }

            inFile.close();

        }

    }
}

bool KmVConverter::validateInput(double value, ConvUnit convUnit) {

    bool dataStatus=false;

    if (KELVIN == convUnit) {

        //ascending order
        if ( value >= dataStore[0][convUnit] && value <= dataStore[recordCnt-1][convUnit] ) {

            dataStatus = true;
        }

    } else {

        //descending order
        if ( value <= dataStore[0][convUnit] && value >= dataStore[recordCnt-1][convUnit] ) {

            dataStatus = true;
        }

    }

    return dataStatus;
}

inline bool KmVConverter::isEqual(double x, double y)
{
  const double epsilon = 0.0000001;
  return std::abs(x - y) <= epsilon * std::abs(x);
  // http://www.cs.technion.ac.il/users/yechiel/c++-faq/floating-point-arith.html
  // see Knuth section 4.2.2 pages 217-218
}
