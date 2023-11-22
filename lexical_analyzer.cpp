#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

const int WORD_NUM = 26;
const string WORD[WORD_NUM] = {
    "int", "void", "return", "const", "main", "struct", "+",  "-",  "*",  "/",
    "%",   "=",    ">",      "<",     "==",   "<=", ">=", "!=", "&&",
    "||",  "(",    ")",      "{",     "}",    ";",  ",",
};
const string OPERATOR = "+-*/%><=&|";
const string SEPARATER = "(){};,[]";
int kws = 0, kwe = 6, ops = 6, ope = 20, ses = 20, see = 26;

class Analyzer {
  private:
    vector<string> lines;
    string fileName;
    ofstream fout;

    int isWord(string word) {
        for(int i = 0; i < WORD_NUM; i++) {
            if(word == WORD[i])
                return i;
        }
        return -1;
    }
    bool isKeyWord(int idx) {
        return kws <= idx && idx < kwe;
    }
    bool isOperator(int idx) {
        return ops <= idx && idx < ope;
    }
    bool isOperator(char ch) {
        return OPERATOR.find(ch) != OPERATOR.npos;
    }
    bool isSeparater(int idx) {
        return ses <= idx && idx < see;
    }
    bool isSeparater(char ch) {
        return SEPARATER.find(ch) != SEPARATER.npos;
    }
    inline bool isNumber(char ch) {
        return ch >= '0' && ch <='9';
    }
    bool isInt(string word) {
        for(int i = 0; i < word.size(); i++) {
            if(!isNumber(word[i]))
                return false;
        }
        return true;
    }
    inline bool isCharacter(char ch) {
        return ch >= 'a' && ch <= 'z' || ch >='A' && ch <= 'Z';
    }
    bool isPartOfIdentifier(char c) {
        return isCharacter(c) || isNumber(c) || c == '_';
    }
    bool isIdentifier(string word) {
        if(isNumber(word[0])) {
            return false;
        }
        for(int i = 1; i < word.size(); i++) {
            if(!isPartOfIdentifier(word[i]))
                return false;
        }
        return true;
    }
    inline void record(string word, string type, string content, bool write) {
        if(write)
            fout << word << " " << "<" << type << "," << content << ">" << endl;
    }
    string to_string(int val) {
        stringstream ss;
        ss << val;
        string result;
        ss >> result;
        return result;
    }
    //分析一个单词
    bool anaylyseWord(string word, bool write) {
        if(word.empty()) {
            return true;
        }
        int idx = isWord(word);
        if(idx > -1) {
            string type;
            if(isKeyWord(idx)) type = "KW";
            if(isOperator(idx)) type = "OP";
            if (isSeparater(idx)) type = "SE";
            record(word, type, to_string(idx + 1), write);
            return true;
        } else {
            if(isIdentifier(word)) {
                record(word,"IND", word , write);
                return true;
            }
            if(isInt(word)) {
                record(word,"INT", word , write);
                return true;
            }
        }
        fout << "ERROR detected!" << endl;
        cout << "ERROR detected!" << endl;
        return false;
    }
    //去除字符串前后空格
    string trim(string s) { 
        if(s == "") {
            return "";
        }
        int l = 0, r = s.size() - 1;
        while(s[l] == ' ' && l < s.size()) l++;
        while(s[r] == ' ' && r > l) r--;
        return s.substr(l,r + 1);
    }

    bool check(char a, char b) {
        if ((isOperator(a) && !isOperator(b)) ||
            (!isOperator(a) && isOperator(b)) || isSeparater(a) ||
            (!isSeparater(a) && isSeparater(b)))
            return false;
        return true;

    }

  public:
    Analyzer(string fileName) {
        readFile(fileName);
    }
    ~Analyzer() {
        fout.close();
    }

    void readFile(string fileName) {
        this->fileName = fileName;
        fstream fin(fileName.c_str());
        if (!fin.is_open()) {
            throw "无法打开文件";
        }
        string line;
        while (getline(fin, line)) {
            line = trim(line);
            if(!line.empty())
                lines.push_back(line);
        }
        fin.close();
        // fout.open("token.txt");
        fout.open(fileName.substr(0,fileName.find_last_of(".")) + ".out");
    }
    void printLines() {
        for (int i = 0; i < lines.size(); i++) {
            cout << i << ":" << lines[i] << endl;
        }
    }
    void analyse() {
        int l = 0;
        string word = "";
        while(l < lines.size()) {
            string line = lines[l++];
            word.clear();
            for(int i = 0; i < line.size(); i++) {
                if(line[i] == ' ' || line[i] == '\t') {
                    if(!anaylyseWord(word,true)) return;
                    word.clear();
                    continue;
                }
                if(!check(word[word.size() - 1], line[i])) {
                    if(!anaylyseWord(word,true)) return;
                    word.clear();
                }
                word += line[i];
            }
            anaylyseWord(word, true);
        }
    }
};

int main() {
    try{
    Analyzer analyzer("a.sy");
    analyzer.analyse();
    system("pause");
    } catch(const char * msg) {
        cout << msg << endl;
    }
    return 0;
}