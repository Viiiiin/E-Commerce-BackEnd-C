#include <iostream>
#include "../../tools/src/main.h"
#include "con2redis.h"
#include "../../con2db/pgsql.h"

void log_scalability_metrics(Con2DB db, double cpu_usage, double memory_usage) {
    char sqlcmd[1000]; 
    sprintf(sqlcmd,  "INSERT INTO LogScalability VALUES (DEFAULT,\'%s\' ,\'%s\', DEFAULT) ON CONFLICT DO NOTHING;",cpu_usage,memory_usage);
    db.ExecSQLcmd(sqlcmd);
}

double measure_cpu_usage() {
    // Simulate CPU usage measurement
    return static_cast<double>(rand() % 100);  // Simulated CPU usage percentage
}

double measure_memory_usage() {
    // Simulate memory usage measurement
    return static_cast<double>(rand() % 100);  // Simulated memory usage percentage
}

int main() {
    // Connect to db
    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce"); 

    while (true) {
        // Measure CPU usage
        double cpu_usage = measure_cpu_usage();
        cout << "CPU Usage: " << cpu_usage << "%" << endl;

        // Measure memory usage
        double memory_usage = measure_memory_usage();
        cout << "Memory Usage: " << memory_usage << "%" << endl;

        // Log metrics to Redis
        log_scalability_metrics(context, cpu_usage, memory_usage);

        // Sleep for a second before the next measurement
        msleep(rand()%1000);
    }

    return 0;
}
