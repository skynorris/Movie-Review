////////////////////////////////////////////////////////////////////////////////
// File Name:      a2.cpp
//
// Author:         Skyler Norris
// CS email:       skyler@cs.wisc.edu
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


/**
 * @brief Cleans the text fie passed in and outputs it to new file
 *
 * @param inFIle file passed in to be cleaned.
 * ..
 * @param outFile  where the new clean data is outputted to.
 *
 * @param stopwords list of words to delete in infile.
 *
 * */
void cleanData(std::ifstream &inFile, std::ofstream &outFile,
               std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // # of lines of code in Gerald's implementation: 13
    //fields
    vector<string> reviewPunc;
    vector<string> review;
    string line;


    //do for whole file
    while (std::getline(inFile, line)) {

        //   1. Replace hyphens with spaces.
        replaceHyphensWithSpaces(line);

        //   2. Split the line of text into individual words.
        splitLine(line, reviewPunc);
    }


    //   3. Remove the punctuation marks from the words.
    //   5. Remove the empty words.
    removePunctuation(reviewPunc, review);

    //   6. Remove words with just one character in them. You should NOT remove
    // numbers in this step because if you do so, you'll lose the ratings.`
    removeSingleLetterWords(review);

    //   7. Remove stopwords.
    removeStopWords(review, stopwords);

    //write clean review to outfile
    for(int i = 0; i < review.size(); i++){
        if((review.at(i).compare("eol")) == 0){
            if(i != review.size() -1){
                outFile << "\n";
            }
        }
        else {

            outFile << review.at(i);

            if((review.at(i+1).compare("eol")) != 0){
                outFile << " ";
            }
        }
    }

    inFile.close();
    outFile.close();

}

/**
 * @brief Calculates how many times a word is mentioned in a file, and is paired with the ratings
 * associated with that word in a dictionary
 *
 * @param newInFile File to get words from
 * ..
 * @param dict  The dictionary to be filled.
 *
 *  */
void fillDictionary(std::ifstream &newInFile,
                    std::unordered_map<std::string, std::pair<long, long>> &dict) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 20

    //sores every line of file
    string line;

    while(getline(newInFile, line)){

        //vecotr to hold words of each line
        vector<string> reviewWords;

        splitLine(line, reviewWords);
        //rating always first word
        int rating = atoi(reviewWords.at(0).c_str());

        for(int i = 1; i < reviewWords.size(); i++){



            //add new keyword if doesnt exist
            if( dict.find(reviewWords.at(i)) == dict.end()){

                dict[reviewWords.at(i)]  = make_pair(rating , 1);

                //update value if already exists
            }else{
                dict[reviewWords.at(i)].first += rating;
                dict[reviewWords.at(i)].second ++;
            }

        }
    }

    newInFile.close();

}


/**
 * @brief Fills a set with stopwords from a file.
 *
 * @param inFile File to get stopwords from
 * ..
 * @param stopwords The set to hold all the stopwords from the file
 *
 *  */
void fillStopWords(std::ifstream &inFile,
                   std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    string line;
    while(getline(inFile, line)){
        stopwords.insert(line);
    }

    inFile.close();
}


/**
 * @brief Predicts the ratings for a rating with no numerical rating.  Does so by
 * using the words in the rating and seeing how they have been rated previously.
 *
 * @param testFile File to get reviews from.
 * ..
 * @param dict  The dictionary that holds the ratings for specific words.
 *
 * @param ratingsFile File to output predicted ratings.
 *
 *  */
void rateReviews(std::ifstream &testFile,
                 std::unordered_map<std::string, std::pair<long, long>> &dict,
                 std::ofstream &ratingsFile) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 20

    //do for whole file
    string line;
    while(getline(testFile, line)){

        double predicted = 0;
        vector<string> reviewWords;
        splitLine(line, reviewWords);

        //gets rid of eol string
        reviewWords.pop_back();

        //calculate predicted review
        for (int i = 0; i < reviewWords.size(); i++) {

            //if not in dictioanry give it rating of 2
            if (dict.find(reviewWords.at(i)) == dict.end()) {
                predicted += 2;
            } else {
                //calculate rating of words
                double first = (dict[reviewWords.at(i)].first);
                double second = (dict[reviewWords.at(i)].second);
                predicted += first / second;
            }
        }

        //output to file
        predicted = predicted / reviewWords.size();
        ratingsFile << fixed << setprecision(2) << predicted;

        //dont output extra line at end of file
        if(testFile.good()){
            ratingsFile << endl;
        }
    }

    testFile.close();
    ratingsFile.close();


}

/**
 * @brief Removes empty words from a  vector of strings.
 *
 * @param tokens the vector of strings.
 *  */

void removeEmptyWords(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
}

/**
 * @brief Removes punctuation from a vector of strings.
 *
 * @param inTokens vector before punc removed
 * ..
 * @param outTokens  vector after punc is removed.
 *
 *  */
void removePunctuation(std::vector<std::string> &inTokens,
                       std::vector<std::string> &outTokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 10

    //remove punc
    for (int i = 0; i <inTokens.size() ; i++) {

        inTokens.at(i).erase(remove_if(inTokens.at(i).begin(), inTokens.at(i).end(), ::ispunct), inTokens.at(i).end());

    }

    //push to new vector
    for(auto i: inTokens){
        if(i.size() > 0 ){

            outTokens.push_back(i);

        }
    }

}


/**
 * @brief Removes single letter worfs from a vector of strings.
 *
 * @param tokens The vector of strings
 *
 *  */
void removeSingleLetterWords(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    for (int j = 0; j < tokens.size(); j++) {
        if( tokens.at(j).size() ==1  ){
            if(! ( isdigit(tokens.at(j)[0]) ) ) {

                //remove and update j so iterator stays in place
                tokens.erase(tokens.begin() + j);
                j--;

            }
        }
    }
}


/**
 * @brief Removes a pre determined set of words from a vector of strings.
 *
 * @param inTokens vectorof strings.
 * ..
 * @param stopwords  Set contating the words to remove from vector.
 *
 *  */
void removeStopWords(std::vector<std::string> &tokens,
                     std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5

    for (int i = 0; i < tokens.size() ; i++) {
        if(stopwords.find(tokens.at(i)) != stopwords.end() ){

            //if its a stop words remove and update j
            tokens.erase(tokens.begin() + i);
            i--;

        }
    }

}

/**
 * @brief Removes whitespaces from a vector of strings.
 *
 * @param tokens vector of strings
 *
 *  */
void removeWhiteSpaces(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    // You may want to use the trim() method from the trim.*pp files in a2.
}


/**
 * @brief Replaces hyphens with spaces in a string.
 *
 * @param line The string to be worked on.
 *
 *  */
void replaceHyphensWithSpaces(std::string &line) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    replace( line.begin(), line.end(), '-', ' '); // replace all '-' to ' '
}


/**
 * @brief Given a string, this function splits the string so at indiviudal words
 * are added one by one into a vector of strings.
 *
 * @param line The string.
 *
 * @param words The vector to hold the words.
 *
 *  */
void splitLine(std::string &line, std::vector<std::string> &words) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 10

    // move to vector of strings
    string temp;
    stringstream ssin(line);
    while (ssin >> temp) {
        words.push_back(temp);
    }

    //end of line marker
    words.push_back("eol");
}



