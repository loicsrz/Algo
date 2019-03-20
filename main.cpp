#include <iostream>
#include <vector>
using namespace std;



struct path {
    int objectif_value;
    vector<int> visited_node;
};

bool checkCondition(path currentPath, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int * shipments, int capacityVehicle)
{
    int quantityToShip = 0;
    int quantityShiped = 0;
    for (int i=1; i< numberOfRetailer ;i++)
    {
        if(inventoryMax[i] - currentInventory[i] >= 0)
        {
            quantityToShip+= inventoryMax[i] - currentInventory[i];
        }
        else
        {
            /// Constraint 8
            return false;
        }
    }
    for (int i=0; i< numberOfRetailer-1 ;i++)
    {
        quantityShiped += shipments[i];
    }
    /// Constraint 1
    currentInventory[0] += increaseSpeed[0] - quantityShiped;

    /// Constraint 2
    if(currentInventory[0] < quantityToShip)
    {
        return false;
    }

    for (int i=1; i< numberOfRetailer ; i++)
    {
        /// Constraint 3
        currentInventory[i] += shipments[i] - increaseSpeed[i];

        /// Constraint 4
        if (currentInventory[i] < 0)
        {
            return false;
        }

        if(shipments[i] != 0)
        {
            /// Constraint 5
            if(shipments[i] != inventoryMax[i] - currentInventory[i])
            {
                return false;
            }


        }
    }

    if (quantityToShip > capacityVehicle)
    {
        /// constraint 6
        return false;
    }


}

vector<int> getNeighbours(path current, int number_retailer, int ** matrice)
{
    vector<int> neighbours;
    for (int i=0;i< number_retailer ; i++)
    {
        neighbours.push_back(matrice[current.visited_node[current.visited_node.size()-1]][i]);
    }
    return neighbours;
}


void objective_function (path *current, int * currentInventory, int number_retailer, int ** matrice)
{
    int objective_number = current->objectif_value;

    for (int i =0;i< number_retailer ; i++)
    {
        objective_number +=  currentInventory[i];
    }
    objective_number +=  matrice[current->visited_node[current->visited_node.size()-2]][current->visited_node[current->visited_node.size()-1]];

    current->objectif_value = objective_number;
}

path recursiveFunction (path currentPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int * shipments, int capacityVehicle)
{
    if(currentPath.visited_node[currentPath.visited_node.size()-1] == 0)
    {
        return currentPath;
    }
    vector<int> neighbours = getNeighbours(currentPath, numberOfRetailer, matrice);
    path minPath = currentPath;
    for (int i=0; i< neighbours.size(); i++)
    {
        path NewcurrentPath = currentPath;
        NewcurrentPath.visited_node.push_back(neighbours[i]);
        bool isValid = checkCondition(NewcurrentPath, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, shipments, capacityVehicle);
        if(!isValid)
        {
            continue;
        }
        objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice);
        NewcurrentPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, shipments, capacityVehicle);

        if (NewcurrentPath.objectif_value < minPath.objectif_value)
        {
            minPath = currentPath;
        }
    }
    return minPath;

}

int creerInitialCondition (int ** MatriceAdjacence, int * inventoryMax, int * currentInventory, int * increaseSpeed)
{


    MatriceAdjacence[0][0] = INT8_MAX;
    MatriceAdjacence[0][1] = 100;
    MatriceAdjacence[0][2] = 100;
    MatriceAdjacence[0][3] = 100;
    MatriceAdjacence[1][0] = 100;
    MatriceAdjacence[1][1] = INT8_MAX;
    MatriceAdjacence[1][2] = 50;
    MatriceAdjacence[1][3] = INT8_MAX;
    MatriceAdjacence[2][0] = 100;
    MatriceAdjacence[2][1] = 50;
    MatriceAdjacence[2][2] = INT8_MAX;
    MatriceAdjacence[2][3] = 50;
    MatriceAdjacence[3][0] = 100;
    MatriceAdjacence[3][0] = INT8_MAX;
    MatriceAdjacence[3][0] = 50;
    MatriceAdjacence[3][0] = INT8_MAX;

    inventoryMax[0] = 5000;
    inventoryMax[1] = 1500;
    inventoryMax[2] = 2000;
    inventoryMax[3] = 3000;

    increaseSpeed[0] = 5000;
    increaseSpeed[1] = 500;
    increaseSpeed[2] = 500;
    increaseSpeed[3] = 500;


    currentInventory[0] = 5000;
    currentInventory[1] = 1500;
    currentInventory[2] = 2000;
    currentInventory[3] = 3000;

    return 4;

}

path min_onjective(path currentPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int * shipments, int capacityVehicle)
{
    vector<int> neighbours = getNeighbours(currentPath, numberOfRetailer, matrice);
    path minPath = currentPath;
    for (int i=0; i< neighbours.size(); i++)
    {
        path NewcurrentPath = currentPath;
        NewcurrentPath.visited_node.push_back(neighbours[i]);
        bool isValid = checkCondition(NewcurrentPath, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, shipments, capacityVehicle);
        if(!isValid)
        {
            continue;
        }
        objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice);
        NewcurrentPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, shipments, capacityVehicle);

        if (NewcurrentPath.objectif_value != -1 && NewcurrentPath.objectif_value < minPath.objectif_value)
        {
            minPath = currentPath;
        }
    }
    return minPath;
}

int main() {

    int capacityVehicle = 3000;
    bool isValid = false;
    int ** matricePointeur;
    int * inventoryMax;
    int * currentInventory;
    int * increaseSpeed;
    int * shipments;
    //TODO : faire la fonction pour diminuer le supplier
    int numberOfClient = creerInitialCondition(matricePointeur, inventoryMax, currentInventory, increaseSpeed);
    path currentPath;
    currentPath.objectif_value = INT8_MAX;
    currentPath.visited_node.push_back(0);

    currentPath = min_onjective(currentPath, matricePointeur, inventoryMax, currentInventory, numberOfClient, increaseSpeed, shipments, capacityVehicle);
    for (int i=0; i< currentPath.visited_node.size(); i++)
    {
        cout << currentPath.visited_node[i] << " ";
    }
    cout << endl;
    cout << currentPath.objectif_value << endl;

    return 0;

}