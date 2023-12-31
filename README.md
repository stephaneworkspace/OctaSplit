# OctaSplit par bressani.dev

Bienvenue dans OctaSplit, une application multiplateforme développée en C++ avec l'ui en JUCE.

![OctaSplit on MacOS](./Assets/screen_macos.png)
![OctaSplit on Windows](./Assets/screen_windows.png)
![OctaSplit on Linux Ubuntu/Debian](./Assets/screen_linux.png)

## À propos
OctaSplit est un outil pratique destiné aux producteurs de musique. Il permet de prendre un fichier généré par un logiciel de musique assistée par ordinateur, tel que Cubase, Ableton Live, Reason, Renoise, Fruity Loops, Logic ou Bitwig, et de le découper en petits morceaux prêts à être utilisés dans l'Octatrack d'Elektron.

## Caractéristiques
- **Compatible multiplateforme** : Fonctionne sur tous les systèmes d'exploitation courants. Testé sur Mac OS 13 Ventura (intel), Windows 11 et Ubuntu 22.04 Gnome.
- **Découpage des fichiers audio** : Transformez vos fichiers audio en fragments prêts à être utilisés dans l'Octatrack.

## Fonctionnement
Avec OctaSplit, manipuler vos fichiers audio n'a jamais été aussi simple. Ouvrez un fichier WAV, puis cliquez sur le bouton « Split ». Instantanément, le programme divise votre fichier en plusieurs segments, créant des fichiers tels que nomfichier_00.wav, nomfichier_01.wav, etc. Un outil pratique et efficace pour ceux qui veulent des boucles sur un sampler tel que l'octatrack d'Elektron.

## Licence
OctaSplit est publié sous la licence open source GNU General Public License v3.0. Vous êtes libre de modifier, distribuer et utiliser le logiciel en accord avec les termes de cette licence.

Pour plus de détails, veuillez consulter le fichier `LICENSE` inclus dans le dépôt.

L'image de fond a été utilisé de GarryKillian on Freepik. Une partie du design du logo a été crée à partir d'une modification de l'image de onfocus sur Freepik.

## Compilation

### MacOs / Windows / Linux
Nécessite cmake pour compiler.

### MacOs

```
cd build
cmake ..
```

### Windows

```
cd build
cmake ..
```

### Ubuntu

Nécessite clang.

```
sudo apt-get update
sudo apt-get install libfreetype6-dev libgtk-3-dev libwebkit2gtk-4.0-dev portaudio19-dev
```

## Contact
Pour toute question ou suggestion, n'hésitez pas à me contacter sur [bressani.dev](http://bressani.dev).
