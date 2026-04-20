#include "conversacion.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {

std::string escape_json(const std::string& input) {
    std::string escaped;
    escaped.reserve(input.size() + 8);

    for (char ch : input) {
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

std::tm to_local_tm(std::time_t value) {
    std::tm local_tm{};
#ifdef _WIN32
    localtime_s(&local_tm, &value);
#else
    localtime_r(&value, &local_tm);
#endif
    return local_tm;
}

}  // namespace

void Conversacion::agregarMensaje(const Mensaje& msg) {
    mensajes.push_back(msg);
}

void Conversacion::guardarEnArchivo(const std::string& ruta) const {
    if (mensajes.empty()) {
        return;
    }

    namespace fs = std::filesystem;
    std::vector<Mensaje> mensajes_ordenados = mensajes;
    std::sort(mensajes_ordenados.begin(), mensajes_ordenados.end(), [](const Mensaje& a, const Mensaje& b) {
        return a.FechaHora < b.FechaHora;
    });

    const std::tm first_tm = to_local_tm(mensajes_ordenados.front().FechaHora);

    std::ostringstream date_folder;
    date_folder << std::put_time(&first_tm, "%Y-%m-%d");

    std::ostringstream hour_folder;
    hour_folder << std::put_time(&first_tm, "%H");

    const fs::path output_folder = fs::path(ruta) / date_folder.str() / hour_folder.str();
    fs::create_directories(output_folder);

    std::ostringstream file_name;
    file_name << "chat_" << std::put_time(&first_tm, "%Y%m%d_%H%M%S") << ".json";

    const fs::path output_file = output_folder / file_name.str();
    std::ofstream archivo(output_file);

    archivo << "{\n";
    archivo << "  \"fecha\": \"" << date_folder.str() << "\",\n";
    archivo << "  \"hora\": \"" << std::put_time(&first_tm, "%H:%M:%S") << "\",\n";
    archivo << "  \"total_mensajes\": " << mensajes_ordenados.size() << ",\n";
    archivo << "  \"mensajes\": [\n";

    for (size_t i = 0; i < mensajes_ordenados.size(); ++i) {
        const auto& msg = mensajes_ordenados[i];
        std::tm msg_tm = to_local_tm(msg.FechaHora);
        std::ostringstream msg_timestamp;
        msg_timestamp << std::put_time(&msg_tm, "%Y-%m-%d %H:%M:%S");

        archivo << "    {\n";
        archivo << "      \"remitente\": \"" << escape_json(msg.remitente) << "\",\n";
        archivo << "      \"timestamp\": \"" << msg_timestamp.str() << "\",\n";
        archivo << "      \"contenido\": \"" << escape_json(msg.contenido) << "\"\n";
        archivo << "    }";

        if (i + 1 < mensajes_ordenados.size()) {
            archivo << ",";
        }
        archivo << "\n";
    }

    archivo << "  ]\n";
    archivo << "}\n";
}
