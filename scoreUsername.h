
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

struct nameScore {
	string name;
	int score;
};


class scoreUsername 
{
public:

	scoreUsername();
	scoreUsername(int, string);
	~scoreUsername();

	string getUsername();
	string highScoreTable();
	void setUsername(string usr);
	int getScore();
	void setScore(int scr);
	void checkScoreAndReplace();
	void Encrypt();
	void Decrypt();
	void fillArray();
	void storeArray();

	int checkScore[10]; //= {10,9,8,7,6,5,4,3,2,1};
	string checkUserName[10];// = { "ten","nine","eight","seven","six","five","four","three","two", "one" };

private:
	int _score;
	string _userName;
	

};
