#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "segment-image.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    showOriginal = false;
    ui->setupUi(this);
    this->sceneTo = new QGraphicsScene(this);
    ui->graphicsViewTo->setScene(this->sceneTo);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setImage(QImage im)
{
    this->im = im;
    this->refresh();
}

void MainWindow::refresh() {
    statusBar()->showMessage("Processing...");
    QElapsedTimer timer;
    timer.start();

    if (showOriginal) {
        QPixmap pixmapFrom = QPixmap::fromImage(im);
        this->sceneTo->clear();
        this->sceneTo->addPixmap(pixmapFrom);
        this->sceneTo->setSceneRect(0, 0, pixmapFrom.width(), pixmapFrom.height());
    } else {
        float sigma = this->ui->sigmaSlider->value() * 0.05;
        this->ui->sigmaVal->setText(QString().setNum(sigma));
        float k = this->ui->thresholdSlider->value();
        this->ui->thresholdVal->setText(QString().setNum(k));
        int min_size = this->ui->minSizeSlider->value();
        this->ui->minSizeVal->setText(QString().setNum(min_size));

        image <rgb> *input = qImageToImage(im);

        int num_ccs;
        image<rgb> *seg = segment_image(input, sigma, k, min_size, &num_ccs);
        res = imageToQImage(seg);

        delete input;
        delete seg;

        QPixmap pixmapTo = QPixmap::fromImage(res);
        this->sceneTo->clear();
        this->sceneTo->addPixmap(pixmapTo);
    }

    qint64 overall = timer.elapsed();
    statusBar()->showMessage("Overall processing time: " + QString().setNum(overall) + QString("ms"));
}


image <rgb> * MainWindow::qImageToImage(QImage im) {
    image<rgb> *input = new image<rgb>(im.width(), im.height(), true);

    for (int x = 0; x < im.width(); x++) {
        for (int y = 0; y < im.height(); y++) {
            rgb pix;
            QRgb pixF = im.pixel(x, y);
            pix.r = qRed(pixF);
            pix.g = qGreen(pixF);
            pix.b = qBlue(pixF);
            input->data[y * im.width() + x] = pix;
        }
    }

    return input;
}

QImage MainWindow::imageToQImage(image<rgb> * im) {
    QImage res(im->width(), im->height(), QImage::Format_RGB32);

    for (int x = 0; x < im->width(); x++) {
        for (int y = 0; y < im->height(); y++) {
            rgb pix = im->data[y * im->width() + x];
            res.setPixel(x, y, qRgb(pix.r, pix.g, pix.b));
        }
    }

    return res;
}

void MainWindow::on_sigmaSlider_valueChanged(int value)
{
    this->refresh();
}

void MainWindow::on_thresholdSlider_valueChanged(int value)
{
    this->refresh();
}

void MainWindow::on_minSizeSlider_valueChanged(int value)
{
    this->refresh();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open image...", QString(), "Image Files (*.png *.jpg *.bmp);;All files (*.*)");
    this->setImage(QImage(filename));
    this->refresh();
}

void MainWindow::on_actionShow_original_triggered(bool checked)
{
    showOriginal = checked;
    this->refresh();
}

void MainWindow::on_actionSave_segmented_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save segmented image as...", QString(), "Image Files (*.png *.jpg *.bmp);;All files (*.*)");
    res.save(filename);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "This software has been implemented by Sébastien Drouyer and is under MIT license.\nhttps://github.com/sdrdis/qt-segment\n\nThis software is derived from \"Efficient Graph-Based Image Segmentation\" by Pedro F. Felzenszwalb and Daniel P. Huttenlocher.\nhttp://cs.brown.edu/~pff/segment/");
}
