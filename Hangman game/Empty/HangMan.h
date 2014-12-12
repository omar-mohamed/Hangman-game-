#include<iostream>
#include<string>
#include <mmsystem.h>
#include<math.h>
#pragma comment( lib, "winmm" ) 
#include<conio.h>
#include<ctime>
using namespace std;	
class HangMan
{
	int COUNTER_FAILS;	//for const in this class only.
	string	clearScreen;
	
	public:
	HangMan()
	{
		COUNTER_FAILS=10;
		clearScreen="                                                                                                                                                      ";
	}
	void CLS()                        //for clear the screen(^_^)
	{
					text_out(&clearScreen[0],20,40);
					text_out(&clearScreen[0],20,60);
					text_out(&clearScreen[0],20,80);
					text_out(&clearScreen[0],20,100);
					text_out(&clearScreen[0],20,120);
					text_out(&clearScreen[0],20,140);
					text_out(&clearScreen[0],20,160);
					text_out(&clearScreen[0],20,180);
					text_out(&clearScreen[0],20,200);
					text_out(&clearScreen[0],20,220);
					text_out(&clearScreen[0],20,240);
					hide_sprite(0, 350, 100);
					hide_sprite(1, 350, 100);
					hide_sprite(2, 350, 100);
					hide_sprite(3, 350, 100);
					hide_sprite(4, 350, 100);
					hide_sprite(5, 350, 100);
					hide_sprite(6, 350, 100);
					hide_sprite(7, 350, 100);
					hide_sprite(8, 350, 100);
					hide_sprite(9, 350, 100);
					hide_sprite(10, 350, 100);
					hide_sprite(11, 350, 100);
					hide_sprite(12, 350, 0);
					hide_sprite(13, 350, 0);
	}
	void startGame()
		{   
			
			CLS();
			put_sprite(0, 350,100);
			char *choice='\0';
			text_out("*Welcome to hang man game*",50,20,4);
			text_out("press(1):one player against the computer.",20,40);
			text_out("press(2):two players against each other.",20,60);
			text_out("press(3):for more information about hang man.",20,80);
			choice=input("your choice is:",20,100);
			switch (*choice)
			{
				case('1'):
					playHangmanWithComputer();
					break;
				case ('2'):
					playHangmanWithAnother();
					break;
				case('3'):
					info();
			}
				CLS();
				hide_sprite(0, 350, 100);
		}
						
	void playHangmanWithComputer()
	{

		CLS();
		hide_sprite(10, 350, 100);
		hide_sprite(0, 350, 100);
		int countFail = 0;	
		string choosenWord = chooseRandomWord();
		string dashedWord = "";
		dashedWord = dashedWord.append(choosenWord.length(), '-');   //print (-) instead of word.
	    for (int i=0;i<choosenWord.length();i++)
	      {
		          if(choosenWord[i]==' ')
		          dashedWord[i]=' ';
	      }
		text_out(&dashedWord[0],200,80);
		text_out("Enter your guess :  ",20,40);
		char guess;
		while(guess = *input("",145,40))
		{
			text_out("Enter your guess :  ",20,40);
			checkChar(guess, choosenWord,countFail, dashedWord);
					if (countFail>0)
					{
					put_sprite(countFail,350,100);     //print hangman pic
					}
			if(countFail >= COUNTER_FAILS)
			{
				CLS();
				put_sprite(10,350,20);
				text_out("You Lose...the word is: ",20,60);
				text_out(&choosenWord[0],180,60);
				playAgain();
			}

			if(choosenWord == dashedWord)
			{
				prize(choosenWord);
				break;
			}
			text_out("Enter your guess :     ",20,40);
		}		

		playAgain();
	}
	void playHangmanWithAnother()
{
		CLS();
	string anotherPlayerWord="";
	int countFail = 0;
	char guess;
	string dashedWord = "";
	
	anotherPlayerWord=input("Player1 : please enter your word: ",20,40);
	CLS();

	dashedWord	= dashedWord.append(anotherPlayerWord.length(),'-');
		for (int i=0;i<anotherPlayerWord.length();i++)
	{
		if(anotherPlayerWord[i]==' ')
		dashedWord[i]=' ';
	}
	text_out(&dashedWord[0],200,80);
	guess=*input("player2:Enter your guess :  ",20,60);
	for(int i=0;i<100;i++)
	{
	if ( guess >= 'A' && guess <= 'Z') 
    guess+=32;
	}
	CLS();
	while(guess != '\r')
	{
		text_out("player2:Enter your guess :    ",20,60);
		checkChar(guess, anotherPlayerWord, countFail, dashedWord);
			if(countFail>0)											//we don't want the first pic any more
			put_sprite(countFail,350,100);
		if(countFail >= COUNTER_FAILS)
		{
			CLS();
			text_out("You Lose...the word is : ",20,60);
			put_sprite(countFail,350,100);
			text_out(&anotherPlayerWord[0],180,60);
			playAgain();
		}
		if(anotherPlayerWord == dashedWord)
		{
			prize(anotherPlayerWord);
			break;
		}
		text_out("player2:Enter your guess :    ",20,60);
		guess=*input("player2:Enter your guess :  ",20,60);
	}		
}
	void checkChar(char guess, string choosenWord, int& countFail, string& dashedWord)
	{
		int matchings=0;
		int x=20;
			for(int i=0;i<100;i++)
	{
	if ( guess >= 'A' && guess <= 'Z') 
    guess+=32;
	}
			char red[2]={guess,NULL};
					for(int i=0; i<(choosenWord.length()); i++ )
					{

						if(guess==choosenWord[i])
						{
							hide_sprite(0, 350, 100);
							dashedWord[i]=guess;
							matchings++;
						}
					}
					if(matchings==0)
					{
						x+=15*countFail;
						text_out(red,x,180,4);
						x+=15;
							countFail++;
					}
					text_out("                          ",200,80);
					text_out(&dashedWord[0],200,80 );
					text_out("Remaining trials:",20,120);
					string remainingCount = "";
					remainingCount = remainingCount.append( 1, (COUNTER_FAILS - countFail + 48));// use implicit int to chat cast
					text_out(&remainingCount[0],130,120 );

	}
	void info()
	{
			CLS();
			char infoChoice='n';
			put_sprite(13, 400,80);
		text_out("1:U can play with another player.any one can enter the word,and the other one guess it.",20,40);
		text_out("2:U can play with the computer,and the word will be random.",20,60);
		text_out("3:U have ( 10 )trials to complete the word.or u lose.",20,80);
		text_out("***GOOD LUCK***",80,140,1);
		text_out("The programmers of this game",20,200,6);
		text_out("OMAR MOHAMED &",40,220,7);
		text_out(" OSMAN ISLAM SAMI",150,240,7);
		playAgain();
	}
	void prize(string selectedWord)
	{
						CLS();
						
						text_out("congratulation.... u win (^__^)", 20, 140);
						text_out("The word is:",20,160);
						text_out(&selectedWord[0],100,160);
						put_sprite(12,350,0);
						playAgain();
	}
	void playAgain()
	{
		char infoChoice;
		infoChoice=*input("Do u want to play now(y or n)?...",20,180);
			if(infoChoice=='y'||infoChoice=='Y')
			 {
				 CLS();

				 startGame();
			 }
			else
			exit(0);

	}
	string chooseRandomWord()
	{
		put_sprite(0, 350,100);
		srand(time(0));	
		int choice;
		char *str[100];
			text_out("press(1):Movies.",20,40);
			text_out("press(2):Countries",20,60);
			text_out("press(3):Animals",20,80);
			text_out("press(4):Football clubs",20,100);
			*str= input("Your choice is : ",20,120);
	        choice = atoi(*str);
        switch (choice)
	{
		
		case (1):
			{
				CLS();
				string 	movies[52]={"inception", "hancock", "immortals","avatar","the avengers","the hunger games","the dark knight","wanted","toy story","the godfather","harry potter","transformesrs","transporter","titanic","star wars","shrek","the lord of the rings","spiderman","jurassic park","finding nemo","the lion king","iron man","indiana jones","pirates of the caribbean","twilight","up","the matrix","jaws","i am legend","star trek","men in black","cars","the notebook","war of the worlds","king kong","mission impossible","braveheart","sherlock holmes","terminator","tangled","happy feet","pearl harbor","gladiator","the fugitive","thor","the karate kid","home alone","the ring","national treasure","tron","tarzan","true grit"};
				return movies[rand() % 52];	
			}
			break;
		case (2):
			{
				CLS();
			string countries[42]={"egypt", "spain", "brazil","italy","turkey","finland","france","england","china","canada","japan","america","syria","sudan","india","iraq","kuwait","korea","libya","ecuador","germany","iceland","greece","honduras","dominica","australia","saudi arabia","taiwan","zambia","yemen","tunisia","zimbabwe","morocco","lebanon","panama","qatar","chile","russia","argentina","portugal","mexico","cameron"};
			return countries[rand() % 42];
			}
			break;
		case (3):
			{
				CLS();
			string animals[55]={"lion", "dog", "cat","cow","alligator","monkey","bear","bat","beaver","chicken","camel","cheetah","crow","coyote","deer","donkey","duck","eagle","elephant","falcon","fox","frog","giraffe","goat","goose","gorilla","gull","rat","hamster","hawk","horse","hyena","kangaroo","leopard","ostrich","owl","ox","panda","parrot","penguin","pig","pigeon","rabbit","racoon","seal","shark","sheep","snake","swan","tiger","turtle","wolf","zebra","whale","squirrel"};
			return animals[rand() % 55];
			}
			break;
		case (4):
			{
				CLS();
			string teams[25]={"ac milan", "arsenal", "inter milan","real madrid","barcelona","getafe","juventus","lyon","bayern munich","valencia","malaga","santos","manchester united","manchester city","chelsea","liverpool","tottenham","borussia dortmund","schalke","boca juniors","ajax","lille","porto","seville","atletico madrid"};
			return teams[rand() % 25];
			}
		}
	}																		
	};