#include "CSVUserRepository.h"
#include <fstream>
#include "errors.h"

CSVUserRepository::CSVUserRepository(std::string CSVfileName)
{
	this->fileName = CSVfileName;
}

Dog CSVUserRepository::findDogUserRepo(std::string name)
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

void CSVUserRepository::addDogUserRepo(Dog dog)
{
	try
	{
		Dog d = this->findDogUserRepo(dog.getName());
		throw RepoError("Dog already adopted!");
	}
	catch (ValueError)
	{
		this->adoptingList.push_back(dog);
		this->writeToFile();
	}
}

void CSVUserRepository::deleteDogUserRepo(std::string name)
{
	auto it = std::remove_if(adoptingList.begin(), adoptingList.end(), [name](Dog d) { return (d.getName() == name); });
	adoptingList.pop_back();
	this->writeToFile();
}

std::vector<Dog> CSVUserRepository::getAdoptingList()
{
	return this->adoptingList;
}

std::string CSVUserRepository::getFileName()
{
	return this->fileName;
}

void CSVUserRepository::writeToFile()
{
	std::ofstream fileOut(fileName);
	if (!adoptingList.empty())
	{
		for (auto dog : adoptingList)
		{
			fileOut << dog << "\n";
		}
	}
	fileOut.close();
}

CSVUserRepository::~CSVUserRepository() = default;
