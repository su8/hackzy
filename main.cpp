/*
  05/24/2022 https://github.com/su8/hackzy
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
#include <ctime>
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
#include <cctype>
#include <regex>
#include <random>

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
static void do_history(const std::string &str);
static void do_uname(const std::string &str);
static void do_date(const std::string &str);
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
    {"history", do_history},
    {"uname", do_uname},
    {"date", do_date},
    {"upgrade", do_upgrade}};

static std::string IP = "1.1.1.1";
static unsigned long int MONEY = 0U;
static short int ConnectCrackDelay = 5000;

static std::vector<std::string> ipArr = {
    "1.1.1.1",
    "44.55.66.77",
    "123.456.789.000",
    "noIP"
};

static std::unordered_map<std::string, unsigned short int> ipCracked   = { {ipArr[0], 1U} };
static std::unordered_map<std::string, unsigned short int> ipFwCracked = { {ipArr[0], 1U} };
static std::unordered_map<std::string, unsigned short int> ipCrypto    = { {ipArr[0], 0U} };
static std::unordered_map<std::string, unsigned short int> ipForkBomb  = { {ipArr[0], 0U} };
static std::unordered_map<std::string, std::string> ipSolved           = { {ipArr[0], ""} };
static std::unordered_map<std::string, std::string> NOTES              = { {ipArr[0], ""} };
static std::vector<std::string> History                                = {                };

int main(void)
{
    unsigned short int ipArrSize = static_cast<unsigned short int>(ipArr.size());
    for (unsigned short int x = 1U; x < ipArrSize; x++) {
        ipCracked.emplace(ipArr[x], 0U);
        ipFwCracked.emplace(ipArr[x], 0U);
        ipCrypto.emplace(ipArr[x], 0U);
        ipForkBomb.emplace(ipArr[x], 0U);
        ipSolved.emplace(ipArr[x], "");
        NOTES.emplace(ipArr[x], "");
    }
    puts("Type 'help' to see the available commands");

    while (1)
    {
        std::string userInput;
        std::cout << "frost@localhost " << IP << " > " << std::flush;
        getline(std::cin, userInput);

        if (userInput.empty())
        {
            continue;
        }

        if (userInput == "exit")
        {
            break;
        }

        History.emplace_back(userInput);
        processInput(userInput);
    }

    return EXIT_SUCCESS;
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
        std::cout << "No such command " << str << '\n' << std::flush;
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
        std::cout << "No such " << str << " file\n";
        return;
    }
    
    for (const auto &[key, val] : NOTES)
    {
        if (key == IP)
        {
            std::cout << val << std::flush;
            break;
        }
    }
}

static void do_scan(const std::string &str)
{
    static_cast<void>(str);
    for (const auto &key : ipArr)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << key << '\n' << std::flush;
    }
}

static void do_ssh(const std::string &str)
{
    char foundIt = 0;

    if (str.empty())
    {
        puts("You need to provide IP");
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
        std::cout << "Connected to: " << str << '\n' << std::flush;
        IP = str;
        break;
    }

    if (foundIt == 0)
    {
        std::cout << "The given ip " << str << " does not exist\n" << std::flush;
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
    std::random_device rd;
    std::uniform_int_distribution<int> udist(0, 26);

    if (str.empty())
    {
        puts("You need to provide IP");
        return;
    }

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
        std::cout << "The given ip " << str << " does not exist\n" << std::flush;
        return;
    }

    for (x = 50U; x < 256U; x++, z++)
    {
        if (z & 1U)
        {
            putchar('1');
            *bufPtr = alphas[udist(rd)];
            putchar(*bufPtr++);
            if (w++ > 28U)
            {
                break;
            }
        }
        else
        {
            putchar('0');
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            fflush(stdout);
        }

        z >>= static_cast<unsigned short int>(1U);
    }
    *bufPtr = '\0';

    ipSolved[keyStr] = static_cast<std::string>(buf);
    putchar('\n');
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
        puts("You need to provide key@IP");
        return;
    }

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
        std::cout << "The given key@ip " << str << " does not exist\n" << std::flush;
        return;
    }

    std::cout << "Successfully solved the key for " << str << '\n' << std::flush;
}

static void do_forkbomb(const std::string &str)
{
    char foundIt = 0;

    if (str.empty())
    {
        puts("You need to provide IP");
        return;
    }

    if (str == "noIP")
    {
        puts("You can't execute a fork bomb for 'noIP'");
        return;
    }

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
            std::cout << "The pc " << key << " is down due to a fork bomb\n" << std::flush;
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
        std::cout << "The given ip " << str << " does not exist\n" << std::flush;
        return;
    }

    std::cout << "Successfully executed a fork bomb for " << str << '\n' << std::flush;
}

static void do_upgrade(const std::string &str) {
    if (str.empty())
    {
        puts("You need to provide what PC part you want to upgrade. Currently we have available upgrades only for the 'cpu'.");
        return;
    }

    if (str == "cpu" || str == "CPU" || str == "Cpu"
      || str == "cPu" || str == "cpU" || str == "cPU" || str == "CPu")
    {
        if (ConnectCrackDelay == 5000 && MONEY >= 10U)
        {
            ConnectCrackDelay = 1000;
            MONEY -= 10U;
            puts("Successfully purchased a CPU upgrade");
            return;
        }
        else if (ConnectCrackDelay == 1000)
        {
            puts("You already upgraded the CPU.");
        }
        else if (MONEY < 10U)
        {
            puts("You don't have $10 which are needed to upgrade your CPU.");
        }
    }
    else
    {
        std::cout << "We don't have any upgrades for this pc part " << "'" << str << "'" << '\n' << std::flush;
    }
}

static void do_addIp(const std::string &str)
{
    unsigned short int z = static_cast<unsigned short int>(str.length());
    char toAddIp = 1;
    const char *strPtr = str.c_str();

    std::this_thread::sleep_for(std::chrono::milliseconds(ConnectCrackDelay));

    for (unsigned short int x = 0U; x < z; x++, strPtr++)
    {
        unsigned char cStrPtr = static_cast<unsigned char>(*strPtr);
        if ((!(isdigit(cStrPtr)) && *strPtr == '.') || isdigit(cStrPtr))
        {
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
        std::cout << "The given IP address " << str << " can't be added because it contains letters or it's empty.\n" << std::flush;
        return;
    }

    if (ipCrypto.count(str))
    {
        std::cout << "The given ip " << str << " already exist, no further actions will be taken.\n" << std::flush;
        return;
    }

    ipArr.emplace_back(str);
    ipFwCracked.emplace(str, 1U);
    ipCracked.emplace(str, 1U);
    ipCrypto.emplace(str, 0U);
    NOTES.emplace(str, "");
    std::cout << "Successfully added " << str << " to the IP database, now you can deploy a crypto miner bot to this IP.\n" << std::flush;
}

static void do_addNote(const std::string &str)
{
    for (const auto &[key, val] : NOTES)
    {
        if (key == IP)
        {
            NOTES[key] += str + '\n';
            puts("Done.");
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
                puts("Done.");
                break;
            }
        }
    }

    if (foundIt == 0)
    {
        std::cout << "We couldn't find a text within your notes.txt to replace " << "' " << str << " '" << '\n' << std::flush;
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
        std::cout << "No such IP address " << str << '\n' << std::flush;
        return;
    }
    else
    {
        NOTES[str] = "";
    }
    puts("Done.");
}

static void do_history(const std::string &str)
{
    static_cast<void>(str);
    for (const auto &key : History)
    {
        std::cout << key << '\n' << std::flush;
    }
}

static void do_uname(const std::string &str)
{
    static_cast<void>(str);
    static const char uname[] = "Linux localhost 5.10.52-gentoo SMP Sun October 30 6:56 PM CEST x86_64 Intel i9-13900K GNU/Linux";
    std::cout << uname << '\n' << std::flush;
}

static void do_date(const std::string &str)
{
    static_cast<void>(str);
    char time_str[128];
    time_t t = 0;
    struct tm *taim = NULL;

    if ((t = time(NULL)) == -1 || (taim = localtime(&t)) == NULL ||
      (strftime(time_str, 128, "%d %m %Y %I:%M %p", taim)) == 0) {
  
        std::cout << "Time failed!\n" << std::flush;
        return;
    }

    std::cout << time_str << std::endl;
}

#define CRACK_PROGRAM(function, dicti, msg1, msg2, msg3, launchCrypto)        \
    static void do_##function(const std::string &str)                         \
    {                                                                         \
        char foundIt = 0;                                                     \
                                                                              \
        if (str.empty())                                                      \
        {                                                                     \
            puts("You need to provide IP");                                   \
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
                std::cout << msg1 << str << '\n' << std::flush;               \
                std::this_thread::sleep_for(std::chrono::milliseconds(ConnectCrackDelay)); \
                if (launchCrypto == 0)                                        \
                {                                                             \
                    std::cout << msg2 << str << '\n' << std::flush;           \
                    dicti[key] = 1U;                                          \
                }                                                             \
                if (launchCrypto == 1)                                        \
                {                                                             \
                    if (checkFwSsh(key) == 1U)                                \
                    {                                                         \
                        return;                                               \
                    }                                                         \
                    std::cout << msg2 << str << '\n' << std::flush;           \
                    dicti[key] = 1U;                                          \
                    std::thread th(updateCrypto);                             \
                    th.detach();                                              \
                }                                                             \
            }                                                                 \
            else                                                              \
            {                                                                 \
                std::cout << msg3 << key << '\n' << std::flush;               \
            }                                                                 \
            break;                                                            \
        }                                                                     \
                                                                              \
        if (foundIt == 0)                                                     \
        {                                                                     \
            std::cout << "The given ip " << str << " does not exist\n" << std::flush;       \
        }                                                                     \
    }

CRACK_PROGRAM(crackssh, ipCracked, "Attempting to crack port 22 on ", "Cracked port 22 on ", "Port 22 already cracked for ", 0)
CRACK_PROGRAM(crypto, ipCrypto, "Attempting to deploy a crypto bot on ", "Crypto bot deployed on: ", "The crypto bot is already deployed for ", 1)
CRACK_PROGRAM(crackfw, ipFwCracked, "Attempting to crack the firewall on ", "Cracked the firewall on: ", "The firewall is already cracked for ", 0)

static unsigned short int checkFwSsh(const std::string &key)
{
    if (ipFwCracked[key] == 0U)
    {
        puts("Cannot connect to this IP as its firewall have to be cracked first with crackfw program");
        return 1U;
    }

    if (ipCracked[key] == 0U)
    {
        puts("Cannot connect to this IP as its ssh port have to be cracked first with crackssh program");
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
            std::cout << "The pc " << key << " is down due to a fork bomb\n" << std::flush;
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
    std::cout << "You have $ " << MONEY << '\n' << std::flush;
}

static void do_ls(const std::string &str)
{
    static_cast<void>(str);
    puts("notes.txt");
}

static void do_help(const std::string &str)
{
    static_cast<void>(str);
    static const char helpMsg[] = "ls: lists all directories and files in the current directory\n\n"
                                  "cat: reads the contents of the given file\n\n"
                                  "cat file.txt\n\n"
                                  "\n\n"
                                  "Networking:\n\n"
                                  "scan: shows computers linked to the current computer\n\n"
                                  "ssh: connects to the given IP\n\n"
                                  "ssh IP\n\n"
                                  "crackssh: Attempts to crack ip port 22\n\n"
                                  "crackssh IP\n\n"
                                  "crackfw Attempts to crack given ip firewall\n\n"
                                  "crackfw ip\n\n"
                                  "analyze Examine the key behind every ip firewall. Must use 'solve' command afterwards\n\n"
                                  "analyze ip\n\n"
                                  "solve will crack the ssh port as well the firewall\n\n"
                                  "solve key@ip\n\n"
                                  "\n\n"
                                  "Misc:\n\n"
                                  "crypto Installs a crypto miner bot for given ip\n\n"
                                  "crypto ip\n\n"
                                  "forkbomb Will cause a shell fork bomb and shutdown given ip\n\n"
                                  "forkbomb ip\n\n"
                                  "upgrade Will upgrade given PC part, must have enough money to purchase it, must install crypto bot first and wait till you have enough money to purchase it, once crypto bot is installed wait till you gain enough money and check them with the 'bank' program.\n\n"
                                  "upgrade: given PC part. Currently there's upgrade only for the 'cpu'.\n\n"
                                  "bank See your bank account after you deploy a crypto miner\n\n"
                                  "addip Add more IP's to the database, without the need to bypass firewalls and ssh protections, so you can deploy a crypto miner bot on these IP's and upgrade your CPU sooner, and make some money\n\n"
                                  "addip: 12.12.12.12\n\n"
                                  "addnote Will cause addition to 'notes.txt' file for the particular IP.\n\n"
                                  "addnote: 'Your text goes here'\n\n"
                                  "replace Will replace text within notes.txt\n\n"
                                  "replace old_text new_text\n\n"
                                  "delnotes Will delete the entire notes.txt for the connected IP address. Optionally you can specify IP argument and it will delete the notes.txt file for the given IP address.\n\n"
                                  "history Will show every command that you entered\n\n"
                                  "history: plain command without arguments\n\n"
                                  "uname Will show system kernel and cpu versions\n\n"
                                  "uname: plain command without arguemnts\n\n"
                                  "date Will show the current day/month/year and time in AM/PM\n\n"
                                  "date: plain command without arguments\n\n"
                                  "help: shows this helpful help page\n";
    puts(helpMsg);
}