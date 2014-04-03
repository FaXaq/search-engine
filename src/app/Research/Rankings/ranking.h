#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

class Page{
public :
    int ID;
	vector<string> Keywords;
	string Text;
	string Title;
	string Url;

    //constructeur
	Page(vector<string> fKeywords,string fText,string fTitle,string fUrl);
	//constructeur copie
	Page& operator=(Page const& pageACopier);

};
Page::Page(vector<string> fKeywords,string fText,string fTitle,string fUrl) : Keywords(fKeywords), Text(fText), Title(fTitle), Url(fUrl)
{
    //rien
}

Page& Page::operator=(Page const& pageACopier)
{
    if(this != &pageACopier)
    //On v�rifie que l'objet n'est pas le m�me que celui re�u en argument
    {
        Keywords = pageACopier.Keyword;
        Text = pageACopier.Text;
        Title = pageACopier.Title;
        Url = pageACopier.Url;
    }
    return *this; //On renvoie l'objet lui-m�me
}

#endif // RANKING_H_INCLUDED
