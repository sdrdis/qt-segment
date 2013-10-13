#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include <QImage>

#include <image.h>
#include <misc.h>

#include <QRgb>


#include <QDebug>

#include <QElapsedTimer>
#include <QFileDialog>
#include <QResizeEvent>

#include <QMessageBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setImage(QImage im);
    void refresh();
    image <rgb> * qImageToImage(QImage image);
    QImage imageToQImage(image<rgb> * im);
    
private slots:
    void on_sigmaSlider_valueChanged(int value);

    void on_thresholdSlider_valueChanged(int value);

    void on_minSizeSlider_valueChanged(int value);

    void on_actionOpen_triggered();

    void on_actionShow_original_triggered(bool checked);

    void on_actionSave_segmented_as_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QImage im;
    QImage res;
    QGraphicsScene * sceneTo;
    bool showOriginal;
};
#endif // MAINWINDOW_H
