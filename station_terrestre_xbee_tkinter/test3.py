from PIL import Image, ImageTk

background = Image.new(mode="RGBA", size=(200,200))
image = Image.open("upcompas.png").resize((150,150), Image.LANCZOS)
x, y = image.size
background.paste(image, (25,25,x+25,y+25), image)

image2 = Image.open("arrow2.png").rotate(20).resize((110,110), Image.LANCZOS)
x, y = image2.size
background.paste(image2, (45, 45,x+45,y+45), image2)

image3 = Image.open("wind_arrow2.png").rotate(180).resize((200,200), Image.LANCZOS)
x, y = image3.size
background.paste(image3, (0, -2,x,y-2), image3)

background.save('newimg.png',"PNG")

