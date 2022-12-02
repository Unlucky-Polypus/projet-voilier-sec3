 *<Nom du groupe et logo>*

# Plan d’exécution du Projet

*Version 1.0*

*29/11/2022*

### Auteurs

- Louis Guibert
- Valentin Boudevin
- Théo Gaboreau
- Thomas Debeire
- Nolann Borjon-Piron

## 1. État du projet

### 1.1. Parties prenantes

#### Clients, Experts, Evaluateurs, Support
Jean-Luc Béchennec, Mikaël Briday, Pierre-Emmanuel Hladik, Olivier Roux 
#### Développeurs
Louis Guibert, Valentin Boudevin, Théo Gaboreau, Thomas Debeire, Nolann Borjon-Piron

### 1.2. Description du produit

Le but de ce projet est de développer un voilier modèle réduit (classe 1m) pouvant fonctionner de manière autonome au radioguidée, sur un plan d'eau de taille définie (inférieure à 200 mètres).

### 1.3. Fonctionnalités
#### 1.3.1 Navigation autonome
Le bateau doit être capable de réaliser un trajet simple (aller-retour jusqu'à une bouée) de manière autonome.
##### Changement de mode 

#####  Sens et direction du vent
##### Position GPS
##### Orientation du bateau
##### Contrôle des servomoteurs
##### Retour d'information station au sol 
OK / KO + charge de la batterie
##### Affichage des informations dans une interface (Web ?)
#### 1.3.2 Contrôle manuel
##### Retour d'information station au sol 
Retour de commande + Debug / Test + charge de la batterie
- Contrôle manuel distinct des voiles et du safran
- Navigation autonome sur un trajet simple (
##### Affichage des informations dans une interface (Web ?)


### 1.4. Exigences opérationnelles

- Poids maximal additionnel : 500 gr
- Passage en mode manuel à n'importe quel moment et lors d'une défaillance
- Lecture du vent toutes les 2 secondes
- Lecture orientation bateau toutes les 2 secondes
- Envoi logs toutes les 2 secondes
- Lecture position GPS temps réels

*<Définir les exigences opérationnelles de votre produit en quantifiant les objectifs à atteindre. Les clients utiliseront ces objectifs pour valider la conformité de votre produit (et votre projet). Organiser cette partie en identifiant de manière unique chaque exigence unitaire.>*

### 1.5. Plan de validation
 Essai à terre de chaque modules
##### Tests fonctionnels
- Commande servomoteur
- 
- Communication maquette / station au sol
- Test unitaire 

*<Définir votre plan de validation du produit pour démontrer que le produit répond bien aux attentes du client. Vous devez définir les procédures de test et les démonstrations d’usage prévus pour valider les exigences définies avant.>*

### 1.6. Fonctionnalités réutilisables
#### TPs SINT et projet serre
Nous avons utilisé durant ces projets les protocoles de communication UART, I2C et série que nous allons intégrer dans ce projet. Nous allons pouvoir récupérer 
Réutilisation des TPs de SINT (projet serre) pour les protocoles de communication (UART, I2C, série...), réutilisation du TP temps réel (sémaphores avec les voitures).
Nous pourrons éventuellement nous inspirer de projets réalisés en entreprises ou en stage.
*<Identifier des projets existants que vous pourriez réutiliser. Pour chacune des fonctionnalités, indiquer les éléments que vous avez identifiés avoir les éventuelles limitations en relation avec vos objectifs. Estimer la difficulté pour réutiliser ces projets en fonctions de la qualité de la documentation, de la qualité du code, etc.>*

## 2. Architecture

### 2.1. Architecture matérielle
- GPS : GPS-RTK-SMA ZED-F9P 
- Récepteur Radio : ANN-MB-02-00 
- Adaptateur Xbee vers micro-USB Adaptateur : Xbee USB 11293 
- Module Xbee : Module Xbee série 2C XB24CZ7WIT-004 
- Accéléromètre : Module 9 DoF MPU9250 Grove 101020080 
- Girouette : Fait à la main

### 2.2. Architecture logicielle

## 3. Équipe
- **Louis Guibert** : Mécano / Testeur
- **Valentin Boudevin (coach phase 1)** : Temps réel / sémaphores + Membre Premium du Tinder Gang
- **Théo Gaboreau** : Consultant en girouette, expert schéma
- **Thomas Debeire** : FPGA
- **Nolann Borjon-Piron** : Navigation, algorithmie

## 4. Planning du projet

### 4.1. Jalons et délivrables
 
On décompose le projet en 4 Phases :
 
- **Phase I** : Spécifications du projet (date de fin : 02/12)
- **Phase II** : Implémentation des capteurs (date de fin : 25/01)
- **Phase III** : Mise en commun (date de fin : 23/03)
- **Phase IV** : Correction et finalisation (date de fin : 26/05)

**Livrable**
 
-Code C++ annoté et compatible avec Doxygene
-Le README du fichier git sera incrémenté au fur et à mesure
-J1: On réussit à implémenter le GPS et une première connexion Xbee. Doit être terminé à la moitié de la phase II
-J2: La girouette est physiquement créée, un démonstrateur permet d’envoyer des trames GPS par Xbee, les calculs théoriques pour l’orientation des voiles et du safran sont validés et on peut interpréter les commandes RF. Date limite : fin phase II
-J3: L’implémentation de la girouette, du compas, ainsi que l’implémentation théorique du safran et de la voile sont réalisés.Doit être terminé à la moitié de la phase III
-J4: Un démonstrateur avec un ventilateur permettant de simuler la girouette est utilisé. Les tests unitaires, fonctionnels et limitants sont implémentés et les données à envoyer par Xbee sont spécifiées. Date limite : fin phase III
-J5: Fin de validation des différents tests et ajout de la gestion temps réelle des différentes fonctionnalités. Doit être terminé à la moitié de la phase IV
-J6: Développement d’un démonstrateur permettant de réunir l’ensemble des fonctionnalités. Développement d’une interface web pour l’interprétation des logs Xbee du transmit. Date limite : fin phase IV
 
### 4.2. Planning de la première période

**Tâches** 

Implémentation d'une première communication Xbee (Thomas)
Implémentation de la communication STM32/GPS (Valentin)
Réception et interprétation des commandes RF (Louis)
Calculs et théorie de l'activation du safran et des voiles (Nolann)

**Rendu**

Démonstrateur permettant d’envoyer des données GPS par Xbee

**Temps et difficulté**

Chaque membre de l’équipe dispose d’une tâche pour cette période. 
Si on doit les classer par criticité : 
-Réception et interprétation des commandes RF 
-Calculs et théorie de l'activation du safran et des voiles 
-Implémentation de la communication STM32/GPS
-Conception de la girouette 
-Implémentation d'une première communication Xbee


### 4.3. Backlog

**Tâches vitales**
 
Validation du fonctionnement du switch mode manuel (Théo/Thomas)
Implémentation de tests fonctionnels et unitaires (Louis)
Tâches importantes : 
Implémentation du compas (Valentin)
Implémentation de la girouette (Théo)
Implémentation de la gestion du safran et des voiles (Nolann)
Ajout et vérification du fonctionnement temps réel de l'ensemble (Valentin)
Création d’un mode dégradé (Théo)
Validation des test fonctionnels en mode auto (Louis)
 
**Tâches secondaires**

Spécification des données échangées par Xbee et spécification des échanges (Thomas)
Implémentation des échanges Xbee (Thomas)
 
**Tâches à compléter si le temps le permet** 
 
Développement d'une interface web qui interprète les logs (Nolann)

**Bilan**
 
Hormis la tâche de développement Web, tout me semble indispensable au MVP.
De même, nous sommes 5 et les tâches sont équitablement réparties. Pour nous, tout est réalisable et sera réalisé

## 5. Management du risque
Durant le développement de ce projet, plusieurs points comportent des risques.

### Mauvaise planification
Il est possible que nous ayons mal estimé la durée et/ou la difficulté de chacune des tâches en lesquels nous avons décomposé le projet et cela pourrait mener à un retard voire un échec. 
Il est assez probable que nous ayons mal estimé la durée des tâches du projet et pour prévenir cet écueil d'avoir un impact trop important à la fin du projet , nous allons réévaluer notre liste de tâche à chaque séance pour vérifier notre avancement par rapport à nos prévisions.

### Mauvaises spécifications
Nous pouvons aussi nous rendre compte au cours du projet que nous nous sommes trompés de composants lors de la phase de spécifications. Le moyen d'éviter ce problème est de faire valider nos choix par des experts dans le domaine.

### Panne matérielle et logicielle
La fiabilité des composants et la robustesse de notre code peuvent être remises en cause et c'est pourquoi nous avons prévu de garder le contrôle manuel. Le mode manuel sera activé en cas de panne matérielle, logicielle ou sur décision d'un opérateur à terre.

### Approvisionnement 
Le retard de livraison de certains composants est un autre problème qui peut se poser, surtout durant cette période de crise des semis-conducteurs. C'est pour cette raison que nous avons défini dès le début du projet les composants dont nous allons avoir besoin, et ce afin de les commander au plus tôt si besoin. Un manque de composant pourrait nous empêcher de développer certaines fonctionnalités dont certaines sont critiques.

### Conditions météorologiques
Les conditions météorologiques peuvent nous empêcher de faire un test en condition réelle ou bien nous empêcher de démontrer le bon fonctionnement du produit. Nous ne pouvons pas trouver de solutions à cette éventualité car les créneaux d'essais et de démonstrations sont fixés.
