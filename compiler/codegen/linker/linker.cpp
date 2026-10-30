#include "linker.h"

// per utilizzo di processi figli di cmd.exe
#if defined(_WIN32) 
    #include <windows.h>
    #define popen _popen
    #define pclose _pclose

#elif defined(__linux__)
    #include <cstdio>
#endif

#include <vector>
#include <filesystem>
#include <iostream>

/*
 * Richiama il programma lld-link come linker per i file oggetto.
 * La funzione ricostruisce il comando da lanciare, con gli argomenti strutturati per lld, includendo
 * directory, librerie, runtimes e target oggetto.
 * Il processo di lld è poi richiamato con metodi diversi in base al sistema operativo ed agli 
 * strumenti a disposizione.
 */

bool linker::lld_link(const std::string &objFile, const std::string &outputExe, bool debug)
{
    #ifdef _WIN32
        wchar_t wbuf[MAX_PATH];
        GetModuleFileNameW(nullptr, wbuf, MAX_PATH);
        std::filesystem::path appDir = std::filesystem::path(wbuf).parent_path();
        std::string linker_name = "lld-link.exe";
        
    #elif defined(__linux__)
        std::filesystem::path appDir = std::filesystem::read_symlink("/proc/self/exe").parent_path();
        std::string linker_name = "ld.lld";
    #endif

    std::string linkerPath = appDir.string() + "/" + linker_name;
    std::string runtimeDir = appDir.string() + "/runtime";

    #ifdef _WIN32
        // lld-link per windows richiede una serie di librerie runtime, 
        // fornite nella folder bin/libs
        std::string libDir     = appDir.string() + "/libs";

        std::vector<std::string> args = 
        {
            objFile,
            "-out:" + outputExe,
            "-subsystem:console",
            "-libpath:" + libDir,
            "-libpath:" + runtimeDir,
            "crt2.o",
            "libmingw32.a",
            "libgcc.a",
            "libgcc_eh.a",
            "libmoldname.a",
            "libmingwex.a",
            "libucrt.a",
            "libadvapi32.a",
            "libshell32.a",
            "libuser32.a",
            "libkernel32.a"
        };
    
    #elif defined(__linux__)
        std::vector<std::string> args =
        {
            "-flavor", "gnu",
            "-static",
            "-z", "noexecstack",
            "-o", outputExe,
            objFile,
            "crt1.o",
            "libc.a"
        };
    #endif

    //aggiunta automatica delle librerie runtime
    for (const auto& entry : std::filesystem::directory_iterator(runtimeDir)) {
        if (entry.is_regular_file()) {
            args.push_back(entry.path().string());
        }
    }

    if(debug) {
        std::cout << "linker path: " << clr::bright_black << linkerPath << clr::reset << std::endl;
        std::cout << "exists: " << std::filesystem::exists(linkerPath) << std::endl;
    }

    std::string cmd = linkerPath;
    for (const auto& a : args) cmd += " " + a;
    cmd += " 2>&1";

    if(debug) std::cout << "executing command: " << clr::bright_black << cmd << clr::reset << std::endl;

    std::string output;
    int exitCode = start_process(cmd, output);

    if (exitCode != 0) {
        if (debug) std::cout << "\nlinker exit code: " << exitCode << "\n" << output << std::endl;
        return false;
    }

    if(debug) {
        std::cout << "linker: " << clr::bright_black << linker_name << "\n" << clr::reset << std::endl;
        std::cout << "linker exit code: " << exitCode  << std::endl;
    }

    return true;
}

/*
 * Avvia un comando su un processo utilizzando le api popen/pclose.
 * Prende come parametri il comando da eseguire e un buffer per leggere l'output del programma.
 * Il comportamento è lo stesso sia per os windows che linux, dato che lo stesso sistema è definito
 * per entrambi.
 */

int linker::start_process(const std::string& command, std::string& output)
{
    FILE* pipe = popen(command.c_str(), "r");
    char _buf[256];

    while (fgets(_buf, sizeof(_buf), pipe)) output += _buf;

    return pclose(pipe);
}