rem Helper script to launch the documentation for the library.
@echo off

if exist docs/html/index.html (
    rem The documentation has been generated, so open it
    echo Opening documentation
    start "" docs/html/index.html
) else (
    rem The documentation has not been generated, exit fail
    echo Documentation has not been generated. Please build it via cmake
    pause
    exit /b 1
)
