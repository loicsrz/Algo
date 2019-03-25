#include <iostream>
#include <vector>
using namespace std;



struct path {
    int objectif_value;
    vector<int> visited_node;
};

void PrintPath (path currentPath)
{
    for (unsigned int i=0; i< currentPath.visited_node.size(); i++)
    {
        cout << currentPath.visited_node[i] << " ";
    }
    cout << endl;
    cout << currentPath.objectif_value << endl;
}

bool checkCondition( int SupplierInventoryStock, int capacityVehicle)
{
    if (SupplierInventoryStock < 0)
    {
        return false;
    }
    if(capacityVehicle < 0)
    {
        return false;
    }
    return true;

}

vector<int> getNeighbours(path current, int number_retailer, int ** matrice)
{
    vector<int> neighbours;
    for (int i=0;i< number_retailer ; i++)
    {
        if (matrice[current.visited_node[current.visited_node.size()-1]][i] != INT8_MAX)
        {
            neighbours.push_back(i);
        }
    }
    return neighbours;
}


void objective_function (path *current, int * currentInventory, int number_retailer, int ** matrice, int SupplierInventory)
{
    int objective_number = current->objectif_value;

    for (int i =1;i< number_retailer ; i++)
    {
        objective_number +=  currentInventory[i];
    }
    objective_number += SupplierInventory;
    objective_number +=  matrice[current->visited_node[current->visited_node.size()-2]][current->visited_node[current->visited_node.size()-1]];
    current->objectif_value = objective_number;

   // cout << "objective function :" << current->objectif_value << endl;
}

path recursiveFunction (path currentPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int capacityVehicle, int SupplierInventoryStock)
{
    vector<int> neighbours = getNeighbours(currentPath, numberOfRetailer, matrice);
    path minPath;
    minPath.objectif_value =-1;

    //cout << currentPath.visited_node[currentPath.visited_node.size()-1] << endl;

    for ( unsigned int i=0; i< neighbours.size(); i++)
    {
        if (find(currentPath.visited_node.begin(), currentPath.visited_node.end(), neighbours[i]) != currentPath.visited_node.end())
        {
            continue;
        }
        path NewcurrentPath = currentPath;
        NewcurrentPath.visited_node.push_back(neighbours[i]);
        bool isValid = checkCondition( SupplierInventoryStock, capacityVehicle);
        if(!isValid)
        {
            continue;
        }
        int tempSupplier = SupplierInventoryStock - inventoryMax[neighbours[i]] + currentInventory[neighbours[i]];
        objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice, SupplierInventoryStock);
        NewcurrentPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier);

        for(int j=1;j< numberOfRetailer;j++)
        {
            if (find(NewcurrentPath.visited_node.begin(), NewcurrentPath.visited_node.end(), j) != NewcurrentPath.visited_node.end())
            {
                continue;
            }

            if(currentInventory[j] + increaseSpeed[j] < 0)
            {
                NewcurrentPath.objectif_value = -1;
            }
        }

        if (NewcurrentPath.objectif_value != -1 && (NewcurrentPath.objectif_value < minPath.objectif_value || minPath.objectif_value ==0 || minPath.objectif_value ==-1))
        {
            minPath = NewcurrentPath;
        }
    }
    currentPath.visited_node.push_back(0);
    objective_function(&currentPath, currentInventory, numberOfRetailer, matrice, SupplierInventoryStock);
    for(int j=1;j< numberOfRetailer;j++)
    {
        if (find(currentPath.visited_node.begin(), currentPath.visited_node.end(), j) != currentPath.visited_node.end())
        {
            continue;
        }

        if(currentInventory[j] + increaseSpeed[j] < 0)
        {
            currentPath.objectif_value = -1;
        }
    }
    if (currentPath.objectif_value != -1 && (currentPath.objectif_value < minPath.objectif_value || minPath.objectif_value ==0 || minPath.objectif_value ==-1))
    {
        minPath = currentPath;
    }

    return minPath;

}

void creerInitialCondition (int ** MatriceAdjacence, int * inventoryMax, int * increaseSpeed, int * currentInventory)
{

    MatriceAdjacence[0][0] = INT8_MAX;
    MatriceAdjacence[0][1] = 100;
    MatriceAdjacence[0][2] = 100;
    MatriceAdjacence[0][3] = 100;
    MatriceAdjacence[0][4] = 100;

    MatriceAdjacence[1][0] = 100;
    MatriceAdjacence[1][1] = INT8_MAX;
    MatriceAdjacence[1][2] = 10;
    MatriceAdjacence[1][3] = INT8_MAX;
    MatriceAdjacence[1][4] = INT8_MAX;

    MatriceAdjacence[2][0] = 100;
    MatriceAdjacence[2][1] = 10;
    MatriceAdjacence[2][2] = INT8_MAX;
    MatriceAdjacence[2][3] = 140;
    MatriceAdjacence[2][4] = INT8_MAX;

    MatriceAdjacence[3][0] = 100;
    MatriceAdjacence[3][1] = INT8_MAX;
    MatriceAdjacence[3][2] = 140;
    MatriceAdjacence[3][3] = INT8_MAX;
    MatriceAdjacence[3][4] = 10;

    MatriceAdjacence[4][0] = 100;
    MatriceAdjacence[4][1] = INT8_MAX;
    MatriceAdjacence[4][2] = INT8_MAX;
    MatriceAdjacence[4][3] = 10;
    MatriceAdjacence[4][4] = INT8_MAX;

    inventoryMax[0] = 5000;
    inventoryMax[1] = 5000;
    inventoryMax[2] = 3000;
    inventoryMax[3] = 2000;
    inventoryMax[4] = 4000;

    increaseSpeed[0] = 5000;
    increaseSpeed[1] = -1000;
    increaseSpeed[2] = -1000;
    increaseSpeed[3] = -2000;
    increaseSpeed[4] = -1500;


    currentInventory[0] = 5000;
    currentInventory[1] = 5000;
    currentInventory[2] = 3000;
    currentInventory[3] = 2000;
    currentInventory[4] = 4000;


}

path min_onjective(path currentPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int capacityVehicle, int SupplierInventoryStock)
{

    vector<int> neighbours = getNeighbours(currentPath, numberOfRetailer, matrice);
    path minPath;
    minPath.objectif_value =-1;

    for (unsigned int i=0; i< neighbours.size(); i++)
    {
        path NewcurrentPath = currentPath;
        NewcurrentPath.visited_node.push_back(neighbours[i]);

        bool isValid = checkCondition(SupplierInventoryStock, capacityVehicle);
        if(!isValid)
        {
            continue;
        }

        int tempSupplier = SupplierInventoryStock - inventoryMax[neighbours[i]] + currentInventory[neighbours[i]];

        objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice, SupplierInventoryStock);
        NewcurrentPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier);

        for(int j=1;j< numberOfRetailer;j++)
        {
            if (find(NewcurrentPath.visited_node.begin(), NewcurrentPath.visited_node.end(), j) != NewcurrentPath.visited_node.end())
            {
                continue;
            }

            if(currentInventory[j] + increaseSpeed[j] < 0)
            {
                NewcurrentPath.objectif_value = -1;
            }
        }

        if (NewcurrentPath.objectif_value != -1 && (NewcurrentPath.objectif_value < minPath.objectif_value || minPath.objectif_value ==0 || minPath.objectif_value ==-1))
        {
            PrintPath(NewcurrentPath);
            minPath = NewcurrentPath;
        }
    }

    return minPath;
}

int main() {
    int numberOfNodes = 5;
    int numberOfTimeRunning = 3;


    int capacityVehicle = 5000;
    int ** matricePointeur = (int **)malloc(numberOfNodes*sizeof(int*));
    for (int i=0; i<numberOfNodes; i++){
        matricePointeur[i]= (int *)malloc(numberOfNodes*sizeof(int));
    }
    int * inventoryMax = (int *)malloc(numberOfNodes* sizeof(int));
    int * currentInventory = (int *)malloc(numberOfNodes* sizeof(int));
    int * increaseSpeed = (int *)malloc(numberOfNodes* sizeof(int));

    creerInitialCondition(matricePointeur, inventoryMax, increaseSpeed, currentInventory);

    for(int j=1; j<= numberOfTimeRunning;j++)
    {
        path currentPath;
        currentPath.objectif_value = 0;
        currentPath.visited_node.push_back(0);

        for (int k=0;k<numberOfNodes;k++)
        {
            currentInventory[k] = currentInventory[k] + increaseSpeed[k];
        }
        if(currentInventory[0] > inventoryMax[0])
        {
            currentInventory[0] = inventoryMax[0];
        }
        currentPath = min_onjective(currentPath, matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, currentInventory[0]);


        PrintPath(currentPath);
        for (int i=1;i<currentPath.visited_node.size()-1;i++)
        {
            currentInventory[i] = inventoryMax[i];
        }

        cout << "End of day"<< endl;
    }
    delete matricePointeur;
    delete inventoryMax;
    delete currentInventory;
    delete increaseSpeed;
    return 0;

}