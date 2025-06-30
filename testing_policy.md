# **{Shimera} Politique de test**
> Un guide pour pouvoir décrire les politiques de test du projet.

## Sommaire
- [Introduction](#introduction)
- [Objectifs des tests](#objectifs-des-tests)
- [Types de tests](#types-de-tests)
  - [Tests unitaires](#tests-unitaires)
  - [Tests d'intégration](#tests-dintégration)
  - [Tests fonctionnels](#tests-fonctionnels)
  - [Tests de performance](#tests-de-performance)


## Introduction

Le projet SHIMERA est une librairie dite _SDK_ ("_software development kit_" ou kit de développement logiciel) qui a pour but de permettre à son utilisateur (principalement technicien) de mettre en place des effets visuels au dessus de son projet graphique.

L'état final du projet vise à permettre tout projet graphique, indépendemment de son architecture, de greffer SHIMERA au sein de sa boucle de fonctionnement. Aussi, la nature des effets visuels disponibles ne se limitent pas une industrie logicielle particulière.

### Objectifs des tests

Les tests unitaires seront utilisés pour vérifier le bon fonctionnement de chaque fonctionalité et des shaders de SHIMERA. Les tests unitaires seront écrits en C++ avec le framework Catch2.

Les tests d'intégration permettront de valider

Les tests de performance seront utilisés pour vérifier la vitesse de génération des shaders sur différentes librairies graphique.

### Types de tests :
- ### Tests unitaires

  Nous utiliserons un outil capable de tester nos shaders en les comparant à des images de référence de rendu.

  Au préalable, les images seront générées à partir du shader testé dans un environnement graphique crée pour la production.
  Les tests seront vérifiés à partir du pourcentage de résultat, lié à la tolérance de correspondance entre les images.

  Le résultat sera traité de 3 manières suivantes:
  - Le test sera validé si la correspondance est supérieure à 95%
  - Le test sera aussi validé si la correspondance est supérieure à 90% mais un warning sera exposé pour que le testeur puisse vérifier si le shader est problématique.*
  - Le test échouera si la correspondance est inférieure à 90%.

  (*) Dans le cas où le test est automatisé sur GitHub, un ticket sera généré.

- ### Tests d'intégration

  Nous ferons en sorte de vérifier que la construction de la librairie se passe correctement à chaque merge sur la branche de développement (dev) et sur la branche de production (main). De plus, ces test integreront les tests unitaires et de performance sur chaques merge.

- ### Tests fonctionnels

  Nous ferons des tests fonctionnels avant une validation sur les fonctionnalités récemment ajoutées. Pour se faire, nous utiliserons un outil reconnu _RenderDoc_ qui permet de récolter l'ordre de chargement graphique. Ainsi nous pourrons vérifier si les shaders sont correctement chargés dans le bon ordre.

- ### Tests de performance

  Nous crérons un programme capable de mesurer la vitesse de génération des shaders sur différentes librairies graphiques. Le but étant d'avoirle temps le plus court possible pour la génération des shaders surchaque librairie graphique.
