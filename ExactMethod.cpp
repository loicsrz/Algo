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
bool checkCondition( int SupplierInventoryStock, int capacityVehicle, int choice, vector<int> urgentNodes, path currentpath, int * currentInventory, int * inventoryMax)
{
    if (SupplierInventoryStock < 0)
    {
        return false;
    }
    if(capacityVehicle < 0)
    {
        return false;
    }
    /// Première Heuristique
    if(choice == 2 || choice == 4)
    {
        if(currentpath.visited_node.size() == 2)
        {
            for (unsigned int i=0;i<urgentNodes.size();i++)
            {
                if(urgentNodes[i] == currentpath.visited_node[currentpath.visited_node.size()-1])
                {
                    return true;
                }
            }
            return false;
        }
    }
    /// Seconde Heuristique
    if(choice == 3 || choice == 4)
    {
        int somme=0;
        for (unsigned int i=0;i<urgentNodes.size();i++)
        {
            somme += inventoryMax[urgentNodes[i]] - currentInventory[urgentNodes[i]];
        }
        if(somme > SupplierInventoryStock)
        {
            return false;
        }
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
void objective_function (path *current, int * currentInventory, int number_retailer, int ** matrice)
{
    int tmpvalues [3];

    int objective_number = 0;

    /// Ajout de l'inventaire à la valeur de la fonction objectif
    for (int i =0;i< number_retailer ; i++)
    {
        objective_number +=  currentInventory[i];
    }
    tmpvalues[0] = objective_number;

    /// Ajout des coûts du transport de la cargaison
    for (unsigned int j=0;j<current->visited_node.size()-1;j++)
    {
       objective_number += matrice[current->visited_node[j]][current->visited_node[j+1]];
    }
    tmpvalues[1] = objective_number;

    /// Ajout de la valeur situé dans le camion et livré aux clients
    current->objectif_value = objective_number;

    tmpvalues[2] = objective_number;

}
/// Fonction récursive permettant de récupérer l'ensemble des chenins possibles
vector<path> recursiveFunction (path currentPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int capacityVehicle, int SupplierInventoryStock ,int choice, vector<int> urgentNodes)
{
    /// On récupère tous les voisins
    vector<int> neighbours = getNeighbours(currentPath, numberOfRetailer, matrice);
    bool erase = false;
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

        /// On met à jour la liste des noeuds critiques
        if(find(urgentNodes.begin(),urgentNodes.end(),neighbours[i]) != urgentNodes.end())
        {
            urgentNodes.erase(find(urgentNodes.begin(),urgentNodes.end(),neighbours[i]));
            erase = true;
        }

        /// on met à jour l'entrepôt en fonction du nouveau noeud et on regarde si cela respecte les conditions
        int tempSupplier = SupplierInventoryStock - inventoryMax[neighbours[i]] + currentInventory[neighbours[i]];
        bool isValid = checkCondition( tempSupplier, capacityVehicle, choice, urgentNodes, currentPath, currentInventory, inventoryMax);

        /// Si le path n'est pas valide et que le noeud ajouté était critique, on le remet avant de passer à la suite
        if(!isValid)
        {
            if(erase)
            {
                urgentNodes.push_back(neighbours[i]);
            }
            continue;
        }

        /// On va récupérer de maniére récursive les paths fils
        vector <path> NewTabPath;
        NewTabPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier, choice, urgentNodes);

        /// Permet de checker si le chemin parcours tous les noeuds critiques
        for (unsigned int k=0;k< NewTabPath.size();k++)
        {
            for(int j=1;j< numberOfRetailer;j++)
            {
                /// Si un noeud critique est déja visité, on ne le prend pas en compte
                if (find(NewTabPath[k].visited_node.begin(), NewTabPath[k].visited_node.end(), j) != NewTabPath[k].visited_node.end())
                {
                    continue;
                }
                /// Si un noeud critique n'a pas été visité, on ne prend pas en compte ce chemin
                if(currentInventory[j] + increaseSpeed[j] < 0)
                {
                    PrintPath(NewTabPath[k]);
                    NewTabPath[k].objectif_value = -1;
                }
            }
            /// Si le chemin est bon, on le renvoie
            if (NewTabPath[k].objectif_value != -1)
            {
                pathToGive.push_back(NewTabPath[k]);
            }
        }
    }
    currentPath.visited_node.push_back(0);
    objective_function(&currentPath, currentInventory, numberOfRetailer, matrice);

    /// Permet de checker si le chemin parcours tous les noeuds critiques
    for(int j=1;j< numberOfRetailer;j++)
    {
        /// Si un noeud critique est déja visité, on ne le prend pas en compte
        if (find(currentPath.visited_node.begin(), currentPath.visited_node.end(), j) != currentPath.visited_node.end())
        {
            continue;
        }
        /// Si un noeud critique n'a pas été visité, on ne prend pas en compte ce chemin
        if(currentInventory[j] + increaseSpeed[j] < 0)
        {
            currentPath.objectif_value = -1;
        }
    }
    /// Si le chemin est bon, on le renvoie
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
    MatriceAdjacence[2][3] = 10;
    MatriceAdjacence[2][4] = INT8_MAX;


    MatriceAdjacence[3][0] = 100;
    MatriceAdjacence[3][1] = INT8_MAX;
    MatriceAdjacence[3][2] = 10;
    MatriceAdjacence[3][3] = INT8_MAX;
    MatriceAdjacence[3][4] = 10;


    MatriceAdjacence[4][0] = 100;
    MatriceAdjacence[4][1] = INT8_MAX;
    MatriceAdjacence[4][2] = INT8_MAX;
    MatriceAdjacence[4][3] = 10;
    MatriceAdjacence[4][4] = INT8_MAX;


    inventoryMax[0] = 7000;
    inventoryMax[1] = 5000;
    inventoryMax[2] = 3000;
    inventoryMax[3] = 2000;
    inventoryMax[4] = 4000;


    increaseSpeed[0] = 7000;
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

vector<path> min_onjective(vector<path> pastPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int capacityVehicle, int SupplierInventoryStock, int choice)
{
    vector<int> urgentNodes;
    for ( int i=0;i<numberOfRetailer;i++)
    {
        currentInventory[i] = inventoryMax[i];
    }
    /// Initialisation des paramètres en fonction du chemin précédent
    for (unsigned int l=0;l<pastPath.size();l++)
    {
        for (unsigned int i=0;i<pastPath[l].visited_node.size();i++)
        {
            SupplierInventoryStock -= inventoryMax[pastPath[l].visited_node[i]] - currentInventory[pastPath[l].visited_node[i]];
            currentInventory[pastPath[l].visited_node[i]] = inventoryMax[pastPath[l].visited_node[i]];
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
    }
    for(int i=0;i<numberOfRetailer;i++)
    {
        if(currentInventory[i] + increaseSpeed[i] < 0)
        {
            urgentNodes.push_back(i);
        }
    }

    /// Calcul des voisins du dernier noeud contenu dans le chemin
    vector<path> pathToGive;
    vector<int> neighbours = getNeighbours(pastPath[pastPath.size()-1], numberOfRetailer, matrice);

    /// Pour chaque voisin
    for (unsigned int i=0; i< neighbours.size(); i++)
    {
        path NewcurrentPath;
        NewcurrentPath.visited_node.push_back(0);
        NewcurrentPath.objectif_value = 0;
        NewcurrentPath.visited_node.push_back(neighbours[i]);

        /// On regarde si le chemin pris respecte les conditions en entrées ( le stock de l'inventaire est suffisant)
        int tempSupplier = SupplierInventoryStock - inventoryMax[neighbours[i]] + currentInventory[neighbours[i]];
        bool isValid = checkCondition(tempSupplier, capacityVehicle, choice, urgentNodes, NewcurrentPath, currentInventory, inventoryMax);
        if(!isValid)
        {
            continue;
        }

        /// On va chercher récursivement tous les fils de ce chemin et on remonte tous ceux qui sont corrects
        vector <path> NewTabPath;
        NewTabPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier, choice, urgentNodes);

        /// Permet de checker si le chemin parcours tous les noeuds critiques
        for (unsigned int k=0;k< NewTabPath.size();k++)
        {
            for(int j=1;j< numberOfRetailer;j++)
            {
                /// Si un noeud critique est déja visité, on ne le prend pas en compte
               if (find(NewTabPath[k].visited_node.begin(), NewTabPath[k].visited_node.end(), j) != NewTabPath[k].visited_node.end())
                {

                    continue;
                }
                /// Si un noeud critique n'a pas été visité, on ne prend pas en compte ce chemin
                if(currentInventory[j] + increaseSpeed[j] < 0)
                {
                    NewTabPath[k].objectif_value = -1;
                }
            }
            /// On ajoute le chemin dans la liste des chemins valides s'il respecte toutes les conditions
            if (NewTabPath[k].objectif_value != -1)
            {
                if(NewTabPath[k].objectif_value == 26200 && NewTabPath[k].visited_node[1] == 2 && NewTabPath[k].visited_node[2] == 7 && NewTabPath[k].visited_node[3] == 8 && NewTabPath[k].visited_node[4] == 5 && NewTabPath[k].visited_node[5]== 4)
                {
                    PrintPath(pastPath[1]);
                    PrintPath(NewTabPath[k]);
                }
                pathToGive.push_back(NewTabPath[k]);
            }
        }
    }
    return pathToGive;

}
/// Fonction récursive qui permet de récupérer l'ensemble des chemins de chaque temps
vector<path> Recursive_temps (vector<path> tabPath, int length, vector<path> precedence, int ** matricePointeur, int * inventoryMax, int * currentInventory, int numberOfNodes, int * increaseSpeed,  int capacityVehicle, int SupplierInventoryStock, int choice)
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
    /// Pour tous les paths valides récupérés au niveau supérieur
    vector<path> newTabPath;
    for (unsigned int i=0;i<tabPath.size();i++)
    {
        /// On push un chemin et on cherche tous les chemins possibles à partir de celui-ci
        precedence.push_back(tabPath[i]);
        newTabPath = min_onjective(precedence, matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, SupplierInventoryStock, choice);
        if(newTabPath.size() == 0)
        {
            precedence.pop_back();
            continue;
        }
        /// On lance le tour suivant
        vector<path> tabPrecedentPath = Recursive_temps(newTabPath, length-1,precedence,matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, SupplierInventoryStock, choice);
        precedence.pop_back();
        int min_tab_optimum = 0;

        /// On regarde si la liste de chemins contient la plus petite somme de fonction objective
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


    int numberOfNodes = 5;
    int numberOfTimeRunning = -1;

    cout << "Veuillez rentrez un nombre de tour de livraison : " << endl;
    cin >> numberOfTimeRunning;
    while (numberOfTimeRunning <0)
    {
        cout << "Veuillez rentrez un nombre positif :" << endl;
        cin >> numberOfTimeRunning;
    }
    cout << "Veuillez choisir le type d'algorithme que vous souhaitez lancer : " << endl;
    cout << "1 : Sans heuristique" << endl;
    cout << "2 : Premiere Heuristique" << endl;
    cout << "3 : Deuxieme Heuristique" << endl;
    cout << "4 : Les deux heuristiques" << endl;
    int choice;
    cin >> choice;

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
    currentPath = Recursive_temps(currentPath,numberOfTimeRunning,precedence, matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, currentInventory[0], choice);
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

    /// On affiche le temps d'execution
    time = clock() - time;
    cout << "Temps d'execution : " << ((float)time)/CLOCKS_PER_SEC;

    return 0;

}