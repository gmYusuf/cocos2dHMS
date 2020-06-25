@echo off

set PYTHON_CHECK=python -c "import sys; version=sys.version_info[:3]; print('{0}'.format(*version))"
set pythonVersionMajor=
for /f "delims=" %%a in ('%PYTHON_CHECK%') do @set pythonVersionMajor=%%a
if not defined pythonVersionMajor (
    echo "**ERR**: Can not find Python 2.x version"
    exit 1
)
if %pythonVersionMajor% equ 2 (
    @python -u "%~dp0\sdkbox.pyc" %*
) else (
    echo "**ERR**: Please use sdkbox with Python 2.x version"
)

