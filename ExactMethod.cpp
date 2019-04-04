#include <iostream>
#include <vector>
#include <ctime>
using namespace std;


/// Structure du chemin que l'algorithme parcoure
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
        /// On crée le nouveau path avec le voisin du noeud précédent
        path NewcurrentPath = currentPath;
        NewcurrentPath.visited_node.push_back(neighbours[i]);

        /// on met à jour l'entrepôt en fonction du nouveau noeud et on regarde si cela respecte les conditions
        int tempSupplier = SupplierInventoryStock - inventoryMax[neighbours[i]] + currentInventory[neighbours[i]];
        bool isValid = checkCondition( tempSupplier, capacityVehicle);
        if(!isValid)
        {
            continue;
        }

        /// On va récupérer de maniére récursive les paths fils
        objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice, tempSupplier, inventoryMax);
        vector <path> NewTabPath;
        NewTabPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier);

        /// Une fois les paths récupérés, on vérifie qu'il vérifie les conditions
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
        pathToGive.push_back(currentPath);
    }

    return pathToGive;

}

void creerInitialCondition (int ** MatriceAdjacence, int * inventoryMax, int * increaseSpeed, int * currentInventory)
{
    ///Création de la matrice et initialisation des différents paramètres du problème
    MatriceAdjacence[0][0] = INT8_MAX;
    MatriceAdjacence[0][1] = 200;
    MatriceAdjacence[0][2] = 200;
    MatriceAdjacence[0][3] = 200;
    MatriceAdjacence[0][4] = 200;
    MatriceAdjacence[0][5] = 200;
    MatriceAdjacence[0][6] = 200;
    MatriceAdjacence[0][7] = 200;
    MatriceAdjacence[0][8] = 200;
    MatriceAdjacence[0][9] = 200;
    MatriceAdjacence[0][10] = 200;

    MatriceAdjacence[1][0] = 200;
    MatriceAdjacence[1][1] = INT8_MAX;
    MatriceAdjacence[1][2] = INT8_MAX;
    MatriceAdjacence[1][3] = INT8_MAX;
    MatriceAdjacence[1][4] = INT8_MAX;
    MatriceAdjacence[1][5] = INT8_MAX;
    MatriceAdjacence[1][6] = 20;
    MatriceAdjacence[1][7] = 20;
    MatriceAdjacence[1][8] = 20;
    MatriceAdjacence[1][9] = INT8_MAX;
    MatriceAdjacence[1][10] =INT8_MAX;

    MatriceAdjacence[2][0] = 200;
    MatriceAdjacence[2][1] = INT8_MAX;
    MatriceAdjacence[2][2] = INT8_MAX;
    MatriceAdjacence[2][3] = INT8_MAX;
    MatriceAdjacence[2][4] = INT8_MAX;
    MatriceAdjacence[2][5] = INT8_MAX;
    MatriceAdjacence[2][6] = INT8_MAX;
    MatriceAdjacence[2][7] = 20;
    MatriceAdjacence[2][8] = INT8_MAX;
    MatriceAdjacence[2][9] = 20;
    MatriceAdjacence[2][10] =INT8_MAX;

    MatriceAdjacence[3][0] = 200;
    MatriceAdjacence[3][1] = INT8_MAX;
    MatriceAdjacence[3][2] = INT8_MAX;
    MatriceAdjacence[3][3] = INT8_MAX;
    MatriceAdjacence[3][4] = INT8_MAX;
    MatriceAdjacence[3][5] = INT8_MAX;
    MatriceAdjacence[3][6] = INT8_MAX;
    MatriceAdjacence[3][7] = INT8_MAX;
    MatriceAdjacence[3][8] = INT8_MAX;
    MatriceAdjacence[3][9] = 20;
    MatriceAdjacence[3][10] =20;

    MatriceAdjacence[4][0] = 200;
    MatriceAdjacence[4][1] = INT8_MAX;
    MatriceAdjacence[4][2] = INT8_MAX;
    MatriceAdjacence[4][3] = INT8_MAX;
    MatriceAdjacence[4][4] = INT8_MAX;
    MatriceAdjacence[4][5] = 20;
    MatriceAdjacence[4][6] = INT8_MAX;
    MatriceAdjacence[4][7] = INT8_MAX;
    MatriceAdjacence[4][8] = INT8_MAX;
    MatriceAdjacence[4][9] = INT8_MAX;
    MatriceAdjacence[4][10] =20;

    MatriceAdjacence[5][0] = 200;
    MatriceAdjacence[5][1] = INT8_MAX;
    MatriceAdjacence[5][2] = INT8_MAX;
    MatriceAdjacence[5][3] = INT8_MAX;
    MatriceAdjacence[5][4] = 20;
    MatriceAdjacence[5][5] = INT8_MAX;
    MatriceAdjacence[5][6] = 20;
    MatriceAdjacence[5][7] = INT8_MAX;
    MatriceAdjacence[5][8] = 20;
    MatriceAdjacence[5][9] = INT8_MAX;
    MatriceAdjacence[5][10] =INT8_MAX;

    MatriceAdjacence[6][0] = 200;
    MatriceAdjacence[6][1] = 20;
    MatriceAdjacence[6][2] = INT8_MAX;
    MatriceAdjacence[6][3] = INT8_MAX;
    MatriceAdjacence[6][4] = INT8_MAX;
    MatriceAdjacence[6][5] = 20;
    MatriceAdjacence[6][6] = INT8_MAX;
    MatriceAdjacence[6][7] = INT8_MAX;
    MatriceAdjacence[6][8] = 20;
    MatriceAdjacence[6][9] = INT8_MAX;
    MatriceAdjacence[6][10] = INT8_MAX;

    MatriceAdjacence[7][0] = 200;
    MatriceAdjacence[7][1] = 20;
    MatriceAdjacence[7][2] = 20;
    MatriceAdjacence[7][3] = INT8_MAX;
    MatriceAdjacence[7][4] = INT8_MAX;
    MatriceAdjacence[7][5] = INT8_MAX;
    MatriceAdjacence[7][6] = INT8_MAX;
    MatriceAdjacence[7][7] = INT8_MAX;
    MatriceAdjacence[7][8] = 20;
    MatriceAdjacence[7][9] = 20;
    MatriceAdjacence[7][10] = INT8_MAX;

    MatriceAdjacence[8][0] = 200;
    MatriceAdjacence[8][1] = 20;
    MatriceAdjacence[8][2] = INT8_MAX;
    MatriceAdjacence[8][3] = INT8_MAX;
    MatriceAdjacence[8][4] = INT8_MAX;
    MatriceAdjacence[8][5] = 20;
    MatriceAdjacence[8][6] = 20;
    MatriceAdjacence[8][7] = 20;
    MatriceAdjacence[8][8] = INT8_MAX;
    MatriceAdjacence[8][9] = INT8_MAX;
    MatriceAdjacence[8][10] =INT8_MAX;

    MatriceAdjacence[9][0] = 200;
    MatriceAdjacence[9][1] = INT8_MAX;
    MatriceAdjacence[9][2] = 20;
    MatriceAdjacence[9][3] = 20;
    MatriceAdjacence[9][4] = INT8_MAX;
    MatriceAdjacence[9][5] = INT8_MAX;
    MatriceAdjacence[9][6] = INT8_MAX;
    MatriceAdjacence[9][7] = 20;
    MatriceAdjacence[9][8] = INT8_MAX;
    MatriceAdjacence[9][9] = INT8_MAX;
    MatriceAdjacence[9][10] =20;

    MatriceAdjacence[10][0] = 200;
    MatriceAdjacence[10][1] = INT8_MAX;
    MatriceAdjacence[10][2] = INT8_MAX;
    MatriceAdjacence[10][3] = 20;
    MatriceAdjacence[10][4] = 20;
    MatriceAdjacence[10][5] = INT8_MAX;
    MatriceAdjacence[10][6] = INT8_MAX;
    MatriceAdjacence[10][7] = INT8_MAX;
    MatriceAdjacence[10][8] = INT8_MAX;
    MatriceAdjacence[10][9] = 20;
    MatriceAdjacence[10][10] =INT8_MAX;

    inventoryMax[0] = 10000;
    inventoryMax[1] = 1500;
    inventoryMax[2] = 600;
    inventoryMax[3] = 3000;
    inventoryMax[4] = 4400;
    inventoryMax[5] = 2600;
    inventoryMax[6] = 3200;
    inventoryMax[7] = 2000;
    inventoryMax[8] = 4000;
    inventoryMax[9] = 5000;
    inventoryMax[10] = 2300;

    increaseSpeed[0] = 10000;
    increaseSpeed[1] = -500;
    increaseSpeed[2] = -600;
    increaseSpeed[3] = -800;
    increaseSpeed[4] = -1300;
    increaseSpeed[5] = -2000;
    increaseSpeed[6] = -200;
    increaseSpeed[7] = -2000;
    increaseSpeed[8] = -500;
    increaseSpeed[9] = -1000;
    increaseSpeed[10] = -2300;


    currentInventory[0] = 10000;
    currentInventory[1] = 1500;
    currentInventory[2] = 600;
    currentInventory[3] = 3000;
    currentInventory[4] = 4400;
    currentInventory[5] = 2600;
    currentInventory[6] = 3200;
    currentInventory[7] = 2000;
    currentInventory[8] = 4000;
    currentInventory[9] = 5000;
    currentInventory[10] = 2300;


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
                cout << "Valeur de 4" << currentInventory[4] << endl;
                pathToGive.push_back(NewTabPath[k]);
            }
        }
    }

    return pathToGive;

}
/// Fonction récursive qui permet de récupérer l'ensemble des chemins de chaque temps
vector<path> Recursive_temps (vector<path> tabPath, int length, vector<path> precedence, int ** matricePointeur, int * inventoryMax, int * currentInventory, int numberOfNodes, int * increaseSpeed,  int capacityVehicle, int SupplierInventoryStock)
{
    vector<path> minTabPath;
    int minTabPathObjective =INT32_MAX;
    /// Si c'est le dernier tour, on renvoie.
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
        return precedence;
    }
    vector<path> newTabPath;
    for (unsigned int i=0;i<tabPath.size();i++)
    {
       newTabPath = min_onjective(tabPath[i], matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, SupplierInventoryStock);
       newTabPath.size();
       if(newTabPath.size() == 0)
       {
           continue;
       }
       precedence.push_back(tabPath[i]);
       vector<path> tabPrecedentPath = Recursive_temps(newTabPath, length-1,precedence,matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, SupplierInventoryStock);
       precedence.pop_back();
       int min_tab_optimum = 0;
       for (unsigned int k=0;k<tabPrecedentPath.size();k++)
       {
           min_tab_optimum += tabPrecedentPath[k].objectif_value;
       }
       if (min_tab_optimum < minTabPathObjective && min_tab_optimum !=0)
       {
           minTabPath.clear();
           for(unsigned int j=0;j<tabPrecedentPath.size();j++)
           {
                minTabPath.push_back(tabPrecedentPath[j]);
           }
           minTabPathObjective = min_tab_optimum;
       }
    }
    return minTabPath;
}

int main() {


    int numberOfNodes = 11;
    int numberOfTimeRunning = -1;

    cout << "Veuillez rentrez un nombre de tour de livraison : " << endl;
    cin >> numberOfTimeRunning;
    while (numberOfTimeRunning <0)
    {
        cout << "Veuillez rentrez un nombre positif :" << endl;
        cin >> numberOfTimeRunning;
    }

    /// On initialise le chrono
    clock_t time = clock();

    /// On récupère la mémoire pour les tableaux
    int ** matricePointeur = (int **)malloc(numberOfNodes*sizeof(int*));
    for (int i=0; i<numberOfNodes; i++){
        matricePointeur[i]= (int *)malloc(numberOfNodes*sizeof(int));
    }
    int * inventoryMax = (int *)malloc(numberOfNodes* sizeof(int));
    int * currentInventory = (int *)malloc(numberOfNodes* sizeof(int));
    int * increaseSpeed = (int *)malloc(numberOfNodes* sizeof(int));

    /// On initialise les tableaux
    creerInitialCondition(matricePointeur, inventoryMax, increaseSpeed, currentInventory);
    int capacityVehicle = inventoryMax[0];
    vector <path> currentPath;
    vector <path> precedence;
    path beginPath;
    beginPath.objectif_value = 0;
    beginPath.visited_node.push_back(0);
    currentPath.push_back(beginPath);

    /// On affiche le résultat
    currentPath = Recursive_temps(currentPath,numberOfTimeRunning,precedence, matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, currentInventory[0]);
    for (unsigned int i=0;i<currentPath.size();i++)
    {
        cout << "Chemin au temps t=" << i << endl;
        PrintPath(currentPath[i]);
        cout << endl;
    }

    /// On libère la mémoire utilisée
    delete matricePointeur;
    delete inventoryMax;
    delete currentInventory;
    delete increaseSpeed;

    time = clock() - time;

    cout << "Temps d'execution : " << ((float)time)/CLOCKS_PER_SEC;

    return 0;

}