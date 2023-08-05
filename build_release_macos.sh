#!/bin/bash

# Supprimer le dossier de build s'il existe
if [ -d "build" ]; then
  rm -r build
fi

# Créer un dossier de build si il n'existe pas
mkdir -p build
cd build

# Supprimer le répertoire 'bin' s'il existe
if [ -d "bin" ]; then
  rm -r bin
fi

# Générer les fichiers de projet avec CMake
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" ..

# Compiler le projet
cmake --build . --config Release

# Si vous souhaitez copier le fichier exécutable dans un emplacement spécifique, ajoutez la commande ici
# par exemple :
# cp OctaSplit.app /chemin/vers/emplacement/souhaité

echo "Compilation terminée."