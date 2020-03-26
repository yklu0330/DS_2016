#include<iostream>
#include<fstream>
#include<time.h>

using namespace std;

char s1[100][2000], s2[2000]; //s1 is the pattern, s2 is the text 
int fail[100][2000]; //fail is the failure function of the pattern

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

void FailureFunction(char *pat, int *f) //generate failure function
{
	int lengthP = strlen(pat);
	f[0] = -1; //the first number of the failure funciton must be -1
	for (int j = 1; j < lengthP; j++)
	{
		int i = f[j - 1];
		while ((toLower(*(pat + j)) != toLower(*(pat + i + 1))) && (i >= 0))
			i = f[i];
		if (toLower(*(pat + j)) == toLower(*(pat + i + 1)))
			f[j] = i + 1;
		else
			f[j] = -1;
	}
}

int KMP(char *pat, char *txt, int *f) //implement the KMP algorithm
{
	int posP = 0, posT = 0; //posP is the index of the pattern, posS is the index of the text 
	int lengthP = strlen(pat), lengthT = strlen(txt);
	while ((posP < lengthP) && (posT < lengthT)) //when posP and posT don't exceed the length of P and T
	{
		if (toLower(pat[posP]) == toLower(txt[posT])) //if the letter of pat and txt is the same
		{
			posP++; //move posP to compare the next letter
			posT++; //move posT to compare the next letter
		}
		else
		{
			if (posP == 0)
                
				posT++;
			else
				posP = f[posP - 1] + 1;
		}
	}
	if (posP < lengthP) return -1;
	else return posT - lengthP;
}

int main(int argc, char **argv)
{
	ifstream ifs1(argv[1], ios::in); //read the first file
	ifstream ifs2(argv[2], ios::in); //read the second file
	
	int line = 1; //the number of line
	int i, j, k ,m;
	for (i = 0; ifs1.getline(s1[i], 2000); i++) //generate all failure functions of all patterns
		FailureFunction(s1[i], fail[i]);
	
	while (ifs2.getline(s2, 2000))
	{
		for (j = 0; j < i; j++)
		{
			int lengthS = strlen(s2); //lengthS is the length of text
			int lastResult = KMP(s1[j], s2, fail[j]); //lastResult is the return value of KMP function
			if (lastResult != -1) //if KMP finds the pattern matched in the text
			{//print the result
				cout << line << " " << lastResult + 1 << " \"" << s1[j] << "\" \"" << s2 << "\"" << endl;
			}
			while (1)
			{
				int kmpResult = KMP(s1[j], s2 + lastResult + 1, fail[j]); //continue to find the rest matching pattern 
				if (kmpResult != -1) //if KMP find the matching pattern
				{
					lastResult = kmpResult + lastResult + 1; //update the new index of the matching text
					cout << line << " " << lastResult + 1 << " \"" << s1[j] << "\" \"" << s2 << "\"" << endl;
				}
				else //if KMP didn't find the matching pattern
					break; //break the loop
			}
		}
		line++;
	}
	cout << endl << clock() << " ms" << endl;
	return 0;
}
