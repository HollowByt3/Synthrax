#include <iostream>
#include <string>
#include <cstdlib> 
#include <curl/curl.h>
#include <chrono>

void send_to_cyborg(const std::string& rsyslog_json, 
                    const std::string& apiKey, 
                    const std::string& indexKey, 
                    const std::string& baseUri) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    std::string authHeader = "X-API-Key: " + apiKey;
    headers = curl_slist_append(headers, authHeader.c_str());

    // 1. Generate a unique ID using nanoseconds
    auto ts = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::string logId = "log_" + std::to_string(ts);

	std::string payload = R"({
    "index_name": "structured_system_logs",
    "index_key": ")" + indexKey + R"(",
    "items": [{
        "id": ")" + logId + R"(",
	"contents": "Syslog entry from )" + logId + R"(", 
        "metadata": )" + rsyslog_json + R"(
    }]
})";


    std::string fullUrl = baseUri + "/v1/vectors/upsert";

    curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    // 3. Perform the request and check for errors
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
    }
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}
int main() {
    // Disable buffering to ensure logs are processed immediately
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // 1. Fetch required variables from the system environment
    const char* envApiKey   = std::getenv("CYBORG_API_KEY");
    const char* envIndexKey = std::getenv("CYBORG_INDEX_KEY");
    const char* envBaseUri  = std::getenv("CYBORG_BASE_URI");

    // 2. Comprehensive validation (Fixed variable names)
    if (!envApiKey || !envIndexKey || !envBaseUri) {
        std::cerr << "CRITICAL ERROR: Environment variables missing!\n"
                  << "Please ensure the following are set in rsyslog global configuration:\n"
                  << "  - CYBORG_API_KEY\n"
                  << "  - CYBORG_INDEX_KEY\n"
                  << "  - CYBORG_BASE_URI\n";
        return 1;
    }

    std::string apiKey(envApiKey);
    std::string indexKey(envIndexKey);
    std::string baseUri(envBaseUri);

    // 3. Continuous processing of stdin from rsyslog
    std::string line;

while (std::getline(std::cin, line)) {
    if (!line.empty()) {
        // Remove trailing \r or \n if they exist
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) {
            line.pop_back();
        }
        send_to_cyborg(line, apiKey, indexKey, baseUri);
    }
}


    return 0;
}


