# **{Shimera} Environnement Technique**
> Un guide pour pouvoir appréhender l'environnement technique du projet.

## Sommaire
1. [Introduction](#introduction)
2. [Librairies & outils](#librairies--outils)
3. [Architecture & Déploiement](#architecture--déploiement)
4. [Pré-requis matériels](#pré-requis-matériels)


## Introduction

Le projet SHIMERA est une librairie dite _SDK_ ("_software development kit_" ou kit de développement logiciel) qui a pour but de permettre à son utilisateur (principalement technicien) de mettre en place des effets visuels au dessus de son projet graphique.

L'état final du projet vise à permettre tout projet graphique, indépendemment de son architecture, de greffer SHIMERA au sein de sa boucle de fonctionnement.
Aussi, la nature des effets visuels disponibles ne se limitent pas une industrie logicielle particulière.


### Librairies & Outils
---

Pour le développement de Shimera, nous utilisons plusieurs librairies, SDK ou autre outils qui sont essentiels pour le bon fonctionnement du projet.

Pour la programmation de notre architecture logicielle, nous utilisons le language C++.

Pour la discussion principale entre le matériel graphique et notre libraire, voici une liste des principales librairies et outils utilisés à cet effet:

- **OpenGL** : Une API graphique pour le rendu 2D et 3D que nous manipulons pour afficher les éléments graphiques.
- **Vulkan** : Une API graphique pour le rendu 2D et 3D en alternative de OpenGL.
- **GLSL** : Un language de shading majoritairement utilisé pour OpenGL, qui nous permet de créer des shaders pour les effets visuels.
- **CMake** : Un outil de gestion de compilation qui nous permet de gérer les dépendances et la compilation de notre projet.
- **ApiTrace** : Un outil de traçage d'API qui nous permet de suivre les appels d'API et de déboguer les problèmes liés à l'API graphique.


### Architecture & Déploiement
---

Pour ce qui est du déploiement de Shimera, étant une librairie SDK open-source nous utiliserons GitHub pour héberger le code source. Les utilisateurs pourront alors soit cloner le dépot, soit télécharger une version compilée de la librairie pour l'intégrer dans leurs projets.

La version compilée comprendra un .so (pour Linux) et un .dll (pour Windows) qui pourra être directement utilisée dans les projets ainsi que les définitions des symboles présents dans la libraire.

### Pré-requis matériels
---

Pour faire s'exéctuer notre librairie plusieurs prérequis sont nécessaires :

1. **Prérequis système**

    - **OS compatibles**

        - Windows 10/11 (64-bit)
        - Linux (Ubuntu 22, Fedora 41, ...)
        - macOS Sillicon*

    - **Carte graphique**

        - Compatible OpenGL 3.3+
        - Compatible Metal*
        - Compatible Vulkan*

2. **Prérequis logiciels**

    - **Compilateur C/C++**
        - Linux/macOS : GCC, Clang ou LLVM
        - Windows : MSVC (via Visual Studio 2022), MINGW, Clang ou LLVM

    - **CMake ≥ 3.20**
        Pour gérer la configuration multi-plateforme

    - **Libraire graphique récente (e.g.)**
        - **SDL3**
        - **SFML3**
        - **...**

3. **Dépendances externes (liées à SHIMERA)**

    - **OpenGl**


> (*) : L'implémentation de ces éléments est considéré comme supplémentaire pour le fonctionnement final de la librairie.