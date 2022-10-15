#include "userService.h"
#include "errors.h"
#include "CSVUserRepository.h"
#include "HTMLUserRepository.h"

UserService::UserService()
{
	//this->userRepo = nullptr;
}

UserService::UserService(UserRepository* userRepo)
{
	this->userRepo = userRepo;
}

void UserService::addDogUserServ(std::string name, std::string breed, int age, std::string photoLink)
{
	Dog d = Dog(name, breed, age, photoLink);
	try
	{
		this->userRepo->addDogUserRepo(d);
	}
	catch (RepoError err)
	{
		throw err;
	}
	std::shared_ptr<UndoRedo> action = std::make_shared<UndoRedoAdopt>(d, this->userRepo);
	this->undoUser.push_back(action);
}

std::vector<Dog> UserService::getAdoptingList()
{
	return this->userRepo->getAdoptingList();
}

std::string UserService::getFileName()
{
	return this->userRepo->getFileName();
}

UserRepository* UserService::getUserRepo()
{
	return this->userRepo;
}

void UserService::chooseRepositoryType(int fileType)
{
	if (fileType == 1)
	{
		std::string file = "adoptionList.csv";
		auto* repo = new CSVUserRepository{ file };
		this->userRepo = repo;
	}
	else if (fileType)
	{
		std::string file = "adoptionList.html";
		auto* repo = new HTMLUserRepository{ file };
		this->userRepo = repo;
	}
}

void UserService::undoUserService()
{
	if (this->undoUser.empty())
	{
		throw RepoError("No more undoes left!");
	}
	this->undoUser.back()->undo();
	this->redoUser.push_back(this->undoUser.back());
	this->undoUser.pop_back();
}

void UserService::redoUserService()
{
	if (this->redoUser.empty())
	{
		throw RepoError("No more redoes left!");
	}
	this->redoUser.back()->redo();
	this->undoUser.push_back(this->redoUser.back());
	this->redoUser.pop_back();
}

UserService::~UserService() = default;