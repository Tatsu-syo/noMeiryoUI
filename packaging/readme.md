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
- Visual Studio 2008
- Visual Studio 2012
- Visual Studio 2022 or later
- [Inno setup 6](https://jrsoftware.org/isinfo.php) with language files below
  - Japanese
  - Korean
  - Portuguese (Brazil)
  - Chinese (Simplified)

## 2. Preparation

1. Install Inno Setup with language files.
2. Install 7-Zip.
3. Add 7-Zip binary folder into path.

## 3. Common first process

1. Build portable version and installed version No!! Meiryo UI by Visual Studio 2008/2012/2022 or later.
2. Check all rerease version executable files by [Virus Total](https://www.virustotal.com/gui/home/upload) .
3. Choice few detected version build. Especially Microsoft defender didn't treat as virus.

## 4. Make portable version

1. Open developer Command prompt.
2. Move packaging directory.
3. Write virus safer version portable release file to `copy` command on makeportable.bat.
4. Type `makeportable.bat`.
5. Then, noMeiryoUI.zip is generated in output directory.

## 5. Make installer

1. Open developer Command prompt.
2. Move packaging directory.
3. Type `nmake clean`
4. Type `nmake all`
5. Copy virus safer version release file to files directory.
6. Run Inno setup with setup.iss.
7. Make installer.

## 6. Why older version Visual Studio?

Microsoft defender treats executable file compiled by later Visual Studio as virus.
Many users use Microsoft defender.
To avoid trouble, I use older compiler.
