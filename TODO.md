# TODO-List (Staff only)

Pour proposer des requêtes, insérez des tâches dans la liste des autres. Entourez une tâche entre deux ~~ pour les barrer.

## [Serveur] : Bachir
- Vérification de l'existence des pseudo
- Modélisation de la liste des utilisateurs (hashmap ? comment gérer les connexions/déconnexion en gardant la bonne liaison entre id et utilisateur ?)
- Modélisation des données utilisateurs (structure ?)
- Lire un message du le tube du serveur
- Envoyer des messages sur les tubes des utilisateurs
- Interprétation des messages codés reçu (comment réagir lors d'une requête de message privé entre deux utilisateurs ?)
- Messages de contrôle [EXTENSION]

## [Client] : Kostia
- Session initiale (choix du pseudo)
- Ecrire un message au serveur
- Choix de la modélisation des paramètres (chemin du serveur et autres configurations)
- Analyse des commandes utilisateurs
- Connexion
- Déconnexion
- Interprétation des messages codés reçu (que faire quand on reçoit un message privé ?)
- Affichage
- Demande de liste utilisateur [EXTENSION]

## [Protocole] : Boris
- Analyse et découpage du protocole
- Encodage dans le protocole
- Décodage dans le protocole
