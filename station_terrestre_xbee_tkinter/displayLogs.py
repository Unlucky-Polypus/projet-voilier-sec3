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
device.port = 'COM3'
print(device.name)
device.open()

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
        self.speed_var = customtkinter.StringVar()
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

        # self.button_1 = customtkinter.CTkButton(master=self.frame_left,
        #                                         text="Set Marker",
        #                                         command=self.set_marker_event)
        # self.button_1.grid(pady=(20, 0), padx=(20, 20), row=0, column=0)




        self.label_speed = customtkinter.CTkLabel(self.frame_left, textvariable=self.speed_var, anchor="w")
        self.label_speed.grid(pady=(0, 0), padx=(20, 20), row=0, column=0)

        self.label_compas = customtkinter.CTkLabel(self.frame_left, textvariable=self.compas_var, anchor="w")
        self.label_compas.grid(pady=(50, 0), padx=(20, 20), row=0, column=0)

        self.label_wind_dir = customtkinter.CTkLabel(self.frame_left, textvariable=self.wind_dir_var, anchor="w")
        self.label_wind_dir.grid(pady=(100, 0), padx=(20, 20), row=0, column=0)



        #self.navigation_frame_label.image = image3

        #canvas_obj = self.canvas.create_image(250, 250, image=tkimage)
        #c.grid(pady=(50, 0), padx=(20, 20), row=1, column=0)

        #self.create_circle(26, 26, 20, c)


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
        new_marker = self.map_widget.set_marker(coords[0], coords[1], text="c'est encore un nouveau marqueur")
        self.marker_list.append(new_marker)
        self.scrollable_checkbox_frame.add_item( "Marker " + str(new_marker.position))

    def set_marker_from_address_event(self):
        current_position = convert_address_to_coordinates(self.entry.get()) #self.map_widget_search.get_position()
        new_marker = self.map_widget.set_marker(current_position[0], current_position[1])
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
        markerConfigWindow.geometry("400x600")

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
            while device.in_waiting:
                # WRITING JSON
                mes = ""
                try :
                    mes = device.readline().decode("utf-8")
                except UnicodeDecodeError:
                    print("reset board")
                #print(str(mes))

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
                            objJson['GPS']['speed'] = 10 + round(random.uniform(0,0.5),4)
                            objJson['GIROUETTE']['degVent'] = round(random.uniform(0,35),0)
                            objJson2 = json.dumps(objJson, indent=3)
                            with open("data.json", "w") as outfile:
                                outfile.write(objJson2)

                        if (self.last_lat != objJson['GPS']['latitude']) | (self.last_long != objJson['GPS']['longitude']): # boat marker update

                            # try:
                            #     self.boatmarker.delete()
                            # except AttributeError:
                            #     pass
                            self.last_lat = objJson['GPS']['latitude']
                            self.last_long = objJson['GPS']['longitude']

                            self.boatmarker.set_position(objJson['GPS']['latitude'], objJson['GPS']['longitude'])

                            time.sleep(0.1)


                        if ((self.last_wind != objJson['GIROUETTE']['degVent']) | (self.last_compas != objJson['GROVE'][3]['degNord'])): # Compas display update

                            background = Image.new(mode="RGBA", size=(200,200))
                            image = Image.open("upcompas.png").resize((150,150), Image.LANCZOS)
                            x, y = image.size
                            background.paste(image, (25,25,x+25,y+25), image)

                            image2 = Image.open("arrow2.png").rotate(objJson['GROVE'][3]['degNord']).resize((110,110), Image.LANCZOS)
                            x, y = image2.size
                            background.paste(image2, (45, 45,x+45,y+45), image2)

                            image3 = Image.open("wind_arrow2.png").rotate(objJson['GIROUETTE']['degVent']).resize((200,200), Image.LANCZOS)
                            x, y = image3.size
                            background.paste(image3, (0, -2,x,y-2), image3)

                            background.save('newimg.png',"PNG")
                            image5 = Image.open("newimg.png")

                            self.logo_image = customtkinter.CTkImage(image5, size=(150, 150))
                            self.navigation_frame_label = customtkinter.CTkLabel(self.frame_left,text="", image=self.logo_image,compound="left" )
                            self.navigation_frame_label.grid(pady=(50, 0), padx=(20, 20), row=1, column=0)

                            self.last_wind = objJson['GIROUETTE']['degVent']
                            self.last_compas = objJson['GROVE'][3]['degNord']

                            # try:
                            #     image4 = Image.open("arrow2.png").resize(((50,30)), Image.LANCZOS) #.rotate(int(objJson['GIROUETTE']['degVent'])).resize(((50,30)), Image.LANCZOS)
                            #     self.boatmarker.image(image4)
                            #
                            # except TypeError:
                            #     print("error")

                        #self.speed_var.set("Speed " + str(objJson['GPS']['speed']))
                        self.compas_var.set("Compas " + str(objJson['GROVE'][0]['Compas']['x']) + " " + str(objJson['GROVE'][0]['Compas']['y']) + " " + str(objJson['GROVE'][0]['Compas']['z']))
                        self.wind_dir_var.set("Wind dir " + str(objJson['GIROUETTE']['degVent']))


                    except ValueError:
                        logger.error('Fichier JSON illisible')
                        print('ERROR : Fichier JSON illisible')

if __name__ == "__main__":
    app = App()
    app.iconbitmap('logo_dark.ico')
    app.start()
