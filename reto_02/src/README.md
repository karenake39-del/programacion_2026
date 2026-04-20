# Cliente de Terminal (C++)

Aplicación básica de terminal en C++ que envía un prompt a un modelo GPT o Gemini mediante API e imprime la respuesta.

## Requisitos

- Compilador C++17
- Biblioteca de desarrollo `libcurl`
- API key para el proveedor que uses:
	- OpenAI: `OPENAI_API_KEY`
	- Google Gemini: `GEMINI_API_KEY` (o `GOOGLE_API_KEY`)
	  https://aistudio.google.com/app/api-keys

### Requisitos en Windows (recomendado)

Este proyecto usa `libcurl`, asi que la forma mas sencilla en Windows es MSYS2.

1. Instala MSYS2:

```powershell
winget install -e --id MSYS2.MSYS2
```

2. Abre la terminal **MSYS2 UCRT64** e instala herramientas:

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-curl
```

3. Verifica que el compilador existe:

```bash
g++ --version
```

4. Configura PATH permanente en Windows (una sola vez):

```powershell
powershell -ExecutionPolicy Bypass -File .\setup_windows_path.ps1
```

5. Cierra y vuelve a abrir VS Code (o al menos la terminal integrada) para que el PATH persistente se aplique a nuevas sesiones.

Si no quieres reiniciar, recarga el PATH en la sesion actual:

```powershell
$env:Path = [Environment]::GetEnvironmentVariable("Path", "User") + ";" + [Environment]::GetEnvironmentVariable("Path", "Machine")
```

## Compilacion

Desde la carpeta `retos/reto_02`:

- En Windows (MSYS2 UCRT64):

```bash
g++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal.exe
```

- En Windows (PowerShell, usando g++ de MSYS2 sin abrir UCRT64):

```powershell
g++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal.exe
```

- En macOS/Linux:

```bash
clang++ -std=c++17 src/main.cpp -lcurl -o chatbot_terminal
```

## Ejecucion

Crea/edita tu archivo local `.env`:

```bash
cat .env
```

Carga variables desde `.env` en tu sesion actual de `zsh` (opcional en macOS/Linux):

```bash
set -a
source .env
set +a
```

`chatbot_terminal` también carga automáticamente `.env` desde el directorio de trabajo actual, por lo que hacer `source` es opcional cuando ejecutas desde la carpeta del proyecto.

En PowerShell (Windows), una forma simple es definir variables manualmente:

```powershell
$env:OPENAI_API_KEY="your_api_key_here"
$env:OPENAI_BASE_URL="https://api.openai.com"
$env:GEMINI_API_KEY="your_gemini_api_key_here"
$env:GEMINI_BASE_URL="https://generativelanguage.googleapis.com/v1beta/openai"
```

Alternativa manual (sin `.env`):

```bash
export OPENAI_API_KEY="your_api_key_here"
export GEMINI_API_KEY="your_gemini_api_key_here"
```

Opcional: define una URL base compatible alternativa:

```bash
export OPENAI_BASE_URL="https://api.openai.com"
export GEMINI_BASE_URL="https://generativelanguage.googleapis.com/v1beta/openai"
```

Ejecuta con OpenAI (explicito):

```powershell
.\chatbot_terminal.exe --provider openai --model gpt-4o-mini --prompt "Escribe un haiku sobre programacion"
```

Ejecuta con Gemini (explicito):

```powershell
.\chatbot_terminal.exe --provider gemini --model gemini-3-flash-preview --prompt "Escribe un haiku sobre programacion"
```

Tambien puedes usar `--provider auto` (por defecto): si el modelo empieza por `gemini`, usa Gemini; en cualquier otro caso usa OpenAI.

En macOS/Linux:

```bash
./chatbot_terminal --provider openai --model gpt-4o-mini --prompt "Escribe un haiku sobre programacion"
./chatbot_terminal --provider gemini --model gemini-3-flash-preview --prompt "Escribe un haiku sobre programacion"
```

O ejecuta en modo interactivo (te pedirá una línea de prompt):

```powershell
.\chatbot_terminal.exe --provider openai --model gpt-4o-mini
```

En macOS/Linux:

```bash
./chatbot_terminal --model gpt-4o-mini
```

## Notas

- El proveedor por defecto es `auto`.
- Con `--provider auto`, si `--model` empieza por `gemini` se usa Gemini; si no, OpenAI.
- El modelo por defecto es `gpt-4o-mini`.
- Si el parseo falla, el programa imprime la respuesta JSON cruda.
- Si ves caracteres raros como `C├│digo`, recompila para usar la version actual que configura UTF-8 automaticamente en Windows.
- Si estas en Windows PowerShell y te aparece `clang++ no se reconoce`, instala MSYS2 y compila desde la terminal **MSYS2 UCRT64** usando `g++`.
- Si `pacman` muestra una pregunta de seleccion para `toolchain`, presiona `Enter` para aceptar todos los paquetes.
- Si aparece `EXIT:-1073741515`, falta una DLL en runtime. Ejecuta una sola vez `setup_windows_path.ps1` y abre una terminal nueva.
