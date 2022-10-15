#include "undoRedo.h"

UndoRedoAdd::UndoRedoAdd(Dog d, Repository& r) : addedDog{ d }, repo{ r } {}

void UndoRedoAdd::undo()
{
	//Dog d = this->repo.findDogRepo(this->addedDog.getName());
	this->repo.deleteDogRepo(this->addedDog.getName());
}

void UndoRedoAdd::redo()
{
	this->repo.addDogRepo(this->addedDog);
}

UndoRedoDelete::UndoRedoDelete(Dog d, Repository& r) : deletedDog{ d }, repo{ r } {}

void UndoRedoDelete::undo()
{
	this->repo.addDogRepo(this->deletedDog);
}

void UndoRedoDelete::redo()
{
	this->repo.deleteDogRepo(this->deletedDog.getName());
}

UndoRedoUpdate::UndoRedoUpdate(Dog oD, Dog d, Repository& r) : oldDog{ oD }, updatedDog { d }, repo{ r } 
{
}

void UndoRedoUpdate::undo()
{
	this->repo.updateDogRepo(this->updatedDog.getName(), oldDog);
}

void UndoRedoUpdate::redo()
{
	this->repo.updateDogRepo(this->oldDog.getName(), updatedDog);
}

UndoRedoAdopt::UndoRedoAdopt(Dog d, UserRepository* r) : adoptedDog{ d }
{
	this->userRepo = r;
}

void UndoRedoAdopt::undo()
{
	this->userRepo->deleteDogUserRepo(this->adoptedDog.getName());
}

void UndoRedoAdopt::redo()
{
	this->userRepo->addDogUserRepo(this->adoptedDog);
}
