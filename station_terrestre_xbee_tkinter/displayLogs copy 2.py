import os
import random
import time
import serial
import json
import logging
from logging.handlers import RotatingFileHandler

import customtkinter
from customtkinter import CTkFont
from tkintermapview import TkinterMapView, convert_address_to_coordinates
from threading import Thread
from PIL import Image, ImageTk

customtkinter.set_default_color_theme("blue")

simulate_moving_boat = 0 # simulate the moving boat

device = serial.Serial()
device.baudrate = 38400
if(simulate_moving_boat == 0) : device.port = 'COM10'
print(device.name)
if(simulate_moving_boat == 0) : device.open()

# création de l'objet logger qui va nous servir à écrire dans les logs
logger = logging.getLogger()

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

class ScrollableCheckBoxFrame(customtkinter.CTkScrollableFrame):
    def __init__(self, master, item_list, command=None, **kwargs):
        super().__init__(master, **kwargs)

        self.command = command
        self.checkbox_list = []
        # for i, item in enumerate(item_list):
        #     self.add_item(item)

    def add_item(self, item):
        checkbox = customtkinter.CTkCheckBox(self, text=item)
        if self.command is not None:
            checkbox.configure(command=self.command)
        checkbox.grid(row=len(self.checkbox_list), column=0, pady=(0, 10))
        self.checkbox_list.append(checkbox)

    def remove_item(self, item):
        for checkbox in self.checkbox_list:
            if item == checkbox.cget("text"):
                checkbox.destroy()
                self.checkbox_list.remove(checkbox)
                return

    def get_checked_items(self):
        return [checkbox.cget("text") for checkbox in self.checkbox_list if checkbox.get() == 1]



class App(customtkinter.CTk):

    APP_NAME = "Le pas mouillé"
    WIDTH = 1000
    HEIGHT = 600



    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.title(App.APP_NAME)
        self.geometry(str(App.WIDTH) + "x" + str(App.HEIGHT))
        self.minsize(App.WIDTH, App.HEIGHT)

        self.marker_list = []

        ############

        self.last_lat = 0.0
        self.last_long = 0.0
        self.last_wind = 0.0
        self.last_compas = 0.0
        self.IMU_STATUS = customtkinter.StringVar()
        self.GPS_STATUS = customtkinter.StringVar()
        self.GIR_STATUS = customtkinter.StringVar()
        self.MODE_STATUS = customtkinter.StringVar()
        self.compas_var = customtkinter.StringVar()
        self.wind_dir_var = customtkinter.StringVar()


        # ============ create two CTkFrames ============

        self.grid_columnconfigure(0, weight=0)
        self.grid_columnconfigure(1, weight=1)
        self.grid_rowconfigure(0, weight=1)

        self.frame_left = customtkinter.CTkFrame(master=self, width=150, corner_radius=0, fg_color=None)
        self.frame_left.grid(row=0, column=0, padx=0, pady=0, sticky="nsew")

        self.frame_right = customtkinter.CTkFrame(master=self, corner_radius=0)
        self.frame_right.grid(row=0, column=1, rowspan=1, pady=0, padx=0, sticky="nsew")



        # ============ frame_left ============

        self.frame_left.grid_rowconfigure(2, weight=1)


        self.label_MODE = customtkinter.CTkLabel(self.frame_left, textvariable=self.MODE_STATUS, anchor="w")
        self.label_MODE.grid(pady=(0, 0), padx=(30, 30), row=0, column=0)

        self.label_IMU = customtkinter.CTkLabel(self.frame_left, textvariable=self.IMU_STATUS, anchor="w")
        self.label_IMU.grid(pady=(60, 0), padx=(30, 30), row=0, column=0)

        self.label_GPS = customtkinter.CTkLabel(self.frame_left, textvariable=self.GPS_STATUS, anchor="w")
        self.label_GPS.grid(pady=(120, 0), padx=(30, 30), row=0, column=0)

        self.label_GIR = customtkinter.CTkLabel(self.frame_left, textvariable=self.GIR_STATUS, anchor="w")
        self.label_GIR.grid(pady=(180, 0), padx=(30, 30), row=0, column=0)

        self.label_wind = customtkinter.CTkLabel(self.frame_left, textvariable=self.wind_dir_var, anchor="w")
        self.label_wind.grid(pady=(240, 0), padx=(30, 30), row=0, column=0)

        self.label_compas = customtkinter.CTkLabel(self.frame_left, textvariable=self.compas_var, anchor="w")
        self.label_compas.grid(pady=(300, 0), padx=(30, 30), row=0, column=0)


        self.map_label = customtkinter.CTkLabel(self.frame_left, text="Map Type:", anchor="w")
        self.map_label.grid(row=3, column=0, padx=(20, 20), pady=(20, 0))
        self.map_option_menu = customtkinter.CTkOptionMenu(self.frame_left, values=["OpenStreetMap", "Google normal", "Google satellite"],
                                                                       command=self.change_map)

        self.map_option_menu.grid(row=4, column=0, padx=(20, 20), pady=(10, 0))

        self.appearance_mode_label = customtkinter.CTkLabel(self.frame_left, text="Apparence:", anchor="w")
        self.appearance_mode_label.grid(row=5, column=0, padx=(20, 20), pady=(20, 0))
        self.appearance_mode_optionemenu = customtkinter.CTkOptionMenu(self.frame_left, values=["Light", "Dark", "System"],
                                                                       command=self.change_appearance_mode)
        self.appearance_mode_optionemenu.grid(row=6, column=0, padx=(20, 20), pady=(10, 20))

        # ============ frame_right ============

        self.frame_right.grid_rowconfigure(1, weight=1)
        self.frame_right.grid_rowconfigure(0, weight=0)
        self.frame_right.grid_columnconfigure(0, weight=1)
        self.frame_right.grid_columnconfigure(1, weight=0)
        self.frame_right.grid_columnconfigure(2, weight=1)
        self.frame_right.grid_columnconfigure(3, weight=0)


        self.map_widget = TkinterMapView(self.frame_right, corner_radius=0)
        self.map_widget.add_right_click_menu_command(label="Add Marker",command=self.add_marker_event,pass_coords=True)

        self.map_widget.grid(row=1, rowspan=2, column=0, columnspan=4, sticky="nswe", padx=(0, 0), pady=(0, 0))

        self.entry = customtkinter.CTkEntry(master=self.frame_right,
                                            placeholder_text="Enter Address ... ")
        self.entry.grid(row=0, column=0, sticky="we", padx=(12, 0), pady=12)
        self.entry.bind("<Return>", self.search_event)

        self.button_5 = customtkinter.CTkButton(master=self.frame_right,
                                                text="Search",
                                                width=90,
                                                command=self.search_event)
        self.button_5.grid(row=0, column=1, sticky="w", padx=(12, 0), pady=12)

        self.button_6 = customtkinter.CTkButton(master=self.frame_right,
                                                text="Place Marker",
                                                width=90,
                                                command=self.set_marker_from_address_event)
        self.button_6.grid(row=0, column=2, sticky="w", padx=(12, 0), pady=12)

        self.button_7 = customtkinter.CTkButton(master=self.frame_right,
                                                text="Config Markers",
                                                width=90,
                                                command=self.config_marker_window)
        self.button_7.grid(row=0, column=3, sticky="w", padx=(0, 10), pady=12)


        # self.button_2 = customtkinter.CTkButton(master=self.frame_right,
        #                                         text="Remove Markers",
        #                                         command=self.clear_marker_event)
        # self.button_2.grid(row=0, column=3, sticky="w", padx=(40, 0), pady=12)

        # Set default values
        self.map_widget.set_address("Ecole Centrale Nantes")
        self.map_option_menu.set("OpenStreetMap")
        self.appearance_mode_optionemenu.set("Dark")

        self.boatmarker = self.map_widget.set_marker(0, 0, text="bato",image=ImageTk.PhotoImage(Image.open("logo_light.ico").resize((50,30), Image.LANCZOS)))

        Thread(target=self.thread_run, args=(self,),daemon=True).start()  # run thread

    def create_circle(self,x, y, r, canvas): #center coordinates, radius
        x0 = x - r
        y0 = y - r
        x1 = x + r
        y1 = y + r
        return canvas.create_oval(x0, y0, x1, y1)

    def search_event(self, event=None):
        self.map_widget.set_address(self.entry.get())

    def add_marker_event(self,coords):
        print("c'est encore un nouveau marqueur :", coords)
        new_marker = self.map_widget.set_marker(coords, coords[1], text="c'est encore un nouveau marqueur")
        self.marker_list.append(new_marker)
        self.scrollable_checkbox_frame.add_item( "Marker " + str(new_marker.position))

    def set_marker_from_address_event(self):
        current_position = convert_address_to_coordinates(self.entry.get()) #self.map_widget_search.get_position()
        new_marker = self.map_widget.set_marker(current_position, current_position[1])
        self.marker_list.append(new_marker)
        self.scrollable_checkbox_frame.add_item( "Marker " + str(new_marker.position))

    def clear_marker_event(self):
        for marker in self.marker_list:
            marker.delete()

    def change_appearance_mode(self, new_appearance_mode: str):
        customtkinter.set_appearance_mode(new_appearance_mode)
        if new_appearance_mode == "Light":
            self.iconbitmap('logo_light.ico')
        if new_appearance_mode == "Dark":
            self.iconbitmap('logo_dark.ico')
        if new_appearance_mode == "System":
            self.iconbitmap('logo_dark.ico')

    def change_map(self, new_map: str):
        if new_map == "OpenStreetMap":
            self.map_widget.set_tile_server("https://a.tile.openstreetmap.org/{z}/{x}/{y}.png")
        elif new_map == "Google normal":
            self.map_widget.set_tile_server("https://mt0.google.com/vt/lyrs=m&hl=en&x={x}&y={y}&z={z}&s=Ga", max_zoom=22)
        elif new_map == "Google satellite":
            self.map_widget.set_tile_server("https://mt0.google.com/vt/lyrs=s&hl=en&x={x}&y={y}&z={z}&s=Ga", max_zoom=22)

    def on_closing(self, event=0):
        self.destroy()

    def start(self):
        self.mainloop()

    def config_marker_window(self):

        markerConfigWindow = customtkinter.CTkToplevel(self)
        markerConfigWindow.title("markerConfigWindow")
        markerConfigWindow.geometry("600x800")

        self.scrollable_checkbox_frame = ScrollableCheckBoxFrame(master=markerConfigWindow, width=300, command=self.checkbox_frame_event,
                                                                 item_list=[f"item {i}" for i in range(0)])
        self.scrollable_checkbox_frame.grid(row=0, column=0, padx=15, pady=15, sticky="ns")

        it = 0
        for marker in self.marker_list:
            it+=1
            self.scrollable_checkbox_frame.add_item( "Marker " + str(it) + " " + str(marker.position))


        #self.scrollable_checkbox_frame.add_item("new item")

    def checkbox_frame_event(self):
        print(f"checkbox frame modified: {self.scrollable_checkbox_frame.get_checked_items()}")




    def thread_run(self,a):
        x = ""
        while 1:
            if(simulate_moving_boat == 0) :
                while device.in_waiting:
                    # WRITING JSON
                    mes = ""
                    try :
                        mes = device.readline().decode("utf-8")
                    except UnicodeDecodeError:
                        print("reset board")
                    print(str(mes))

                    if "SOF" in mes:
                        x = ""
                        try :
                            mes = device.readline().decode("utf-8")
                        except UnicodeDecodeError:
                            print("reset board")

                    if not "EOF" in mes and not "SOF" in mes:
                        x = str(x) + mes

                    elif "EOF" in mes:
                        x = "{"+x+"}"
                        f = open("data.json", mode="w", newline='')
                        f.write(x)
                        f.close
                        x = "" 
            
                # UPDATING UI
                try :
                    f = open("data.json", "r")
                    jsonContent = f.read()
                    objJson = json.loads(jsonContent)

                    if simulate_moving_boat :
                        objJson['GPS']['latitude'] -= 0.000005
                        objJson['IMU']['degNord'] += 1 #= round(random.uniform(0,90),4)
                        objJson['IMU']['degNord'] %=360
                        objJson['GIROUETTE']['degVent'] -= 1 #= round(random.uniform(0,90),0)
                        objJson['GIROUETTE']['degVent'] %= 360
                        objJson2 = json.dumps(objJson, indent=3)
                        with open("data.json", "w") as outfile:
                            outfile.write(objJson2)

                    background = Image.new(mode="RGBA", size=(200,200))
                    image = Image.open("upcompas.png").resize((150,150), Image.LANCZOS)
                    x, y = image.size
                    background.paste(image, (25,25,x+25,y+25), image)

                    self.MODE_STATUS.set("MODE : Manuel" if objJson['SYSTEM STATUS']['MODE'] == 1 else "MODE : Auto")
                    self.IMU_STATUS.set("IMU STATUS : OK" if objJson['SYSTEM STATUS']['IMU'] == 1 else "IMU STATUS : KO")
                    self.GPS_STATUS.set("GPS STATUS : OK" if objJson['SYSTEM STATUS']['GPS'] == 1 else "GPS STATUS : KO")
                    self.GIR_STATUS.set("GIR STATUS : OK" if objJson['SYSTEM STATUS']['GIR'] == 1 else "GIR STATUS : KO")
                    self.wind_dir_var.set("Direction vent : " + str(self.last_wind) + "°")
                    self.compas_var.set("Direction Nord : " + str(self.last_compas) + "°")

                    if (objJson['SYSTEM STATUS']['GPS'] == 1):
                        
                        
                        if (self.last_lat != objJson['GPS']['latitude']) | (self.last_long != objJson['GPS']['longitude']): # boat marker update

                            # try:
                            #     self.boatmarker.delete()
                            # except AttributeError:
                            #     pass
                            if objJson['GPS']['latitude'] != 0 : self.last_lat = objJson['GPS']['latitude']
                            if objJson['GPS']['longitude'] != 0 : self.last_long = objJson['GPS']['longitude']

                            self.boatmarker.set_position(objJson['GPS']['latitude'], objJson['GPS']['longitude'])

                            time.sleep(3.01)

                    if (objJson['SYSTEM STATUS']['GIR'] == 1):

                        if (self.last_wind != objJson['GIROUETTE']['degVent']):
                            if objJson['GIROUETTE']['degVent'] != -999.99 : self.last_wind = round(objJson['GIROUETTE']['degVent'],2)


                    if (objJson['SYSTEM STATUS']['IMU'] == 1):

                        if (self.last_compas != objJson['IMU']['degNord']) :
                            if objJson['IMU']['degNord'] != -999.99 : self.last_compas = round(objJson['IMU']['degNord'],2)
                            else : self.last_compas = 0

                    image3 = Image.open("wind_arrow2.png").rotate(self.last_wind-90).resize((200,200), Image.LANCZOS)
                    image3 = image3.transpose(Image.FLIP_LEFT_RIGHT)
                    x, y = image3.size
                    background.paste(image3, (0, -2,x,y-2), image3)

                    image2 = Image.open("arrow2.png").rotate(self.last_compas).resize((110,110), Image.LANCZOS)
                    image2 = image2.transpose(Image.FLIP_LEFT_RIGHT)
                    x, y = image2.size
                    background.paste(image2, (45, 45,x+45,y+45), image2)  
                    
                    background.save('newimg.png',"PNG")
                    image5 = Image.open("newimg.png")

                    self.logo_image = customtkinter.CTkImage(image5, size=(150, 150))
                    self.navigation_frame_label = customtkinter.CTkLabel(self.frame_left,text="", image=self.logo_image,compound="left" )
                    self.navigation_frame_label.grid(pady=(50, 0), padx=(20, 20), row=1, column=0)

                            # try:
                            #     image4 = Image.open("arrow2.png").resize(((50,30)), Image.LANCZOS) #.rotate(int(objJson['GIROUETTE']['degVent'])).resize(((50,30)), Image.LANCZOS)
                            #     self.boatmarker.image(image4)
                            #
                            # except TypeError:
                            #     print("error")

                        #self.speed_var.set("Speed " + str(objJson['GPS']['speed']))
                        
                        
                except ValueError:
                    logger.error('Fichier JSON illisible')
                    print('ERROR : Fichier JSON illisible')

if __name__ == "__main__":
    app = App()
    app.iconbitmap('logo_dark.ico')
    app.start()
