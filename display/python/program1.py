import display
def homeScreen():
    twittericon = display.loadImage("twitter.png")
    facebookicon = display.loadImage("facebook.png")
    cameraicon = display.loadImage("camera.png")
    gmailicon = display.loadImage("gmail.png")
    mapsicon = display.loadImage("maps.png")
    placeholdericon = display.loadImage("placeholder.png")
    menuicon = display.loadImage("menu.png")
    #Background
    display.FillRect(0,0,319,479, 0x262626)
    #first row
    display.FillRect(14,14, 14+88, 14+88,0xef5b33)
    display.drawImage(14+14+88, 14, twittericon)
    display.FillRect(14+14+14+88+88,14,14+14+14+88+88+88,14+88,0xf4b658)
    #second row
    display.drawImage(14,14+88+14, facebookicon)
    display.FillRect(116,116,116+88,116+88, 0x8bbeb2)
    display.FillRect(218,116,218+88,116+88, 0xf2625d)    
    #Third row
    display.FillRect(14,219,14+40,219+40,0xef5b33)    
    display.drawImage(62,218,cameraicon)
    display.FillRect(14, 267, 14+40, 267+40, 0xf4b658)
    display.FillRect(62,266,62+40,266+40,0x50483c)
    display.FillRect(116,218,116+88,218+88,0x50483c)
    display.drawImage(218,218,gmailicon)
    #forth row
    display.drawImage(14,319,mapsicon)
    display.drawImage(116, 318, placeholdericon)
    #menu
    display.drawImage(0,417,menuicon)
#initDisplay.init()
display.init()
homeScreen()
display.close()