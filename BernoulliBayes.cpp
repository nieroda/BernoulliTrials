#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

//Prototypes
vector<string> File_Read(char mystring[]);
void Remove_Space(vector<string>&vect);
vector <string> ReadRemoveReturn(char string[] );
vector<int> SumCols(vector<string>&vect);
vector <float> Probability (vector <int>&vect, const int total);
vector <float> FinalProb (vector<float>&probvect, string portion);
float MultiProb (vector<float>&vect, float multiplier);
void Splitter(vector<string>&ToPull, vector<string>&given, vector<string>&Spam, vector<string>&Clean);
void Accumulator(string &data, float case1, float case2, int &base, int &hits);


/*
 Function Name: main
 Function Steps: 
    1. Allocates space for 4 filenames which input stores in the space
    2. Classifies TrainData
    3. Learns from TrainData
    4. Uses TrainData to infer which of the files in testdata is spam/clean
    5. Checks the inference against the answer key in testclass
    6. Returns the average success rate out of given trials
 
 
 Input Parameters:
    None
 
 Goal - Find how accurate the Bernoulli Naive Bayes was
 
 */


int main() {
    
    clock_t startTime = clock();
    
    cout << "Enter the names of the training data, training class, test data and test class files (in that order): " << endl;
    
    char data[50], classs[50], testdata[50], testclass[50];
    
    cin >> data >> classs >> testdata >> testclass;

    vector <string> Both = ReadRemoveReturn(data);
    vector <string> Key = ReadRemoveReturn(classs);
    
    vector <string> Spam, Clean;
    
    Splitter(Both, Key, Spam, Clean);
    
    int SpamSize = Spam.size();
    int CleanSize = Clean.size();
    
    vector<int> SpamCols = SumCols(Spam);
    vector<int> CleanCols = SumCols(Clean);
    
    vector<float>SpamProb = Probability(SpamCols, SpamSize);
    vector<float>CleanProb = Probability(CleanCols, CleanSize);
    
    vector<string> TestData = ReadRemoveReturn(testdata);

    float ps = (float)SpamSize/(float)(SpamSize+CleanSize);
    float pi = (float)CleanSize/(float)(SpamSize+CleanSize);
    
    int acum = 0, hits = 0;

    vector <string> Raw_Data = ReadRemoveReturn(testclass);

    int eval = TestData.size();
    
    for (int i = 0; i < TestData.size(); i++) {
        vector<float> Prob1 = FinalProb(SpamProb, TestData[i]);
        vector<float> Prob2 = FinalProb(CleanProb, TestData[i]);
        float case1 = MultiProb(Prob1, ps);
        float case2 = MultiProb(Prob2, pi);
        
        Accumulator(Raw_Data[i], case1, case2, acum, hits);

    }
    
    float detection = ((float)hits/(float)acum) * 100.0;
    
    cout << "Training using " << SpamSize+CleanSize << " cases" << endl;
    cout << "Evaluating " << eval << " test cases" << endl;
    cout << "Successful detection rate of " << detection << "%" << endl;
    cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
    
    return 0;
}


/*
    Function Name: Accumulator
    Function Purpose: Checks to see if the Bayes Assumption was true or not
    Input Parameters:   
        1. Both Probabilities of Spam / Clean
        2. Base & Hits (Passed by reference so no return)
        3. Counter, so we know which index of data to evaluate
        4. Address of Vector Data, which is the key of what is spam/clean
    Return Value - N/A
 
 */


void Accumulator(string &Data, float case1, float case2, int &base, int &hits) {
    if ((case1 > case2 && Data == "1") || (case2 > case1 && Data == "0")) {
        base++;
        hits++;
    }
    else
        base++;
}


/*
 Function Name: Splitter
 Function Purpose: Allows us to learn and make assumptions based off given data
 Based off given, we are able to split ToPull into 2 classified vectors.
 Input Parameters:
    1. Vector ToPull which has all our data in it
    2. Vector Given which is the key to classifying ToPull
    3. Spam (pass by reference so no return) Contains Spam
    4. Clean (pass by reference so no return) Contains Clean
 Return Value - N/A
 
 */


void Splitter(vector<string>&ToPull, vector<string>&given, vector<string>&Spam, vector<string>&Clean) {

    for (int i = 0; i < ToPull.size(); i++)
        given[i] == "1" ? Spam.push_back(ToPull[i]) : Clean.push_back(ToPull[i]);
}



/*
 Function Name: MultiProb
 Function Purpose: Multiplies the contents of the vector with eachother, and multiplier
 Input Parameters:
    1. Vector Vect, which contains the values to be multiplied
    2. Multiplier, the value we multiply the product of Vect by
 Return Value - floating number of the product of everything
 
 */

float MultiProb(vector<float>&vect, float multiplier) {

    float AllMultiplied = 1;
    for (int i = 0; i < vect.size(); i++)
        AllMultiplied *= vect[i];
    
    AllMultiplied *= multiplier;
    return AllMultiplied;
}

/*
 Function Name: FinalProb
 Function Purpose: Computes Probabilities given 3 inputs
 Input Parameters:
    1. ProbVect, which has all our precomputed probabilities
    2. OnOffVect, binary string that allows the program to compute the final probability given the data
    3. Counter, the which is the index of OnOff (called from loop)
 Return Value - A vector of type float with all the computed fraction/floats.
 
 */

vector <float> FinalProb (vector<float>&probvect, string portion) {

    vector <float> Final;
    for (int i = 0; i < probvect.size(); i++)
        portion[i] == '1' ? Final.push_back(probvect[i]) : Final.push_back((float)1-probvect[i]);
    
    return Final;
}

/*
 Function Name: Probability
 Function Purpose: Divides every int in the vector by passed number 
 Input Parameters:
    1. Vect, which has all the precomputed numbers in it
    2. Number, what we are dividing every vector value by
 Return Value - Vector of type float with fractions.
 
 */

vector <float> Probability (vector <int>&vect, int number) {

    vector <float> Stats;
    float total;
    for (int i = 0; i < vect.size(); i++) {
        total = (float)vect[i] / (float)number;
        Stats.push_back(total);
    }
    
    return Stats;
}

/*
 Function Name: SumCols
 Function Purpose: Sums the columns inside the passed vector
 Input Parameters:
    1. Vect, the string we are evaluating to sum the columns.

 Return Value - Vector of type integer containing an index of the sum of each column
 
 */

vector<int> SumCols(vector<string>&vect) {

    vector <int> ColumnSum;
    ColumnSum.resize(vect[0].size()); //>>WILL<< cause crash if sent empty vector
    
    for (int row = 0; row < vect.size(); row++) {
        for (int column = 0; column < vect[row].size(); column++) {
            vect[row][column] == '1' ? ColumnSum[column] += 1 : ColumnSum[column] += 0;
        }
    }
    return ColumnSum;
}

/*
 Function Name: Remove_Space
 Function Purpose: Iterates through given string vector to remove spaces to save time later on.
 Input Parameters:
    1. Vector address of the vector we want to edit

 Return Value - N/A
 
 */

void Remove_Space(vector<string>&vect) {

    for (int i = 0; i < vect.size(); i++)
        vect[i].erase(remove(vect[i].begin(), vect[i].end(), ' '), vect[i].end());
    
}

/*
 Function Name: File_Read
 Function Purpose: Reads a file and inserts the lines in a vector of type string
 Input Parameters:
    1. Character Array of filename

 Return Value - Vector of type string with the data
 
 */

vector<string> File_Read(char mystring[]) {

    vector <string> numbers;
    
    ifstream file;
    string line;
    file.open(mystring);
    while(getline(file, line))
        numbers.push_back(line);
    return numbers;
}

/*
 Function Name: ReadRemoveReturn
 Function Purpose: Reads Data and then strips it of spaces
 Input Parameters:
    1. Character Array of filename

 Return Value - String Vector of the contents of filename with no spaces
 
 */

vector <string> ReadRemoveReturn(char filename[] ) {

    vector <string> numbers = File_Read(filename);
    Remove_Space(numbers);
    return numbers;
}
