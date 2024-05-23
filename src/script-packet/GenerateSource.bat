@echo off
pushd %~dp0

protoc -I=.\ --cpp_out=.\ .\Packet.proto

PktHandlerGen.exe --path=.\Packet.proto --output=..\app-server\network\PacketHandler --recv=C_ --send=S_
PktHandlerGen.exe --path=.\Packet.proto --output=..\app-client\network\PacketHandler --recv=S_ --send=C_

If ERRORLEVEL 1 PAUSE

XCOPY /Y .\Packet.pb.cc ..\app-server\network
XCOPY /Y .\Packet.pb.h ..\app-server\network

XCOPY /Y .\Packet.pb.cc ..\app-client\network
XCOPY /Y .\Packet.pb.h ..\app-client\network

DEL /Q /F *.pb.cc
DEL /Q /F *.pb.h

popd
@echo on
