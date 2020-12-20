#include <iostream>
#include <fstream>
#include<string>
#include<cstdlib>
#include <sstream>
#include <cstring>
#include<time.h>
#include<math.h>
#include<ctime>
#include<vector>

using namespace std;

void randomPath(int* path,int citiesNum) {
    vector<int> tempNums;
    for (int i = 1; i < citiesNum; i++)
        tempNums.push_back(i);

    path[0] = 0;
    path[citiesNum] = 0;
    int currentVertex;
    vector<int> cleaningVector;
    for (int i = 1; i < citiesNum; i++) {
        currentVertex = rand() % tempNums.size();
        vector<int>::iterator it;
        it = tempNums.begin() + currentVertex;
        path[i] = tempNums.at(currentVertex);
        tempNums.erase(it);
       
    }
}

void printPath(int arr[], int size) {

    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            cout << arr[i] << endl;
        }
        else
            cout << arr[i] << " -> ";

    }
}
void copyArray(int* arrA, int* arrB, int size) {
    for (int i = 0; i < size; i++)
        arrA[i] = arrB[i];
}

int countCost(int* path, int citiesNum, int** cityMatrix) {
    int cost = 0;
    for (int i = 0; i < citiesNum; i++) {
        cost += cityMatrix[path[i]][path[i + 1]];
    }
    cost += cityMatrix[path[citiesNum-1]][path[0]];
    return cost;
}

bool aspirationCriteria(int* path, int* currentPath, int difference, int citiesNum, int** cityMatrix) {
    if (countCost(currentPath, citiesNum , cityMatrix) - countCost(path, citiesNum , cityMatrix) > difference)
        return true;
    else
        return false;
}
/* nalezy poprawic to ze jak sa taie same indeksy i oraz j to zeby nie swapowac ich !*/
void newNeighbours(int* path, int citiesNum,int** tabuList,int** cityMatrix,int diff,int tabuPeroid) {
    int* tempPath = new int[citiesNum + 1]; // tablica przechowujaca oryginalna tablice path, aby jej nie zmieniac 
    int* nextPath = new int[citiesNum + 1];
    copyArray(tempPath, path, citiesNum + 1);
    int difference = INT_MIN;
    int bestDifference = difference;
    int temp = 1;
    int a = 0;
    int b = 0;
    for (int i = 1; i < citiesNum-1; i++) {
        for (int j = temp; j < citiesNum - 1; j++) {
            swap(tempPath[i], tempPath[j]);

            difference = 1 * (countCost(path, citiesNum, cityMatrix) - countCost(tempPath, citiesNum, cityMatrix));

            if (bestDifference < difference) {
                a = i;
                b = j + 1;
                if (a > b) {
                    swap(a, b);
                }
                if ((tabuList[i][j + 1] > 0 && aspirationCriteria(tempPath, path, diff, citiesNum, cityMatrix))
                    || tabuList[i][j + 1] == 0) {
                    bestDifference = difference;
                    copyArray(nextPath, tempPath, citiesNum + 1);

                    
                }

            }
            copyArray(tempPath,path,citiesNum+1);
           
        }
        

        temp++;
    }
    tabuList[a][b] = tabuPeroid;
    copyArray(path, nextPath, citiesNum + 1);
    delete[]tempPath;
    delete[]nextPath;
}


void decrementTabuList(int** tabuList, int citiesNum) {
    int temp = 0;
    for (int i = 0; i < citiesNum - 1;i++) {
        for (int j = temp; j < citiesNum-1; j++) {
            if (tabuList[i][j]!=0) {
                tabuList[i][j]--;
            }
        }
        temp++;
    }
}


int main() {
    srand(time(NULL));
    fstream file;
    string tempLine;
    int hamiltonOpt;
    int citiesNum;
    string fileName;
    cout << "Name of instance : ";
    cin >> fileName;
    file.open(fileName , std::ios::in | std::ios::out);
    if (file.is_open()) {
        getline(file, tempLine);
        getline(file, tempLine);
        citiesNum = atoi(tempLine.c_str());
        int** cityMatrix = new int* [citiesNum];
        for (int i = 0; i < citiesNum; i++)
            cityMatrix[i] = new int[citiesNum];


        for (int i = 0; i < citiesNum; i++) {
            getline(file, tempLine);
            stringstream ss;
            ss << tempLine;
            int found;
            string temp;
            int y = 0;
            while (!ss.eof()) {
                ss >> temp;
                if (stringstream(temp) >> found) {
                    cityMatrix[i][y] = found;
                    y++;
                }
            }

            temp = "";
        }
        getline(file, tempLine);
        hamiltonOpt = atoi(tempLine.c_str());

        cout << "------------city matrix------------" << endl;

        for (int i = 0; i < citiesNum; i++) {
            for (int j = 0; j < citiesNum; j++)
                cout << *(*(cityMatrix + i) + j) << " ";
            cout << endl;
        }

        /*    deklarowanie zmiennych potrzebnych do wykonania algorytmu   */
        int* currentPath = new int[citiesNum + 1];
        int* bestPath = new int[citiesNum + 1];
        int* tempPath = new int[citiesNum + 1];
        int tabuPeroid = citiesNum/2;
        int** tabuList = new int* [citiesNum-1];
        int tempIterator = citiesNum - 1;

        for (int i = 0; i < citiesNum-1; i++) {
            tabuList[i] = new int[citiesNum-1];
        }
        tempIterator = citiesNum - 1;
            
        /* wypelnianie listy zakazow -1*/
        for (int i = 0; i < citiesNum - 1; i++) {
                for (int j = 0; j < i; j++) {
                    tabuList[i][j] = -1;
                }
        }
        tempIterator = 0;
        /* wypelnienie listy zakazow 0*/
        for (int i = 0; i < citiesNum - 1; i++) {
            for (int j = tempIterator ; j < citiesNum -1 ; j++) {
                tabuList[i][j] = 0;
            }
            tempIterator++;
        }
        

        /* inicjalizowanie sciezki startowej */
        for (int i = 0; i <= citiesNum; i++) {
            if (i != citiesNum)
                currentPath[i] = i;
            else
                currentPath[i] = 0;
        }
        copyArray(bestPath, currentPath, citiesNum + 1);

        int iterationNum = citiesNum * citiesNum * citiesNum;

     /* for (int i = 0; i < citiesNum-1; i++) {
            for (int j = 0; j < citiesNum-1; j++) {
                cout << *(*(tabuList + i) + j) << " ";
            }
            cout << endl;
            
        } */
        
        
        
        
     
     
     randomPath(currentPath, citiesNum);
     cout << "Koszt to " << countCost(currentPath, citiesNum, cityMatrix) << endl;
     printPath(currentPath, citiesNum + 1);
     int i, u,m;
     i = 0;
     u = 100;
     m = 5;
     clock_t start = clock();
        /*   tutaj zaczyna sie algorytm TS    */

        while(i<u) {

            newNeighbours(currentPath, citiesNum, tabuList, cityMatrix, 1, tabuPeroid);
            if (countCost(currentPath, citiesNum, cityMatrix) < countCost(bestPath, citiesNum, cityMatrix)) {
                copyArray(bestPath, currentPath, citiesNum + 1);
                i = 0;
            }
            else
                i++;

            if (i == m) {
                randomPath(currentPath, citiesNum);
                for (int j = 0; j < tabuPeroid; j++)
                   decrementTabuList(tabuList, citiesNum);
              
            }

            decrementTabuList(tabuList, citiesNum);
           
        }
        

           
           
        

     
     
        /*   tutaj konczy  sie algorytm TS     */

        clock_t stop = clock();
        double  elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
        cout << elapsed << " [s]" << endl;

        printPath(bestPath, citiesNum + 1);
        cout << "Koszt best : " << countCost(bestPath, citiesNum, cityMatrix) << endl;
        cout << "Koszt optymalny : " << hamiltonOpt << endl;
        double PRD =((double)countCost(bestPath, citiesNum, cityMatrix) /(double) hamiltonOpt);
        cout << "PRD = " << PRD * 100 << " % " << endl;
        /*        zwalnianie pamieci               */
        for (int i = 0; i < citiesNum; i++)
            delete[] cityMatrix[i];
        delete[]cityMatrix;

        delete[]currentPath;
        delete[]bestPath;
        delete[]tempPath;
      

    }
    else cerr << "Dostep do pliku zostal zabroniony!" << endl;



    file.close();
    return 0;
} 
