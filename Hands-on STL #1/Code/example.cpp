#include <iostream>
#include <string>
#include <vector>

using namespace std;

void PrintInts(const vector<int>& ints)
{
    for (int i = 0; i < ints.size(); i++)
        cout << ints[i] << ' ';
    cout << endl;

    cout << "Vector size: " << ints.size() << endl;
    cout << "Vector capacity: " << ints.capacity() << endl;
    cout << endl;
}

void Example1()
{
    vector<int> vec(2, 5);
    PrintInts(vec);

    vec.push_back(7);
    PrintInts(vec);

    vec.emplace_back(5);
    PrintInts(vec);

    vec.erase(vec.begin() + 1);
    PrintInts(vec);

}

struct Volunteer
{
    string name;
    int age;
};

void PrintVolunteers(const vector<Volunteer>& volunteers)
{
    for (auto it = volunteers.begin(); it != volunteers.end(); it++)
        cout << "Voluteer name: " << it->name << " and age: " << it->age << endl;

    cout << endl;
}

void Example2()
{
    vector<Volunteer> youngVolunteers;

    Volunteer volunteer1;
    volunteer1.name = "Branza Alexandru";
    volunteer1.age = 39;

    youngVolunteers.push_back(volunteer1);
    PrintVolunteers(youngVolunteers);

    youngVolunteers.emplace_back("Smantana Andrei", 10);
    PrintVolunteers(youngVolunteers);

    Volunteer volunteer2;
    volunteer2.name = "Mamaliga Ionut";
    volunteer2.age = 4;

    youngVolunteers.insert(youngVolunteers.begin(), volunteer2);
    PrintVolunteers(youngVolunteers);

    youngVolunteers.erase(youngVolunteers.begin(), youngVolunteers.begin() + 2);
    PrintVolunteers(youngVolunteers);

}

int main()
{
    Example1();
    Example2();

    return 0;
}