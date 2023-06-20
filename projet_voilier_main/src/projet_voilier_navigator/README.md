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

Tout les angles dans la librairies sont exprimées en degrés, dans la plage [-180;180] avec 0° étant l’étrave du bateau.

## Concept de base de navigation

La première chose à déterminer est la direction vers laquelle se situe la bouée, l'azimut. L'azimut est un angle entre le Nord, le bateau et la destination. Les angles sur le bateau sont exprimés sur la plage [-180;180] avec le 0° étant l'avant du bateau. Une fois l'azimut déterminé, il faut vérifier que la trajectoire en ligne droite est faisable en fonction du vent (le bateau ne peut pas remonter à plus de 35° de vent). Si la ligne droite est faisable alors le cap donné est l'azimuth, sinon il faut choisir de quel côté remonter le vent.

<p align="center">
  <img src="./images/decision.png" />
</p>

## Utilisation de la librairie

La librairie met à disposition un objet Navigator ainsi que trois fonctions publiques. L'objet Navigator prend en paramètre une destination initiale. 

La première fonction à utiliser est `navigate(Point boat, float wind_angle, float course)`. Cette fonction prend en paramètre la position du bateau, la direction du vent par rapport au bateau ainsi que le cap du bateau par rapport au Nord. Elle renvoie un objet `CommandOutput`qui contient a valeur à passer au safran ainsi qu'aux voiles.

La deuxième fonction utile est `arrived(float precision)`, cette fonction renvoie si le bateau est arrivé aux coordonnées de sa destination plus ou moins la précision, en fonction de la position donnée à `navigate()`.

La dernière fonction à utiliser est la fonction `setBuoy(Point buoy)`, cette fonction redéfinit la destination du Navigator.

## Exemple d'utilisation

	#include <Navigator.h>
	
	// Création de la destination avec ses coordonnées géographiques
	Point buoy = {47.2734979, -2.213848};
	// Création de l'objet Navigator avec la destination initiale
	Navigator nav = Navigator(buoy);
	// Récupération des commandes des actioneurs à partir des données d'entrée
	CommandOutput commands = nav.navigate(boat, wind_angle, course);
	if (nav.arrived()) {
		nav.setBuoy(nextPoint);
	}
	// Utilisation des commandes
	safran = commands.safran;
	sails = commands.sails
