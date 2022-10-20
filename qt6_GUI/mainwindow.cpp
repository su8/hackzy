/*
  10/17/2022 https://github.com/su8/hackzy
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
  MA 02110-1301, USA.
*/
#include <iostream>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <ctype.h>
#include <regex>

#include "mainwindow.h"
#include <QApplication>
#include <QCompleter>
#include <QStringList>
#include <QColor>
#include <QPalette>
#include <QAbstractItemView>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "./ui_mainwindow.h"

static void do_ls(const std::string &str);
static void do_help(const std::string &str);
static void do_cat(const std::string &str);
static void do_scan(const std::string &str);
static void do_ssh(const std::string &str);
static void do_crackssh(const std::string &str);
static void do_crackfw(const std::string &str);
static void do_bank(const std::string &str);
static void do_crypto(const std::string &str);
static void do_analyze(const std::string &str);
static void do_solve(const std::string &str);
static void do_forkbomb(const std::string &str);
static void do_upgrade(const std::string &str);
static void do_addIp(const std::string &str);
static void do_addNote(const std::string &str);
static void do_replace(const std::string &str);
static void do_delNotes(const std::string &str);
static inline void processInput(const std::string &str);
static inline void trimQuotes(char *bufPtr, const char *strPtr);
static void updateCrypto(void);
static unsigned short int checkForkBomb(const std::string &str);
static unsigned short int checkFwSsh(const std::string &key);

struct Opt
{
    const char *cmd;
    void (*my_func)(const std::string &str);
};

static const struct Opt opt[] = {
    {"ls", do_ls},
    {"help", do_help},
    {"cat", do_cat},
    {"scan", do_scan},
    {"ssh", do_ssh},
    {"crackfw", do_crackfw},
    {"crypto", do_crypto},
    {"bank", do_bank},
    {"analyze", do_analyze},
    {"solve", do_solve},
    {"forkbomb", do_forkbomb},
    {"crackssh", do_crackssh},
    {"addip", do_addIp},
    {"addnote", do_addNote},
    {"replace", do_replace},
    {"delnotes", do_delNotes},
    {"upgrade", do_upgrade}
};

static std::string IP = "1.1.1.1";
static unsigned long int MONEY = 0U;
static short int ConnectCrackDelay = 5000;
static QString oldText = "";

static std::vector<std::string> ipArr = {
    "1.1.1.1",
    "44.55.66.77",
    "123.456.789.000",
    "268.99.301.543",
    "noIP"
};

static std::unordered_map<std::string, unsigned short int> ipCracked   = { {ipArr[0], 1U} };
static std::unordered_map<std::string, unsigned short int> ipFwCracked = { {ipArr[0], 1U} };
static std::unordered_map<std::string, unsigned short int> ipCrypto    = { {ipArr[0], 0U} };
static std::unordered_map<std::string, unsigned short int> ipForkBomb  = { {ipArr[0], 0U} };
static std::unordered_map<std::string, std::string> ipSolved           = { {ipArr[0], ""} };
static std::unordered_map<std::string, std::string> NOTES              = { {ipArr[0], ""} };

QStringList wordList = {
    "scan", "help", "forkbomb", "cat", "ssh", "crypto",
    "crackssh", "crackfw", "analyze", "solve", "upgrade", "addip",
    "addnote", "delnotes", "bank", "ls", "replace"
};
QCompleter *completer = new QCompleter(wordList, nullptr);

QMediaPlayer *player = new QMediaPlayer();
QAudioOutput *audioOutput = new QAudioOutput();


Ui::MainWindow *UI;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UI = ui;

    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    ui->textEdit->setPalette(p);

    ui->textEdit->setText(static_cast<QString>("Type 'help' to see the available commands"));
    unsigned short int ipArrSize = static_cast<unsigned short int>(ipArr.size());
    for (unsigned short int x = 1U; x < ipArrSize; x++) {
        ipCracked.emplace(ipArr[x], 0U);
        ipFwCracked.emplace(ipArr[x], 0U);
        ipCrypto.emplace(ipArr[x], 0U);
        ipForkBomb.emplace(ipArr[x], 0U);
        ipSolved.emplace(ipArr[x], "");
        NOTES.emplace(ipArr[x], "");
    }
    ui->lineEdit->setCompleter(completer);
    completer->popup()->setStyleSheet("background-color:rgb(54, 57, 63); color:white;");

    player->setAudioOutput(audioOutput);
    player->setSource(QUrl::fromLocalFile("media/Whitesnake.mp3"));
    audioOutput->setVolume(100);
    player->play();

    connect(player, &QMediaPlayer::mediaStatusChanged, player, &QMediaPlayer::play);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete UI;
    delete completer;
    player->stop();
    delete player;
    delete audioOutput;
}

void MainWindow::on_pushButton_clicked()
{
    std::string userInput = static_cast<std::string>(ui->lineEdit->text().toStdString());

    if (userInput.empty())
    {
        return;
    }

    QString inputStr = ui->lineEdit->text();
    ui->textEdit->setText(inputStr + static_cast<QString>('\n') + oldText);
    ui->lineEdit->setText(static_cast<QString>(""));
    processInput(userInput);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

static inline void processInput(const std::string &str)
{
    unsigned short int x = 0U;
    char buf[128] = {'\0'};
    char cmd[10] = {'\0'};
    char matchCmd = 0;
    const char *strPtr = str.c_str();
    char *cmdPtr = cmd;

    for (x = 0U; x < 9U && *strPtr; x++, strPtr++)
    {
        if (*strPtr == ' ')
        {
            strPtr++;
            break;
        }
        *cmdPtr++ = *strPtr;
    }
    *cmdPtr = '\0';

    for (x = 0U; x < sizeof(opt) / sizeof(opt[0]); x++)
    {
        if (!(strcmp(opt[x].cmd, cmd)))
        {
            matchCmd = 1;
            break;
        }
    }

    if (matchCmd == 0)
    {
        QString outStr = static_cast<std::string>("Couldn't find the entered " + str + " command.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    trimQuotes(buf, strPtr);
    opt[x].my_func(static_cast<std::string>(buf));
}

static inline void trimQuotes(char *bufPtr, const char *strPtr)
{
    for (unsigned short int x = 0U; x < 127U && *strPtr; x++, strPtr++)
    {
        if (*strPtr == '"' || *strPtr == '\'')
        {
            continue;
        }

        *bufPtr++ = *strPtr;
    }
    *bufPtr = '\0';
}

static void do_cat(const std::string &str)
{
    if (str != "notes.txt")
    {
        QString outStr = static_cast<std::string>("No such " + str + " file\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    for (const auto &[key, val] : NOTES)
    {
        if (key == IP)
        {
            QString outStr = static_cast<std::string>(val).c_str();
            UI->textEdit->setText(outStr + static_cast<QString>('\n'));
            break;
        }
    }
}

static void do_scan(const std::string &str)
{
    static_cast<void>(str);
    oldText = "";
    for (const auto &key : ipArr)
    {
        QString outStr = static_cast<std::string>(key).c_str();
        UI->textEdit->setText(oldText + outStr + static_cast<QString>('\n'));
        oldText = oldText + outStr + '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        qApp->processEvents();
    }
    oldText = "";
}

static void do_ssh(const std::string &str)
{
    char foundIt = 0;

    if (str.empty())
    {
        QString outStr = static_cast<std::string>("You need to provide IP.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    if (checkForkBomb(str) == 1U)
    {
        return;
    }

    for (const auto &[key, val] : ipFwCracked)
    {
        if (key != str)
        {
            continue;
        }

        if (checkFwSsh(key) == 1U)
        {
            return;
        }

        foundIt = 1;
        QString outStr = static_cast<std::string>("Connected to: " + str + '\n').c_str();
        UI->textEdit->setText(outStr);
        IP = str;
        break;
    }

    if (foundIt == 0)
    {
        QString outStr = static_cast<std::string>("The given IP " + str + " does not exist\n").c_str();
        UI->textEdit->setText(outStr);
    }
}

static void do_analyze(const std::string &str)
{
    unsigned short int x = 50U;
    unsigned short int z = 50U;
    unsigned short int w = 0U;
    char foundIt = 0;
    char buf[30] = {'\0'};
    char *bufPtr = buf;
    static const char alphas[] = "abcdefghijklmnopqrstuvwxyz";
    std::string keyStr = "";
    time_t t;

    if (str.empty())
    {
        QString outStr = static_cast<std::string>("You need to provide IP.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    UI->textEdit->setText(static_cast<QString>("Please wait..."));
    qApp->processEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(ConnectCrackDelay));

    if (checkForkBomb(str) == 1U)
    {
        return;
    }

    for (const auto &[key, val] : ipFwCracked)
    {
        if (key != str)
        {
            continue;
        }

        foundIt = 1;
        keyStr = key;
        break;
    }

    if (foundIt == 0)
    {
        QString outStr = static_cast<std::string>("The given IP " + str + " does not exist.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    if ((t = time(NULL)) == -1)
    {
        UI->textEdit->setText(static_cast<QString>("time(NULL) failed."));
        return;
    }
    srandom(static_cast<unsigned int>(t) ^ static_cast<unsigned int>(getpid()));

    oldText = "";
    for (x = 50U; x < 256U; x++, z++)
    {
        if (z & 1U)
        {
            *bufPtr = alphas[static_cast<unsigned short int>(rand()) % sizeof(alphas) - 1 / sizeof(char)];
            UI->textEdit->setText(oldText + static_cast<QString>(*bufPtr));
            oldText = oldText + *bufPtr++;
            if (w++ > 28U)
            {
                break;
            }
        }
        else
        {
            QString outStr = "0";
            UI->textEdit->setText(oldText + outStr);
            oldText = oldText + outStr;
            qApp->processEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        z >>= static_cast<unsigned short int>(1U);
    }
    *bufPtr = '\0';
    ipSolved[keyStr] = static_cast<std::string>(buf);
}

static void do_solve(const std::string &str)
{
    unsigned short int x = 0U;
    char foundIt = 0;
    char buf[10] = {'\0'};
    char *bufPtr = buf;
    const char *strPtr = str.c_str();

    if (str.empty())
    {
        QString outStr = static_cast<std::string>("You need to provide key@IP.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    UI->textEdit->setText(static_cast<QString>("Please wait..."));
    qApp->processEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(ConnectCrackDelay));

    for (; *strPtr && x < 9U; strPtr++, x++)
    {
        if (*strPtr == '@')
        {
            strPtr++;
            break;
        }
        *bufPtr++ = *strPtr;
    }
    *bufPtr = '\0';

    std::string bufStr = static_cast<std::string>(buf);
    for (const auto &[key, val] : ipSolved)
    {
        if (val != bufStr)
        {
            continue;
        }

        ipFwCracked[key] = 1U;
        ipCracked[key] = 1U;
        foundIt = 1;
        break;
    }

    if (foundIt == 0)
    {
        QString outStr = static_cast<std::string>("The given key@ip " + str + "does not exist.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    QString outStr = static_cast<std::string>("Successfully solved the key for " + str + '\n').c_str();
    UI->textEdit->setText(outStr);
}

static void do_forkbomb(const std::string &str)
{
    char foundIt = 0;

    if (str.empty())
    {
        QString outStr = static_cast<std::string>("You need to provide IP.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    if (str == "noIP")
    {
        QString outStr = static_cast<std::string>("You can't execute a fork bomb for 'noIP'.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    UI->textEdit->setText(static_cast<QString>("Please wait...."));
    qApp->processEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(ConnectCrackDelay));

    for (const auto &[key, val] : ipForkBomb)
    {
        if (key != str)
        {
            continue;
        }

        if (checkFwSsh(key) == 1U)
        {
            return;
        }

        if (val == 1U)
        {
            QString outStr = static_cast<std::string>("The pc " + key + " is down due to fork bomb\n").c_str();
            UI->textEdit->setText(outStr);
            return;
        }

        if (key == IP)
        {
            IP = "noIP";
        }
        foundIt = 1;
        ipForkBomb[key] = 1U;
        break;
    }

    if (foundIt == 0)
    {
        QString outStr = static_cast<std::string>("The given ip " + str + " does not exist\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    QString outStr = static_cast<std::string>("Successfully executed a fork bomb for " + str + '\n').c_str();
    UI->textEdit->setText(outStr);
}

static void do_upgrade(const std::string &str) {
    if (str.empty())
    {
        QString outStr = static_cast<std::string>("You need to provide what PC part you want to upgrade. Currently we have available upgrades only for the 'cpu'.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    if (str == "cpu" || str == "CPU" || str == "Cpu"
      || str == "cPu" || str == "cpU" || str == "cPU" || str == "CPu")
    {
        if (ConnectCrackDelay == 5000 && MONEY >= 10U)
        {
            ConnectCrackDelay = 1000;
            MONEY -= 10U;
            QString outStr = static_cast<std::string>("Successfully purchased a CPU upgrade.\n").c_str();
            UI->textEdit->setText(outStr);
            return;
        }
        else if (ConnectCrackDelay == 1000)
        {
            QString outStr = static_cast<std::string>("You already upgraded the CPU.\n").c_str();
            UI->textEdit->setText(outStr);
        }
        else if (MONEY < 10U)
        {
            QString outStr = static_cast<std::string>("You don't have $10 which are needed to upgrade your CPU.\n").c_str();
            UI->textEdit->setText(outStr);
        }
    }
    else
    {
        QString outStr = static_cast<std::string>("We don't have any upgrades for this pc part " + str + '\n').c_str();
        UI->textEdit->setText(outStr);
    }
}

static void do_addIp(const std::string &str)
{
    unsigned short int z = static_cast<unsigned short int>(str.length());
    char toAddIp = 0;
    const char *strPtr = str.c_str();

    UI->textEdit->setText(static_cast<QString>("Please wait..."));
    qApp->processEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(ConnectCrackDelay));

    for (unsigned short int x = 0U; x < z; x++, strPtr++)
    {
        unsigned char cStrPtr = static_cast<unsigned char>(*strPtr);
        if ((!(isdigit(cStrPtr)) && *strPtr == '.') || isdigit(cStrPtr))
        {
            toAddIp = 1;
            continue;
        }
        else
        {
            toAddIp = 0;
            break;
        }
    }

    if (toAddIp == 0)
    {
        QString outStr = static_cast<std::string>("The given IP address " + str + " can't be added because it contains letters or it's empty.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    if (ipCrypto.count(str))
    {
        QString outStr = static_cast<std::string>("The given IP address " + str + "already exist, no further actions will be taken.\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }

    ipArr.emplace_back(str);
    ipFwCracked.emplace(str, 1U);
    ipCracked.emplace(str, 1U);
    ipCrypto.emplace(str, 0U);
    NOTES.emplace(str, "");

    QString outStr = static_cast<std::string>("Successfully added " + str + " to the IP database, now you can deploy a crypto miner bot to this IP.\n").c_str();
    UI->textEdit->setText(outStr);
}

static void do_addNote(const std::string &str)
{
    for (const auto &[key, val] : NOTES)
    {
        if (key == IP)
        {
            NOTES[key] += str + '\n';
            QString outStr = static_cast<std::string>("Done.\n").c_str();
            UI->textEdit->setText(outStr);
            break;
        }
    }
}

static void do_replace(const std::string &str)
{
    unsigned short int strLen = static_cast<unsigned short int>(str.length());
    char foundIt = 0;
    char gotFirstStr = 0;
    char buf1[128] = {'\0'};
    char buf2[128] = {'\0'};
    char *bufPtr1 = buf1;
    char *bufPtr2 = buf2;
    const char *strPtr = str.c_str();

    for (unsigned short int x = 0U; x < strLen && x < 127U; x++)
    {
        if (gotFirstStr == 0)
        {
            if (*strPtr == ' ')
            {
                gotFirstStr = 1;
                strPtr++;
                continue;
            }
            *bufPtr1++ = *strPtr++;
        }
        else
        {
            *bufPtr2++ = *strPtr++;
        }
    }
    *bufPtr1 = '\0';
    *bufPtr2 = '\0';

    for (const auto &[key, val] : NOTES)
    {
        if (key == IP)
        {
            if (NOTES[key].find(buf1) != std::string::npos)
            {
                NOTES[key] = std::regex_replace(NOTES[key], std::regex(buf1), buf2);
                foundIt = 1;
                QString outStr = static_cast<std::string>("Done.\n").c_str();
                UI->textEdit->setText(outStr);
                break;
            }
        }
    }

    if (foundIt == 0)
    {
        QString outStr = static_cast<std::string>("We couldn't find a text within your notes.txt to replace " + str + "\n").c_str();
        UI->textEdit->setText(outStr);
    }
}

static void do_delNotes(const std::string &str)
{
    if (str.empty())
    {
        NOTES[IP] = "";
    }
    else if (!(NOTES.count(str)))
    {
        QString outStr = static_cast<std::string>("No such IP address " + str + "\n").c_str();
        UI->textEdit->setText(outStr);
        return;
    }
    else
    {
        NOTES[str] = "";
    }
    QString outStr = static_cast<std::string>("Done.\n").c_str();
    UI->textEdit->setText(outStr);
}

#define CRACK_PROGRAM(function, dicti, msg1, msg2, msg3, launchCrypto)        \
    static void do_##function(const std::string &str)                         \
    {                                                                         \
        char foundIt = 0;                                                     \
                                                                              \
        if (str.empty())                                                      \
        {                                                                     \
            QString outStr = static_cast<std::string>("You need to provide IP.\n").c_str(); \
            UI->textEdit->setText(outStr);                                    \
            return;                                                           \
        }                                                                     \
                                                                              \
        for (const auto &[key, val] : dicti)                                  \
        {                                                                     \
            if (key != str)                                                   \
            {                                                                 \
                continue;                                                     \
            }                                                                 \
                                                                              \
            foundIt = 1;                                                      \
                                                                              \
            if (val == 0U)                                                    \
            {                                                                 \
                QString outStr = static_cast<std::string>(msg1 + str + '\n').c_str(); \
                UI->textEdit->setText(outStr);                                \
                qApp->processEvents();                                        \
                std::this_thread::sleep_for(std::chrono::milliseconds(ConnectCrackDelay)); \
                if (launchCrypto == 0)                                        \
                {                                                             \
                    QString outStr = static_cast<std::string>(msg2 + str + '\n').c_str(); \
                    UI->textEdit->setText(outStr);                            \
                    dicti[key] = 1U;                                          \
                }                                                             \
                if (launchCrypto == 1)                                        \
                {                                                             \
                    if (checkFwSsh(key) == 1U)                                \
                    {                                                         \
                        return;                                               \
                    }                                                         \
                    QString outStr = static_cast<std::string>(msg2 + str + '\n').c_str(); \
                    UI->textEdit->setText(outStr);                            \
                    dicti[key] = 1U;                                          \
                    std::thread th(updateCrypto);                             \
                    th.detach();                                              \
                }                                                             \
            }                                                                 \
            else                                                              \
            {                                                                 \
                QString outStr = static_cast<std::string>(msg3 + str + '\n').c_str(); \
                UI->textEdit->setText(outStr);                                \
            }                                                                 \
            break;                                                            \
        }                                                                     \
                                                                              \
        if (foundIt == 0)                                                     \
        {                                                                     \
            QString outStr = static_cast<std::string>("The given IP " + str + " does not exist").c_str(); \
            UI->textEdit->setText(outStr + static_cast<QString>('\n'));       \
        }                                                                     \
    }

CRACK_PROGRAM(crackssh, ipCracked, "Attempting to crack port 22 on ", "Cracked port 22 on ", "Port 22 already cracked for ", 0)
CRACK_PROGRAM(crypto, ipCrypto, "Attempting to deploy a crypto bot on ", "Crypto bot deployed on: ", "The crypto bot is already deployed for ", 1)
CRACK_PROGRAM(crackfw, ipFwCracked, "Attempting to crack the firewall on ", "Cracked the firewall on: ", "The firewall is already cracked for ", 0)

static unsigned short int checkFwSsh(const std::string &key)
{
    if (ipFwCracked[key] == 0U)
    {
        QString outStr = static_cast<std::string>("Cannot connect to this IP as it's firewall have to be cracked first with the crackfw program.").c_str();
        UI->textEdit->setText(outStr);
        return 1U;
    }

    if (ipCracked[key] == 0U)
    {
        QString outStr = static_cast<std::string>("Cannot connect to this IP as it's ssh port have to be cracked first with the crackssh program.\n").c_str();
        UI->textEdit->setText(outStr);
        return 1U;
    }
    return 0U;
}

static unsigned short int checkForkBomb(const std::string &str)
{
    for (const auto &[key, val] : ipForkBomb)
    {
        if (key != str)
        {
            continue;
        }

        if (val == 1U)
        {
            QString outStr = static_cast<std::string>("The pc " + key + " is down due to a fork bomb\n").c_str();
            UI->textEdit->setText(outStr);
            return 1U;
        }
        break;
    }
    return 0U;
}

static void updateCrypto(void)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    MONEY++;
    updateCrypto();
}

static void do_bank(const std::string &str)
{
    static_cast<void>(str);
    std::string cash = std::to_string(MONEY);
    QString outStr = static_cast<std::string>("You have $" + cash + '\n').c_str();
    UI->textEdit->setText(outStr);
}

static void do_ls(const std::string &str)
{
    static_cast<void>(str);
    QString outStr = static_cast<std::string>("notes.txt\n").c_str();
    UI->textEdit->setText(outStr);
}

static void do_help(const std::string &str)
{
    static_cast<void>(str);
    static const QString helpMsg = "ls: lists all directories and files in the current directory\n"
                                  "cat: reads the contents of the given file\n"
                                  "cat file.txt\n"
                                  "\n"
                                  "Networking:\n"
                                  "scan: shows computers linked to the current computer\n"
                                  "ssh: connects to the given IP\n"
                                  "ssh IP\n"
                                  "crackssh: Attempts to crack ip port 22\n"
                                  "crackssh IP\n"
                                  "crackfw Attempts to crack given ip firewall\n"
                                  "crackfw ip\n"
                                  "\n"
                                  "Misc:\n"
                                  "crypto Installs a crypto miner bot for given ip\n"
                                  "crypto ip\n"
                                  "forkbomb Will cause a shell fork bomb and shutdown given ip\n"
                                  "forkbomb ip\n"
                                  "upgrade Will upgrade given PC part, must have enough money to purchase it, must install crypto bot first and wait till you have enough money to purchase it, once crypto bot is installed wait till you gain enough money and check them with the 'bank' program.\n"
                                  "upgrade: given PC part. Currently there's upgrade only for the 'cpu'.\n"
                                  "bank See your bank account after you deploy a crypto miner\n"
                                  "addip Add more IP's to the database, without the need to bypass firewalls and ssh protections, so you can deploy a crypto miner bot on these IP's and upgrade your CPU sooner, and make some money\n"
                                  "addip: 12.12.12.12\n"
                                  "addnote Will cause addition to 'notes.txt' file for the particular IP.\n"
                                  "addnote: 'Your text goes here'\n"
                                  "replace Will replace text within notes.txt\n"
                                  "replace old_text new_text\n"
                                  "delnotes Will delete the entire notes.txt for the connected IP address. Optionally you can specify IP argument and it will delete the notes.txt file for the given IP address.\n"
                                  "help: shows this helpful help page\n";
    UI->textEdit->setText(helpMsg);
}
