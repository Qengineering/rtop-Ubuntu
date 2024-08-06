#include "dialog.h"
#include "ui_dialog.h"
#include <iomanip>
#include <sstream>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDialog>
#include <QApplication>
//------------------------------------------------------------------------------------------
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog),
    progressValue(0), nrCPUs(4), nrNPUs(0), maxFan(0)
{
//    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint); //Qt::WindowStaysOnTopHint);

    ui->setupUi(this);

    gpuLoadPath="";
    npuLoadPath="";
    fanLoadPath="";

    findPwmFanDevice();
    findGPULoadPath();
    findNPULoadPath();

    nrCPUs=getNumberOfCores();

    ui->barCPU1->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #35D642; width: 1px;}" );
    ui->barCPU2->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #35D642; width: 1px;}" );
    ui->barCPU3->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #35D642; width: 1px;}" );
    ui->barCPU4->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #35D642; width: 1px;}" );
    if(nrCPUs > 4){
        ui->barCPU5->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #35D642; width: 1px;}" );
        ui->barCPU6->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #35D642; width: 1px;}" );
        ui->barCPU7->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #35D642; width: 1px;}" );
        ui->barCPU8->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #35D642; width: 1px;}" );
    }
    else{
        ui->labCPU5->hide();
        ui->labCPU6->hide();
        ui->labCPU7->hide();
        ui->labCPU8->hide();

        ui->barCPU5->hide();
        ui->barCPU6->hide();
        ui->barCPU7->hide();
        ui->barCPU8->hide();

        ui->freqCPU5->hide();
        ui->freqCPU6->hide();
        ui->freqCPU7->hide();
        ui->freqCPU8->hide();
        adjustSize();
    }
    if(npuLoadPath!=""){
        nrNPUs=readNPULoad();

        ui->barNPU1->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #359DD6; width: 1px;}" );
        ui->barNPU2->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #359DD6; width: 1px;}" );
        ui->barNPU3->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #359DD6; width: 1px;}" );
    }
    if(nrNPUs<3){
        ui->labNPU3->hide();
        ui->barNPU3->hide();
        ui->freqNPU3->hide();
        adjustSize();
    }
    if(nrNPUs<2){
        ui->labNPU2->hide();
        ui->barNPU2->hide();
        ui->freqNPU2->hide();
        adjustSize();
    }
    if(nrNPUs<1){
        ui->labNPU1->hide();
        ui->barNPU1->hide();
        ui->freqNPU1->hide();
        adjustSize();
    }

    ui->barMem->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #9563D3; width: 1px;}" );

    ui->barSwap->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #D6AB35; width: 1px;}" );

    if(gpuLoadPath!=""){
        ui->barGPU->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #35D6D6; width: 1px;}" );
    }
    else{
        ui->labGPU->hide();
        ui->barGPU->hide();
        ui->freqGPU->hide();
        adjustSize();
    }

    ui->barTemp->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #FF7C7C; width: 1px;}" );

    if(maxFan>0){
        ui->barFan->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                                   "QProgressBar::chunk { background-color: #E2E238; width: 1px;}" );
    }
    else{
        ui->labFan->hide();
        ui->barFan->hide();
        ui->freqFan->hide();
        adjustSize();
    }

    ui->barRGA1->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #35D3B9; width: 1px;}" );
    ui->barRGA2->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #35D3B9; width: 1px;}" );
    ui->barRGA3->setStyleSheet("QProgressBar { border: 1px solid grey; border-radius: 2px; text-align: center;}"
                               "QProgressBar::chunk { background-color: #35D3B9; width: 1px;}" );

    timer = new QTimer(this);

    currStats.resize(nrCPUs);
    prevStats.resize(nrCPUs);

    for(int i=0;i<nrCPUs;i++){
        prevStats[i] = readCPUStats(i);
    }

    updateSoCName();

    readRGAFreq();      //update once the RGA outlook.

    connect(timer, &QTimer::timeout, this, &Dialog::updateProgressBar);
    timer->start(1000);
}
//------------------------------------------------------------------------------------------
Dialog::~Dialog()
{
    delete ui;
}
//------------------------------------------------------------------------------------------
int Dialog::getNumberOfCores(void)
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}
//------------------------------------------------------------------------------------------
void Dialog::findPwmFanDevice(void)
{
    QDir dir("/sys/class/thermal");
    QStringList coolingDevices = dir.entryList(QStringList() << "cooling_device*", QDir::Dirs);

    for (const QString &device : coolingDevices) {
        QString typePath = dir.filePath(device + "/type");
        QFile file(typePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString type = in.readLine();
            if (type.contains("pwm-fan")) {
                //set max fan
                fanLoadPath = dir.filePath(device + "/max_state");
                QFile file(fanLoadPath);
                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    bool ok;
                    QTextStream in(&file);
                    QString pwmValue = in.readAll().trimmed();
                    file.close();
                    maxFan = pwmValue.toInt(&ok);
                }

                //set current fan
                fanLoadPath = dir.filePath(device + "/cur_state");
                break;
            }
            file.close();
        }
    }
}
//------------------------------------------------------------------------------------------
// Function to read CPU statistics from /proc/stat
std::vector<long> Dialog::readCPUStats(int cpuNumber)
{
    std::ifstream file("/proc/stat");
    std::string line;
    std::vector<long> stats;

    if (file.is_open()) {
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string cpuLabel;
            iss >> cpuLabel;
            if (cpuLabel == "cpu" + std::to_string(cpuNumber)) {
                long value;
                while (iss >> value) {
                    stats.push_back(value);
                }
                break;
            }
        }
        file.close();
    }

    return stats;
}
//------------------------------------------------------------------------------------------
// Function to calculate the CPU load percentage
double Dialog::calculateCPULoad(std::vector<long>& prevStats, std::vector<long>& currStats)
{
    long prevIdle = prevStats[3] + prevStats[4];
    long currIdle = currStats[3] + currStats[4];

    long prevNonIdle = prevStats[0] + prevStats[1] + prevStats[2] + prevStats[5] + prevStats[6] + prevStats[7];
    long currNonIdle = currStats[0] + currStats[1] + currStats[2] + currStats[5] + currStats[6] + currStats[7];

    long prevTotal = prevIdle + prevNonIdle;
    long currTotal = currIdle + currNonIdle;

    long totalDelta = currTotal - prevTotal;
    long idleDelta = currIdle - prevIdle;

    return (double)(totalDelta - idleDelta) / totalDelta * 100.0;
}
//------------------------------------------------------------------------------------------
// Function to read the CPU frequency
double Dialog::readCPUFrequency(int cpuNumber)
{
    std::string path = "/sys/devices/system/cpu/cpu" + std::to_string(cpuNumber) + "/cpufreq/scaling_cur_freq";
    std::ifstream file(path);
    long frequency = 0;

    if (file.is_open()) {
        file >> frequency;
        file.close();
    } else {
        std::cerr << "Error: Could not open " << path << std::endl;
    }

    return static_cast<double>(frequency)/1000000.0;
}
//------------------------------------------------------------------------------------------
QString Dialog::readNpuLoad(void)
{
    QProcess process;
    process.start("sudo", QStringList() << "cat" << "/sys/kernel/debug/rknpu/load");
    process.waitForFinished();
    return process.readAllStandardOutput().trimmed();
}
//------------------------------------------------------------------------------------------
void Dialog::readCPULoad(void)
{
    int i;
    double cpuLoad[8];

    for(i=0;i<nrCPUs;i++){
        currStats[i] = readCPUStats(i);
        cpuLoad[i]   = calculateCPULoad(prevStats[i], currStats[i]);
        prevStats[i] = currStats[i];
    }
    for(   ;i<8;i++) cpuLoad[i] = 0;  //fill the remaining cpuLoad[x]

    ui->barCPU1->setValue(cpuLoad[0]);
    ui->barCPU2->setValue(cpuLoad[1]);
    ui->barCPU3->setValue(cpuLoad[2]);
    ui->barCPU4->setValue(cpuLoad[3]);
    ui->barCPU5->setValue(cpuLoad[4]);
    ui->barCPU6->setValue(cpuLoad[5]);
    ui->barCPU7->setValue(cpuLoad[6]);
    ui->barCPU8->setValue(cpuLoad[7]);
}
//------------------------------------------------------------------------------------------
void Dialog::readCPUFreq(void)
{
    std::ostringstream oss;
    double frequency[8];

    for(int i=0;i<nrCPUs;i++){
        frequency[i] = readCPUFrequency(i);
    }
    oss << std::fixed << std::setprecision(2) << frequency[0];
    ui->freqCPU1->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
    oss << std::fixed << std::setprecision(2) << frequency[1];
    ui->freqCPU2->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
    oss << std::fixed << std::setprecision(2) << frequency[2];
    ui->freqCPU3->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
    oss << std::fixed << std::setprecision(2) << frequency[3];
    ui->freqCPU4->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
    oss << std::fixed << std::setprecision(2) << frequency[4];
    ui->freqCPU5->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
    oss << std::fixed << std::setprecision(2) << frequency[5];
    ui->freqCPU6->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
    oss << std::fixed << std::setprecision(2) << frequency[6];
    ui->freqCPU7->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
    oss << std::fixed << std::setprecision(2) << frequency[7];
    ui->freqCPU8->setText(QString::fromStdString(oss.str()) + " GHz");
}
//------------------------------------------------------------------------------------------
int Dialog::readNPULoad(void)
{
    long load;
    std::string StrLoad = readNpuLoad().toStdString();

    std::stringstream ss(StrLoad);
    std::vector<long> loads;
    std::string coreStr;
    std::string sub;

    loads.clear();

    // Assuming the format "Core0: X%, Core1: Y%, Core2: Z%" or "X%"
    if (StrLoad.find("Core") != std::string::npos) {
        // Parse multi-core format
        // Find the positions of the percentages
        size_t pos0 = StrLoad.find("Core0:");
        size_t pos1 = StrLoad.find("Core1:");
        size_t pos2 = StrLoad.find("Core2:");

        if(pos0 != std::string::npos){
            sub = StrLoad.substr(pos0 + 6, 3);
            load = std::stoi(sub);
            loads.push_back(load);
        }
        if(pos1 != std::string::npos){
            sub = StrLoad.substr(pos1 + 6, 3);
            load = std::stoi(sub);
            loads.push_back(load);
        }
        if(pos2 != std::string::npos){
            sub = StrLoad.substr(pos2 + 6, 3);
            load = std::stoi(sub);
            loads.push_back(load);
        }
    } else {
        // Parse single load format
        // Find the positions of the percentages
        size_t pos0 = StrLoad.find("%");

        if(pos0 != std::string::npos){
            sub = StrLoad.substr(pos0 - 2, 3);
            load = std::stoi(sub);
            loads.push_back(load);
        }
    }

    if(loads.size() > 0) ui->barNPU1->setValue(loads[0]);
    if(loads.size() > 1) ui->barNPU2->setValue(loads[1]);
    if(loads.size() > 2) ui->barNPU3->setValue(loads[2]);

    return static_cast<int>(loads.size());
}
//------------------------------------------------------------------------------------------
void Dialog::readNPUFreq(void)
{
    std::ostringstream oss;
    long frequency;
    getNPUfreq(frequency);

    oss << std::fixed << std::setprecision(2) << static_cast<double>(frequency)/1000000000;

    ui->freqNPU1->setText(QString::fromStdString(oss.str()) + " GHz");
    if(nrNPUs>1){
        ui->freqNPU2->setText(QString::fromStdString(oss.str()) + " GHz");
        ui->freqNPU3->setText(QString::fromStdString(oss.str()) + " GHz");
    }
}
//------------------------------------------------------------------------------------------
void Dialog::findNPULoadPath(void)
{
    QDir dir("/sys/class/devfreq/");
    QStringList filter;
    filter << "*.npu";
    dir.setNameFilters(filter);

    QFileInfoList list = dir.entryInfoList();
    if (!list.isEmpty()) {
        npuLoadPath = list.first().absoluteFilePath().toStdString() + "/cur_freq";
    }
}
//------------------------------------------------------------------------------------------
void Dialog::getNPUfreq(long &maxFrequency)
{
    std::ifstream file(npuLoadPath);

    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        file.close();

        std::stringstream ss(line);
        ss >> maxFrequency;
    } else {
        std::cerr << "Error: Could not open " << gpuLoadPath << std::endl;
        maxFrequency = -1;
    }
}
//------------------------------------------------------------------------------------------
QString Dialog::readRgaFreq(void)
{
    QProcess process;
    process.start("sudo", QStringList() << "cat" << "/sys/kernel/debug/clk/clk_summary");
    process.waitForFinished();
    return process.readAllStandardOutput().trimmed();
}
//------------------------------------------------------------------------------------------
void Dialog::readRGAFreq(void)
{
    std::string line;
    std::vector<long> freq;
    std::ostringstream oss;
    QRegularExpressionMatch match;

    QString st=readRgaFreq();

    nrRGAs=0;

    QRegularExpression regex1(R"(aclk_rga\s+\d+\s+\d+\s+\d+\s+(\d+)\s+\d+\s+\d+\s+\d)");
    match = regex1.match(st);
    if (match.hasMatch()) {
        QString valueStr = match.captured(1);
        int value = valueStr.toInt();
        oss << std::fixed << std::setprecision(2) << static_cast<double>(value)/1000000000;
        ui->freqRGA1->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
        nrRGAs++;
    } else {
        QRegularExpression regex1(R"(aclk_rga3_0\s+\d+\s+\d+\s+\d+\s+(\d+)\s+\d+\s+\d+\s+\d)");
        match = regex1.match(st);
        if (match.hasMatch()) {
            QString valueStr = match.captured(1);
            int value = valueStr.toInt();
            oss << std::fixed << std::setprecision(2) << static_cast<double>(value)/1000000000;
            ui->freqRGA1->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
            nrRGAs++;
        } else {
            ui->labRGA1->hide();
            ui->barRGA1->hide();
            ui->freqRGA1->hide();
            adjustSize();
        }
    }

    QRegularExpression regex2(R"(aclk_rga3_1\s+\d+\s+\d+\s+\d+\s+(\d+)\s+\d+\s+\d+\s+\d)");
    match = regex2.match(st);
    if (match.hasMatch()) {
        QString valueStr = match.captured(1);
        int value = valueStr.toInt();
        oss << std::fixed << std::setprecision(2) << static_cast<double>(value)/1000000000;
        ui->freqRGA2->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
        nrRGAs++;
    } else {
        ui->labRGA2->hide();
        ui->barRGA2->hide();
        ui->freqRGA2->hide();
        adjustSize();
    }

    QRegularExpression regex3(R"(aclk_rga2\s+\d+\s+\d+\s+\d+\s+(\d+)\s+\d+\s+\d+\s+\d)");
    match = regex3.match(st);
    if (match.hasMatch()) {
        QString valueStr = match.captured(1);
        int value = valueStr.toInt();
        oss << std::fixed << std::setprecision(2) << static_cast<double>(value)/1000000000;
        ui->freqRGA3->setText(QString::fromStdString(oss.str()) + " GHz"); oss.str(""); oss.clear();
        nrRGAs++;
    } else {
        ui->labRGA3->hide();
        ui->barRGA3->hide();
        ui->freqRGA3->hide();
        adjustSize();
    }
}
//------------------------------------------------------------------------------------------
QString Dialog::readRgaLoad(void)
{
    QProcess process;
    process.start("sudo", QStringList() << "cat" << "/sys/kernel/debug/rkrga/load");
    process.waitForFinished();
    return process.readAllStandardOutput().trimmed();
}
//------------------------------------------------------------------------------------------
void Dialog::readRGALoad(void)
{
    std::vector<int> loads;

    QString st=readRgaLoad();

    std::istringstream iss(st.toStdString());
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find("load =") != std::string::npos) {
            if (line.find("== load ==") == std::string::npos) {
                size_t pos = line.find("=");
                std::string loadStr = line.substr(pos + 1);
                loadStr = loadStr.substr(0, loadStr.find("%"));
                loads.push_back(std::stoi(loadStr));
            }
        }
    }
    if(loads.size()>0) ui->barRGA1->setValue(loads[0]);
    if(nrRGAs>1){
        if(loads.size()>1) ui->barRGA2->setValue(loads[1]);
        if(loads.size()>2) ui->barRGA3->setValue(loads[2]);
    }
}
//------------------------------------------------------------------------------------------
void Dialog::findGPULoadPath(void)
{
    QDir dir("/sys/class/devfreq/");
    QStringList filter;
    filter << "*.gpu";
    dir.setNameFilters(filter);

    QFileInfoList list = dir.entryInfoList();
    if (!list.isEmpty()) {
        gpuLoadPath = list.first().absoluteFilePath().toStdString() + "/load";
    }
}
//------------------------------------------------------------------------------------------
void Dialog::readGPULoad(long &load, long &maxFrequency)
{
    std::ifstream file(gpuLoadPath);

    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        file.close();

        std::stringstream ss(line);
        char at;
        ss >> load >> at >> maxFrequency;
    } else {
        std::cerr << "Error: Could not open " << gpuLoadPath << std::endl;
        load = -1;
        maxFrequency = -1;
    }
}
//------------------------------------------------------------------------------------------
void Dialog::readGPU(void)
{
    std::ostringstream oss;
    long load, maxFrequency;
    readGPULoad(load, maxFrequency);

    if (load != -1 && maxFrequency != -1) {
        double maxFrequencyGHz = maxFrequency / 1000000000.0; // Convert to GHz
        oss << std::fixed << std::setprecision(2) << maxFrequencyGHz;
        ui->freqGPU->setText(QString::fromStdString(oss.str()) + " GHz");
        ui->barGPU->setValue(load);
    }
}
//------------------------------------------------------------------------------------------
std::unordered_map<std::string, long> Dialog::readMemInfo()
{
    std::ifstream file("/proc/meminfo");
    std::unordered_map<std::string, long> memInfo;
    std::string key;
    long value;
    std::string unit;

    if (file.is_open()) {
        while (file >> key >> value >> unit) {
            key.pop_back(); // Remove the trailing colon
            memInfo[key] = value;
        }
        file.close();
    } else {
        std::cerr << "Error: Could not open /proc/meminfo" << std::endl;
    }

    return memInfo;
}
//------------------------------------------------------------------------------------------
void Dialog::readRAM(void)
{
    std::ostringstream oss;
    auto memInfo = readMemInfo();

    long totalRAM = memInfo["MemTotal"];            // Total RAM in kB
    long availableRAM = memInfo["MemAvailable"];    // Available RAM in kB
    long usedRAM = totalRAM - availableRAM;         // Used RAM in kB
    long totalSwap = memInfo["SwapTotal"];          // Total Swap in kB
    long freeSwap = memInfo["SwapFree"];            // Free Swap in kB
    long usedSwap = totalSwap - freeSwap;           // Used Swap in kB

    oss << std::fixed << std::setprecision(2) << totalRAM / (1024.0*1024.0);
    ui->freqMem->setText(QString::fromStdString(oss.str()) + " GB"); oss.str(""); oss.clear();
    ui->barMem->setValue(100*usedRAM/totalRAM);

    oss << std::fixed << std::setprecision(2) << totalSwap / (1024.0*1024.0);
    ui->freqSwap->setText(QString::fromStdString(oss.str()) + " GB");
    ui->barSwap->setValue(100*usedSwap/totalSwap);
}
//------------------------------------------------------------------------------------------
double Dialog::readTemperature(const std::string &path)
{
    std::ifstream file(path);
    double temp = 0.0;

    if (file.is_open()) {
        file >> temp;
        file.close();
    } else {
        std::cerr << "Error: Could not open " << path << std::endl;
    }
    // The temperature is usually reported in millidegrees Celsius, so convert to degrees Celsius
    return temp / 1000.0;
}
//------------------------------------------------------------------------------------------
void Dialog::readTemp(void)
{
    std::ostringstream oss;

    std::string cpuTempPath = "/sys/class/thermal/thermal_zone0/temp";

    double cpuTemp = readTemperature(cpuTempPath);
    ui->barTemp->setValue(cpuTemp);
    double fahrenheit = (cpuTemp * 9.0 / 5.0) + 32;
    oss << std::fixed << std::setprecision(0) << fahrenheit;
    ui->freqTemp->setText(QString::fromStdString(oss.str()) + " °F");
}
//------------------------------------------------------------------------------------------
void Dialog::readFan(void)
{
    QFile file(fanLoadPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bool ok;
        QTextStream in(&file);
        QString pwmValue = in.readAll().trimmed();
        file.close();

        int pwm = pwmValue.toInt(&ok);
        if (ok) {
            int percentage = (100*pwm)/maxFan;
            ui->barFan->setValue(percentage);
        }
    } else {
        std::cerr << "Error: Could not open " << fanLoadPath.toStdString() << std::endl;
    }
}
//------------------------------------------------------------------------------------------
void Dialog::updateSoCName(void)
{
    QFile file("/sys/firmware/devicetree/base/compatible");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString content = in.readAll().trimmed();
        file.close();

        // Use a regular expression to find the rk**** part
        QRegularExpression re("rk\\d{4}");
        QRegularExpressionMatch match = re.match(content);

        if (match.hasMatch()) {
            QString socName = match.captured(0).toUpper();
            setWindowTitle(socName);
        }
    }
}
//------------------------------------------------------------------------------------------
void Dialog::updateProgressBar(void)
{
    readCPULoad();
    readCPUFreq();
    readRAM();
    readTemp();
    if(npuLoadPath!=""){
        readNPULoad();
        readNPUFreq();
    }
    if(gpuLoadPath!=""){
        readGPU();
    }
    if(fanLoadPath!=""){
        readFan();
    }
    readRGALoad();
}
/*-----------------------------------------------------------------------------------------
notes:

rock@5c:~$ cat /sys/class/devfreq/fb000000.gpu/load
rock@3W:~$ cat /sys/class/devfreq/fde60000.gpu/load
0@1000000000Hz

/sys/kernel/debug/rknpu/load
NPU load:  Core0:  0%, Core1:  0%, Core2:  0%,    <-- Rock@5c
NPU load:  0%   <-- Rock@3W

rock@3W
/sys/class/devfreq/fde40000.npu/cur_freq
600000000

rock@5b
/sys/class/devfreq/fdab0000.npu/cur_freq
1000000000

/sys/firmware/devicetree/base/compatible
radxa,rock-5crockchip,rk3588
radxa,zero3-aic8800ds2rockchip,rk3566

sudo cat /sys/kernel/debug/clk/clk_summary | grep rga
sudo cat /sys/kernel/debug/rkrga/load
------------------------------------------------------------------------------------------*/

