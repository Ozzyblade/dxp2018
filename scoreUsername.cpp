#include "scoreUsername.h"

scoreUsername::scoreUsername(){
	fillArray();
}

scoreUsername::scoreUsername(int score, string userName)
	: _score(score), _userName(userName) {
	fillArray();
}

string scoreUsername::getUsername() {
	return _userName;
}

string scoreUsername::highScoreTable()
{
	string s;
	for (int i = 0; i < 9; ++i)
	{
		s += checkUserName[i] + ": " + to_string(checkScore[i]) + "\n";
	}
	return s;
}

void scoreUsername::setUsername(string usr)
{
	_userName = usr;
}

int scoreUsername::getScore() {
	return _score;
}

void scoreUsername::setScore(int scr)
{
	_score = scr;
}

scoreUsername::~scoreUsername()
{
	storeArray();
}

void scoreUsername::checkScoreAndReplace()
{
	bool swapped = false;
	int position = 10;
	for( int i(0); i < 10; i++)
	if (_score > checkScore[i])
	{
	position = i;
	i = 10;
	swapped = true;
	}
	if (swapped != true)
		return;
	for (int i = 9; i > position; --i)
	{
		checkScore[i] = checkScore[i - 1];
		checkUserName[i] = checkUserName[i - 1];


	}
	checkScore[position] = _score;
	checkUserName[position] = _userName;
}
void scoreUsername::Encrypt() 
{

	swap(checkUserName[2], checkUserName[7]);
	swap(checkUserName[9], checkUserName[6]);
	swap(checkUserName[1], checkUserName[4]);
	swap(checkUserName[6], checkUserName[4]);
	swap(checkUserName[0], checkUserName[8]);
	swap(checkUserName[3], checkUserName[7]);
	
	swap(checkScore[7], checkScore[3]);
	swap(checkScore[9], checkScore[1]);
	swap(checkScore[6], checkScore[2]);
	swap(checkScore[6], checkScore[7]);
	swap(checkScore[0], checkScore[5]);
	swap(checkScore[4], checkScore[8]);

	for (int i = 0; i < 10; i++)
	{

		int Key = 54213;
		//string Skey = "7";
		int encryptScore = checkScore[i];
		string encryptUsername = checkUserName[i];

		//for (int i = 1; i < encryptUsername.size(); i++)
		swap(encryptUsername[0], encryptUsername[1]);
		swap(encryptUsername[1], encryptUsername[2]);
		



		encryptScore = (encryptScore ^ Key);
		//encryptUsername = encryptUsername[1] ^ Skey[1];

		checkScore[i] = encryptScore;
		checkUserName[i] = encryptUsername;

	}
}


void scoreUsername::Decrypt()
{
	swap(checkUserName[3], checkUserName[7]);
	swap(checkUserName[0], checkUserName[8]);
	swap(checkUserName[6], checkUserName[4]);
	swap(checkUserName[1], checkUserName[4]);
	swap(checkUserName[9], checkUserName[6]);
	swap(checkUserName[2], checkUserName[7]);

	
	
	
	
	swap(checkScore[4], checkScore[8]);
	swap(checkScore[0], checkScore[5]);
	swap(checkScore[6], checkScore[7]);
	swap(checkScore[6], checkScore[2]);
	swap(checkScore[9], checkScore[1]);
	swap(checkScore[7], checkScore[3]);
	

	for (int i = 0; i < 10; i++)
	{

		int Key = 54213;
		//string Skey = "7";
		int decryptScore = checkScore[i];
		string decryptUsername = checkUserName[i];
		//for (int i = 1; i < decryptUsername.size(); i++)
		
		swap(decryptUsername[1], decryptUsername[2]);
		swap(decryptUsername[0], decryptUsername[1]);

		decryptScore = (decryptScore ^ Key);
		//decryptUsername = decryptUsername[1] ^ Skey[1];

		checkScore[i] = decryptScore;
		checkUserName[i] = decryptUsername;

	}
}

//get from txt file
void scoreUsername::fillArray()
{
	ifstream inFile;

	inFile.open("scoreUsername.txt");


	//check error
	if (inFile.fail())
	{
		cerr << "error no file" << endl;
		exit(1);
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{

			inFile >> checkScore[i] >> checkUserName[i];
		}
	}
	inFile.close();

	
	Decrypt();
}

//write into text file
void scoreUsername::storeArray()
{
	ofstream outFile;
	Encrypt();

	outFile.open("scoreUsername.txt");
	for (int i = 0; i < 10; i++)
	{
		
		outFile << checkScore[i] <<" "<< checkUserName[i] << endl;
	}

	outFile.close();
}