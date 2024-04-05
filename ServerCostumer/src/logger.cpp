// logger.cpp

#include "logger.h"
#include <fstream>
#include <iostream>
#include <chrono>

void log(const std::string& message, int product) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::ofstream logfile("log_server.txt", std::ios_base::app); // Apre il file in modalità append
    if (logfile.is_open()) {
        logfile << std::ctime(&now_c) << ": " << message << " - Prodotto: " << product << std::endl;
        logfile.close();
    } else {
        std::cerr << "Impossibile aprire il file di log!" << std::endl;
    }
}
