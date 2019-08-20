#include <iostream>
#include  <conio.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>  
#include <sstream>  
#include <experimental/filesystem>  

using namespace std;
using json = nlohmann::json;
namespace fs = std::experimental::filesystem;

void JSONDirectoryTree(const fs::path& pathToShow, json& j_main, int level = 0)
{
	std::vector<json> files, children;
	for (const auto& entry : fs::directory_iterator(pathToShow))
	{
		json j_entry;
		j_entry["Name"] = entry.path().filename().u8string();
		time_t cftime = chrono::system_clock::to_time_t(fs::last_write_time(entry));
	    j_entry["Date Created"] = asctime(localtime(&cftime));;
		if (fs::is_directory(entry.status()))
		{
			JSONDirectoryTree(entry.path(), j_entry, level + 1);
			children.emplace_back(move(j_entry));
		}
		else
		{
			j_entry["Size"] = fs::file_size(entry);
			j_entry["Path"] = fs::absolute(entry.path()).u8string();
			files.emplace_back(move(j_entry));
		}
	}
	j_main["Files"] = files;
	j_main["Children"] = children;
}

int main()
{
	string folder_path;
	cout << "Please input name of folder with full path: " << endl;
	cin >> folder_path;
	json j_main;
	JSONDirectoryTree(fs::path(folder_path), j_main);
	ofstream o("hierarchy.json");
	o << j_main.dump(4) << endl;
	o.close();
	cout << "File was created."<<endl;
	_getch();
	return 0;
}

