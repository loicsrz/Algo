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
void objective_function (path *current, int * currentInventory, int number_retailer, int ** matrice, int * inventoryMax)
{
    int tmpvalues [3];
    //PrintPath(*current);

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
//       objective_number += inventoryMax[current->visited_node[j+1]] - currentInventory[current->visited_node[j+1]];
    }
    tmpvalues[1] = objective_number;

    /// Ajout de la valeur situé dans le camion et livré aux clients
//    objective_number += inventoryMax[current->visited_node[current->visited_node.size()-1]] - currentInventory[current->visited_node[current->visited_node.size()-1]];
    current->objectif_value = objective_number;

    tmpvalues[2] = objective_number;
//    if (objective_number >= 26220 && objective_number <= 26600)
//    {
//        PrintPath(*current);
//        for (int i=0;i<3;i++)
//        {
//            cout << tmpvalues[i]<< endl;
//        }
//    }

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

        if(find(urgentNodes.begin(),urgentNodes.end(),neighbours[i]) != urgentNodes.end())
        {
            urgentNodes.erase(find(urgentNodes.begin(),urgentNodes.end(),neighbours[i]));
            erase = true;
        }

        /// on met à jour l'entrepôt en fonction du nouveau noeud et on regarde si cela respecte les conditions
        int tempSupplier = SupplierInventoryStock - inventoryMax[neighbours[i]] + currentInventory[neighbours[i]];
        bool isValid = checkCondition( tempSupplier, capacityVehicle, choice, urgentNodes, currentPath, currentInventory, inventoryMax);
        if(!isValid)
        {
            if(erase)
            {
                urgentNodes.push_back(neighbours[i]);
            }
            continue;
        }

        /// On va récupérer de maniére récursive les paths fils
        //objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice, inventoryMax);
        vector <path> NewTabPath;
        NewTabPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier, choice, urgentNodes);
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
                    PrintPath(NewTabPath[k]);
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
    objective_function(&currentPath, currentInventory, numberOfRetailer, matrice, inventoryMax);
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

vector<path> min_onjective(vector<path> pastPath, int **  matrice, int * inventoryMax, int * currentInventory, int numberOfRetailer, int * increaseSpeed, int capacityVehicle, int SupplierInventoryStock, int choice)
{
    vector<int> urgentNodes;
    for (unsigned int i=0;i<numberOfRetailer;i++)
    {
        currentInventory[i] = inventoryMax[i];
    }
    //cout << "4.0 : " << currentInventory[4] << endl;
    //cout << "pastPath : " << pastPath.size() << endl;
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
    for(unsigned int i=0;i<numberOfRetailer;i++)
    {
        if(currentInventory[i] + increaseSpeed[i] < 0)
        {
            urgentNodes.push_back(i);
        }
    }
   // cout << "4.1 : " << currentInventory[4] << endl;

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
        //objective_function(&NewcurrentPath, currentInventory, numberOfRetailer, matrice, inventoryMax);
        vector <path> NewTabPath;
        NewTabPath = recursiveFunction(NewcurrentPath, matrice, inventoryMax, currentInventory, numberOfRetailer, increaseSpeed, capacityVehicle, tempSupplier, choice, urgentNodes);
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
                if(NewTabPath[k].objectif_value == 26200 && NewTabPath[k].visited_node[1] == 2 && NewTabPath[k].visited_node[2] == 7 && NewTabPath[k].visited_node[3] == 8 && NewTabPath[k].visited_node[4] == 5 && NewTabPath[k].visited_node[5]== 4)
                {
                    PrintPath(pastPath[1]);
                    PrintPath(NewTabPath[k]);
//        cout << j << endl;
//        cout << currentInventory[j] << endl;
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
           // cout << "Precendence 1 : " <<precedence.size() << endl;
            precedence.push_back(MinPath);
           // cout << "Precendence 2 : " <<precedence.size() << endl;
        }
        return precedence;
    }

    vector<path> newTabPath;
    for (unsigned int i=0;i<tabPath.size();i++)
    {
        precedence.push_back(tabPath[i]);
        newTabPath = min_onjective(precedence, matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, SupplierInventoryStock, choice);
        if(newTabPath.size() == 0)
        {
            precedence.pop_back();
            continue;
        }
        //cout << "Precendece size : " << precedence.size()<<endl;
        vector<path> tabPrecedentPath = Recursive_temps(newTabPath, length-1,precedence,matricePointeur, inventoryMax, currentInventory, numberOfNodes, increaseSpeed, capacityVehicle, SupplierInventoryStock, choice);
        precedence.pop_back();
        int min_tab_optimum = 0;
        for (unsigned int k=0;k<tabPrecedentPath.size();k++)
       {
           min_tab_optimum += tabPrecedentPath[k].objectif_value;
       }
     //  cout << "En cours de calcul :" << min_tab_optimum << endl;
       if (min_tab_optimum < minTabPathObjective && min_tab_optimum !=0)
       {
           minTabPath.clear();
           //cout<<"NOUVEAU"<< endl;

           for(unsigned int j=0;j<tabPrecedentPath.size();j++)
           {
                minTabPath.push_back(tabPrecedentPath[j]);
//                if(j==2 )
//                {
//                    cout << "Length" << length << endl;
//                    PrintPath(tabPrecedentPath[1]);
//                    PrintPath(tabPrecedentPath[j]);
//                    for (int l=0; l<numberOfNodes;l++)
//                    {
//                        cout << currentInventory[l] << endl;
//                    }
//                    cout << endl;
//                }

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

    time = clock() - time;

    cout << "Temps d'execution : " << ((float)time)/CLOCKS_PER_SEC;

    return 0;

}