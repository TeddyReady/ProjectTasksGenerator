#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H
#include <QRandomGenerator>
#include <QTextStream>
#include <QFileDialog>
#include <QMainWindow>
#include "dialogmebiusfunction.h"
#include "dialogeulerfunction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GeneratorWindow; }
QT_END_NAMESPACE

enum AllTasks {
    TaskEulerFunction, TaskMebiusFunction
};

class GeneratorWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit GeneratorWindow(QWidget *parent = nullptr);
    ~GeneratorWindow();
public slots:
    //Эйлер
    void slotDialogEulerFunctionMeta(int);
    void slotDialogEulerFunction(int, int, int, EulerFunctionOptions);
    //Мёбиус
    void slotDialogMebiusFunctionMeta(int);
    void slotDialogMebiusFunction(int, int, int, MebiusFunctionOptions);
private slots:
    void on_genButton_clicked();
    void on_actionTXT_triggered();
private:
    QVector<int> generatedData;
    QFile file;
    QRandomGenerator *random;
    Ui::GeneratorWindow *ui;
};
#endif
