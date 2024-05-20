#include <iostream>
#include "../../tools/src/main.h"
#include "../../con2db/pgsql.h"

using namespace std;

void log_performance_metrics(Con2DB db, long response_time, int throughput) {
    char sqlcmd[1000]; 
    sprintf(sqlcmd,  "INSERT INTO LogPerformance VALUES (DEFAULT,\'%ld\' ,\'%d\', DEFAULT) ON CONFLICT DO NOTHING;",response_time,throughput);
    db.ExecSQLcmd(sqlcmd);
}

long measure_response_time() {
    // Simulate request processing time
    long start = get_nanos();
    msleep(rand()%1000);  // Simulated processing delay
    long end = get_nanos();
    return (end - start)/1000;
}

void monitorPrestazioni() {
    // Connect to db
    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce"); 
    
    while (true) {
        // Measure response time
        long response_time = measure_response_time();
        cout << "Response Time: " << response_time << " ms" << endl;

        // Simulate throughput (requests per second)
        int throughput = rand() % 100;  // Simulated throughput
        cout << "Throughput: " << throughput << " req/s" << endl;

        // Log metrics to Redis
        log_performance_metrics(db, response_time, throughput);

        // Sleep for a second before the next measurement
        msleep(rand()%1000);
    }

    
}
