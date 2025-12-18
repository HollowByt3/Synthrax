#include <iostream>
#include <string>
#include <cstdlib> 
#include <curl/curl.h>

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

    // Construct the payload with the dynamic indexKey
    std::string payload = R"({
        "index_name": "structured_system_logs",
        "index_key": ")" + indexKey + R"(",
        "items": [{
            "id": "log_)" + std::to_string(time(NULL)) + R"(",
            "vector": [0.0, 0.0, 0.0],
            "metadata": )" + rsyslog_json + R"(
        }]
    })";

    // Combine Base URI with the specific endpoint path
    std::string fullUrl = baseUri + "/v1/vectors/upsert";

    curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

int main() {
    // 1. Fetch all three required variables from the system
    const char* envApiKey   = std::getenv("CYBORG_API_KEY");
    const char* envIndexKey = std::getenv("CYBORG_INDEX_KEY");
    const char* envBaseUri  = std::getenv("CYBORG_BASE_URI");

    // 2. Comprehensive validation
    if (!envApiKey || !envIndexKey || !envBaseUri) {
        std::cerr << "CRITICAL ERROR: Environment variables missing!\n"
                  << "Please ensure the following are set:\n"
                  << "  - CYBORG_API_KEY\n"
                  << "  - CYBORG_INDEX_KEY\n"
                  << "  - CYBORG_BASE_URI (e.g., http://localhost:8000)\n";
        return 1;
    }

    std::string apiKey(envApiKey);
    std::string indexKey(envIndexKey);
    std::string baseUri(envBaseUri);

    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            send_to_cyborg(line, apiKey, indexKey, baseUri);
        }
    }
    return 0;
}

