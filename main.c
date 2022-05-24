
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

static void do_ls(const std::string &str);
static void do_help(const std::string &str);
static void do_cat(const std::string &str);
static void do_whoami(const std::string &str);
static void do_nmap(const std::string &str);
static void do_scan(const std::string &str);
static void do_connect(const std::string &str);
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
    {"whoami", do_whoami},
    {"nmap", do_nmap},
    {"scan", do_scan},
    {"connect", do_connect},
    {"crackssh", do_crackssh}};

int main(void)
{
    while (1)
    {
        std::string userInput;
        getline(std::cin, userInput);
        std::cout << "> " << userInput << '\n';

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
    unsigned short int x = 0, z = 0;
    char buf[256] = {'\0'};
    char matchCmd = 0;
    const char *strPtr = str.c_str();
    char *bufPtr = buf;

    for (; x < 8U; x++)
    {
        if (!(strcmp(opt[x].cmd, strPtr)))
        {
            matchCmd = 1;
            break;
        }
    }

    if (matchCmd == 1)
    {
        for (z = 0U; z < 255U && *strPtr; z++, strPtr++)
        {
            if (*strPtr == '"' || *strPtr == '\'')
            {
                continue;
            }
            *bufPtr++ = *strPtr;
        }
        *bufPtr = '\0';

        opt[x].my_func((std::string)buf);
    }
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
                                  "connect: connects to the given IP\n"
                                  "connect IP"
                                  "whoami: shows your own computer\n"
                                  "\n"
                                  "Security:\n"
                                  "nmap: shows a list of the open ports of given ip\n"
                                  "\n"
                                  "Misc:\n"
                                  "help: shows this helpful help page\n";

    puts(helpMsg);
}

static void do_cat(const std::string &str)
{
}

static void do_whoami(const std::string &str)
{
    puts("frost@localhost 1.1.1.1");
}

static void do_nmap(const std::string &str)
{
}

static void do_scan(const std::string &str)
{
}

static void do_connect(const std::string &str)
{
}
static void do_crackssh(const std::string &str)
{
}

static void do_ls(const std::string &str)
{
