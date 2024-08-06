#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void updateProgressBar(void);
    std::vector<long> readCPUStats(int cpuNumber);
    double readCPUFrequency(int cpuNumber);
    double calculateCPULoad(std::vector<long>& prevStats, std::vector<long>& currStats);

private:
    Ui::Dialog *ui;
    QTimer *timer;
    int progressValue;
    int nrCPUs;
    int nrNPUs;
    int nrRGAs;
    int maxFan;
    std::vector< std::vector<long> > prevStats;
    std::vector< std::vector<long> > currStats;
    QString fanLoadPath;
    std::string gpuLoadPath;
    std::string npuLoadPath;
    int getNumberOfCores(void);
    void findPwmFanDevice(void);
    void findGPULoadPath(void);
    void findNPULoadPath(void);
    void getNPUfreq(long &maxFrequency);
    QString readNpuLoad(void);
    QString readRgaFreq(void);
    QString readRgaLoad(void);
    void readRGALoad(void);
    double updateNPUFreq(void);
    std::unordered_map<std::string, long> readMemInfo(void);
    void readGPULoad(long &load, long &maxFrequency);
    double readTemperature(const std::string &path);
    void updateSoCName(void);
protected:
    void readCPULoad(void);
    void readCPUFreq(void);
    int  readNPULoad(void);
    void readNPUFreq(void);
    void readRGAFreq(void);
    void readGPU(void);
    void readRAM(void);
    void readTemp(void);
    void readFan(void);
};
#endif // DIALOG_H
