# **{Shimera} Déploiement**
> Un guide qui décrit comment est structuré notre dépot et notre CI/CD.

## Sommaire
1. [Introduction](#introduction)
2. [Organisation de versionnage](#organisation-de-versionnage)
3. [CI/CD](#cicd)


## Introduction

Le projet SHIMERA est une librairie dite _SDK_ ("_software development kit_" ou kit de développement logiciel) qui a pour but de permettre à son utilisateur (principalement technicien) de mettre en place des effets visuels au dessus de son projet graphique.

L'état final du projet vise à permettre tout projet graphique, indépendemment de son architecture, de greffer SHIMERA au sein de sa boucle de fonctionnement. Aussi, la nature des effets visuels disponibles ne se limitent pas une industrie logicielle particulière.


## Organisation de versionnage
---

Le projet, étant en communication avec plusieurs sources externes et nécessitant une suite de vérification pour mise en production robuste, doit etre préparé l'architecture du dépot _git_ est de théoriquement la suivante:

```
                      ┌─ feat
                      │
       ┌─ dev ┬───────┼─ task
       │      │       │
main ──┤      └─ fix  └─ research
       │
       └─ hotfix
```

Les types de branches sont définis par:
 - main : la branche principale qui contient le code stable mis en production.
 - hotfix : pour les corrections urgentes de bugs en production.
 - dev : pour le test des nouvelles fonctionalités avant MeP.
 - fix : pour les corrections de bugs.
 - task : pour les tâches spécifiques ou les améliorations.
 - feat : pour les nouvelles fonctionnalités en développement.
 - research : pour les recherches et expérimentations.

## CI/CD
---
