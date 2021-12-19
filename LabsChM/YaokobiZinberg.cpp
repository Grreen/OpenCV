#include <iostream>
#include <math.h>

using namespace std;
#define epsilon 0.00001

void ShowSystem(double** a, double* y, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << a[i][j] << "*x" << j;
            if (j < n - 1)
                cout << " + ";
        }
        cout << " = " << y[i] << endl;
    }
    return;
}

double* Seidel(double** a, double* y, int n)
{
    double* x = new double[n];
    double* TempX = new double[n];
    double norm;
    int it = 0;

    do {
        for (int i = 0; i < n; i++)
        {
            TempX[i] = y[i];
            for (int g = 0; g < n; g++) {
                if (i != g)
                    TempX[i] -= a[i][g] * TempX[g];
            }
            TempX[i] /= a[i][i];
        }
        it++;
        norm = abs(x[0] - TempX[0]);
        for (int h = 0; h < n; h++)
        {
            if (abs(x[h] - TempX[h]) > norm)
                norm = abs(x[h] - TempX[h]);
            x[h] = TempX[h];
        }
    } while (norm > epsilon);
    delete[] TempX;

    cout << "Seidel iterations: " << it << endl;
    return x;
}

double* Jacobi (double** a, double* y, int n)
{
    double* x = new double[n];
    double* TempX = new double[n];
    double norm;
    int it = 0;

    do {
        for (int i = 0; i < n; i++)
        {
            TempX[i] = y[i];
            for (int g = 0; g < n; g++) {
                if (i != g)
                    TempX[i] -= a[i][g] * x[g];
            }
            TempX[i] /= a[i][i];
        }
        it++;
        norm = abs(x[0] - TempX[0]);
        for (int h = 0; h < n; h++)
        {
            if (abs(x[h] - TempX[h]) > norm)
                norm = abs(x[h] - TempX[h]);
            x[h] = TempX[h];
        }
    } while (norm > epsilon);
    delete[] TempX;

    cout << "Jacobi iterations: " << it << endl;
    return x;
}


int main()
{
    double** mInitialMatrix, * arRoots, * x;
    int n = 4;
    mInitialMatrix = new double* [n];
    arRoots = new double[n];
    x = new double[n];

    for (int i = 0; i < n; ++i)
        mInitialMatrix[i] = new double[n];

    mInitialMatrix[0][0] = 20.21; mInitialMatrix[0][1] = 3.65;  mInitialMatrix[0][2] = 1.69;  mInitialMatrix[0][3] = 6.99;
    mInitialMatrix[1][0] = 8.30;  mInitialMatrix[1][1] = 24.62; mInitialMatrix[1][2] = 4.10;  mInitialMatrix[1][3] = 1.90;
    mInitialMatrix[2][0] = 3.92;  mInitialMatrix[2][1] = 8.45;  mInitialMatrix[2][2] = 37.78; mInitialMatrix[2][3] = 2.46;
    mInitialMatrix[3][0] = 3.77;  mInitialMatrix[3][1] = 7.21;  mInitialMatrix[3][2] = 8.04;  mInitialMatrix[3][3] = 25.28;

    arRoots[0] = -8.35;
    arRoots[1] = -10.65;
    arRoots[2] = 12.21;
    arRoots[3] = 15.45;
    
    ShowSystem(mInitialMatrix, arRoots, n);
    x = Jacobi(mInitialMatrix, arRoots, n);
    
    cout << "------------Jacobi------------" << endl;
    for (int i = 0; i < n; ++i)
        printf("X%d = %.6f\n", i + 1, x[i]);

    cout << "------------Seidel------------" << endl;
    x = Seidel(mInitialMatrix, arRoots, n);

    for (int i = 0; i < n; ++i)
        printf("X%d = %.6f\n", i + 1, x[i]);

    return 0;
}