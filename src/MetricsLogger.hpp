#ifndef AMIBERRY_METRICSLOGGER_HPP
#define AMIBERRY_METRICSLOGGER_HPP

#include <string>
#include <fstream>
#include <chrono>
#include <map>

class MetricsLogger {
public:
    static MetricsLogger& get_instance();

    void initialize(const std::string& filename);
    void shutdown();

    void log_event(const std::string& event_name, const std::map<std::string, std::string>& tags = {});
    void log_metric(const std::string& metric_name, double value, const std::string& unit, const std::map<std::string, std::string>& tags = {});

private:
    MetricsLogger();
    ~MetricsLogger();
    MetricsLogger(const MetricsLogger&) = delete;
    MetricsLogger& operator=(const MetricsLogger&) = delete;

    void write_log(const std::string& json_payload);
    std::string get_timestamp();
    std::string build_tags_json(const std::map<std::string, std::string>& tags);

    std::ofstream metrics_log_file;
    bool initialized = false;
};

#endif //AMIBERRY_METRICSLOGGER_HPP
