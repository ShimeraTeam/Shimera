# **{Shimera} Accéssibilité**
> Un guide qui décrit comment nous abordons les problèmes d'accécibilité aux personnes en situation de handicap.

## Introduction

Le projet SHIMERA est une librairie dite _SDK_ ("_software development kit_" ou kit de développement logiciel) qui a pour but de permettre à son utilisateur (principalement technicien) de mettre en place des effets visuels au dessus de son projet graphique.

L'état final du projet vise à permettre tout projet graphique, indépendemment de son architecture, de greffer SHIMERA au sein de sa boucle de fonctionnement. Aussi, la nature des effets visuels disponibles ne se limitent pas une industrie logicielle particulière.

## Sommaire

1. [Introduction](#introduction)
2. [Public Visé](#public-visé)
3. [Malvoyants](#malvoyants)

## Public Visé

Le projet SHIMERA est destiné pour des développeurs travaillant sur des projets graphiques, qu'ils soient dans le domaine du jeu vidéo, de la réalité virtuelle, de la réalité augmentée ou d'autres applications graphiques. Il est conçu pour être utilisé par des développeurs qui souhaitent intégrer des effets visuels avancés dans leurs projets. SHIMERA est principalement axé pour le développement et ne permet pas de mettre en place des solutions adaptées aux personnes ayant d'autres handicap que visuels. Pour ce qui est de l'accessibilité vis à vis du développeur, c'est à lui de mettre en place des solutions adaptées à son handicap.

## Malvoyants
SHIMERA étant une librairie composé de shaders, nous méttons à disposition des shaders corrigeant multiples problèmes de vision dont :
- **Daltonisme** : Nous proposons des shaders permettant de corriger multiple types de daltonisme, tels que le protanopie, le deutéranopie et le tritanopie. Ces shaders ajustent les couleurs pour rendre les éléments visuels plus accessibles aux personnes atteintes de ces formes de daltonisme.
- **Vision floue** : Des shaders sont disponibles pour simuler une vision floue, permettant aux développeurs de tester comment leurs applications apparaissent pour les personnes ayant une vision floue.
- **Contraste élevé** : Des shaders sont fournis pour améliorer le contraste des éléments visuels, facilitant la lecture et la navigation pour les personnes malvoyantes.

## Autres Handicaps
SHIMERA étant principalement basé sur des effets visuels, elle n'est pas en mesure de traiter les handicaps autres que visuels.
