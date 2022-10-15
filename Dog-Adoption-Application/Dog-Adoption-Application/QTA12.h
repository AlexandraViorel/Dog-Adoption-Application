#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#include <QtWidgets/QMainWindow>
#include <QShortcut>
#include <QAbstractTableModel>
#include <QTableView>
#include "service.h"
#include "userService.h"
#include "ui_QTA12.h"

class DogTableModel : public QAbstractTableModel 
{
private:
    std::vector<Dog> adoptedDogs;

public:
    DogTableModel(std::vector<Dog> v);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

};

class QTA12 : public QMainWindow
{
    Q_OBJECT

public:
    QTA12(Service& s, UserService& u, QWidget *parent = Q_NULLPTR);

private:
    Ui::QTA12Class ui;

    Service& serv;
    UserService& userServ;
    QShortcut* undoShortcut;
    QShortcut* redoShortcut;
    QWidget* adoptionTableWindow;
    QTableView* adoptionListTable;
    DogTableModel* adoptionListTableModel;
    bool userRepoTypeSelected;

    void connectSignalsAndSlots();

    int getSelectedIndex() const;

    void populateDogsList(std::string filterText);

    void addDog();

    void deleteDog();

    void updateDog();

    void filterDogs();

    void charts();

    void undoAdmin();

    void redoAdmin();

    int getSelectedIndexUser() const;

    void populateDogsListUser(std::string filterText, int ageFilter);

    void filterDogsUser();

    void populateAdoptingList();

    void adoptDog();

    void openFile();

    void undoUser();

    void createTable();

    void redoUser();
};

#endif