#include <iostream>
#include <vector>
using namespace std;



struct path {
    int objectif_value;
    vector<int> visited_node;
};


/// Fonction qui sert à afficher un path avec sa valeur de la fonction objectif
void PrintPath (path currentPath)
{
    for (unsigned int i=0; i< currentPath.visited_node.size(); i++)
    {
        cout << currentPath.visited_node[i] << " ";
    }
    cout << endl;
    cout << currentPath.objectif_value << endl;
}

/// On vérifie que le stock passé en entrer est conforme, cad qu'il est supérieure à 0
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

/// Pour un path en entrée, la fonction ressort tous les voisins du dernier noeud du path
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

/// Fonction permettant le calcul de la valeur de la fonction objective
void objective_function (path *current, int * currentInventory, int number_retailer, int ** matrice, int SupplierInventory, int * inventoryMax)
{
    int objective_number = 0;

    /// Ajout de l'inventaire à la valeur de la fonction objectif
    for (int i =1;i< number_retailer ; i++)
    {
        objective_number +=  currentInventory[i];
    }

    /// Ajout du stock de l'entrepôt à la valeur de la fonction obkectif
    objective_number += SupplierInventory;

    /// Ajout des coûts du transport de la cargaison
    for (unsigned int j=0;j<current->visited_node.size()-1;j++)
    {
       objective_number += matrice[current->visited_node[j]][current->visited_node[j+1]];

       objective_number += inventoryMax[current->visited_node[j]] - currentInventory[current->visited_node[j]];
    }
    /// Ajout de la valeur situé dans le camion et livré aux clients
    objective_number += inventoryMax[current->visited_node[current->visited_node.size()-1]] - currentInventory[current->visited_node[current->visited_node.size()-1]];
    current->objectif_value = objective_number;

}
/// Fonction récursive permettant de récupérer l'ensemble des chenins possibles
vector<path> recursiveFunction (path currentPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int capacityVehicle, int SupplierInventoryStock)
{
    /// On récupère tous les voisins
    vector<int> neighbours = getNeighbours(currentPath, numberOfRetailer, matrice);

    vector<path> pathToGive;

    /// On itère pour tous les voisins du dernier noeud
    for ( unsigned int i=0; i< neighbours.size(); i++)
    {
        /// On élimine des voisins des noeuds où l'on est déja passé
        if (find(currentPath.visited_node.begin(), currentPath.visited_node.end(), neighbours[i]) != currentPath.visited_node.end())
        {
            continue;
        }
        path NewcurrentPath = currentPath;
        NewcurrentPath.visited_node.push_back(neighbours[i]);
        int tempSupplier = SupplierInventoryStock - inventoryMax[neighbours[i]] + currentInventory[neighbours[i]];
        bool isValid = checkCondition( tempSupplier, capacityVehicle);
        if(!isValid)
        {
            continue;
        }
        objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice, tempSupplier, inventoryMax);
        vector <path> NewTabPath;
        NewTabPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier);

        for (unsigned int k=0;k< NewTabPath.size();k++)
        {
            for(int j=1;j< numberOfRetailer;j++)
            {
                if (find(NewTabPath[k].visited_node.begin(), NewTabPath[k].visited_node.end(), j) != NewTabPath[k].visited_node.end())
                {
                    continue;
                }

                if(currentInventory[j] + increaseSpeed[j] < 0)
                {
                    NewTabPath[k].objectif_value = -1;
                }
            }

            if (NewTabPath[k].objectif_value != -1)
            {
                pathToGive.push_back(NewTabPath[k]);
            }
        }
    }
    currentPath.visited_node.push_back(0);
    objective_function(&currentPath, currentInventory, numberOfRetailer, matrice, SupplierInventoryStock, inventoryMax);
    for(int j=1;j< numberOfRetailer;j++)
    {
        /// Permet de checker si le chemin parcours tous les noeuds critiques
        if (find(currentPath.visited_node.begin(), currentPath.visited_node.end(), j) != currentPath.visited_node.end())
        {
            continue;
        }

        if(currentInventory[j] + increaseSpeed[j] < 0)
        {
            currentPath.objectif_value = -1;
        }
    }
    if (currentPath.objectif_value != -1)
    {
        //PrintPath(currentPath);
        pathToGive.push_back(currentPath);
    }

    return pathToGive;

}

void creerInitialCondition (int ** MatriceAdjacence, int * inventoryMax, int * increaseSpeed, int * currentInventory)
{
    ///Création de la matrice et initialisation des différents paramètres du problème
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


    currentInventory[0] = 7000;
    currentInventory[1] = 5000;
    currentInventory[2] = 3000;
    currentInventory[3] = 2000;
    currentInventory[4] = 4000;


}

vector<path> min_onjective(path currentPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int capacityVehicle, int SupplierInventoryStock)
{
    /// Initialisation des paramètres en fonction du chemin précédent
    for (unsigned int i=0;i<currentPath.visited_node.size();i++)
    {
        SupplierInventoryStock -= inventoryMax[currentPath.visited_node[i]] - currentInventory[currentPath.visited_node[i]];
        currentInventory[currentPath.visited_node[i]] = inventoryMax[currentPath.visited_node[i]];
    }
    for (int k=0;k<numberOfRetailer;k++)
    {
        currentInventory[k] = currentInventory[k] + increaseSpeed[k];
    }
    if(currentInventory[0] > inventoryMax[0])
    {
        currentInventory[0] = inventoryMax[0];
    }
    SupplierInventoryStock = currentInventory[0];

    /// Calcul des voisins du dernier noeud contenu dans le chemin
    vector<path> pathToGive;
    vector<int> neighbours = getNeighbours(currentPath, numberOfRetailer, matrice);

    /// Pour chaque voisin
    for (unsigned int i=0; i< neighbours.size(); i++)
    {
        path NewcurrentPath;
        NewcurrentPath.visited_node.push_back(0);
        NewcurrentPath.objectif_value = 0;
        NewcurrentPath.visited_node.push_back(neighbours[i]);

        /// On regarde si le chemin pris respecte les conditions en entrées ( le stock de l'inventaire est suffisant)
        int tempSupplier = SupplierInventoryStock - inventoryMax[neighbours[i]] + currentInventory[neighbours[i]];
        bool isValid = checkCondition(tempSupplier, capacityVehicle);
        if(!isValid)
        {
            continue;
        }

        /// On va chercher récursivement tous les fils de ce chemin et on remonte tous ceux qui sont corrects
        objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice, tempSupplier, inventoryMax);
        vector <path> NewTabPath;
        NewTabPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier);

        /// On regarde que le chemin une fois calculé respecte les conditions finales
       // cout << "CurrentPath" << endl;
       // PrintPath(currentPath);
        for (unsigned int k=0;k< NewTabPath.size();k++)
        {
            for(int j=1;j< numberOfRetailer;j++)
            {
                /// On vérifie qu'il n'y est pas de doublon
               if (find(NewTabPath[k].visited_node.begin(), NewTabPath[k].visited_node.end(), j) != NewTabPath[k].visited_node.end())
                {

                    continue;
                }
                /// On regarde si aucun inventaire ne va se vider au tour prochain
                if(currentInventory[j] + increaseSpeed[j] < 0)
                {
                    NewTabPath[k].objectif_value = -1;
                }
            }
            /// On ajoute le chemin dans la liste des chemins valides s'il respecte toutes les conditions
            if (NewTabPath[k].objectif_value != -1)
            {
                pathToGive.push_back(NewTabPath[k]);
                PrintPath(NewTabPath[k]);
            }
        }
    }

    return pathToGive;

}

vector<path> Recursive_temps (vector<path> tabPath, int length, vector<path> precedence, int ** matricePointeur, int * inventoryMax, int * currentInventory, int numberOfNodes, int * increaseSpeed,  int capacityVehicle, int SupplierInventoryStock)
{
    vector<path> minTabPath;
    int minTabPathObjective =INT32_MAX;
    if(length == 0)
    {
        path MinPath;
        for (unsigned int i=0;i<tabPath.size();i++)
        {
            if (tabPath[i].objectif_value < minTabPathObjective)
            {
                minTabPathObjective = tabPath[i].objectif_value;
                MinPath = tabPath[i];
            }
        }
        if(tabPath.size() !=0)
        {
            precedence.push_back(MinPath);
        }
//        for (int j = 0;j<precedence.size();j++)
//        {
//            PrintPath(precedence[j]);
//        }
        return precedence;
    }
    vector<path> newTabPath;
    for (unsigned int i=0;i<tabPath.size();i++)
    {
       // cout << tabPath.size() << endl;
      // cout << length << endl;
       newTabPath = min_onjective(tabPath[i], matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, SupplierInventoryStock);
       cout << "New tab path size : ";
       newTabPath.size();
       cout << endl;
       if(newTabPath.size() == 0)
       {
           continue;
       }
       precedence.push_back(tabPath[i]);
//        for (int j = 0;j<precedence.size();j++)
//        {
//            PrintPath(precedence[j]);
//        }
       vector<path> tabPrecedentPath = Recursive_temps(newTabPath, length-1,precedence,matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, SupplierInventoryStock);
//       for (int j = 0;j<tabPrecedentPath.size();j++)
//       {
//           PrintPath(tabPrecedentPath[j]);
//       }
       precedence.pop_back();
       int min_tab_optimum = 0;
       for (unsigned int k=0;k<tabPrecedentPath.size();k++)
       {
           min_tab_optimum += tabPrecedentPath[k].objectif_value;
       }
       //cout << min_tab_optimum << endl;
       //cout << minTabPathObjective << endl;
       if (min_tab_optimum < minTabPathObjective && min_tab_optimum !=0)
       {
           minTabPath.clear();
           for(unsigned int j=0;j<tabPrecedentPath.size();j++)
           {
                minTabPath.push_back(tabPrecedentPath[j]);
           }
//           for (int j = 0;j<minTabPath.size();j++)
//           {
//              PrintPath(minTabPath[j]);
//           }
           minTabPathObjective = min_tab_optimum;
       }
    }
    return minTabPath;
}

int main() {
    int numberOfNodes = 5;
    int numberOfTimeRunning = 5;


    int capacityVehicle = 5000;
    int ** matricePointeur = (int **)malloc(numberOfNodes*sizeof(int*));
    for (int i=0; i<numberOfNodes; i++){
        matricePointeur[i]= (int *)malloc(numberOfNodes*sizeof(int));
    }
    int * inventoryMax = (int *)malloc(numberOfNodes* sizeof(int));
    int * currentInventory = (int *)malloc(numberOfNodes* sizeof(int));
    int * increaseSpeed = (int *)malloc(numberOfNodes* sizeof(int));

    creerInitialCondition(matricePointeur, inventoryMax, increaseSpeed, currentInventory);
    vector <path> currentPath;
    vector <path> precedence;
    path beginPath;
    beginPath.objectif_value = 0;
    beginPath.visited_node.push_back(0);
    currentPath.push_back(beginPath);

    currentPath = Recursive_temps(currentPath,numberOfTimeRunning,precedence, matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, currentInventory[0]);
    for (unsigned int i=0;i<currentPath.size();i++)
    {
        PrintPath(currentPath[i]);
        cout << endl;
    }

    delete matricePointeur;
    delete inventoryMax;
    delete currentInventory;
    delete increaseSpeed;
    return 0;

}