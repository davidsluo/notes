#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

double calcVariance(double sigmaX, double sigmaXSquared, int n) {
  return (1/(double) n * sigmaXSquared) - pow((1/(double) n * sigmaX), 2);
}

double calcAverage(double sigmaX, int n) {
  return sigmaX / n;
}

int main() {
        cout << "asdfasdf" << endl;

        double sigmaX = 0;
        double sigmaXSquared = 0;

        int n = 0;
        string line;
        getline(cin, line);
        stringstream lineStream(line);

        double number;
        while (lineStream >> number) {
                sigmaX += number;
                sigmaXSquared += pow(number, 2);
                n++;
        }

        cout << "Sum\t" << sigmaX << endl;
        cout << "Average\t" << calcAverage(sigmaX, n) << endl;
        cout << "Sum of squares\t" << sigmaXSquared << endl;
        cout << "Variance\t" << calcVariance(sigmaX, sigmaXSquared, n) << endl;
}
