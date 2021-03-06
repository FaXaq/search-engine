#include <iostream>
#include <sstream>

#include "ranking.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc >= 2)
    {
        int level = 2;
        string recherche = "";

        //take information for argument
        istringstream ss(argv[1]); //take the level of search
        if (!(ss >> level))
        {
            cerr << "Invalid number " << argv[1] << '\n';
            return 0;
        }


        for(int i = 2; i < argc; i++)
            recherche += argv[i] + std::string(" "); // here all string

        std::vector<Page> allPage = ranked(recherche, level); //ranking

        //split information into 2 data : 25 on a vector page and other in vector int (the id)
        std::vector<int> sec;

        if(allPage.vector::size() > 25)
        {
            const int sizePage = allPage.vector::size();

            for(int i = 25; i<sizePage; i++)
                sec.push_back(allPage[i].get_ID());
        }

        if(allPage.vector::size() >= 1)
            cout << to_JSON(true, allPage.vector::size(), 0, allPage, sec);
        else 
            cout << to_JSON(false, 0, 0, allPage, sec);
    }

    else
        cout << "You must enter parameters. Use : ./program <level> <keywords>";

    return 0;
}
