# Documentación Técnica — Cliente de Terminal Chatbot (C++)

## 1. Resumen General

`chatbot_terminal` es una aplicación de línea de comandos en C++ de un solo binario que envía un prompt del usuario a modelos compatibles (OpenAI y Google Gemini en modo OpenAI-compatible) mediante llamadas HTTP e imprime la respuesta en la terminal.

Objetivos principales:
- Dependencias mínimas (`libcurl` + biblioteca estándar de C++)
- Uso orientado a terminal
- Configuración local simple basada en `.env`

Archivo de implementación actual:
- `src/main.cpp`

## 2. Estructura del Proyecto

- `src/main.cpp`: lógica completa de la aplicación (parseo CLI, carga dotenv, request, manejo de respuesta)
- `.env`: configuración local de ejecución (API key, URL base opcional)
- `.gitignore`: excluye secretos locales (`.env`) y artefactos de compilación
- `README.md`: guía rápida de uso

## 3. Requisitos de Ejecución

- Sistema operativo: Windows, macOS o Linux
- Compilador: compatible con C++17 (`g++` en Windows con MSYS2 UCRT64, `clang++` en macOS/Linux)
- Biblioteca: `libcurl`

Comandos de compilacion:

Windows (MSYS2 UCRT64):

```bash
g++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal.exe
```

Windows (PowerShell usando binarios de MSYS2):

```powershell
g++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal.exe
```

macOS/Linux:

```bash
clang++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal
```

### 3.1 Instalacion recomendada en Windows

Instalar MSYS2 y paquetes requeridos:

```powershell
winget install -e --id MSYS2.MSYS2
```

Luego, en terminal MSYS2 UCRT64:

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-curl
```
Configurar PATH persistente en PowerShell (una sola vez, desde la carpeta del proyecto):

```powershell
powershell -ExecutionPolicy Bypass -File .\setup_windows_path.ps1
```

Nota: VS Code hereda variables de entorno al abrirse. Si el PATH se actualiza mientras VS Code ya esta abierto, reinicia VS Code (o recarga PATH en la sesion actual) para que `chatbot_terminal.exe` encuentre las DLL de MSYS2.

```powershell
$env:Path = [Environment]::GetEnvironmentVariable("Path", "User") + ";" + [Environment]::GetEnvironmentVariable("Path", "Machine")
```

## 4. Modelo de Configuración

La aplicación usa variables de entorno por proveedor:

- OpenAI:
  - `OPENAI_API_KEY` (obligatoria cuando el proveedor activo es OpenAI)
  - `OPENAI_BASE_URL` (opcional, por defecto `https://api.openai.com`)
- Gemini:
  - `GEMINI_API_KEY` (obligatoria cuando el proveedor activo es Gemini)
  - `GOOGLE_API_KEY` (alternativa a `GEMINI_API_KEY`)
  - `GEMINI_BASE_URL` (opcional, por defecto `https://generativelanguage.googleapis.com/v1beta/openai`)

### 4.1 Comportamiento de carga de `.env`

Al iniciar, la app llama a `load_dotenv_file(".env")`.

Comportamiento:
- Si `.env` no existe, la app continúa sin fallar.
- Ignora líneas vacías y comentarios (`# ...`).
- Parsea entradas `KEY=VALUE`.
- Recorta espacios alrededor de claves/valores.
- Elimina comillas simples o dobles envolventes en los valores.
- Define cada variable solo cuando el valor actual del proceso está ausente o vacío.

Esto permite:
- Ejecutar directamente desde la carpeta del proyecto sin `source .env`.
- Mantener `export` explícitos de la shell como prioridad cuando no están vacíos.

## 5. Interfaz CLI

Uso:

```bash
chatbot_terminal [--provider <openai|gemini|auto>] [--model <model>] [--prompt <text>]
```

Opciones:
- `--provider <openai|gemini|auto>`: proveedor de API. `auto` (por defecto) usa Gemini si el modelo empieza por `gemini`; en otro caso usa OpenAI.
- `--model <model>`: nombre del modelo objetivo (por defecto: `gpt-4o-mini`)
- `--prompt <text>`: prompt único en texto
- `-h`, `--help`: muestra ayuda y sale

Comportamiento de entrada:
- Si `--prompt` se omite, la app lee una línea desde stdin en modo interactivo.

## 6. Arquitectura Interna

Todas las funciones auxiliares están en un namespace anónimo dentro de `src/main.cpp`.

### 6.1 Funciones Utilitarias

- `trim(...)`: elimina espacios en blanco al inicio y al final
- `load_dotenv_file(...)`: parser/cargador liviano de dotenv
- `escape_json(...)`: escapa valores de prompt/modelo para seguridad del payload JSON
- `unescape_json_string(...)`: decodifica secuencias de escape comunes de JSON

### 6.2 Funciones HTTP y de Respuesta

- `write_callback(...)`: callback de libcurl para anexar chunks de respuesta en un buffer string
- `request_chat_completion(...)`: construye y ejecuta el request POST al endpoint de chat del proveedor activo
- `extract_content_field(...)`: extracción basada en regex del primer string `"content"` de la respuesta API

### 6.3 Flujo de Control (`main`)

1. Parsea argumentos CLI con `parse_args(...)`
2. Carga `.env`
3. Resuelve proveedor (`openai`, `gemini`, `auto`)
4. Valida API key según proveedor activo
4. Resuelve prompt (argumento CLI o stdin interactivo)
5. Resuelve URL base según proveedor (`OPENAI_BASE_URL` / `GEMINI_BASE_URL` o default)
6. Resuelve path de endpoint según proveedor (`/v1/chat/completions` o `/chat/completions`)
6. Inicializa libcurl globalmente
7. Ejecuta request
8. Limpia libcurl
9. Imprime contenido parseado o fallback JSON crudo
10. Retorna `0` en éxito, `1` en error

## 7. Entradas y Salidas de Métodos (`src/main.cpp`)

### 7.1 `std::string trim(const std::string& value)`

- Entradas:
  - `value`: texto crudo que puede contener espacios en blanco al inicio/final.
- Salida:
  - Retorna un nuevo string con espacios, tabs y saltos de línea removidos al inicio y al final.

### 7.2 `void load_dotenv_file(const std::string& path)`

- Entradas:
  - `path`: ruta del archivo dotenv (la implementación actual usa `.env`).
- Salida:
  - No tiene valor de retorno.
  - Efecto secundario: define variables de entorno del proceso mediante `setenv` para claves parseadas cuyo valor actual está ausente o vacío.

### 7.3 `size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)`

- Entradas:
  - `contents`: puntero a bytes de red recibidos desde libcurl.
  - `size`: tamaño en bytes de cada elemento.
  - `nmemb`: cantidad de elementos.
  - `userp`: puntero al buffer `std::string` donde se anexa la data.
- Salida:
  - Retorna bytes totales procesados (`size * nmemb`) a libcurl.
  - Efecto secundario: agrega el chunk de respuesta al buffer referenciado por `userp`.

### 7.4 `std::string escape_json(const std::string& input)`

- Entradas:
  - `input`: texto sin escapar para colocarlo en campos del payload JSON.
- Salida:
  - Retorna texto seguro para JSON (por ejemplo, comillas, backslashes y caracteres de control escapados).

### 7.5 `std::string unescape_json_string(const std::string& input)`

- Entradas:
  - `input`: fragmento de string JSON escapado.
- Salida:
  - Retorna texto des-escapado con secuencias soportadas convertidas (`\n`, `\t`, `\"`, etc.).

### 7.6 `std::string extract_content_field(const std::string& json_response)`

- Entradas:
  - `json_response`: cuerpo completo de respuesta API en formato JSON.
- Salida:
  - Retorna el primer string `content` encontrado vía regex y des-escapado.
  - Retorna string vacío cuando no encuentra coincidencias.

### 7.7 `CliOptions parse_args(int argc, char* argv[])`

- Entradas:
  - `argc`: número de argumentos CLI.
  - `argv`: vector de argumentos CLI.
- Salida:
  - Retorna `CliOptions` con valores parseados:
    - `provider` (por defecto `auto`, sobreescribible con `--provider`)
    - `model` (por defecto `gpt-4o-mini`, sobreescribible con `--model`)
    - `prompt` (definido con `--prompt` cuando se provee)
  - Comportamiento adicional:
    - En `-h/--help`, imprime uso y finaliza el proceso con código `0`.
    - En argumento desconocido, lanza `std::runtime_error`.

### 7.8 `std::string request_chat_completion(const std::string& api_key, const std::string& base_url, const std::string& chat_path, const std::string& model, const std::string& prompt)`

- Entradas:
  - `api_key`: bearer token para autorización API.
  - `base_url`: URL base de la API.
  - `chat_path`: ruta del endpoint de chat (`/v1/chat/completions` para OpenAI, `/chat/completions` para Gemini en modo OpenAI-compatible).
  - `model`: identificador de modelo a invocar.
  - `prompt`: texto del prompt de usuario.
- Salida:
  - Retorna el cuerpo de respuesta crudo de la API como `std::string` cuando el HTTP status es 2xx.
  - Lanza `std::runtime_error` cuando falla la inicialización/request curl o el status HTTP no es 2xx.

### 7.9 `int main(int argc, char* argv[])`

- Entradas:
  - `argc`, `argv`: argumentos de línea de comandos del proceso.
- Salida:
  - Retorna `0` en éxito.
  - Retorna `1` en errores de validación/ejecución (key faltante, prompt vacío, fallos de request, errores de parseo).
  - Efectos secundarios:
    - Lee `.env`.
    - Lee prompt desde stdin cuando no se usa `--prompt`.
    - Escribe salida/errores en stdout/stderr.
    - Realiza request HTTPS saliente a la API.

### 7.10 Tabla Compacta de E/S

| Método | Entradas | Salida | Excepciones / Efectos Secundarios |
|---|---|---|---|
| `trim(const std::string& value)` | String crudo con posible whitespace alrededor | String recortado | Ninguno |
| `load_dotenv_file(const std::string& path)` | Ruta del archivo dotenv | `void` | Lee archivo, define env vars cuando faltan/están vacías |
| `write_callback(void* contents, size_t size, size_t nmemb, void* userp)` | Punteros/tamaños de chunk curl y puntero a buffer de salida | `size * nmemb` bytes consumidos | Anexa bytes al buffer `std::string` |
| `escape_json(const std::string& input)` | Texto sin escapar | Texto escapado para JSON | Ninguno |
| `unescape_json_string(const std::string& input)` | Fragmento de string JSON escapado | String decodificado | Ninguno |
| `extract_content_field(const std::string& json_response)` | Respuesta JSON cruda | Primer `content` extraído o string vacío | Extracción basada en regex |
| `parse_args(int argc, char* argv[])` | Conteo/vector de argumentos CLI | `CliOptions` (`provider`, `model`, `prompt`) | Sale en `--help`; lanza en argumentos desconocidos |
| `request_chat_completion(const std::string& api_key, const std::string& base_url, const std::string& chat_path, const std::string& model, const std::string& prompt)` | API key, URL base, path de chat, modelo, prompt | String con cuerpo de respuesta crudo | Ejecuta HTTP POST; lanza en errores curl/HTTP |
| `main(int argc, char* argv[])` | Args CLI del proceso | Código de salida `0` o `1` | Carga `.env`, lee stdin opcional, escribe stdout/stderr, llama API |

## 8. Detalles de Integración HTTP

Endpoint por proveedor:
- OpenAI: `${OPENAI_BASE_URL}/v1/chat/completions`
- Gemini (OpenAI-compatible): `${GEMINI_BASE_URL}/chat/completions`

Headers del request:
- `Content-Type: application/json`
- `Authorization: Bearer <API_KEY_DEL_PROVEEDOR>`

Forma del JSON request:

```json
{
  "model": "gpt-4o-mini",
  "messages": [
    { "role": "user", "content": "<prompt>" }
  ]
}
```

Criterio de éxito:
- HTTP status code en `[200, 299]`

Comportamiento en no-éxito:
- Lanza excepción con código HTTP y body de respuesta crudo

## 9. Manejo de Errores y Códigos de Salida

La aplicación usa excepciones y retorna códigos de estado determinísticos:

- `0`: ruta de request/impresión exitosa
- `1`: cualquier falla (parseo de argumentos, key faltante, falla curl, error HTTP, prompt vacío)

Ejemplos de errores visibles para usuario:
- `Missing OPENAI_API_KEY environment variable.`
- `Missing GEMINI_API_KEY (or GOOGLE_API_KEY) environment variable.`
- `Prompt cannot be empty.`
- `CURL request failed: ...`
- `HTTP 429 returned by API. Response body: ...`

## 10. Notas de Seguridad

- La API key se carga desde entorno/`.env`, no está hardcodeada en código fuente.
- `.env` está excluido de git mediante `.gitignore`.
- No registrar API keys completas en salida de debug.
- Rotar claves inmediatamente si se exponen.

## 11. Limitaciones Conocidas

- La extracción de respuesta es basada en regex y es intencionalmente mínima.
  - Estructuras de respuesta complejas/multipart pueden requerir parseo JSON robusto en el futuro.
- Modelo de request de una sola vuelta (sin memoria conversacional multi-turn).
- Sin lógica de retry/backoff para fallos transitorios.
- Sin customización de timeout de request expuesta por CLI.

## 12. Mejoras Futuras Recomendadas

- Reemplazar el parseo de respuesta por regex con una librería parser JSON.
- Agregar opciones `--timeout`, `--max-tokens` y `--temperature`.
- Agregar soporte de streaming para salida incremental en terminal.
- Agregar logging estructurado con modo verbose opcional.
- Agregar tests unitarios para parseo dotenv y parseo de argumentos.
- Agregar retry elegante para errores recuperables 5xx/de red.

## 13. Comandos Operativos

Compilar en Windows (MSYS2 UCRT64):

```bash
cd /c/Users/uccetina/Documents/Code/programacion_2026/retos/reto_02
g++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal.exe
```

Compilar en Windows (PowerShell):

```powershell
cd C:\Users\uccetina\Documents\Code\programacion_2026\retos\reto_02
$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
g++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal.exe
```

Compilar en macOS/Linux:

```bash
cd /ruta/a/programacion_2026/retos/reto_02
clang++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal
```

Ejecutar en Windows (no interactivo):

```powershell
.\chatbot_terminal.exe --provider openai --model gpt-4o-mini --prompt "Hola desde C++"
.\chatbot_terminal.exe --provider gemini --model gemini-3-flash-preview --prompt "Hola desde C++"
```

Ejecutar en macOS/Linux (no interactivo):

```bash
./chatbot_terminal --provider openai --model gpt-4o-mini --prompt "Hola desde C++"
./chatbot_terminal --provider gemini --model gemini-3-flash-preview --prompt "Hola desde C++"
```

Ejecutar en Windows (interactivo):

```powershell
.\chatbot_terminal.exe --provider openai --model gpt-4o-mini
```

Ejecutar en macOS/Linux (interactivo):

```bash
./chatbot_terminal --model gpt-4o-mini
```

Carga opcional de `.env` en shell (macOS/Linux):

```bash
set -a
source .env
set +a
```

Carga manual de variables en PowerShell (Windows):

```powershell
$env:OPENAI_API_KEY="your_api_key_here"
$env:OPENAI_BASE_URL="https://api.openai.com"
$env:GEMINI_API_KEY="your_gemini_api_key_here"
$env:GEMINI_BASE_URL="https://generativelanguage.googleapis.com/v1beta/openai"
```

### 13.1 Troubleshooting Windows

- Si `clang++` no se reconoce en PowerShell, usa MSYS2 UCRT64 con `g++`.
- Si `pacman` pide seleccion de paquetes para el grupo `toolchain`, presiona `Enter` para aceptar todos.
- Asegurate de compilar desde la carpeta `retos/reto_02`.
- Si el proceso termina con `EXIT:-1073741515`, faltan DLLs en runtime. Ejecuta `setup_windows_path.ps1` una sola vez y abre una nueva terminal.
