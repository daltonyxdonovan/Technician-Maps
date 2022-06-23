#include <SFML/Graphics.hpp>
#include <atlstr.h>
#include <SFML/Network.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <filesystem>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <shobjidl.h> 

//in the standard namespace
using namespace std;

//variables
const int WIDTH{ 620 };
const int HEIGHT{ 800 };

int scroll_position{ 20 };

bool terminal_disabled{ true };
bool import_hover{ false };
bool typing{ false };
bool type_clicked{ false };

string fname;
string text_input_buffer;
//set starting directory to directory of executable
string starting_directory;

sf::String playerInput;
sf::Text playerText;

class Technician
{
public:
	string name; // 0
	string location; // 25
	string state; //26
	string number; // 36
	string radius; //27
	int position;
	bool favorite;

	Technician()
	{
		//default
		this->name = name;
		this->location = location;
		this->state = state;
		this->number = number;
		this->radius = radius;
		this->position = position;
		this->favorite = favorite;
	}

	Technician(string name, string location, string state, string number, string radius, int position, bool favorite)
	{
		this->name = name;
		this->location = location;
		this->state = state;
		this->number = number;
		this->radius = radius;
		this->position = position;
		this->favorite = favorite;
	}
};

vector<Technician> technicians;
vector<Technician> technicians_buffer;
vector<Technician> technicians_master;



void getworkingdirectory()
{
	// cwd = filesystem::currentpath 
	starting_directory = filesystem::current_path().string();
	
}

void chdir()
{

	


	
	
	
	
	
	
	//set directory to assets folder
	
	string path{ starting_directory + "\\assets\\" };
	SetCurrentDirectoryA(path.c_str());
	cout << "setting directory to assets folder (i hope)";
	
}

//run python script
void run_python_script( string city, string state, string radius)
{
	string command = "python tech_maps.py " + city + " " + state + " " + radius;
	system(command.c_str());
}

void intall_dependencies()
{
	string command = "python installer.py";
	system(command.c_str());
	command = "pipwin install gdal";
	system(command.c_str());
	command = "pipwin install fiona";
	system(command.c_str());
	command = "pipwin install geopandas";
	system(command.c_str());
}


int main()
{
	intall_dependencies();
	getworkingdirectory();
	//set up window
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Technician Map                                                        -daltonyx-");
	sf::Event event;


	//import the font
	sf::Font font;
	if (!font.loadFromFile("assets/camingo.ttf"))
	{
		cout << "Error loading font" << endl;
		return 1;
	}

	//import the card image
	sf::Texture card_texture;
	if (!card_texture.loadFromFile("assets/card.png"))
	{
		cout << "Error loading card texture" << endl;
		return 1;
	}
	
	//import the import image
	sf::Texture import_texture;
	if (!import_texture.loadFromFile("assets/import.png"))
	{
		cout << "Error loading import texture" << endl;
		return 1;
	}

	//set up the card sprite
	sf::Sprite card_sprite;
	card_sprite.setTexture(card_texture);

	//import the arrow image
	sf::Texture arrow_texture;
	if (!arrow_texture.loadFromFile("assets/arrow.png"))
	{
		cout << "Error loading arrow texture" << endl;
		return 1;
	}

	//set up the import sprite
	sf::Sprite import_sprite;
	import_sprite.setTexture(import_texture);

	//set up the arrow sprite
	sf::Sprite arrow_sprite;
	arrow_sprite.setTexture(arrow_texture);
	arrow_sprite.setPosition(sf::Vector2f(5, HEIGHT - (arrow_sprite.getGlobalBounds().height + 5)));
	

	sf::RectangleShape textbox;
	textbox.setSize(sf::Vector2f(400, 50));
	textbox.setPosition(sf::Vector2f(WIDTH / 2 - 200, HEIGHT - 35));
	textbox.setFillColor(sf::Color(255, 255, 255));
	textbox.setOutlineThickness(2);

	sf::Text textbox_hint;
	textbox_hint.setFont(font);
	textbox_hint.setString("Click to type here!");
	textbox_hint.setCharacterSize(20);
	textbox_hint.setPosition(sf::Vector2f(WIDTH / 2 - 100, HEIGHT - 25));
	textbox_hint.setFillColor(sf::Color(0, 0, 0));
	
	

	//hide terminal if needed
	if (terminal_disabled)
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);
	}

	//look for events while the window is open
	while (window.isOpen())
	{
		if (scroll_position > 21)
		{
			scroll_position = 21;
		}
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::TextEntered && typing)
			{
				//if key pressed isn't backspace
				if (event.text.unicode != 8)
				{
					//add the character to the buffer
					playerInput += event.text.unicode;
					textbox_hint.setString(playerInput);
				}
				
			}
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				//if backspace is pressed, remove last character from textbox hint
				if (event.key.code == sf::Keyboard::BackSpace && textbox_hint.getString().getSize() > 0)
				{
					playerInput.erase(playerInput.getSize() - 1);
					textbox_hint.setString(playerInput);
				}
				//if enter is pressed
				if (event.key.code == sf::Keyboard::Return && typing)
				{
					
					if (textbox_hint.getString() != "")
					{
						technicians = technicians_master;
						technicians_buffer.clear();
						scroll_position = 20;
						
						for (int i = 0; i < technicians.size(); i++)
						{
							
							//convert technicians[i].location to lowercase
							string locatione = technicians[i].location;
							for (int j = 0; j < locatione.length(); j++)
							{
								locatione[j] = tolower(locatione[j]);
							}

							//convert technicians[i].state to lowercase
							string statee = technicians[i].state;
							for (int j = 0; j < statee.length(); j++)
							{
								statee[j] = tolower(statee[j]);
							}
							
							string haystack = locatione;
							string haystack2 = statee;
							
							//convert textbox_hint.getString() to lowercase
							string searche = textbox_hint.getString();
							for (int j = 0; j < searche.length(); j++)
							{
								searche[j] = tolower(searche[j]);
							}
							
							string needle = searche;
							

							
							if (haystack.find(needle) != std::string::npos)
							{
								//remake the technician with new position
								technicians_buffer.push_back(Technician(technicians[i].name, technicians[i].location, technicians[i].state, technicians[i].number, technicians[i].radius, technicians_buffer.size() + 1, technicians[i].favorite));

							}
							else if (haystack2.find(needle) != std::string::npos)
							{
								technicians_buffer.push_back(Technician(technicians[i].name, technicians[i].location, technicians[i].state, technicians[i].number, technicians[i].radius, technicians_buffer.size() + 1, technicians[i].favorite));

							}
							


						}
						technicians.clear();
						technicians = technicians_buffer;
						technicians_buffer.clear();
						typing = false;
						playerInput.clear();
						textbox_hint.setString("");
					}
					else
					{
						technicians = technicians_master;
						typing = false;
					}
				}
				
			}
			//if mouse wheel is scrolled, alter the scroll position
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (technicians.size() > 0)
				{
					if (event.mouseWheel.delta < 0)
						scroll_position -= 50;
					else
					{
						if (scroll_position < 21)
							scroll_position += 50;
					}
				}
				
					
			}
			//if mouse is clicked, check if import_hover is true
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseWheel.delta == 0 && textbox.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
				{
					type_clicked = true;
				}
				
				if (type_clicked)
				{
					typing = true;
					textbox.setFillColor(sf::Color(255, 255, 255));
					playerInput = "";
					playerText.setString("");
					type_clicked = false;
				}
				else if (import_hover)
				{	
					technicians.clear();
					wchar_t filename[MAX_PATH];

					OPENFILENAMEW ofn;
					ZeroMemory(&filename, sizeof(filename));
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
					ofn.lpstrFilter = L"CSV Files\0*.csv\0Any File\0*.*\0";
					ofn.lpstrFile = filename;
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrTitle = L"Select a CSV file";
					ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

					if (GetOpenFileNameW(&ofn))
					{
						fname = CW2A(filename);
						Sleep(1000);
					}
					else
					{
						// All this stuff below is to tell you exactly how you messed up above. 
						// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
						switch (CommDlgExtendedError())
						{
						case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
						case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
						case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
						case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
						case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
						case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
						case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
						case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
						case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
						case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
						case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
						case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
						case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
						case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
						case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
						default: std::cout << "User cancelled the operation.\n";
						}
					}

					vector<vector<string>> content;
					vector<string> row;
					string line, word;

					fstream file(fname, ios::in);
					if (file.is_open())
					{
						while (getline(file, line))
						{
							row.clear();
							stringstream str(line);
							while (getline(str, word, ','))
								row.push_back(word);
							content.push_back(row);
							
							
						}
					}
					

					for (int i = 0; i < content.size(); i++)
					{
						//if content[i][0] does not equal "Name"
						if (1 == 1)
						{
							string untouched_string{ content[i][25] };
							string untouched_string_2{ content[i][26] };
							

							
							for (int j = 0; j < untouched_string.size(); j++)
							{
								if (untouched_string[j] == '"')
								{
									untouched_string.erase(j, 1);
								}
								
								
								if (untouched_string_2[j] == '"')
								{
									untouched_string_2.erase(j, 1);
								}
								content[i][25] = untouched_string;
								content[i][26] = untouched_string_2;
								
								
							}
							Technician tech(content[i][0], content[i][25], content[i][26], content[i][34], content[i][27], technicians.size() + 1, false);
							technicians.push_back(tech);
							
							
							
						}
					}
					
					technicians_master = technicians;
				}
				else
				{
				typing = false;
				type_clicked = false;
				}
			}
		}
		
		//clear the window
		window.clear(sf::Color(3,218,198));
		
		for (int i = 0; i < technicians.size(); i++)
		{
			//set the position of the card
			card_sprite.setPosition(sf::Vector2f(10, scroll_position + 10 + ((technicians[i].position - 1) * 210)));

			//names
			sf::Text tech_name;
			string name = technicians[i].name;
			tech_name.setString(name);
			tech_name.setFont(font);
			tech_name.setFillColor(sf::Color::Black);
			tech_name.setCharacterSize(40);
			tech_name.setPosition(sf::Vector2f((WIDTH / 2) - (tech_name.getGlobalBounds().width / 2), scroll_position + 15 + ((technicians[i].position - 1) * 210)));

			//locations
			sf::Text tech_location;
			string location = technicians[i].location;
			tech_location.setString(location);
			tech_location.setFont(font);
			tech_location.setFillColor(sf::Color::Black);
			tech_location.setCharacterSize(30);
			tech_location.setPosition(sf::Vector2f((WIDTH / 2) - (tech_location.getGlobalBounds().width / 2), scroll_position + 70 + ((technicians[i].position - 1) * 210)));

			//state
			sf::Text tech_state;
			string state = technicians[i].state;
			tech_state.setString(state);
			tech_state.setFont(font);
			tech_state.setFillColor(sf::Color::Black);
			tech_state.setCharacterSize(30);
			tech_state.setPosition(sf::Vector2f((WIDTH / 2) - (tech_state.getGlobalBounds().width / 2), scroll_position + 100 + ((technicians[i].position - 1) * 210)));


			//numbers
			sf::Text tech_number;
			string number = technicians[i].number;
			tech_number.setString(number);
			tech_number.setFont(font);
			tech_number.setFillColor(sf::Color::Black);
			tech_number.setCharacterSize(30);
			tech_number.setPosition(sf::Vector2f((WIDTH / 2) - (tech_number.getGlobalBounds().width / 2), scroll_position + 145 + ((technicians[i].position - 1) * 210)));


			window.draw(card_sprite);
			window.draw(tech_name);
			window.draw(tech_location);
			window.draw(tech_number);
			window.draw(tech_state);



			//if card sprite is clicked
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (card_sprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
				{
					//if textbox does not contain mouse
					if (!textbox.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
					{
						//set current working directory to assets folder
						chdir();
						run_python_script(technicians[i].location, technicians[i].state, technicians[i].radius);
					}




					
				}
			}




		}
		import_sprite.setPosition((WIDTH / 2) - (import_sprite.getGlobalBounds().width / 2), scroll_position - 15);
		//import text
		sf::Text import_text;
		string import_text_string = "Import";
		import_text.setString(import_text_string);
		import_text.setFont(font);
		//if mouse collides with import sprite
		if (import_sprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
		{
			import_text.setFillColor(sf::Color::Blue);
			import_hover = true;
		}
		else
		{
			import_text.setFillColor(sf::Color::Black);
			import_hover = false;
		}
		
			
		
		import_text.setCharacterSize(18);
		import_text.setPosition(sf::Vector2f((WIDTH / 2) - (import_text.getGlobalBounds().width / 2), scroll_position - 17));
		
		window.draw(import_sprite);
		window.draw(import_text);
		window.draw(textbox);

		if (!typing)
		{
			textbox.setOutlineColor(sf::Color(0, 0, 0));
		}
			
		if (typing)
			textbox.setOutlineColor(sf::Color::Blue);
		textbox_hint.setPosition(sf::Vector2f((WIDTH / 2) - (textbox_hint.getGlobalBounds().width/2), HEIGHT - 30));
		window.draw(textbox_hint);



		
		//draw the window
		window.display();
	}

	
	

	//return okay if the program exits properly
    return 0;
}