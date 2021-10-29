#include <iostream>
using namespace std;

int main()
{
    int numofyr;
    float price, interest, fnlinterest, totalamnt;
    cout << " Input Price";
    cin >> price;
    cout << " Input Number of year";
    cin >> numofyr;
    if (numofyr == 1)
    {
        interest = price * 0.01;
    }
    else if (numofyr == 2)
    {
        interest = price * 0.02;
    }
    else
    {
        interest = price * 0.03;
    }
    fnlinterest = interest * (numofyr * 12);
    totalamnt = price + fnlinterest;
    cout << "Numver of Years:";
    cout << numofyr;
    cout << "Interest Amount:";
    cout << fnlinterest;
    cout << endl;
    cout << "Total amount to pay";
    cout << totalamnt;
    return 0;
}