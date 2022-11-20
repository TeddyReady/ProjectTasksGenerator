#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GeneratorWindow)
{
    random = QRandomGenerator::global();

    ui->setupUi(this);
    ui->genButton->setEnabled(true);
    ui->taskView->setSelectionMode(QAbstractItemView::NoSelection);
}
GeneratorWindow::~GeneratorWindow()
{
    delete ui;
}

void GeneratorWindow::on_genButton_clicked()
{
    if (ui->tasksList->currentItem()->text() == "Функция Эйлера") {
        DialogEulerFunction *window = new DialogEulerFunction(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogEulerFunctionMeta(int)),
                this, SLOT(slotDialogEulerFunctionMeta(int)));
        connect(window, SIGNAL(dialogEulerFunction(int, int, int, EulerFunctionOptions)),
                this, SLOT(slotDialogEulerFunction(int, int, int, EulerFunctionOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Функция Мёбиуса") {
        DialogMebiusFunction *window = new DialogMebiusFunction(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogMebiusFunctionMeta(int)),
                this, SLOT(slotDialogMebiusFunctionMeta(int)));
        connect(window, SIGNAL(dialogMebiusFunction(int, int, int, MebiusFunctionOptions)),
                this, SLOT(slotDialogMebiusFunction(int, int, int, MebiusFunctionOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Символ Лежандра") {
        DialogSymbolLegandre *window = new DialogSymbolLegandre(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogSymbolLegandreMeta(int)),
                this, SLOT(slotDialogSymbolLegandreMeta(int)));
        connect(window, SIGNAL(dialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions)),
                this, SLOT(slotDialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions)));
    }
}
void GeneratorWindow::on_actionTXT_triggered()
{
    QString path = QFileDialog::getSaveFileName(0, QObject::tr("Сохранить сгенерированные задания"),
        QDir::currentPath() + "test.txt", QObject::tr("txt files (*.txt)"));
    file.setFileName(path);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        int taskIndex = 0;
        for (int i = 0; i < generatedTasks.size(); i += 2) {
            int task = generatedTasks[i];
            int countOfTasks = generatedTasks[i + 1];
            out << task << " " << countOfTasks << "\n";
            switch (task) {
            case TaskEulerFunction:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1] << "\n";
                    taskIndex += 2;
                } break;
            case TaskMebiusFunction:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1] << "\n";
                    taskIndex += 2;
                } break;
            case TaskSymbolLegandre:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1]
                        << " " << generatedData[taskIndex + 2] << "\n";
                    taskIndex += 3;
                } break;
            }
        }
    }
    file.close();
}
void GeneratorWindow::on_actionQuit_triggered()
{
    qApp->exit();
}

void GeneratorWindow::slotDialogEulerFunctionMeta(int countOfTasks){
    QListWidgetItem *item = new QListWidgetItem("Генерируем задания на функцию Эйлера...", ui->taskView);
    ui->taskView->addItem(item);
    generatedTasks.push_back(TaskEulerFunction);
    generatedTasks.push_back(countOfTasks);
}
void GeneratorWindow::slotDialogEulerFunction(int countOfTasks, int minNum, int maxNum, EulerFunctionOptions option)
{
    EulerFunction task;
    switch (option) {
    case EulerFunctionOptions::Default:
        for (int i = 0; i < countOfTasks; i++) {
            task.setTask(random->bounded(minNum, maxNum));
            QListWidgetItem *item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
            ui->taskView->addItem(item);
            generatedData.push_back(task.getTask());
            generatedData.push_back(task.solve());
        } break;
    case EulerFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) {
                QListWidgetItem *item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask());
                generatedData.push_back(task.solve());
            }
        } break;
    case EulerFunctionOptions::PrimesDegrees:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1 && data[0].second > 1) {
                QListWidgetItem *item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask());
                generatedData.push_back(task.solve());
            }
        } break;
    case EulerFunctionOptions::MultiplyPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second != 1) {
                    accessFlag = false;
                    break;
                }
            } if (accessFlag) {
                QListWidgetItem *item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask());
                generatedData.push_back(task.solve());
            }
        } break;
    case EulerFunctionOptions::MultiplyPrimesDegrees:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second == 1) {
                    accessFlag = false;
                    break;
                }
            } if (accessFlag) {
                QListWidgetItem *item = new QListWidgetItem("f(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask());
                generatedData.push_back(task.solve());
            }
        } break;
    }
}

void GeneratorWindow::slotDialogMebiusFunctionMeta(int countOfTasks){
    QListWidgetItem *item = new QListWidgetItem("Генерируем задания на функцию Мёбиуса...", ui->taskView);
    ui->taskView->addItem(item);
    generatedTasks.push_back(TaskMebiusFunction);
    generatedTasks.push_back(countOfTasks);
}
void GeneratorWindow::slotDialogMebiusFunction(int countOfTasks, int minNum, int maxNum, MebiusFunctionOptions option){
    MebiusFunction task;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (int i = 0; i < countOfTasks; i++) {
            task.setTask(random->bounded(minNum, maxNum));
            QListWidgetItem *item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
            ui->taskView->addItem(item);
            generatedData.push_back(task.getTask());
            generatedData.push_back(task.solve());
        } break;
    case MebiusFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!isPrime(task.getTask())) {
                QListWidgetItem *item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask());
                generatedData.push_back(task.solve());
            }
        } break;
    case MebiusFunctionOptions::EvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) continue;
            auto data = decompositionToSimple(task.getTask());
            if (data.size() % 2 != 0) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second > 1) {
                    accessFlag = false;
                    break;
                }
            }
            if (accessFlag) {
                QListWidgetItem *item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask());
                generatedData.push_back(task.solve());
            }
        } break;
    case MebiusFunctionOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) continue;
            auto data = decompositionToSimple(task.getTask());
            if (data.size() % 2 == 0) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second > 1) {
                    accessFlag = false;
                    break;
                }
            }
            if (accessFlag) {
                QListWidgetItem *item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask());
                generatedData.push_back(task.solve());
            }
        } break;
    case MebiusFunctionOptions::PrimeDegree:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            bool accessFlag = false;
            for (int i = 0; i < data.size(); i++) {
                if (data[i].second > 1) {
                    accessFlag = true;
                    break;
                }
            }
            if (accessFlag) {
                QListWidgetItem *item = new QListWidgetItem("m(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask());
                generatedData.push_back(task.solve());
            }
        } break;
    }
}

void GeneratorWindow::slotDialogSymbolLegandreMeta(int countOfTasks){
    QListWidgetItem *item = new QListWidgetItem("Генерируем задания на символ Лежандра...", ui->taskView);
    ui->taskView->addItem(item);
    generatedTasks.push_back(TaskSymbolLegandre);
    generatedTasks.push_back(countOfTasks);
}
void GeneratorWindow::slotDialogSymbolLegandre(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolLegandreOptions option){
    SymbolLegandre task;
    switch (option) {
    case SymbolLegandreOptions::Default:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                QListWidgetItem *item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask().first);
                generatedData.push_back(task.getTask().second);
                generatedData.push_back(task.solve());
            }
        } break;
    case SymbolLegandreOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first)) {
                QListWidgetItem *item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask().first);
                generatedData.push_back(task.getTask().second);
                generatedData.push_back(task.solve());
            }
        } break;
    case SymbolLegandreOptions::aEqual_1:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                QListWidgetItem *item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask().first);
                generatedData.push_back(task.getTask().second);
                generatedData.push_back(task.solve());
            }
        } break;
    case SymbolLegandreOptions::aEqual2:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                QListWidgetItem *item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask().first);
                generatedData.push_back(task.getTask().second);
                generatedData.push_back(task.solve());
            }
        } break;
    case SymbolLegandreOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0) {
                QListWidgetItem *item = new QListWidgetItem("L(" + QString::number(task.getTask().first) + ", " + QString::number(task.getTask().second) + ") = " + QString::number(task.solve()), ui->taskView);
                ui->taskView->addItem(item); i++;
                generatedData.push_back(task.getTask().first);
                generatedData.push_back(task.getTask().second);
                generatedData.push_back(task.solve());
            }
        } break;
    }
}
