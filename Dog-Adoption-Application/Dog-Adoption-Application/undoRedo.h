#pragma once
#include "repository.h"
#include "userRepository.h"

class UndoRedo {
public:
	virtual void undo() = 0;
	virtual void redo() = 0;
};

class UndoRedoAdd : public UndoRedo {
private:
	Dog addedDog;
	Repository& repo;

public:
	UndoRedoAdd(Dog d, Repository& r);
	void undo() override;
	void redo() override;
};

class UndoRedoDelete : public UndoRedo {
private:
	Dog deletedDog;
	Repository& repo;

public:
	UndoRedoDelete(Dog d, Repository& r);
	void undo() override;
	void redo() override;
};

class UndoRedoUpdate : public UndoRedo {
private:
	Dog oldDog;
	Dog updatedDog;
	Repository& repo;

public:
	UndoRedoUpdate(Dog oD, Dog d, Repository& r);
	void undo() override;
	void redo() override;
};

class UndoRedoAdopt : public UndoRedo {
private:
	Dog adoptedDog;
	UserRepository* userRepo;

public:
	UndoRedoAdopt(Dog d, UserRepository* r);
	void undo() override;
	void redo() override;
};