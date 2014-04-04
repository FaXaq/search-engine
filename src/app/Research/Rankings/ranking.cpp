#include <cstdlib>
#include <ctime>

#include "ranking.h"

#define NB_ALEA_GENE 10
#define MAX_LENGHT 50

using namespace std;

int nbWord(string chaine,string word);
vector<Page> ranking(vector<Page> pageFound,vector<string> rechercheUser);
void echanger(int tableau[], vector<Page> page, int a, int b);
void quickSort(int tableau[],vector<Page> page,  int debut, int fin);

int main()
{

    //on obtient la recherche user
    string recherche="",word="";
    vector<string> keywordSearch;
    cout << "Votre recherche : ";
    getline(cin, recherche);

    for(int i=0; recherche[i]!='\0';i++)
    {
        if(recherche[i]==' ')
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
    vector<Page> allPage();
    //page 1 : Sur le traitement des bateaux
    vector<string> listWord;
    string titrePage="Les bateaux sont cool.", urlPage="http://www.bateau-cool.com",textPage="Venez voir mes bateaux carrement classe. Venez les tester, la vie est cool !";
    listWord.push_back("Bateau"); listWord.push_back("Ocean"); listWord.push_back("Navigation");
    allPage.push_back(Page(listWord,textPage,titrePage,urlPage) );

    //page 2 : informatique
    titrePage="Informatique pour geek", urlPage="http://www.geek.com", textPage="Un geek aime l informatique car l informatique c est le futur. Le futur est notre maitre.";
    listWord.clear();
    listWord.push_back("Informatique"); listWord.push_back("Geek"); listWord.push_back("futur"); listWord.push_back("cheat"); listWord.push_back("cool");
    allPage.push_back(Page(listWord,textPage,titrePage,urlPage) );

    /*--------FIN---------------------*/

    //ranking(allPage,keywordSearch);

    cout <<"Temps prit : "<< (t-clock()) <<  endl;

    return 0;
}

void echanger(int tableau[], vector<Page> page, int a, int b)
{
    //variable temporaire
    Page tmp(page[a]);

    //Trie score
    tableau[a] ^= tableau[b];
    tableau[b] ^= tableau[a];
    tableau[a] ^= tableau[b];

    //Trie page
    page[a]=page[b];
    page[b]=tmp;
}

void quickSort(int tableau[],vector<Page> page,  int debut, int fin)
{
    int gauche = debut-1;
    int droite = fin+1;
    const int pivot = tableau[debut];

    if(debut >= fin)
        return ;

    while(1)
    {
        do droite--; while(tableau[droite] > pivot);
        do gauche++; while(tableau[gauche] < pivot);

        if(gauche < droite)
            echanger(tableau, page, gauche, droite);
        else break;
    }

    quickSort(tableau, page, debut, droite);
    quickSort(tableau, page, droite+1, fin);
}

/*
CaseSensitive : Yes
If the word is seperate by space : No
*/
int nbWord(string chaine,string word)
{
    int nb(0),i(0),j(0),similaire(0);

    for(i=0;chaine[i]!='\0';i++)
    {
        if(chaine[i]==word[0])
        {
            for(j=1;word[j]!='\0' && chaine[i+j]!='\0';j++)
            {
                if(chaine[i+j]==word[j])
                    similaire++;
            }

            if( (j-1)==similaire)
                nb++;

            i+=(j-1);
            similaire=0;
        }
    }
    return nb;
}

vector<Page> ranking(vector<Page> pageFound,vector<string> rechercheUser)
{
    int score[rechercheUser.size()];
    const int coefTitle=16,coefDomaineName=8,coefKeyword=4,coefUrl=2,coefText=1;

    //------------------ETAPE 1----------------------
    //Parcour de toute les pages
    for(int i; i<pageFound.size();i++)
    {
        //parcour toute la recherche user
        for(int j; j<rechercheUser.size();j++)
        {
            score[i]+=coefTitle*nbWord(pageFound[i].Title,rechercheUser[j]);
            score[i]+=coefDomaineName*nbWord(pageFound[i].Url,rechercheUser[j]);
            for(int o; o<pageFound[i].Keywords.size();o++)
                score[i]+=coefKeyword*nbWord(pageFound[i].Keywords[o],rechercheUser[j]);
            score[i]+=coefUrl*nbWord(pageFound[i].Url,rechercheUser[j]);
            score[i]+=coefText*nbWord(pageFound[i].Text,rechercheUser[j]);
        }
    }


    //------------------ETAPE 2----------------------

    quickSort(score,pageFound,1,pageFound.size());

    return pageFound;

}