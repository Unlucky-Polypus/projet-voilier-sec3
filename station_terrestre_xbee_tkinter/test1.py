import json
import logging
from logging.handlers import RotatingFileHandler,WatchedFileHandler
import time


# création de l'objet logger qui va nous servir à écrire dans les logs
logger = logging.getLogger()

# on met le niveau du logger à DEBUG, comme ça il écrit tout
logger.setLevel(logging.DEBUG)

# création d'un formateur qui va ajouter le temps, le niveau
# de chaque message quand on écrira un message dans le log
formatter = logging.Formatter('%(asctime)s :: %(levelname)s :: %(message)s')

# création d'un handler qui va rediriger une écriture du log vers
# un fichier en mode 'append', avec 1 backup et une taille max de 1Mo
file_handler = RotatingFileHandler('activity.log', 'a', 1000000, 1)

'''logging.basicConfig(level=logging.DEBUG,
                    filename="activity.log",
                    filemode="a",
                    format='%(asctime)s - %(levelname)s - %(message)s')'''


# on lui met le niveau sur DEBUG, on lui dit qu'il doit utiliser le formateur
# créé précédement et on ajoute ce handler au logger
file_handler.setLevel(logging.DEBUG)
file_handler.setFormatter(formatter)
logger.addHandler(file_handler)

while True:
    try :
        f = open("data.json", "r")
        jsonContent = f.read()
        objJson = json.loads(jsonContent)

        logger.info('Angle nord : %f', (objJson['GROVE'][3]['degNord']))

    except ValueError:
         logger.error('Fichier JSON illisible')
         print('ERROR : Fichier JSON illisible')


    time.sleep(2)