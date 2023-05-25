# Navigator

--
*Version 1.0*

*23/05/2023*

#### Auteur : Nolann Borjon-Piron

## Introduction

L'objectif de cette librairie est de calculer les sorties nécessaires au pilotage d'un bateau autonome en fonction des entrées de différents capteurs. Le bateau est équipé de trois capteurs : 

- Un GPS : renvoie la position GPS du bateau 
- Une IMU : renvoie l'orientation du bateau par rapport au Nord
- Une girouette : renvoie la direction du vent par rapport au bateau

Les deux actionneurs du bateau sont deux moteurs pilotés par PWM, un pour le safran et un autre pour le réglage des deux voiles.

Tout les angles dans la librairies sont exprimées 

## Concept de base de navigation

La première chose à déterminer est la direction vers laquelle se situe la bouée, l'azimut. L'azimut est un angle entre le Nord, le bateau et 