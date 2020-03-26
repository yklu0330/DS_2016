#include<iostream>
#include<fstream>
#include<time.h>

using namespace std;

char s1[100][2000], s2[2000];

char toLower(char c) //convert capital letter to lowercase letter
{
	if (c >= 'A' && c <= 'Z') //if c is a capital letter 
		return c - 'A' + 'a'; //return the lowercase letter 
	return c; //if c is a lowercase letter, return c 
}

int strlen(char *str) //calculate the string length 
{
	int i;
	for (i = 0; *(str + i) != '\0'; i++) {} //break the for loop if the string ends 
	return i;
}

int BruteForce(char *pat, char *txt) //implement brute-force
{
	int lengthP, lengthT;
	lengthP = strlen(pat); //lengthP is the length of pattern
	lengthT = strlen(txt); //lengthT is the length of text
	
	for (int i = 0; i < lengthT - lengthP + 1; i++)
	{
		int j = 0; //j is the index of the compared letters
		while (j < lengthP && toLower(pat[j]) == toLower(txt[i + j])) //when the compared letters are the same
			j++; //move the index to compare the next letters
		if (j == lengthP) //the matching patterns are the same
			return i; //return the index
	}
	return -1;
}

int main(int argc, char **argv)
{
	ifstream ifs1(argv[1], ios::in);
	ifstream ifs2(argv[2], ios::in);
	
	int line = 1;
	int i, j, k ,m;
	for (i = 0; ifs1.getline(s1[i], 2000); i++) {};
	
	while (ifs2.getline(s2, 2000))
	{
		for (j = 0; j < i; j++)
		{
			int idx = 0;
			int lastResult;
			lastResult = BruteForce(s1[j], s2);
			if (lastResult != -1)
				cout << line << " " << lastResult + 1 << " \"" << s1[j] << "\" \"" << s2 << "\"" << endl;
			while (1)
			{
				int bfResult = BruteForce(s1[j], s2 + lastResult + 1);
				if (bfResult != -1)
				{
					lastResult = bfResult + lastResult + 1;
					cout << line << " " << lastResult + 1 << " \"" << s1[j] << "\" \"" << s2 << "\"" << endl;
				}
				else
					break;
			}
		}
		line++;
	}
	cout << endl << clock() << " ms" << endl;
	return 0; 
}
