#include "MetricsLogger.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

MetricsLogger& MetricsLogger::get_instance() {
    static MetricsLogger instance;
    return instance;
}

MetricsLogger::MetricsLogger() : initialized(false) {}

MetricsLogger::~MetricsLogger() {
    shutdown();
}

void MetricsLogger::initialize(const std::string& filename) {
    if (initialized) {
        return;
    }
    metrics_log_file.open(filename, std::ios_base::app);
    initialized = true;
}

void MetricsLogger::shutdown() {
    if (metrics_log_file.is_open()) {
        metrics_log_file.close();
    }
    initialized = false;
}

std::string MetricsLogger::get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&in_time_t), "%Y-%m-%dT%H:%M:%S");
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count() << "Z";
    return ss.str();
}

std::string MetricsLogger::build_tags_json(const std::map<std::string, std::string>& tags) {
    if (tags.empty()) {
        return "{}";
    }
    std::stringstream ss;
    ss << "{";
    for (auto it = tags.begin(); it != tags.end(); ++it) {
        ss << "\"" << it->first << "\":\"" << it->second << "\"";
        if (std::next(it) != tags.end()) {
            ss << ",";
        }
    }
    ss << "}";
    return ss.str();
}

void MetricsLogger::log_event(const std::string& event_name, const std::map<std::string, std::string>& tags) {
    if (!initialized || !metrics_log_file.is_open()) {
        return;
    }
    std::stringstream ss;
    ss << "{\"timestamp\":\"" << get_timestamp() << "\","
       << "\"type\":\"event\","
       << "\"name\":\"" << event_name << "\","
       << "\"tags\":" << build_tags_json(tags) << "}";
    write_log(ss.str());
}

void MetricsLogger::log_metric(const std::string& metric_name, double value, const std::string& unit, const std::map<std::string, std::string>& tags) {
    if (!initialized || !metrics_log_file.is_open()) {
        return;
    }
    std::stringstream ss;
    ss << "{\"timestamp\":\"" << get_timestamp() << "\","
       << "\"type\":\"metric\","
       << "\"name\":\"" << metric_name << "\","
       << "\"value\":" << value << ","
       << "\"unit\":\"" << unit << "\","
       << "\"tags\":" << build_tags_json(tags) << "}";
    write_log(ss.str());
}

void MetricsLogger::write_log(const std::string& json_payload) {
    metrics_log_file << json_payload << std::endl;
}
