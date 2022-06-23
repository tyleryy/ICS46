// WordChecker.cpp
//
// ICS 46 Spring 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <algorithm>



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    if(words.contains(word)){
        return true;
    }
    return false;
}

void WordChecker::deleteWord(std::vector<std::string>& suggestions, std::string word) const
{
    std::string copy_word;
    for(int i = 0; i < word.size(); i++)
    {
        copy_word = word;
        copy_word.erase(i,1);
        if(wordExists(copy_word))
        {
            suggestions.push_back(copy_word);
        }
    }
}

void WordChecker::swapChars(std::vector<std::string>& suggestions, std::string word) const
{
    std::string copy_word;
    for(int i = 0; i < word.size()-1; i++)
    {
        copy_word = word;
        std::swap(copy_word[i], copy_word[i+1]);
        if(wordExists(copy_word))
        {
            suggestions.push_back(copy_word);
        }
    }
}

void WordChecker::addAlphabet(std::vector<std::string>& suggestions, std::string word) const 
{
    std::string copy_word;
    std::string insert;
    for(int i = 0; i < word.size()+1; i++)
    {
        for(char c = 'A'; c<='Z'; c++)
        {
            copy_word = word;
            insert = std::string{c};
            copy_word.insert(i, insert);
            if(wordExists(copy_word))
            {
                suggestions.push_back(copy_word);
            }
        }
    }
}

void WordChecker::replaceAlphabet(std::vector<std::string>& suggestions,  std::string word) const
{
    std::string copy_word;
    for(int i = 0; i < word.size(); i++)
    {
        for(char c = 'A'; c<='Z'; c++)
        {
            copy_word = word;
            copy_word.at(i) = c;
            if (wordExists(copy_word))
            {
                suggestions.push_back(copy_word);
            }
        }
    }
}

void WordChecker::splitWord(std::vector<std::string>& suggestions, std::string word) const
{
    std::string copy_word;
    std::string substring1;
    std::string substring2;

    for(int i = 1; i < word.size()-1; i++)
    {
        copy_word = word;
        copy_word.insert(i, " "); 
        substring1 = copy_word.substr(0,i);
        substring2 = copy_word.substr(i+1);

        if ((wordExists(substring1)) && (wordExists(substring2)))
        {
            suggestions.push_back(copy_word);
        }
    }
}






std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    // Nonsensical code because the compiler requires the member variables
    // 'words' to be used somewhere, or else it becomes a warning (which
    // turns into an error).
    //words.isImplemented();
    std::vector<std::string> suggestions;
    splitWord(suggestions, word);
    replaceAlphabet(suggestions, word);
    addAlphabet(suggestions, word);
    swapChars(suggestions, word);
    deleteWord(suggestions, word);
    suggestions.erase(unique(suggestions.begin(), suggestions.end()), suggestions.end() );



    

    return suggestions;
}

