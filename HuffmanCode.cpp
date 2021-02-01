//
//  HuffmanCode.cpp
//  
//
//  Created by Cecilia on 2020/12/28.
//

#include <string>
#include <iostream>
#include <cstring>
#include "stdio.h"
using namespace std;

struct HuffmanNode {
    int weight;
    char ch;
    string code = "";
    int left = -1;
    int right = -1;
    int parent = -1;
};

class HuffmanCode {
public:
    HuffmanCode(int ascii[]);
    ~HuffmanCode();
    void getMin(int &first, int &second, int parent);
    void merge(int first, int second, int parent);
    void encode(string Code[]);
private:
    HuffmanNode *HuffmanTree;
    int leafNum;
};

HuffmanCode::HuffmanCode(int ascii[]) {
    HuffmanTree = new HuffmanNode[256];
    leafNum = 0;
    //initialize HuffmanTree array

    for (int i=0; i<256; i++) {
        if (ascii[i] != 0) {
            HuffmanTree[leafNum].ch = i;
            HuffmanTree[leafNum].weight = ascii[i];
            leafNum++;
        }
    }

    // merge the two smallest nodes
    int first, second;
    cout << leafNum << endl;
    for (int i=leafNum; i<(2*leafNum-1); i++) {
        getMin(first, second, i);
        merge(first,second,i);
    }
}

HuffmanCode::~HuffmanCode() {
    delete [] HuffmanTree;
}

void HuffmanCode::getMin(int &first, int &second, int parent) {
    int weight = 0;
    int i;
    // find the node with the smallest weight
    for (i=0; i<parent; i++){
        if (HuffmanTree[i].parent != -1) continue;
        if (weight == 0) {
            weight = HuffmanTree[i].weight;
            first = i;
        }
        else if (HuffmanTree[i].weight < weight) {
            weight = HuffmanTree[i].weight;
            first = i;
        }
        else if (HuffmanTree[i].weight == weight) {
            if (HuffmanTree[i].ch < HuffmanTree[first].ch) {
                weight = HuffmanTree[i].weight;
                first = i;
            }
        }
    }

    // find the node with the second smallest weight
    weight = 0;
    for (i=0; i<parent; i++){
        if (HuffmanTree[i].parent != -1 || i == first) continue;
        if (weight == 0) {
            weight = HuffmanTree[i].weight;
            second = i;
        }
        else if (HuffmanTree[i].weight < weight) {
            weight = HuffmanTree[i].weight;
            second = i;
        }
        else if (HuffmanTree[i].weight == weight) {
            if (HuffmanTree[i].ch < HuffmanTree[second].ch) {
                weight = HuffmanTree[i].weight;
                second = i;
            }
        }
    }
}

void HuffmanCode::merge(int first, int second, int parent) {
    HuffmanTree[first].parent = HuffmanTree[second].parent = parent;
    HuffmanTree[parent].left = first;
    HuffmanTree[parent].right = second;
    HuffmanTree[parent].weight = HuffmanTree[first].weight + HuffmanTree[second].weight;
    HuffmanTree[parent].ch = HuffmanTree[first].ch;
}

void HuffmanCode::encode(string Code[]) {
    string code;
    int parent;
    for (int i=0; i<leafNum; i++) {
        int j = i;
        code = "";
        while (HuffmanTree[j].parent != -1) {
            parent = HuffmanTree[j].parent;
            if (j == HuffmanTree[parent].left) code += "0";
            else code += "1";
            j = parent;
        }
        for (int k=int(code.size()-1); k>=0; k--) {
            HuffmanTree[i].code +=code[k];
        }
        Code[int(HuffmanTree[i].ch)] = HuffmanTree[i].code;
    }

}
int main() {
    int ch;
    string str;
    int ascii[256] = {0};
    fflush(stdin);
    getline(cin, str);
    for (int i=0; i<str.size();i++) {
        ascii[str[i]]++;
        cout << str[i] << endl;
    }
    // get the char and corresponding frequency
//    while ((ch = getchar()) !='\0' && ch != EOF && ch !='\n' && ch !='\r') {
//        ascii[ch] = ascii[ch] + 1;
//        str.push_back(ch);
//    }
    for (int i=0; i<256; i++) cout << i << " " << ascii[i] << endl;
    HuffmanCode build(ascii);

    string* Codeset = new string[256];
    build.encode(Codeset);
    string code = "";
    for (int i=0; i<int(str.size());i++) {
        code += Codeset[int(str[i])];
    }
    cout << code;

}

