#include <stdio.h>
#include <conio.h>
#include <cmath>
#include <clocale>
#include <iostream>
#include <cstdlib>
 
void ShowVector(int nCountRow, double ** mInitialMatrix, double * arRoots);
void StraightStroke(int nCountRow, double **mInitialMatrix, double *arRoots);
void ReverseStroke(int nCountRow, double **mInitialMatrix, double *arRoots, double *arNewRoots);
 
int main()
{
    setlocale(LC_ALL, "Russian");
 
    const int nCountRow = 4;
    double **mInitialMatrix, **mInitialMatrixBackup, *arRoots, *arRootsBackup, *arNewRoots;
    
    mInitialMatrix = new double*[nCountRow];
    mInitialMatrixBackup = new double*[nCountRow];
    
    for (int i = 0; i < nCountRow; ++i)
    {
        mInitialMatrix[i] = new double[nCountRow];
        mInitialMatrixBackup[i] = new double[nCountRow];
    }
        
    mInitialMatrix[0][0] = 2.21;  mInitialMatrix[0][1] = 3.65;  mInitialMatrix[0][2] = 1.69;  mInitialMatrix[0][3] = 6.99;
    mInitialMatrix[1][0] = 8.30;  mInitialMatrix[1][1] = 2.62;  mInitialMatrix[1][2] = 4.10;  mInitialMatrix[1][3] = 1.90;
    mInitialMatrix[2][0] = 3.92;  mInitialMatrix[2][1] = 8.45;  mInitialMatrix[2][2] = 7.78;  mInitialMatrix[2][3] = 2.46;
    mInitialMatrix[3][0] = 3.77;  mInitialMatrix[3][1] = 7.21;  mInitialMatrix[3][2] = 8.04;  mInitialMatrix[3][3] = 2.28;
    
    for (int i = 0; i < nCountRow; ++i)
        for (int j = 0; j < nCountRow; ++j)
            mInitialMatrixBackup[i][j] = mInitialMatrix[i][j];
    
    arRoots = new double[nCountRow];
    arRootsBackup = new double[nCountRow];
    arNewRoots = new double[nCountRow];
    
    arRoots[0] = -8.35;         arRootsBackup[0] = -8.35;
    arRoots[1] = -10.65;        arRootsBackup[1] = -10.65;
    arRoots[2] = 12.21;         arRootsBackup[2] = 12.21;
    arRoots[3] = 15.45;         arRootsBackup[3] = 15.45;

    
    ShowVector(nCountRow, mInitialMatrix, arRoots);
    StraightStroke(nCountRow, mInitialMatrix, arRoots);
    ShowVector(nCountRow, mInitialMatrix, arRoots);
    ReverseStroke(nCountRow, mInitialMatrix, arRoots, arNewRoots);

    printf("Ответ :\r\n");
    for(int i = 0; i < nCountRow; ++i)
        printf("X%d = %2.6f \n",i + 1, arNewRoots[i]);
    
    printf("\n");
    
    double H = 0.0;
    double F[4] = {0.0};

    for (int i = 0; i < nCountRow; ++i)
    {
        for (int j = 0; j < nCountRow; ++j)
            H += mInitialMatrixBackup[i][j] * arNewRoots[j];
            
        F[i] = H - arRootsBackup[i];
        H = 0.0;
        printf("F[%d] = %.6f\n", i, F[i]);
    }

    H = fabs(F[0]);

    for (int i = 0; i < nCountRow; ++i)
    {
        double dTempValue = fabs(F[i]);
        if (H < dTempValue)
            H = dTempValue;
    }

    printf("Норма = %.6f\n", H);
    
    free((void *)mInitialMatrix);
    free((void *)mInitialMatrixBackup);
    free((void *)arRoots);
    free((void *)arRootsBackup);
    free((void *)arNewRoots);
    
    return 0;
}
 
void ShowVector(int nCountRow, double ** mInitialMatrix, double * arRoots)
{
    for(int i = 0; i < nCountRow; ++i)
    {
        for(int j = 0; j < nCountRow + 1; ++j)
        {
            if(j == nCountRow)
                printf("| %2.2f ",arRoots[i]);
            else
                printf(" %2.2f",mInitialMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\r\n");
}
 
void StraightStroke(int nCountRow, double **mInitialMatrix, double *arRoots)
{
    double dTempValue;
    
    for(int k = 0, i, j, im; k < nCountRow - 1; ++k)
    {
        im = k;
        for(i = k + 1; i < nCountRow; ++i)
            if(fabs(mInitialMatrix[im][k]) < fabs(mInitialMatrix[i][k]))
                im = i;
                
        if(im != k)
        {
            for(j = 0; j < nCountRow; ++j)
            {
                dTempValue = mInitialMatrix[im][j];
                mInitialMatrix[im][j] = mInitialMatrix[k][j];
                mInitialMatrix[k][j]  = dTempValue;
            }
            dTempValue = arRoots[im];
            arRoots[im] = arRoots[k];
            arRoots[k]  = dTempValue;
        }
        for(i = k + 1; i< nCountRow; ++i)
        {
            dTempValue = mInitialMatrix[i][k] / mInitialMatrix[k][k];
            mInitialMatrix[i][k] = 0;
            arRoots[i]   = arRoots[i] - dTempValue * arRoots[k];
            
            if(dTempValue != 0)
                for(j = k + 1; j < nCountRow; ++j)
                    mInitialMatrix[i][j] -= dTempValue * mInitialMatrix[k][j];
        }
    }
}
 
void ReverseStroke(int nCountRow, double **mInitialMatrix, double *arRoots, double *arNewRoots)
{
    double dTempValue = 0;
    int nTempCount = nCountRow - 1;
    
    arNewRoots[nTempCount] = arRoots[nTempCount] / mInitialMatrix[nTempCount][nTempCount];
    
    for(int i = nCountRow - 2, j; 0 <= i; --i)
    {
        dTempValue = 0;
        for(j = i + 1; j < nCountRow; ++j)
        {
            dTempValue += mInitialMatrix[i][j] * arNewRoots[j];
        }
        arNewRoots[i] = (arRoots[i] - dTempValue) / mInitialMatrix[i][i];
    }
}