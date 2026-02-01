# Distribution package create manual

## license

noMeiryoUI (C) 2005,2012-2026 Tatsuhiko Shoji  
The sources for noMeiryoUI are distributed under the MIT open source license

## index

1. Prerequisites
2. Preparation
3. Common first process
4. Make portable version
5. Make installer

## 1.Prerequisites

- [7-Zip](https://www.7-zip.org/)

## 2. Preparation

1. Install 7-Zip.
2. Add 7-Zip binary folder into path.

## 3. Common first process

1. Build portable version and installed version No!! Meiryo UI by Visual Studio 2008/2012/2022.
2. Check all rerease version executable files by [Virus Total](https://www.virustotal.com/gui/home/upload) .
3. Choice few detected version build. Especially Microsoft didn't treat as virus.

## 4. Make portable version

1. Open developer Command prompt.
2. Move packaging directory.
3. Copy portable version of noMeiryoUI.exe into files directory.
4. Type `makeportable.bat`.
5. Then, noMeiryoUI.zip is generated in packaging directory.
