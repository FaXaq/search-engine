/*
		*******************
		** Crawler Class **
		*******************

#Constructor :
	- Takes an URL in parameter
	- Insert the URL into the vector of links that will be used during the crawl

#WriteCallback :
	- Function to retieve the buffer of the page downloaded by CURL

#Crawl :
	- Main function of the Crawler

#DownloadFile :
	- Does it deserve an explanation !?

*/


//Includes libs

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <curl/curl.h>

//Include files

#include "crawler.h"
#include "../Misc/misc.h"
#include "../../Database/DatabaseManager.h"

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) //fonction pour récupérer le texte de la page. On est obligé de passer par un buffer.
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


Crawler::Crawler(string url){

	//Insert an url in the stack of URL

	_url.push_back(url);

	//Launch the main function of the crawler

	crawl();
}

/*

Description of Crawl Algo :

For each link : 
	- Download page
	- Parse it (extract links & keywords)
	- Add links to URL stack
	- Saving page in the DB
	- At each start of the loop check the size of our stack (even if links have been added during the first loop they will be considered in the further ones)

*/

void Crawler::crawl(){

	//Bool will check if stack of url is fully crawled

	bool crawling = true;
	while (crawling == true){

		//Each time it'll check the size of _url stack (even if url have been added during the loop)

		for (int i = 0; i < _url.size(); i++)
		{
			//cout << "starting download ..." << endl;


			string tmp = downloadFile(_url[i]); //Downloading the file
			if (tmp != ""){//Checking if page contains something

				cout << endl << "crawling : " << _url[i] << endl << "-----------------------------" << endl << endl;
				//cout << "starting new page : " << _url[i] << endl;

				Page p = Page(_url[i],tmp);  //Creating a new Page with URL & Content downloaded of the page.

				p.displayKeywords();

				vector<string> tmp = p.get_links(); //Retrieview links from the parser

				_pages.push_back(p); //Stocking the page in a vector 
				int sizeTmp = tmp.size(); //Checking if links are ok to stock in the _url stack

				for(int k = 0; k < sizeTmp; k++)
				{
					if (find(_url.begin(), _url.end(), tmp[k]) - _url.begin() == _url.size())
		        		{
		        			if (!has_suffix(tmp[k],".css") && !has_suffix(tmp[k],".js")){
		        					_url.push_back(tmp[k]);
		        				}
		        		}
		        }

		        if (p.get_title() == ""){
	        		p.set_title(p.get_url()); //Check the title
		        }

		        if (p.get_description() == ""){ //Check the description
		        	string tmp_desc = p.get_plain_text();
		        	string from = "\n";
				    string to = " ";
				    size_t start_pos = 0;
				    size_t end_pos = 0;
				    while((start_pos = tmp_desc.find(from, start_pos)) != std::string::npos) {
				        size_t end_pos = start_pos + from.length();
				        tmp_desc.replace(start_pos, end_pos, to);
				        start_pos = 0;
				    }
				    p.set_plain_text(tmp_desc);
		        	p.set_description(p.get_plain_text().substr(0,200));
		        }

		        cout << "title : " << p.get_title() << endl;
		        cout << "description : " << p.get_description() << endl;

	        	cout << endl << "+++++++++++++++++++++++" << endl << endl;

				DatabaseManager manager = DatabaseManager("tcp://192.168.1.27:3306", "root", "bitnami", "searchengine"); //Stocking in the DB
				manager.savePage(p);
		    }
	    }
	    crawling = false;
	}
}

string Crawler::downloadFile(string url){

	CURL *curl;
	CURLcode res;
	string readBuffer;
    long http_code = 0;
    if (has_suffix(url, "/"))
	const string url_tmp = url;
	else
	const string url_tmp = url+"/";
	 
	curl = curl_easy_init();
	if(curl) {
		//Setting the Curl easy
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,true);

	// Perform the request, res will get the return code 
	res = curl_easy_perform(curl);
	curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

	// Check for errors
	if(res != CURLE_OK)
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

	// always cleanup
	curl_easy_cleanup(curl);
	}
	cout << "download ok (with code : " << http_code << ")" << endl;
	if (http_code == 200 || http_code == 301 || http_code == 302)
		return readBuffer;
	else
		return "";
}

//Getter & Setters

vector<Page> Crawler::getPages(){
	return _pages;
}