#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Problem
{
    string name;
    string speciality;
};

int main()
{
    ifstream inFile("input.txt");

    int n, m;
    inFile >> n;

    vector<Problem> problems(n);
    for (Problem& p : problems)
    {
        inFile >> p.name >> p.speciality;
    }

	// Write your code here
 
    return 0;
}