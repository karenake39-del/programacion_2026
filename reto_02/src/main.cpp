#include <curl/curl.h>

#include "conversacion.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>   

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

void set_env_value(const std::string& key, const std::string& value) {
#ifdef _WIN32
    _putenv_s(key.c_str(), value.c_str());
#else
    setenv(key.c_str(), value.c_str(), 1);
#endif
}

std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    const size_t start = value.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }

    const size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

void load_dotenv_file(const std::string& path) {
    std::ifstream dotenv_file(path);
    if (!dotenv_file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(dotenv_file, line)) {
        const std::string cleaned_line = trim(line);
        if (cleaned_line.empty() || cleaned_line[0] == '#') {
            continue;
        }

        const size_t separator = cleaned_line.find('=');
        if (separator == std::string::npos || separator == 0) {
            continue;
        }

        const std::string key = trim(cleaned_line.substr(0, separator));
        std::string value = trim(cleaned_line.substr(separator + 1));

        if (value.size() >= 2 &&
            ((value.front() == '"' && value.back() == '"') ||
             (value.front() == '\'' && value.back() == '\''))) {
            value = value.substr(1, value.size() - 2);
        }

        if (key.empty()) {
            continue;
        }

        const char* existing_value = std::getenv(key.c_str());
        const bool is_missing_or_empty = (existing_value == nullptr || std::string(existing_value).empty());
        if (is_missing_or_empty) {
            set_env_value(key, value);
        }
    }
}

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    const size_t total_size = size * nmemb;
    auto* output = static_cast<std::string*>(userp);
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

std::string escape_json(const std::string& input) {
    std::string escaped;
    escaped.reserve(input.size() + 8);

    for (const char ch : input) {
        switch (ch) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\b': escaped += "\\b"; break;
            case '\f': escaped += "\\f"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped += ch; break;
        }
    }

    return escaped;
}

std::string unescape_json_string(const std::string& input) {
    std::string output;
    output.reserve(input.size());

    for (size_t index = 0; index < input.size(); ++index) {
        if (input[index] != '\\') {
            output += input[index];
            continue;
        }

        if (index + 1 >= input.size()) {
            break;
        }

        const char next = input[++index];
        switch (next) {
            case '"': output += '"'; break;
            case '\\': output += '\\'; break;
            case '/': output += '/'; break;
            case 'b': output += '\b'; break;
            case 'f': output += '\f'; break;
            case 'n': output += '\n'; break;
            case 'r': output += '\r'; break;
            case 't': output += '\t'; break;
            default: output += next; break;
        }
    }

    return output;
}

std::string extract_content_field(const std::string& json_response) {
    const std::regex content_regex("\"content\"\\s*:\\s*\"((?:\\\\.|[^\\\"])*)\"");
    std::smatch match;

    if (std::regex_search(json_response, match, content_regex) && match.size() > 1) {
        return unescape_json_string(match[1].str());
    }

    return "";
}

enum class Provider {
    OpenAI,
    Gemini
};

struct CliOptions {
    std::string model = "gpt-4o-mini";
    std::string prompt;
    std::string provider = "auto";
};

std::string to_lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

bool starts_with(const std::string& value, const std::string& prefix) {
    return value.size() >= prefix.size() &&
           value.compare(0, prefix.size(), prefix) == 0;
}

Provider resolve_provider(const CliOptions& options) {
    const std::string provider = to_lower(options.provider);
    if (provider.empty() || provider == "auto") {
        const std::string model = to_lower(options.model);
        if (starts_with(model, "gemini")) {
            return Provider::Gemini;
        }
        return Provider::OpenAI;
    }

    if (provider == "openai") {
        return Provider::OpenAI;
    }

    if (provider == "gemini" || provider == "google" || provider == "google-gemini") {
        return Provider::Gemini;
    }

    throw std::runtime_error("Unknown provider: " + options.provider + ". Use openai, gemini, or auto.");
}

std::string resolve_api_key(Provider provider) {
    if (provider == Provider::OpenAI) {
        const char* value = std::getenv("OPENAI_API_KEY");
        if (value != nullptr && !std::string(value).empty()) {
            return value;
        }
        throw std::runtime_error("Missing OPENAI_API_KEY environment variable.");
    }

    const char* gemini_value = std::getenv("GEMINI_API_KEY");
    if (gemini_value != nullptr && !std::string(gemini_value).empty()) {
        return gemini_value;
    }

    const char* google_value = std::getenv("GOOGLE_API_KEY");
    if (google_value != nullptr && !std::string(google_value).empty()) {
        return google_value;
    }

    throw std::runtime_error("Missing GEMINI_API_KEY (or GOOGLE_API_KEY) environment variable.");
}

std::string resolve_base_url(Provider provider) {
    if (provider == Provider::OpenAI) {
        const char* env_value = std::getenv("OPENAI_BASE_URL");
        if (env_value == nullptr || std::string(env_value).empty()) {
            return "https://api.openai.com";
        }
        return std::string(env_value);
    }

    const char* env_value = std::getenv("GEMINI_BASE_URL");
    if (env_value == nullptr || std::string(env_value).empty()) {
        return "https://generativelanguage.googleapis.com/v1beta/openai";
    }
    return std::string(env_value);
}

std::string resolve_chat_path(Provider provider) {
    if (provider == Provider::Gemini) {
        return "/chat/completions";
    }
    return "/v1/chat/completions";
}

void configure_console_utf8() {
#ifdef _WIN32
    // Avoid mojibake when printing UTF-8 model output on Windows consoles.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

CliOptions parse_args(int argc, char* argv[]) {
    CliOptions options;

    for (int index = 1; index < argc; ++index) {
        const std::string arg = argv[index];

        if (arg == "--model" && index + 1 < argc) {
            options.model = argv[++index];
            continue;
        }

        if (arg == "--prompt" && index + 1 < argc) {
            options.prompt = argv[++index];
            continue;
        }

        if (arg == "--provider" && index + 1 < argc) {
            options.provider = argv[++index];
            continue;
        }

        if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: chatbot_terminal [--provider <openai|gemini|auto>] [--model <model>] [--prompt <text>]\n";
            std::cout << "If --prompt is omitted, the program reads a single line from stdin.\n";
            std::exit(0);
        }

        throw std::runtime_error("Unknown argument: " + arg);
    }

    return options;
}

std::string request_chat_completion(
    const std::string& api_key,
    const std::string& base_url,
    const std::string& chat_path,
    const std::string& model,
    const std::string& prompt
) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }

    std::string response_body;
    std::string request_url = base_url;
    if (!request_url.empty() && request_url.back() == '/') {
        request_url.pop_back();
    }
    request_url += chat_path;

    const std::string payload =
        "{"
        "\"model\":\"" + escape_json(model) + "\"," 
        "\"messages\":[{\"role\":\"user\",\"content\":\"" + escape_json(prompt) + "\"}]"
        "}";

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

    CURLcode result = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK) {
        throw std::runtime_error(std::string("CURL request failed: ") + curl_easy_strerror(result));
    }

    if (http_code < 200 || http_code >= 300) {
        std::ostringstream message;
        message << "HTTP " << http_code << " returned by API. Response body:\n" << response_body;
        throw std::runtime_error(message.str());
    }

    return response_body;
}

}  // namespace

int main(int argc, char* argv[]) {
    try {
        configure_console_utf8();
        Conversacion conversacion;

        const CliOptions options = parse_args(argc, argv);
        const Provider provider = resolve_provider(options);

        load_dotenv_file(".env");

        const std::string api_key = resolve_api_key(provider);

        std::string prompt = options.prompt;
        if (prompt.empty()) {
            std::cout << "Enter prompt: ";
            std::getline(std::cin, prompt);
        }

        if (prompt.empty()) {
            std::cerr << "Prompt cannot be empty.\n";
            return 1;
        }

        const std::string base_url = resolve_base_url(provider);
        const std::string chat_path = resolve_chat_path(provider);

        curl_global_init(CURL_GLOBAL_DEFAULT);
        const std::string response_json = request_chat_completion(
            api_key,
            base_url,
            chat_path,
            options.model,
            prompt
        );
        curl_global_cleanup();

        const std::string content = extract_content_field(response_json);
        const std::string assistant_text = content.empty() ? response_json : content;

        conversacion.agregarMensaje(Mensaje(prompt, "usuario"));
        conversacion.agregarMensaje(Mensaje(assistant_text, "ia"));
        conversacion.guardarEnArchivo("src/output");

        if (!content.empty()) {
            std::cout << "\nModel response:\n" << content << "\n";
        } else {
            std::cout << "\nRaw JSON response:\n" << response_json << "\n";
        }

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }
}