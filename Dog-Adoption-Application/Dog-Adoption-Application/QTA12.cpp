#include "QTA12.h"
#include <QMessageBox>
#include "errors.h"
#include "pictureDelegate.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

QTA12::QTA12(Service& s, UserService& u, QWidget *parent)
    : QMainWindow(parent), serv{s}, userServ{u}
{
    this->userRepoTypeSelected = false;
    this->undoShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
    this->redoShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this);

    ui.setupUi(this);

    populateDogsList("");
    populateDogsListUser("", 1000);
    connectSignalsAndSlots();
}

void QTA12::connectSignalsAndSlots()
{
    QObject::connect(this->ui.dogsListWidget, &QListWidget::itemSelectionChanged, [this]() {
        int selectedIndex = getSelectedIndex();
        if (selectedIndex < 0)
        {
            return;
        }
        Dog d = serv.getDogsList()[selectedIndex];
        ui.nameLineEdit->setText(QString::fromStdString(d.getName()));
        ui.breedLineEdit->setText(QString::fromStdString(d.getBreed()));
        ui.ageLineEdit->setText(QString::fromStdString(std::to_string(d.getAge())));
        ui.photoLineEdit->setText(QString::fromStdString(d.getPhotoLink()));
        });
    
    QObject::connect(this->ui.dogsUserList, &QListWidget::itemSelectionChanged, [this]() {
        int selectedIndex = getSelectedIndexUser();
        if (selectedIndex < 0)
        {
            return;
        }
        Dog d = serv.getDogsList()[selectedIndex];
        ui.nameUserLineEdit->setText(QString::fromStdString(d.getName()));
        ui.breedUserLineEdit->setText(QString::fromStdString(d.getBreed()));
        ui.ageUserLineEdit->setText(QString::fromStdString(std::to_string(d.getAge())));
        ui.photoUserLineEdit->setText(QString::fromStdString(d.getPhotoLink()));
        });

    QObject::connect(this->ui.addButton, &QPushButton::clicked, this, &QTA12::addDog);

    QObject::connect(this->ui.deleteButton, &QPushButton::clicked, this, &QTA12::deleteDog);

    QObject::connect(this->ui.updateButton, &QPushButton::clicked, this, &QTA12::updateDog);

    QObject::connect(this->ui.filterLineEdit, &QLineEdit::textChanged, this, &QTA12::filterDogs);

    QObject::connect(this->ui.chartsButton, &QPushButton::clicked, this, &QTA12::charts);

    QObject::connect(this->ui.undoButton, &QPushButton::clicked, this, &QTA12::undoAdmin);

    QObject::connect(this->undoShortcut, &QShortcut::activated, this, &QTA12::undoAdmin);

    QObject::connect(this->ui.redoButton, &QPushButton::clicked, this, &QTA12::redoAdmin);

    QObject::connect(this->redoShortcut, &QShortcut::activated, this, &QTA12::redoAdmin);

    if (this->userRepoTypeSelected == false)
    {
        QObject::connect(this->ui.CSVradioButton, &QRadioButton::clicked, [this]() {
            userServ.chooseRepositoryType(1);
            userRepoTypeSelected = true;
            });
    }

    if (this->userRepoTypeSelected == false)
    {
        QObject::connect(this->ui.HTMLradioButton, &QRadioButton::clicked, [this]() {
            userServ.chooseRepositoryType(2);
            userRepoTypeSelected = true;
            });
    }

    QObject::connect(this->ui.adoptButton, &QPushButton::clicked, this, &QTA12::adoptDog);
    
    QObject::connect(this->ui.filterNameLineEdit, &QLineEdit::textChanged, this, &QTA12::filterDogsUser);
    
    QObject::connect(this->ui.filterAgeLineEdit, &QLineEdit::textChanged, this, &QTA12::filterDogsUser);

    QObject::connect(this->ui.openFileButton, &QPushButton::clicked, this, &QTA12::openFile);

    QObject::connect(this->ui.undoUserButton, &QPushButton::clicked, this, &QTA12::undoUser);

    QObject::connect(this->ui.redoUserButton, &QPushButton::clicked, this, &QTA12::redoUser);

    QObject::connect(this->ui.tableViewButton, &QPushButton::clicked, this, &QTA12::createTable);
}

int QTA12::getSelectedIndex() const
{
    QModelIndexList selectedIndexes = this->ui.dogsListWidget->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 0)
    {
        this->ui.nameLineEdit->clear();
        this->ui.breedLineEdit->clear();
        this->ui.ageLineEdit->clear();
        this->ui.photoLineEdit->clear();
        return -1;
    }
    int selectedIndex = selectedIndexes.at(0).row();
    return selectedIndex;
}

void QTA12::populateDogsList(std::string filterText)
{
    this->ui.dogsListWidget->clear();
    std::vector<Dog> allDogs = this->serv.getDogsList();
    for (Dog d : allDogs) 
    {
        if (d.getName().find(filterText) != d.getName().npos)
        {
            QString dogString = QString::fromStdString(d.toString());
            QListWidgetItem* item = new QListWidgetItem{ dogString };
            this->ui.dogsListWidget->addItem(item);
        }
    }
}

void QTA12::addDog()
{
    try
    {
        std::string name = this->ui.nameLineEdit->text().toStdString();
        std::string breed = this->ui.breedLineEdit->text().toStdString();
        std::string ageStr = this->ui.ageLineEdit->text().toStdString();
        int age = std::stoi(ageStr);
        std::string photographyLink = this->ui.photoLineEdit->text().toStdString();

        this->serv.addDogServ(name, breed, age, photographyLink);
        this->populateDogsList("");
        this->populateDogsListUser("", 1000);

        int lastElement = this->serv.getDogsList().size() - 1;
        this->ui.dogsListWidget->setCurrentRow(lastElement);
    }
    catch (RepoError)
    {
        QMessageBox::critical(this, "Error", "A dog with the same name already exists!");
        return;
    }
}

void QTA12::deleteDog()
{
    int selectedIndex = this->getSelectedIndex();
    if (selectedIndex < 0)
    {
        QMessageBox::critical(this, "Error", "No dog was selected!");
        return;
    }
    Dog d = this->serv.getDogsList()[selectedIndex];
    this->serv.removeDogServ(d.getName());

    this->populateDogsList("");
    this->populateDogsListUser("", 1000);
}

void QTA12::updateDog()
{
    int selectedIndex = this->getSelectedIndex();
    if (selectedIndex < 0)
    {
        QMessageBox::critical(this, "Error", "No dog was selected!");
        return;
    }
    try
    {
        std::string oldName = this->serv.getDogsList()[selectedIndex].getName();
        std::string newBreed = this->ui.breedLineEdit->text().toStdString();
        std::string newAgeStr = this->ui.ageLineEdit->text().toStdString();
        int newAge = std::stoi(newAgeStr);
        std::string newPhotographyLink = this->ui.photoLineEdit->text().toStdString();
        this->serv.updateDogServ(oldName, newBreed, newAge, newPhotographyLink);
        this->populateDogsList("");
        this->populateDogsListUser("", 1000);
    }
    catch (RepoError)
    {
        QMessageBox::critical(this, "Error", "Error at updating dog!");
        return;
    }
}

void QTA12::filterDogs()
{
    std::string filterText = this->ui.filterLineEdit->text().toStdString();
    this->populateDogsList(filterText);
}

void QTA12::charts()
{
    std::vector<std::string> breeds = this->serv.getBreeds();

    auto* chart = new QChart();
    auto* pieSeries = new QPieSeries();
    for (auto breed : breeds) {
        int currentNrOfDogs = this->serv.getNbOfDogsWithGivenBreed(breed);
        pieSeries->append(QString::fromStdString(breed + ":" + std::to_string(currentNrOfDogs)), currentNrOfDogs);
    }
    chart->addSeries(pieSeries);
    chart->legend()->setAlignment(Qt::AlignBottom);
    auto* chartView = new QChartView{};
    chartView->setChart(chart);

    auto* chartLayout = new QVBoxLayout{};
    auto* chartWidget = new QWidget{};
    chartLayout->addWidget(chartView);
    chartWidget->setLayout(chartLayout);
    chartWidget->show();
}

void QTA12::undoAdmin()
{
    try
    {
        this->serv.undoService();
        this->populateDogsList("");
        this->populateDogsListUser("", 1000);
    }
    catch (RepoError)
    {
        QMessageBox::critical(this, "Error", "Error at undo!");
    }
}

void QTA12::redoAdmin()
{
    try
    {
        this->serv.redoService();
        this->populateDogsList("");
        this->populateDogsListUser("", 1000);
    }
    catch (RepoError)
    {
        QMessageBox::critical(this, "Error", "Error at redo!");
    }
}

int QTA12::getSelectedIndexUser() const
{
    QModelIndexList selectedIndexes = this->ui.dogsUserList->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() < 0)
    {
        this->ui.nameUserLineEdit->clear();
        this->ui.breedUserLineEdit->clear();
        this->ui.ageUserLineEdit->clear();
        this->ui.photoUserLineEdit->clear();
        return -1;
    }
    int selectedIndex = selectedIndexes.at(0).row();
    return selectedIndex;
}

void QTA12::populateDogsListUser(std::string filterText, int ageFilter)
{
    this->ui.dogsUserList->clear();
    std::vector<Dog> allDogs = this->serv.getDogsList();
    for (Dog d : allDogs)
    {
        if (d.getName().find(filterText) != d.getName().npos && d.getAge() < ageFilter)
        {
            QString dogString = QString::fromStdString(d.toString());
            QListWidgetItem* item = new QListWidgetItem{ dogString };
            this->ui.dogsUserList->addItem(item);
        }
    }
}

void QTA12::filterDogsUser()
{
    std::string filterText = this->ui.filterNameLineEdit->text().toStdString();
    std::string ageStr = this->ui.filterAgeLineEdit->text().toStdString();
    int filterAge = 1000;
    if (ageStr != "")
    {
        filterAge = std::stoi(ageStr);
    }
    this->populateDogsListUser(filterText, filterAge);
}

void QTA12::populateAdoptingList()
{
    this->ui.adoptingList->clear();
    std::vector<Dog> adoptedDogs = this->userServ.getAdoptingList();
    for (Dog d : adoptedDogs)
    {
        QString dogString = QString::fromStdString(d.toString());
        QListWidgetItem* item = new QListWidgetItem{ dogString };
        this->ui.adoptingList->addItem(item);
    }
}

void QTA12::adoptDog()
{
    int selectedIndex = this->getSelectedIndexUser();
    if (selectedIndex < 0)
    {
        QMessageBox::critical(this, "Error", "No dog was selected!");
        return;
    }
    try
    {
        if (!this->userRepoTypeSelected)
        {
            QMessageBox::critical(this, "Error", "Please select the file type!");
            return;
        }
        std::string name = this->ui.nameUserLineEdit->text().toStdString();
        std::string breed = this->ui.breedUserLineEdit->text().toStdString();
        int age = this->ui.ageUserLineEdit->text().toInt();
        std::string photographyLink = this->ui.photoUserLineEdit->text().toStdString();
        this->userServ.addDogUserServ(name, breed, age, photographyLink);
        this->populateAdoptingList();
    }
    catch (RepoError)
    {
        QMessageBox::critical(this, "Error", "A dog with the same name is already adopted!");
        return;
    }
}

void QTA12::openFile()
{
    if (this->userRepoTypeSelected == false)
    {
        QMessageBox::critical(this, "Error", "Please select the file type!");
        return;
    }
    std::string link = std::string("start ").append(this->userServ.getFileName());
    system(link.c_str());
}

void QTA12::undoUser()
{
    try
    {
        this->userServ.undoUserService();
        this->populateAdoptingList();
    }
    catch (RepoError)
    {
        QMessageBox::critical(this, "Error", "Error at undo!");
    }
}

void QTA12::redoUser()
{
    try
    {
        this->userServ.redoUserService();
        this->populateAdoptingList();
    }
    catch (RepoError)
    {
        QMessageBox::critical(this, "Error", "Error at redo!");
    }
}

DogTableModel::DogTableModel(std::vector<Dog> v)
{
    this->adoptedDogs = v;
}

int DogTableModel::rowCount(const QModelIndex& parent) const
{
    return this->adoptedDogs.size();
}

int DogTableModel::columnCount(const QModelIndex& parent) const
{
    return 5;
}

QVariant DogTableModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    int column = index.column();
    
    Dog currentDog = this->adoptedDogs[row];
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (column)
        {
        case 0:
            return QString::fromStdString(currentDog.getName());
        case 1:
            return QString::fromStdString(currentDog.getBreed());
        case 2:
            return QString::fromStdString(std::to_string(currentDog.getAge()));
        case 3:
            return QString::fromStdString(currentDog.getPhotoLink());
        case 4:
            return QString::fromStdString(currentDog.getBreed());
        default:
            break;
        }
    }
    return QVariant();
}

QVariant DogTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("Name");
            case 1:
                return QString("Breed");
            case 2:
                return QString("Age");
            case 3:
                return QString("Photography link");
            case 4:
                return QString("Photo");
            default:
                break;
            }
        }
    }
    return QVariant();
}


void QTA12::createTable()
{
    if (this->userRepoTypeSelected == false)
    {
        QMessageBox::critical(this, "Error", "Please select the file type!");
        return;
    }
    this->adoptionTableWindow = new QWidget{};
    auto* adoptionWindowLayout = new QVBoxLayout(this->adoptionTableWindow);
    this->adoptionListTable = new QTableView{};

    this->adoptionListTableModel = new DogTableModel(this->userServ.getAdoptingList());
    this->adoptionListTable->setModel(this->adoptionListTableModel);
    this->adoptionListTable->setItemDelegate(new PictureDelegate{});
    this->adoptionListTable->resizeColumnsToContents();
    this->adoptionListTable->resizeRowsToContents();
    adoptionWindowLayout->addWidget(this->adoptionListTable);
    this->adoptionTableWindow->resize(840, 720);
    this->adoptionTableWindow->show();
}