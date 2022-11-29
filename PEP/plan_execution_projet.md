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
- **02/12/22** : Revue PEP
- **18/01/23** : Revue + rétrospective
- **22/03/23** : Revue + rétrospective
- **15/05/23** : Revue + rétrospective   

Les livrables attendus pour ce projet sont le code (commenté bien entendu) ainsi que sa documentation, la nomenclature des composants utilisés
*<Rappeler les dates importantes du projet (périodes, revues, etc.) et les délivrables attendus : ce document, la documentation finale, code, etc.>*

### 4.2. Planning de la première période

*<Donner les objectifs de réalisation sur la première période. Lister les fonctionnalités qui seront opérationnelle à la fin de la période (nom, description) ainsi que leurs tests d’acceptation (autrement dit qu’est-ce qui permettra de dire que la fonctionnalité est réalisée).>*

*<Pour chaque fonctionnalité, évaluer leur difficulté/durée et décomposer les en tâches à mener pour les réaliser.>*

*<Préciser par fonctionnalité quels membres de l’équipe y sera affecté lors de la première période.>*

### 4.3. Backlog

*<Backlog: donner la liste des fonctionnalités (nom, description) qui sont planifiées sur les périodes suivantes (pas besoin de préciser sur quelle période). Donner une priorité à ces fonctionnalités (au cas où il faudrait faire un choix).>*

## 5. Management du risque

<Lister les risques (retard, échec, organisation, etc.) possible pour le projet. Pour chacun évaluer le facteur de risque (probalité et impact) et les actiosn à entreprendre pour prévenir le risque et y répondre s'il survient.>
