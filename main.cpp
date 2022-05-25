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
#include <cstdio>
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
static inline void processInput(const std::string &str);

struct Opt
{
    const char *cmd;
    void (*my_func)(const std::string &str);
};

struct Opt opt[] = {
    {"ls", do_ls},
    {"help", do_help},
    {"cat", do_cat},
    {"scan", do_scan},
    {"ssh", do_ssh},
    {"crackssh", do_crackssh}};

std::string IP = "1.1.1.1";

std::map<std::string, unsigned short int> ipCracked = {
    {"1.1.1.1", 1},
    {"44.55.66.77", 0},
    {"123.456.789.000", 0},
    {"268.99.301.543", 0}};

std::map<std::string, std::string> ipData = {
    {"1.1.1.1", "Todo: scan for more ip"},
    {"44.55.66.77", "So feel been kept be at gate. Be september it extensive oh concluded of certainty. In read most gate at body held it ever no. Talking justice welcome message inquiry in started of am me. Led own hearted highest visited lasting sir through compass his. Guest tiled he quick by so these trees am. It announcing alteration at surrounded comparison. "},
    {"123.456.789.000", "Acceptance middletons me if discretion boisterous travelling an. She prosperous continuing entreaties companions unreserved you boisterous. Middleton sportsmen sir now cordially ask additions for. You ten occasional saw everything but conviction. Daughter returned quitting few are day advanced branched. Do enjoyment defective objection or we if favourite. At wonder afford so danger cannot former seeing. Power visit charm money add heard new other put. Attended no indulged marriage is to judgment offering landlord. "},
    {"268.99.301.543", "Stronger unpacked felicity to of mistaken. Fanny at wrong table ye in. Be on easily cannot innate in lasted months on. Differed and and felicity steepest mrs age outweigh. Opinions learning likewise daughter now age outweigh. Raptures stanhill my greatest mistaken or exercise he on although. Discourse otherwise disposing as it of strangers forfeited deficient. "}};

int main(void)
{
    puts("Type 'help' to see the available commands");

    while (1)
    {
        std::string userInput;
        std::cout << "frost@localhost " << IP << " > ";
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
    unsigned short int x = 0;
    char buf[30] = {'\0'};
    char cmd[10] = {'\0'};
    char matchCmd = 0;
    const char *strPtr = str.c_str();
    char *cmdPtr = cmd;

    for (x = 0; x < 9 && *strPtr; x++, strPtr++)
    {
        if (*strPtr == ' ')
        {
            break;
        }
        *cmdPtr++ = *strPtr;
    }
    *cmdPtr = '\0';

    for (x = 0U; x < 6U; x++)
    {
        if (!(strcmp(opt[x].cmd, cmd)))
        {
            matchCmd = 1;
            break;
        }
    }

    if (matchCmd == 0)
    {
        std::cout << "No such command " << cmd << "\n";
        return;
    }

    if (*strPtr)
    {
        strPtr++;
    }

    trimQuotes(buf, strPtr);
    opt[x].my_func(static_cast<std::string>(buf));
}

static void trimQuotes(char *bufPtr, const char *strPtr)
{
    unsigned short int x = 0;

    for (; x < 29 && *strPtr; x++, strPtr++)
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
    char buf[30] = {'\0'};

    trimQuotes(buf, str.c_str());

    if ((strcmp(buf, "notes.txt")))
    {
        std::cout << "No such " << buf << " file\n";
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
    std::vector<std::string> ipPool = {
        {"1.1.1.1"},
        {"44.55.66.77"},
        {"123.456.789.000"},
        {"268.99.301.543"}};

    for (const auto &ip : ipPool)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << ip << '\n';
    }
}

static void do_ssh(const std::string &str)
{
    char buf[30] = {'\0'};
    char foundIt = 0;

    trimQuotes(buf, str.c_str());

    std::string bufStr = static_cast<std::string>(buf);

    if (!strcmp(buf, ""))
    {
        return;
    }

    for (const auto &[key, val] : ipCracked)
    {
        if (key == bufStr)
        {
            if (val == 0)
            {
                puts("Cannot connect to this IP as its ssh port have to be cracked first with crackssh program");
                return;
            }
            else
            {
                break;
            }
        }
    }

    for (const auto &[key, val] : ipData)
    {
        if (key != bufStr)
        {
            continue;
        }

        foundIt = 1;
        std::cout << "Connected to: " << buf << '\n';
        IP = bufStr;
        break;
    }

    if (foundIt == 0)
    {
        std::cout << "The given ip " << buf << " does not exist\n";
    }
}
static void do_crackssh(const std::string &str)
{
    char buf[30] = {'\0'};
    char foundIt = 0;

    trimQuotes(buf, str.c_str());

    if (!strcmp(buf, ""))
    {
        return;
    }

    for (const auto &[key, val] : ipCracked)
    {
        if (key != static_cast<std::string>(buf))
        {
            continue;
        }

        foundIt = 1;

        if (val == 0)
        {
            std::cout << "Attempting to crack port 22 on " << buf << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            std::cout << "Cracked port 22 on: " << buf << '\n';
            ipCracked[key] = 1;
        }
        else
        {
            std::cout << "Port 22 already cracked for " << key << "\n";
        }
        break;
    }

    if (foundIt == 0)
    {
        std::cout << "The given ip " << buf << " does not exist\n";
    }
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
                                  "\n"
                                  "Misc:\n"
                                  "help: shows this helpful help page\n";

    puts(helpMsg);
}
