@echo off
pushd %~dp0

pyinstaller --onefile PacketHandlerGenerator.py

MOVE .\dist\PacketHandlerGenerator.exe .\PktHandlerGen.exe
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /Q /F .\PacketHandlerGenerator.spec

popd
@echo on
