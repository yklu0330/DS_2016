//
//  main.cpp
//  HW3
//
//  Created by 呂翊愷 on 2016/12/21.
//  Copyright © 2016年 呂翊愷. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <math.h>

using namespace std;

struct Node{ // nodes of the optimal binary tree
    int left; // the index of the left child
    int right; // the index of the right child
    int count; // the frequency of the word
    int word; // the code of the word
}node[700];
int nodeCount; // count the number of nodes

class Compare // the comparator of the priority queue
{
public:
    bool operator() (int a, int b) // use operator overloading to compare the count of the node
    {
        return node[a].count > node[b].count;
    }
};

int binaryTree(int *count) // build the optimal binary tree
{
    nodeCount = 0;
    priority_queue<int, vector<int>, Compare> leaf; // the priority queue which stores the indexs of the nodes
    int i;
    for (i = 0; i < 256; i++)
    {
        if (count[i] != 0)
        {
            // initialize the data of the nodes
            int currentIndex = nodeCount;
            node[currentIndex].count = count[i];
            node[currentIndex].left = -1;
            node[currentIndex].right = -1;
            node[currentIndex].word = i;
            // push the index of the node into the priority queue
            leaf.push(currentIndex);
            nodeCount++;
        }
    }
    
    while (leaf.size() >= 2) // when leaf still have the datas
    {
        // get the index of the node which have the smallest count
        int a = leaf.top(); leaf.pop();
        int b = leaf.top(); leaf.pop();
        // create a new node
        int currentIndex = nodeCount;
        node[currentIndex].count = node[a].count + node[b].count;
        node[currentIndex].left = a;
        node[currentIndex].right = b;
        // push the new node to the priority queue
        leaf.push(currentIndex);
        nodeCount++;
    }
    return leaf.top(); // return the first data of the priority queue
}

int path[50]; // the path from the root to the leaf
int wordCode[256][50]; // the compressed code of every word
int codeLen[256]; // the length of the compressed code
void encode(int i, int level) // walk in the tree from root to leaf
{
    if (node[i].left == -1 && node[i].right == -1) // if find the leaf
    {
        for (int j = 0; j < level; j++)
        {
            // record the compressed code and the length of the code
            wordCode[node[i].word][j] = path[j];
            codeLen[node[i].word] = level;
        }
        return;
    }
    
    path[level] = 0;
    encode(node[i].left, level + 1); // walk to the left child
    path[level] = 1;
    encode(node[i].right, level + 1); // walk to the right child
}


char originCode; // the original code from the compressed code
int decode(unsigned char data, int root, int idx) // walk in the tree from root to leaf
{
    if (data == 0)
    {
        idx = node[idx].left;
    }
    else if (data == 1)
    {
        idx = node[idx].right;
    }
    if (node[idx].left == -1 && node[idx].right == -1)
    {
        originCode = node[idx].word;
        return root;
    }
    return idx;
}

int main(int argc, const char * argv[])
{
    while (1)
    {
        string cmd;
        cin >> cmd;
        if (cmd == "Compress")
        {
            string inputfile, outputfile;
            
            // open the input file
            cout << "Input filename: ";
            cin >> inputfile;
            ifstream ifs(inputfile.c_str(), ios::binary);
            
            // calculate the frequency of the word
            int count[256];project3
            int i;
            for (i = 0; i < 256; i++)
                count[i] = 0;
            char word;
            while (ifs.read(&word, 1))
            {
                count[(unsigned char)word]++;
            }
            ifs.close();
            ifs.open(inputfile.c_str());
            
            int root = binaryTree(count); // build the optimal binary tree
            encode(root, 0); // compressed the code
            
            // open the output file
            cout << "Output filename: ";
            cin >> outputfile;
            ofstream ofs(outputfile.c_str(), ios::binary);
            
            // write the address of nodes and root into the output file
            ofs.write((char*)node, sizeof(node));
            ofs.write((char*)&root, sizeof(root));
            
            int bitNum = 0;
            ofs.write((char*)&bitNum, sizeof(bitNum));
            char c = 0; // c records the new code
            while (ifs.read(&word, 1)) // read the word in the input file
            {
                int i = 0;
                for (i = 0; i < codeLen[(unsigned char)word]; i++)
                {
                    c = c << 1; // shift left 1 bit to put the new code
                    c = c | wordCode[(unsigned char)word][i]; // use "or" to add the new code in c
                    bitNum++;
                    if (bitNum == 8) // if c is 8-bit long
                    {
                        ofs.write(&c, 1); // write c into the output file
                        c = 0;
                        bitNum = 0;
                    }
                }
            }
            ofs << c; // write the exist code which isn't 8-bit long
            ofs.seekp(sizeof(node) + sizeof(root), ios::beg);
            ofs.write((char*)&bitNum, sizeof(bitNum));
            
            
            ifs.close();
            ofs.close();
            
        }
        else if (cmd == "Extract")
        {
            string inputfile, outputfile;
            
            // open the input file
            cout << "Input filename: ";
            cin >> inputfile;
            ifstream ifs(inputfile.c_str(), ios::binary);
            
            // read the address of the nodes and the root in input file
            ifs.read((char*)node, sizeof(node));
            int rootIdx;
            ifs.read((char*)&rootIdx, sizeof(rootIdx));
            int bitNum;
            ifs.read((char*)&bitNum, sizeof(bitNum));
            
            // open the output file
            cout << "Output filename: ";
            cin >> outputfile;
            ofstream ofs(outputfile.c_str(), ios::binary);
            
            // read the code in input file
            char data;
            int idx = rootIdx;
            while (ifs.read(&data, 1))
            {
                int time = 8;
                if (ifs.peek() == EOF)
                {
                    time = bitNum;
                }
                for (int i = 0; i < time; i++)
                {
                    int dataBit;
                    double base = 2.0;
                    dataBit = ((data & (int)pow(base, (double)time - 1)) >> (time - 1));
                    idx = decode(dataBit, rootIdx, idx);
                    if (idx == rootIdx)
                    {
                        ofs << (char)originCode;
                    }
                    data = data << 1;
                }
            }
        }
        else if (cmd == "quit")
        {
            break;
        }
    }
    return 0;
}

