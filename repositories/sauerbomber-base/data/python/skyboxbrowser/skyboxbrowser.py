from core.events import registerServerEventHandler
import logging
import sauerbomber
import string
import random
import glob

#def onTrySetmaster(m):
#    if m == 1:
#        sauerbomber.setMasterMode(1)
#        sauerbomber.guiClear()
#        sauerbomber.guiTab("Info")
#        # sauerbomber.guiPushList()
#        sauerbomber.guiText("setmaster", 255, "0")
#        sauerbomber.guiBar()
#        sauerbomber.guiText("on", 0, "0")
#        # sauerbomber.guiPopList()
#        sauerbomber.guiTab("Info2")
#        sauerbomber.guiText("Blah", 255, "0")
#        sauerbomber.guiShow(True)
#    else:
#        sauerbomber.setMasterMode(0)
#        sauerbomber.guiClear()
#        sauerbomber.guiTab("Info")
#        # sauerbomber.guiPushList()
#        sauerbomber.guiText("setmaster", 255, "0")
#        sauerbomber.guiBar()
#        sauerbomber.guiText("off", 0, "0")
#        # sauerbomber.guiPopList()
#        sauerbomber.guiShow(True)

def showSkyboxBrowser():
    sauerbomber.guiClear()
    for item in glob.glob('repositories/*/packages/skyboxes/*/*_up.*'):
        logging.info(item)
        sauerbomber.guiText(item, 255, "0")
    sauerbomber.guiShow(True)
    

def onPlayerFrag(cn, text):
    logging.info("onPlayerMessage")
    showSkyboxBrowser()
    if text == "showskyboxbrowser":
        logging.info("showskyboxbrowser")
    if text == "hideskyboxbrowser":
        logging.info("hideskyboxbrowser")
        sauerbomber.guiShow(False)

logging.info("skyboxbrowser")
registerServerEventHandler('player_frag', onPlayerFrag)
