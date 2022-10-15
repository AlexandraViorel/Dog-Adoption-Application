#include "HTMLUserRepository.h"
#include <fstream>
#include "errors.h"

HTMLUserRepository::HTMLUserRepository(std::string HTMLfileName)
{
	this->fileName = HTMLfileName;
}

Dog HTMLUserRepository::findDogUserRepo(std::string name)
{
	for (Dog d : this->adoptingList)
	{
		if (d.getName() == name)
		{
			return d;
		}
	}
	throw ValueError("Dog not found!");
}

void HTMLUserRepository::addDogUserRepo(Dog dog)
{
	try
	{
		Dog d = findDogUserRepo(dog.getName());
		throw RepoError("Dog already adopted!");
	}
	catch (ValueError)
	{
		this->adoptingList.push_back(dog);
		this->writeToFile();
	}
}

void HTMLUserRepository::deleteDogUserRepo(std::string name)
{
	auto it = std::remove_if(adoptingList.begin(), adoptingList.end(), [name](Dog d) { return (d.getName() == name); });
	adoptingList.pop_back();
	this->writeToFile();
}

std::vector<Dog> HTMLUserRepository::getAdoptingList()
{
	return this->adoptingList;
}

std::string HTMLUserRepository::getFileName()
{
	return this->fileName;
}

void HTMLUserRepository::writeToFile()
{
	std::ofstream fileOut(fileName);
	fileOut << "<!DOCTYPE html>\n<html><head><title>Adopting list</title></head><body>\n";
	fileOut << "<table border=\"1\">\n";
	fileOut << "<tr><td>Name</td><td>Breed</td><td>Age</td><td>Photo link</td></tr>\n";
	for (auto dog : adoptingList)
	{
		fileOut << "<tr><td>" << dog.getName() << "</td><td>" << dog.getBreed() << "</td><td>"
			<< std::to_string(dog.getAge()) << "</td>" << "<td><a href = \"" << dog.getPhotoLink()
			<< "\">" << dog.getPhotoLink() << "</a></td></tr>\n";
	}
	fileOut << "</table></body></html>";
	fileOut.close();
}

HTMLUserRepository::~HTMLUserRepository() = default;
