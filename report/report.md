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
bibliography: true
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

Notre première tentative pour définir l'orientation du collier était d'utiliser simplement le gyroscope. Nous nous somme ensuite rendu compte que le gyroscope ne permettait que d'avoir l'orientation entre 0 et 90 degrés, sauf que nous devions avoir une orientation sur 360 degrés.

Nous avons découvert qu'il était possible de calculer le roulis "roll" en utilisant les quaternions. Malheureusement, malgrés les différentes tentatives, nous ne sommes pas arrivés à quelque chose de concluant.

Nous avons donc décider d'utiliser le filtre de Madgwick. C'est un algorithme de fusion de capteurs (Fusion sensor algorithm). Ces algorithmes sont des techniques mathématiques qui combinent des données provenant de plusieurs capteurs afin de fournir une estimation plus précise et plus fiable de l'état d'un système ou d'un environnement.

Le filtre de Madgwick utilise les données du gyroscope et de l'accéléromètre. Pour ce faire, il utilise la descente de gradient pour optimiser un quaternion qui oriente les données de l'accéléromètre par rapport à une référence connue de la gravité. Ce quaternion est pondéré et intégré au quaternion du gyroscope et à l'orientation précédente. Le résultat est normalisé et converti en angles d'Euler.

Nous avons utiliser l'implémentation arduino officiel ([librairie](https://github.com/arduino-libraries/MadgwickAHRS)) de Madgwick qui s'occupe de faire les calculs pour nous et permet de récupérer le roulis, le lacet et le tanguage (roll, yaw, pitch).

Les calculs effectués derrière les décors sont complexes et nous nous attarderons pas dessus.

Néanmoins, il est possible de lire le [papier](https://courses.cs.washington.edu/courses/cse474/17wi/labs/l4/madgwick_internal_report.pdf) sur ce filtre.


### Modèle de classification

#### Modèle
Nous avons découvert qu'il est possible de faire tourner des modèles de classification sur Arduino avec par exemple la librairie [MicroFlow](https://github.com/Bobingstern/MicroFlow).

Il est donc tout a fait possible d'entraîner un modèle et ensuite le faire tourner sur un RP2040. 
Malheureusement, nous n'avions pas les connaissances nécessaire pour construire un dataset nous même et nous avons trouver que très peu de documentation en ligne sur la possibilité de prédire un comportement animal a partir des données d'un gyroscope et d'un accéléromètre.

Toutefois nous avons trouvé un dataset\ [@DBLP_KammingaJMH19] provenant d'un prototype similaire qui prédisait le comportement d'un cheval, nous avons utilisé ces données pour créer un modèle de type MLP. Le but final étant de montrer qu'il est possible d'embarquer un modèle dans la puce pour directement effectuer les prédictions.

Dans notre cas, nous avons utilisé les données d'un cheval. Nous avons ensuite entrainé le modèle et réussi à uploader les valeurs sur l'arduino à l'aide de MicroFlow. Par soucis de matériel il a été difficile d'entrainer ce modèle (temps et matériel) le dataset utilisé à donc été très réduit afin de pouvoir produire ce modèle "prototype" dans les temps.
L'upload sur l'arduino fonctionne bien cependant les prédictions sont un peu étranges. Les valeurs peuvent peut-être varier en fonction de la sensibilité des accéléromètres et gyroscopes, mais nous n'avions également pas de cheval ni un collier assemblé à lui passer autours du cou afin de tester. Quand bien même, rien que la constitution du collier ou le cheval en question peuvent faire varier les données d'acceleromètre et gyroscope.
Ainsi, il est très difficile de vérifier que le modèle fonctionne correctement. Mais il est également difficile de prédire le comportement d'animaux du à tous ces élements.

En bref, les modèles pour prédire le comportement des animaux sont prometteurs mais nous avions besoin de plus de matériel (Données, PCs, etc.) afin de pouvoir réaliser la tâche demandée.
Par ailleurs, il est important de sous-ligner que si un modèle doit pouvoir prédire les différents comportemment des plusieurs animaux, la demande en données et matériel requis explose encore plus.

Concernant l'intégration du modèle de classification avec les autres composants, celle-ci n'a pas été effectué par manque de temps.

#### Fonctionnement

Tous les documents se trouvent dans le dossier "model". \
Le model est entrainé dans le jupyter notebook "collar.ipynb". Ce notebook contient également de quoi générer 2 fichier,
un fichier avec les informations de notre modèle (export pour MicroFlow) et un fichier de code arduino d'exemple ("model_parameters.cpp")

Les informations du modèle ont été copiés et collés dans le setup de notre code arduino ("arduino_code.ino").

# Conclusion et pour aller plus loin

Nous sommes arrivés a la conclusion qu'il était tout a fait possible de construire un collier qui respecte les contrainte qui nous ont été donné. Nous avons pu avoir un système qui arrive a connaître sa rotation et envoyer des données selon son orientation. 

Concernant le travail futur, dans les grandes lignes, il faut intégrer le modèle avec les autres composants, effectuer le montage des composants électroniques et calibrer les capteurs afin d'activer les bonnes antennes selon l'orientation.

# Références

::: {#refs}
:::