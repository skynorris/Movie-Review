////////////////////////////////////////////////////////////////////////////////
// File Name:      a2.cpp
//
// Author:         Gerald
// CS email:       gerald@cs.wisc.edu
//
// Description:    The source file for a2.
//
// IMPORTANT NOTE: THIS IS THE ONLY FILE THAT YOU SHOULD MODIFY FOR A2.
//                 You SHOULD NOT MODIFY any of the following:
//                   1. Name of the functions/methods.
//                   2. The number and type of parameters of the functions.
//                   3. Return type of the functions.
////////////////////////////////////////////////////////////////////////////////

#include "a2.hpp"
#include "trim.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    // TODO: Implement this method.
    // # of lines of code in Gerald's implementation: 13

    //fields
    vector<string> reviewPunc;
    vector<string> review;
    string line;

    ifstream inFile("rawReviewRatings.txt");
    ifstream stopFile("stopwords.txt");
    ofstream outFile("cleanReviewRatings.txt");

    std::unordered_set<std::string> stop;
    while(getline(stopFile, line)){
        stop.insert(line);
    }


    //do for whole file
    while (std::getline(inFile, line)) {

        //   1. Replace hyphens with spaces.
        replaceHyphensWithSpaces(line);

        //   2. Split the line of text into individual words.
        splitLine(line, reviewPunc);
    }
    inFile.close();
    stopFile.close();


    //   3. Remove the punctuation marks from the words.
    //   5. Remove the empty words.
    removePunctuation(reviewPunc, review);

    //   6. Remove words with just one character in them. You should NOT remove
    // numbers in this step because if you do so, you'll lose the ratings.`
    removeSingleLetterWords(review);

    //   7. Remove stopwords.
    removeStopWords(review, stop);

    //write clean review to outfile
    for(int i = 0; i < review.size(); i++){
        if((review.at(i).size() == 1)){
            if(i != 0){
                outFile << "\n";
            }
            outFile << review.at(i);
        }
        else {

            outFile << " " << review.at(i);
        }
    }


    //fill dict
    ifstream inFile1("cleanReviewRatings.txt");
    std::unordered_map<std::string, std::pair<long, long> > dictionary;
    fillDictionary(inFile1, dictionary);
    inFile.close();

    //generate predicted ratings
    ifstream inFile2;
    ofstream outFile2;
    rateReviews(inFile2, dictionary, outFile2);


    return 0;

}

void fillDictionary(std::ifstream &newInFile,
                    std::unordered_map<std::string, std::pair<long, long> > &dict) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 20

    //sores every line of file
    string line;

    while(getline(newInFile, line)){

        vector<string> reviewWords;

        splitLine(line, reviewWords);
        int rating = atoi(reviewWords.at(0).c_str());

        for(int i = 1; i < reviewWords.size(); i++){

            //add new keyword if doesnt exist
            if( dict.find(reviewWords.at(i)) == dict.end()){

                dict[reviewWords.at(i)]  = make_pair(rating , 1);
            }else{
                dict[reviewWords.at(i)].first += rating;
                dict[reviewWords.at(i)].second ++;
            }


        }
    }
}


void fillStopWords(std::ifstream &inFile,
                   std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    string line;
    while(getline(inFile, line)){
        stopwords.insert(line);
    }

}

void rateReviews(std::ifstream &testFile,
                 std::unordered_map<std::string, std::pair<long, long> > &dict,
                 std::ofstream &ratingsFile) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 20


    string line;
    cout << "bang";
    while(getline(testFile, line)) {

        cout << "line";

        int predicted;
        vector<string> reviewWords;
        splitLine(line, reviewWords);


        for(int i = 0; i < reviewWords.size(); i++){
            predicted +=  (dict[reviewWords.at(i)].first) / (dict[reviewWords.at(i)].second);
        }

        predicted = predicted / reviewWords.size();

        ratingsFile << predicted;



    }




}

void removeEmptyWords(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
}

void removePunctuation(std::vector<std::string> &inTokens,
                       std::vector<std::string> &outTokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 10
    for (int i = 0; i <inTokens.size() ; i++) {
        inTokens.at(i).erase(remove_if(inTokens.at(i).begin(), inTokens.at(i).end(), ::ispunct), inTokens.at(i).end());
    }

    for(auto i: inTokens){
        if(i.size() > 0 ){
            outTokens.push_back(i);
        }
    }

}

void removeSingleLetterWords(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    for (int j = 0; j < tokens.size(); j++) {
        if( tokens.at(j).size() ==1  ){

            if(! ( isdigit(tokens.at(j)[0]) ) ) {

                tokens.erase(tokens.begin() + j);
                j--;
            }
        }
    }
}

void removeStopWords(std::vector<std::string> &tokens,
                     std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5

    for (int i = 0; i < tokens.size() ; i++) {
        if(stopwords.find(tokens.at(i)) != stopwords.end() ){
            tokens.erase(tokens.begin() + i);
            i--;
        }
    }

}

void removeWhiteSpaces(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    // You may want to use the trim() method from the trim.*pp files in a2.
}

void replaceHyphensWithSpaces(std::string &line) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    replace( line.begin(), line.end(), '-', ' '); // replace all 'x' to 'y'
}

void splitLine(std::string &line, std::vector<std::string> &words) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 10
    string temp;
    stringstream ssin(line);
    while (ssin >> temp) {
        words.push_back(temp);
    }
}



