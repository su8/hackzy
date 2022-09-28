/*
  05/24/2022
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
#include <map>
#include <chrono>
#include <thread>

static void trimQuotes(char *bufPtr, const char *strPtr);
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
static inline void processInput(const std::string &str);
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
    {"upgrade", do_upgrade}};

static std::string IP = "1.1.1.1";
static unsigned long int MONEY = 0U;
static short int ConnectCrackDelay = 5000;

static const std::vector<std::string> arr = {
    "1.1.1.1",
    "44.55.66.77",
    "123.456.789.000",
    "268.99.301.543"};

static std::map<std::string, unsigned short int> ipCracked = {
    {arr[0], 1},
    {arr[1], 0},
    {arr[2], 0},
    {arr[3], 0}};

static std::map<std::string, unsigned short int> ipFwCracked = {
    {arr[0], 1},
    {arr[1], 0},
    {arr[2], 0},
    {arr[3], 0}};

static std::map<std::string, unsigned short int> ipCrypto = {
    {arr[0], 0},
    {arr[1], 0},
    {arr[2], 0},
    {arr[3], 0}};

static std::map<std::string, std::string> ipSolved = {
    {arr[0], ""},
    {arr[1], ""},
    {arr[2], ""},
    {arr[3], ""}};

static std::map<std::string, unsigned short int> ipForkBomb = {
    {arr[0], 0},
    {arr[1], 0},
    {arr[2], 0},
    {arr[3], 0}};

int main(void)
{
    puts("Type 'help' to see the available commands");

    while (1)
    {
        std::string userInput;
        std::cout << "frost@localhost " << IP << " > ";
        fflush(stdout);
        getline(std::cin, userInput);

        if (userInput.size() == 0)
        {
            continue;
        }

        if (userInput == "exit")
        {
            break;
        }

        processInput(userInput);
    }

    return EXIT_SUCCESS;
}

static inline void processInput(const std::string &str)
{
    unsigned short int x = 0U;
    char buf[30] = {'\0'};
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

    for (x = 0U; x < 13U; x++)
    {
        if (!(strcmp(opt[x].cmd, cmd)))
        {
            matchCmd = 1;
            break;
        }
    }

    if (matchCmd == 0)
    {
        std::cout << "No such command " << str << '\n';
        return;
    }

    trimQuotes(buf, strPtr);
    opt[x].my_func(static_cast<std::string>(buf));
}

static void trimQuotes(char *bufPtr, const char *strPtr)
{
    unsigned short int x = 0U;

    for (; x < 29U && *strPtr; x++, strPtr++)
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
    static const std::map<std::string, std::string> ipData = {
        {arr[0], "Todo: scan for more ip"},
        {arr[1], "So feel been kept be at gate. Be september it extensive oh concluded of certainty. In read most gate at body held it ever no. Talking justice welcome message inquiry in started of am me. Led own hearted highest visited lasting sir through compass his. Guest tiled he quick by so these trees am. It announcing alteration at surrounded comparison. "},
        {arr[2], "Acceptance middletons me if discretion boisterous travelling an. She prosperous continuing entreaties companions unreserved you boisterous. Middleton sportsmen sir now cordially ask additions for. You ten occasional saw everything but conviction. Daughter returned quitting few are day advanced branched. Do enjoyment defective objection or we if favourite. At wonder afford so danger cannot former seeing. Power visit charm money add heard new other put. Attended no indulged marriage is to judgment offering landlord. "},
        {arr[3], "Stronger unpacked felicity to of mistaken. Fanny at wrong table ye in. Be on easily cannot innate in lasted months on. Differed and and felicity steepest mrs age outweigh. Opinions learning likewise daughter now age outweigh. Raptures stanhill my greatest mistaken or exercise he on although. Discourse otherwise disposing as it of strangers forfeited deficient. "}};

    if ((strcmp(str.c_str(), "notes.txt")))
    {
        std::cout << "No such " << str << " file\n";
        return;
    }

    for (const auto &[key, val] : ipData)
    {
        if (key == IP)
        {
            std::cout << val << '\n';
            break;
        }
    }
}

static void do_scan(const std::string &str)
{
    (void)str;
    for (const auto &[key, val] : ipCracked)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << key << '\n';
    }
}

static void do_ssh(const std::string &str)
{
    char foundIt = 0;

    if (!strcmp(str.c_str(), ""))
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
        std::cout << "Connected to: " << str << '\n';
        IP = str;
        break;
    }

    if (foundIt == 0)
    {
        std::cout << "The given ip " << str << " does not exist\n";
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

    if (!strcmp(str.c_str(), ""))
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
        std::cout << "The given ip " << str << " does not exist\n";
        return;
    }

    if ((t = time(NULL)) == -1)
    {
        puts("time(NULL) failed");
        return;
    }
    srandom(static_cast<unsigned int>(t) ^ static_cast<unsigned int>(getpid()));

    for (x = 50U; x < 256U; x++, z++)
    {
        if (z & 1U)
        {
            putchar('1');
            *bufPtr = alphas[static_cast<unsigned short int>(rand()) % sizeof(alphas) - 1 / sizeof(char)];
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

        z >>= 1U;
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

    if (!strcmp(str.c_str(), ""))
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

    for (const auto &[key, val] : ipSolved)
    {
        if (val != static_cast<std::string>(buf))
        {
            continue;
        }

        foundIt = 1;
        ipFwCracked[key] = 1U;
        ipCracked[key] = 1U;
        break;
    }

    if (foundIt == 0)
    {
        std::cout << "The given key@ip " << str << " does not exist\n";
        return;
    }

    std::cout << "Successfully solved the key for " << str << '\n';
}

static void do_forkbomb(const std::string &str)
{
    char foundIt = 0;

    if (!strcmp(str.c_str(), ""))
    {
        puts("You need to provide IP");
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
            std::cout << "The pc " << key << " is down due to fork bomb\n";
            return;
        }

        if (IP == key)
        {
            IP = "noIP";
        }
        foundIt = 1;
        ipForkBomb[key] = 1U;
        break;
    }

    if (foundIt == 0)
    {
        std::cout << "The given ip " << str << " does not exist\n";
        return;
    }

    std::cout << "Successfully executed a fork bomb for " << str << '\n';
}

static void do_upgrade(const std::string &str) {
    if (!strcmp(str.c_str(), ""))
    {
        puts("You need to provide what PC part you want to upgrade. Currently we have available upgrades for the 'cpu'.");
        return;
    }
    if (str == "cpu" || str == "CPU" || str == "Cpu"
        || str == "cPu" || str == "cpU") {
            if (ConnectCrackDelay == 5000 && MONEY >= 10U) {
                ConnectCrackDelay = 1000;
                MONEY -= 10U;
                puts("Successfully purchased a CPU upgrade");
                return;
            }
            if (ConnectCrackDelay == 1000) {
                puts("You already upgraded the CPU.");
                return;
            }
            if (MONEY < 10U) {
                puts("You don't have $10 which are needed to upgrade your CPU.");
                return;
            }
        }

}

#define CRACK_PROGRAM(function, dicti, msg1, msg2, msg3, launchCrypto)        \
    static void do_##function(const std::string &str)                         \
    {                                                                         \
        char foundIt = 0;                                                     \
                                                                              \
        if (!strcmp(str.c_str(), ""))                                         \
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
                std::cout << msg1 << str << '\n';                             \
                std::this_thread::sleep_for(std::chrono::milliseconds(ConnectCrackDelay)); \
                if (launchCrypto == 0)                                        \
                {                                                             \
                    std::cout << msg2 << str << '\n';                         \
                    dicti[key] = 1U;                                          \
                }                                                             \
                if (launchCrypto == 1)                                        \
                {                                                             \
                    if (checkFwSsh(key) == 1U)                                \
                    {                                                         \
                        return;                                               \
                    }                                                         \
                    std::cout << msg2 << str << '\n';                         \
                    dicti[key] = 1U;                                          \
                    std::thread th(updateCrypto);                             \
                    th.detach();                                              \
                }                                                             \
            }                                                                 \
            else                                                              \
            {                                                                 \
                std::cout << msg3 << key << '\n';                             \
            }                                                                 \
            break;                                                            \
        }                                                                     \
                                                                              \
        if (foundIt == 0)                                                     \
        {                                                                     \
            std::cout << "The given ip " << str << " does not exist\n";       \
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
            std::cout << "The pc " << key << " is down due to fork bomb\n";
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
    (void)str;
    std::cout << "You have $ " << MONEY << '\n';
}

static void do_ls(const std::string &str)
{
    (void)str;
    puts("notes.txt");
}

static void do_help(const std::string &str)
{
    (void)str;
    static const char helpMsg[] = "ls: lists all directories and files in the current directory\n"
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
                                  "analyze Examine the key behind every ip firewall. Must use 'solve' command afterwards\n"
                                  "analyze ip\n"
                                  "solve will crack the ssh port as well the firewall\n"
                                  "solve key@ip\n"
                                  "\n"
                                  "Misc:\n"
                                  "crypto Installs a crypto miner bot for given ip\n"
                                  "crypto ip\n"
                                  "forkbomb Will cause a shell fork bomb and shutdown given ip\n"
                                  "forkbomb ip\n"
                                  "upgrade Will upgrade given PC part, must have enough money to purchase it, must install crypto bot first and wait till you have enough money to purchase it, once crypto bot is installed wait till you gain enough money and check them with the 'bank' program.\n"
                                  "upgrade: given PC part\n"
                                  "bank See your bank account after you deploy a crypto miner\n"
                                  "help: shows this helpful help page\n";
    puts(helpMsg);
}
