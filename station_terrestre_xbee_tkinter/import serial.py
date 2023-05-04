import serial
import json
import logging
from logging.handlers import RotatingFileHandler

device = serial.Serial()
device.baudrate = 38400
device.port = 'COM3'
print(device.name)
device.open()

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

# on lui met le niveau sur DEBUG, on lui dit qu'il doit utiliser le formateur
# créé précédement et on ajoute ce handler au logger
file_handler.setLevel(logging.DEBUG)
file_handler.setFormatter(formatter)
logger.addHandler(file_handler)

x = ""
i = 0

while True:   
     while device.in_waiting:
          mes = device.readline().decode("utf-8")

          if "SOF" in mes:
               x = ""
               mes = device.readline().decode("utf-8")
          if not "EOF" in mes and not "SOF" in mes:
               x = x + mes

          elif "EOF" in mes:
               x = "{"+x+"}"
               f = open("data.json", mode="w", newline='')
               f.write(x)
               f.close
               x = ""
               print("written n"+str(i))
               i += 1

               f = open("data.json", "r")
               jsonContent = f.read()
               objJson = json.loads(jsonContent)


               logger.info('Angle nord : %f', (objJson['GROVE'][3]['degNord']))
               

'''
               message['delta_t'] = 5
               message['field2'] = (objJson['GROVE'][3]['degNord'])
               message['field3'] = (objJson['GIROUETTE']['degVent'])
               message['latitude'] = str((objJson['GPS']['latitude']))
               message['longitude'] = str((objJson['GPS']['longitude']))
               message['elevation'] = 100

               latitude = (objJson['GPS']['latitude'])
               longitude = (objJson['GPS']['longitude'])


               message_buffer.append(message)

               bulk_data = json.dumps({'write_api_key':apiKey,'updates':message_buffer}) # Format the json data buffer
               print(bulk_data)
               message_buffer = []

               request_headers = {"User-Agent":"mw.doc.bulk-update","Content-Type":"application/json","Content-Length":str(len(bulk_data))}

               print(request_headers)
               response = requests.post(url,headers=request_headers,data=bulk_data)
               print (response) # A 202 indicates that the server has accepted the request

'''

