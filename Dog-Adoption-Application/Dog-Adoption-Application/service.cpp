#include "service.h"
#include "errors.h"

Service::Service(Repository& repo) : repo{ repo } {}

Dog Service::findDogServ(std::string name)
{
	for (Dog d : this->repo.getDogsList()) 
	{
		if (d.getName() == name)
		{
			return d;
		}
	}
	throw RepoError("Dog not found!");
}

void Service::addDogServ(std::string name, std::string breed, int age, std::string photoLink)
{
	Dog d = Dog(name, breed, age, photoLink);
	try
	{
		this->repo.addDogRepo(d);
	}
	catch (RepoError err)
	{
		throw err;
	}
	std::shared_ptr<UndoRedo> action = std::make_shared<UndoRedoAdd>(d, this->repo);
	this->undoAdmin.push_back(action);
}

void Service::removeDogServ(std::string name)
{
	try
	{
		Dog d = this->findDogServ(name);
		this->repo.deleteDogRepo(name);
		std::shared_ptr<UndoRedo> action = std::make_shared<UndoRedoDelete>(d, this->repo);
		this->undoAdmin.push_back(action);
	}
	catch (RepoError err)
	{
		throw err;
	}
}

void Service::updateDogServ(std::string name, std::string newBreed, int newAge, std::string newPhotoLink)
{
	Dog newDog = Dog(name, newBreed, newAge, newPhotoLink);
	try
	{
		Dog d = this->findDogServ(name);
		this->repo.updateDogRepo(name, newDog);
		std::shared_ptr<UndoRedo> action = std::make_shared<UndoRedoUpdate>(d, newDog, this->repo);
		this->undoAdmin.push_back(action);
	}
	catch (RepoError err)
	{
		throw err;
	}
}

std::vector<Dog> Service::getDogsList()
{
	return this->repo.getDogsList();
}

std::vector<std::string> Service::getBreeds()
{
	std::vector<std::string> breeds;
	std::vector<Dog> dogs = this->repo.getDogsList();

	for (Dog d : dogs) {
		if (std::find(breeds.begin(), breeds.end(), d.getBreed()) == breeds.end()) {
			breeds.push_back(d.getBreed());
		}
	}

	return breeds;
}

int Service::getNbOfDogsWithGivenBreed(std::string breed)
{
	int count = 0;
	std::vector<Dog> dogs = this->repo.getDogsList();
	for (Dog d : dogs) {
		if (d.getBreed() == breed) {
			count++;
		}
	}
	return count;
}

void Service::undoService()
{
	if (this->undoAdmin.empty()) 
	{
		throw RepoError("No more undoes left!");
	}
	this->undoAdmin.back()->undo();
	this->redoAdmin.push_back(this->undoAdmin.back());
	this->undoAdmin.pop_back();
}

void Service::redoService()
{
	if (this->redoAdmin.empty())
	{
		throw RepoError("No more redoes left!");
	}
	this->redoAdmin.back()->redo();
	this->undoAdmin.push_back(this->redoAdmin.back());
	this->redoAdmin.pop_back();

}

Service::~Service() = default;