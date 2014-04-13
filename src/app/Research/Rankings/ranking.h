#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>


//-------------------------------------------CLASS PAGE--------------------------------
class Page{
public :
    int ID;
	std::vector<std::string> Keywords;
	std::string Text;
	std::string Title;
	std::string Url;

    //constructeur
	Page(std::vector<std::string> fKeywords,std::string fText,std::string fTitle,std::string fUrl);
	//constructeur copie
	Page& operator=(Page const& pageCopy);

};
Page::Page(std::vector<std::string> fKeywords,std::string fText,std::string fTitle,std::string fUrl) : Keywords(fKeywords), Text(fText), Title(fTitle), Url(fUrl)
{
    //rien
}

Page& Page::operator=(Page const& pageCopy)
{
    if(this != &pageCopy)
    //On v�rifie que l'objet n'est pas le m�me que celui re�u en argument
    {
        const int sizeW = pageCopy.Keywords.vector::size();
        Keywords.clear(); //par s�curit�
        for(int i = 0; i<sizeW;i++ )
            Keywords.push_back(pageCopy.Keywords[i]);

        Text = pageCopy.Text;

        Title = pageCopy.Title;

        Url = pageCopy.Url;
    }
    return *this; //On renvoie l'objet lui-m�me
}
//-------------------------------------------CLASS PAGE--------------------------------


int nbWord(const std::string& chaine,const std::string& word,const int& level);
void ranking(std::vector<Page>& pageFound,std::vector<std::string>& rechercheUser,const int& lvlSrch);
void swapPage(int tableau[], std::vector<Page>& page,const int& a,const int& b);
void quickSort(int tableau[],std::vector<Page>& page,const int debut,const int fin);
std::string lowerString(const std::string& str);


#endif // RANKING_H_INCLUDED
