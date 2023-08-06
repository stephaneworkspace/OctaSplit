#!/bin/bash

OS=$(uname)

if [ "$OS" != "Linux" ]; then
    echo "Ce script doit être exécuté uniquement sur Linux!"
    exit 1
fi

# Lire les informations depuis le fichier .env
APP_NAME=$(grep "APP_NAME" .env | cut -d '=' -f2)
APP_VERSION=$(grep "APP_VERSION" .env | cut -d '=' -f2)

# clean
echo "Clean du répertoire, nottament des fichiers ignoré par .gitignore"

if [ -d "_deps" ]; then
    rm -rf _deps
fi
if [ -d "OctaSplit_artefacts" ]; then
    rm -rf OctaSplit_artefacts
fi

if [[ -f "./build.ninja" ]]; then
    rm "./build.ninja"
fi
if [[ -f "./cmake_install.cmake" ]]; then
    rm "./cmake_install.cmake"
fi
if [[ -f "./CMakeCache.txt" ]]; then
    rm "./CMakeCache.txt"
fi
if [[ -f "./CPackConfig.cmake" ]]; then
    rm "./CPackConfig.cmake"
fi
if [[ -f "./CPackSourceConfig.cmake" ]]; then
    rm "./CPackSourceConfig.cmake"
fi


# Supprimer le dossier de build s'il existe
if [ -d "build" ]; then
  rm -rf build
fi

# Créer un dossier de build si il n'existe pas
mkdir -p build
cd build

# Supprimer le répertoire 'bin' s'il existe
if [ -d "bin" ]; then
  rm -r bin
fi

# Générer les fichiers de projet avec CMake
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64" ..
#cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" ..

# Compiler le projet
cmake --build . --config Release

# Si vous souhaitez copier le fichier exécutable dans un emplacement spécifique, ajoutez la commande ici
# par exemple :
# cp OctaSplit.app /chemin/vers/emplacement/souhaité
#/build/OctaSplit_artefacts/Release



mkdir bin
#cp -rf ./OctaSplit_artefacts/Release ./bin/opt/OctaSplit
cd bin

mkdir -p octasplit-deb/DEBIAN
#mkdir -p octasplit-deb/usr/bin
mkdir -p octasplit-deb/usr/share/applications
mkdir -p octasplit-deb/usr/share/pixmaps
mkdir -p octasplit-deb/opt/
cp -rf ../OctaSplit_artefacts/Release ./octasplit-deb/opt/OctaSplit
chmod +x ./octasplit-deb/opt/OctaSplit/OctaSplit
cp ../../.env octasplit-deb/opt/OctaSplit/
cp -r ../../Assets/ octasplit-deb/opt/OctaSplit/
#cp octasplit octasplit-deb/usr/bin
cp ../../icon/icon.png octasplit-deb/usr/share/pixmaps/

# Créer ou écraser le fichier OctaSplit.desktop avec le contenu approprié
cat <<EOL > octasplit-deb/usr/share/applications/OctaSplit.desktop
[Desktop Entry]
Version=$APP_VERSION
Name=$APP_NAME
Comment=Description de $APP_NAME
Exec=/opt/OctaSplit/OctaSplit
Icon=/usr/share/pixmaps/OctaSplit.png
Terminal=false
Type=Application
Categories=Utility;
EOL

echo "Fichier .desktop créé avec succès!"

cat <<EOL > octasplit-deb/DEBIAN/control
Package: $APP_NAME
Version: $APP_VERSION
Section: base
Priority: optional
Architecture: amd64
Maintainer: Stéphane Bressani <stephane@bressani.dev>
Description: OctaSplit est un outil pour découper un wav en plusieurs parties sur un bpm avec des mesures définies.
EOL

echo "Fichier control créé avec succès!"

cd ..
dpkg-deb --build ./bin/octasplit-deb
#sudo dpkg -i octasplit-deb.deb

echo "Compilation terminée."
