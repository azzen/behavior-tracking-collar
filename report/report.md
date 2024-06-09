---
title: "Collier analysant le comportement des animaux sauvages"
subtitle: "Internet of Things"
author: "Annen Rayane, Barreira Romero Adrian, Streckeisen Jarod"
lang: fr
date: \today
geometry: margin=2cm
colorlinks: true
link-citations: true
toc: true
header-includes:
    - \usepackage{bm}
    - \usepackage{xcolor}
    - \usepackage{multicol}
    - \definecolor{cgreen}{RGB}{0, 120, 0}
    - \newcommand{\hideFromPandoc}[1]{#1}
    - \hideFromPandoc{
        \let\Begin\begin
        \let\End\end}
---

# Introduction

Ce rapport présente le projet de collier permettant de classifier les comportements d'animaux en fonctions de données récoltées par des capteurs (accéléromètre et gyroscope).

## Problématique

Ce projet fait suite à une proposition de M. Mitouassiwou, un projet similaire a été fait à l'école au sein de l'institut IICT. Toutefois le premier prototype créé par l'institut a encore quelques défauts. 

La communication entre le collier et internet se fait par satellite. Le collier doit être donc équipé d'une antenne spéciale, afin de garantir que la transmission des données s'effectue correctement à chaque passe de satellite, cette antenne doit être orientée vers le haut. La première mesure pour qu'elle soit orientée vers le haut a été de mettre un système de contre-poids (voir\ Figure\ \ref{fig:collar}).

![Coupe simplifiée du premier prototype de collier par l'IICT\label{fig:collar}](./figures/Collar_IICT.drawio.svg){width=30%}

Le principal problème de cette conception est que le système de contre-poids est lourd et dérange l'animal qui porte le collier et peut affecter la manière dont il se comporte.

Notre contribution sera donc de fournir un collier qui devra répondre aux critères suivants :

- Collier léger
- Basse consommation d'énergie
- Communication par satellite

## Équipe

- Rayane Annen : communication satellite et intégration des différents composants dans le dispositif
- Adrian Barreira Romero : modèle de classification du comportement en fonction des données des capteurs obtenus
- Jarod Streckeisen : gestion de l'activation des antennes en fonction de l'orientation du dispositif (et donc du collier).

# Architecture

![Architecture du système\label{fig:architecture}](./figures/Architecture_IOT.drawio.svg){width=80%}

L'architecture présentée en\ Figure\ \ref{fig:architecture} est celle du système dans son ensemble et comment elle pourrait être imaginée dans un cas futur.

L'animal porte le collier, celui-ci transmet à des intervalles d'environ 30 minutes des nouvelles données vers le portal Astrocast en utilisant leur réseaux de satellites, les données sont calculés à partir d'un modèle directement intégré dans le dispositif RP2040.


## Matériel

Pour réaliser le prototype du projet nous avons eu besoin de :

- Un Arduino Nano RP2040 Connect
- Un development kit d'Astronode qui embarquait un Astronode S+ et une carte Wi-Fi permettant de simuler le réseau satellite.
- Du matériel électronique de base pour le cablage et le prototypage (résistances 220 $\Omega$, LEDs, jumper cables)

En vue de finir le prototype, nous avons eu besoin de : 

- 4x Antenna Patch Astronode
- Un multiplexeur / demultiplexeur permettant de switcher sur quelle antenne on veut activer.
- Une batterie 

## Conception

L'idée de base est la suivante : installer plusieurs antennes (des patches) et programmer un mécanisme qui, en fonction de l'orientation du collier active une ou l'autre antenne. Nous avons ensuite schématisé à quoi devrait ressembler le collier (voir\ Figure\ \ref{fig:collar_new})

![Coupe simplifiée du prototype de collier qu'on a réalisé\label{fig:collar_new}](./figures/Collar_Prototype.drawio.svg){width=40%}


### Communication Satellite

Concernant la communication satellite, nous avons utlisé une librairie créée par Astrocast permettant de configurer le module Astronode et l'envoie de message.

Par dessus, on a créé un wrapper permettant d'interagir avec la librairie simplement et de pouvoir tester l'envoie de message sans avoir de module Astronode connecté directement en simulant simplement l'envoie de message.

Les messages sont directement envoyés chez Astrocast et sont accessibles directement sur le portal. Dans un travail futur il est envisageable de pouvoir utiliser leur API pour connecter un dashboard ou tout autre application Web.

### Gestion de l'orientation du collier

### Modèle de classification

# Conclusion