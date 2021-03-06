#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <sstream>

#include "../Crawler/Page/page.h"
#include "../Database/DatabaseManager.h"

std::vector<Page> ranked(std::string recherche, int level); // Va chercher dans la base de donn�e les pages, sort les mots cl� dans la chaine de recherche
int nbWord(const std::string& chaine,const std::string& word,const int& level); // Compte le nombre de mot (3 niveaux)
void ranking(std::vector<Page>& pageFound,std::vector<std::string>& rechercheUser,const int& lvlSrch); //Attribue un score puis trie
void swapPage(int tableau[], std::vector<Page>& page,const int& a,const int& b); //�change les pages a et b ainsi que le score a et b
void quickSort(int tableau[],std::vector<Page>& page,const int& debut,const int& fin); //Trie les pages en fonction du score
std::string lowerString(const std::string& str); //Met une chaine de charact�re en minuscule

std::vector<Page> ranked(std::string recherche, int level)
{
    //on obtient la recherche user
    std::string word = "";
    std::vector<std::string> keywordSearch;

    for (int i = 0; recherche[i] != '\0'; i++)
    {
        if (recherche[i] == ' ')
        {
            word.push_back('\0');
            keywordSearch.push_back(word);
            word.clear();
        }
        else
            word.push_back(recherche[i]);
    }
    word.push_back('\0');
    keywordSearch.push_back(word);
    word.clear();

    //initialisation du temps
    srand(time(0));
    clock_t t;
    t = clock();

    /*---------CREATION PAGE-----------*/
    std::vector<Page> allPage;
    std::vector<Page> result;

    //BASE DE DONNEE
    //connexion et reccuperation des pages
    DatabaseManager manager("tcp://192.168.1.27:3306", "root", "bitnami", "searchengine");
    allPage = manager.getPages(recherche);

    /*--------FIN---------------------*/

    ranking(allPage, keywordSearch, level);

    const int sizePage = allPage.size();

    return allPage;
}

string to_JSON(bool success, int size, int time, std::vector<Page>& pages, std::vector<int> secundaries)
{
    std::string output = "";

    // On cr�e l'objet JSON
    output += "{ \"search\": {";

    // Ajout du succ�s
    if (success)
        output += "\"success\":true,";
    else
    {
        output += "\"success\":false";
        output += "}}";
        return output;
    }

    // Ajout de la taille et du temps
    // On convertit d'abord les donn�es en string
    std::string size_string;
    std::string time_string;
    ostringstream convert_size;
    ostringstream convert_time;

    convert_size << size;
    size_string = convert_size.str();
    convert_time << time;
    time_string = convert_time.str();

    output += "\"size\":" + size_string + ",";
    output += "\"time\":" + time_string + ",";

    // On cr�e le premier tableau de pages
    output += "\"results\": [";

    int pages_size = pages.size() > 25 ? 25 : pages.size();

    // On y ajoute chaque page du vecteur de page
    for (int i = 0 ; i < pages_size ; i++)
    {
        output += pages[i].to_JSON();

        if (i < (pages_size - 1))
            output += ",";
        else 
            break;
    }

    output += "],";

    // On cr�e ensuite le tableau des ID restantes
    output += "\"secundaries\": [";
    int size_secundaries = secundaries.size();

    for (int j = 0 ; j < size_secundaries ; j++)
    {
        std::string id;
        ostringstream convert_id;
        convert_id << secundaries[j];
        id = convert_id.str();

        output += id;

        if (j < (size_secundaries - 1))
            output += ",";
    }

    // On ferme le tableau, l'objet et la chaine JSON
    output += "] } }";

    return output;
}

void swapPage(int tableau[], std::vector<Page>& page,const int& a,const int& b)
{
    try{
        //page temporaire
        Page tmp = page.at(a);

        //Trie score
        tableau[a] ^= tableau[b];
        tableau[b] ^= tableau[a];
        tableau[a] ^= tableau[b];

        //Trie page
        page.at(a) = page.at(b);
        page.at(b) = tmp;
    }catch(std::exception &e)
    {
        std::cout << e.what();
    }

}

void quickSort(int tableau[],std::vector<Page>& page,const int& debut,const int& fin)
{
    int gauche = debut-1;
    int droite = fin+1;
    const int pivot = tableau[debut];

    if(debut >= fin)
        return ;

    while(1)
    {
        do droite--; while(tableau[droite] < pivot);
        do gauche++; while(tableau[gauche] > pivot);

        if(gauche < droite)
            swapPage(tableau, page, gauche, droite);
        else break;
    }

    quickSort(tableau, page, debut, droite);
    quickSort(tableau, page, droite+1, fin);
}

std::string lowerString(const std::string& str)
{
    const int lg = str.length();
    std::string low(str);

    for(int i = 0; i < lg; i++)
    {
        if(low[i] >='A' && 'Z' >= low[i])
            // 'a'-'A'= 32 <=> 'A'+32='a'
            low[i] += 32;
    }

    return low;
}

int nbWord(const std::string& chaine,const std::string& word,const int& level)
{
    int nb(0), i(0), j(0), similaire(0);

    //Si le niveau est strict
    if(level == 3)
    {
        for(i = 0; chaine[i] != '\0'; i++) //parcour de la chaine
        {
            if(chaine[i] == word[0])
            {
                for(j=1; word[j] != '\0' && chaine[i+j] != '\0'; j++) //comparaison lettre par lettre
                {
                    if(chaine[i+j] == word[j])
                        similaire++;
                }

                // -2 car on commence a comparait a l'emplacement 0 (tab[0]) et qu'on commence similaire++ a 1.
                if( (int)(word.size()-2) == similaire)
                    nb++;

                i += (j-1);
                similaire = 0;
            }
        }
    }
    //si il est mod�r�
    else if(level == 2)
    {
        std::string chaineLow(lowerString(chaine)), wordLow(lowerString(word));

        for(i=0;chaineLow[i] != '\0';i++)
        {
            if(chaineLow[i] == wordLow[0])
            {
                for(j=1; wordLow[j] != '\0' && chaineLow[i+j] != '\0'; j++)
                {
                    if(chaineLow[i+j] == wordLow[j])
                        similaire++;
                }

                if( (int)(wordLow.size()-2) == similaire)
                    nb++;

                i+=(j-1);
                similaire=0;
            }
        }

    }
    //Si il est tolerant
    else
    {
        std::string chaineLow(lowerString(chaine)), wordLow(lowerString(word));
        std::vector<bool>chaineBin( chaineLow.size() - 2 );
        int maxi = 0, minSize = (int)(wordLow.size()/2);

        for(i = 0; chaineLow[i] != '\0'; i++) //on parcour la chaine
        {
            for(int iw = 0; iw < minSize+1; iw++) //on parcour le word
            {
                if(chaineLow[i] == wordLow[iw]) //Si la premi�re lettre est identique
                {
                    chaineBin[i] = true;
                    for(j = 1; wordLow[j+iw] != '\0' && chaineLow[i+j] != '\0' && chaineBin[i+j-1] == true; j++) //on compare
                    {
                        if(chaineLow[i+j] == wordLow[j+iw])
                            chaineBin[i+j] = true; //On a une similitude !
                        else
                            chaineBin[i+j] = false;
                    }

                    if(maxi < j)
                        maxi = j;
                }
            }
            if(maxi < minSize) // si on trouve aucun word similaire faisant plus de la moitier du word, on initialise le tableau binaire
            {
                for(int z = 0; z < maxi; z++)
                    chaineBin[i+z] = false;
            }
            else //si la chaine identique la plus longue est sup�rieur ou �gal a la moitier du word
            {
                i += maxi - 1;
                nb++;
            }
        }
    }

    return nb;
}

void ranking(std::vector<Page>& pageFound,std::vector<std::string>& rechercheUser,const int& lvlSrch)
{
    //Alocation dynamique du score en fonction du nombre de page
    int *score = new int[pageFound.size() + 1];
    const int coefTitle = 16, coefDomaineName = 8, coefKeyword = 4, coefUrl = 2, coefText = 1, sizePage = pageFound.size();
    const int sizeSearch = rechercheUser.size();

    //------------------ETAPE 1----------------------
    //Parcour de toute les pages
    for(int i = 0; i < sizePage; i++)
    {
        score[i] = 0;
        //parcour toute la recherche user
        for(int j = 0; j < sizeSearch; j++)
        {
            score[i] += coefTitle * nbWord( pageFound[i].get_title(), rechercheUser[j], lvlSrch);
            //cout << "Titre : " << score[i] << ' ';
            score[i] += coefDomaineName * nbWord( pageFound[i].get_url(), rechercheUser[j], lvlSrch);
            //cout << "Domaine : " << score[i] << ' ';
            const int sizeKeyword = pageFound[i].get_keywords().size();
            for(int o = 0; o < sizeKeyword; o++)
                score[i] += coefKeyword * nbWord( pageFound[i].get_keywords()[o], rechercheUser[j], lvlSrch);
            //cout << "Keywords : " << score[i] << ' ';
            score[i] += coefUrl * nbWord( pageFound[i].get_url(), rechercheUser[j], lvlSrch);
            //cout << "Url : " << score[i] << ' ';
        }

        //cout << "Score : " << score[i] << " de :" << pageFound[i].get_title() << endl;
    }


    //------------------ETAPE 2----------------------

    quickSort(score, pageFound, 0, pageFound.size() - 1);


    //---------------Etape pour les 10 meilleur pages--------------
    for (int i = 0; i < sizePage && i < 10; i++)
    {
        for(int j = 0; j < sizeSearch; j++)
        {
            score[i] += coefText * nbWord(pageFound[i].get_content(), rechercheUser[j], lvlSrch);
            //cout << "Words : " << score[i] << endl;
        }
    }

    quickSort(score, pageFound, 0, (sizePage <= 10) ? pageFound.size() - 1 : 10);


    //delete[] score;

}

/*void erasePage(std::vector<Page>& pageFound,int *score)
{
    //surprime les pages avec un score de 0
    searchDichotomique(pageFound.size(),0);
}

void searchDichotomique(const int& sizeTab,const int *score,const int& value)
{
    //recherche dichotomique en fonction de la valeur

    //On recherche
    int pos = 0;
    for( pos = (int)(sizeTab/2); pos >= 0 && pos < sizeTab && score[pos] != value; (score[pos] > value) ? pos += (sizeTab-pos)/2 : pos -= (sizeTab-pos)/2);

    //Si il y a des doublons de valeur, on recherche la place de la premi�re valeur aparraissante

}*/

#endif // RANKING_H_INCLUDED
