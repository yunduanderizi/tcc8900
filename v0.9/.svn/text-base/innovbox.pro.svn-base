include(./product.conf)

TEMPLATE      = subdirs

SUBDIRS = wm \
          audioControlCenter \
          appmanager \
          caraction \
          diskManager  \
          diskManagerCenter \
          camera  \
          bsManager \
          systemSetting \
          audioControl \
          audioControlLib \
          dbusManager \
          inputmethod \
          mediaControlv1 \
          mediaControlv2 \
          recoveryDefaultData \
          bsManagerProcess \
          dataManager \
          dataBasePlugin \
          utility \
          tables \
          jt808 \
	  freight-jt808 \
          printer \
  	  capapp \
          commonUi/scrollText \
          recordVoice \
          devicemanager \
          gpsinterface \
          gb19056 \
          ic_card \
          deviceManager_sleep_mode \
          schoolcustom \
	  remotecontrol \
	  tools/readsimiccid \
	  remoteupdate  \
          audioManager
contains(DEFINES,WITH_NEW_AUDIO_CONTROL_SYSTEM){
}else{
SUBDIRS += \
          voicev1 \
          voicev2 \
          voicev2Lib \
}

	  
contains(DEFINES, _8925_VERSION){
SUBDIRS += \
          radioManager 
}

SUBDIRS  += modem_ztemf5202
SUBDIRS  += pppd_ztemf5202

audioManager.subdir = audioManager
audioManager.depends = dbusManager

modem_ztemf5202.subdir = modem/zte_mf5202
modem_ztemf5202.depends = camera capapp bsManager dbusManager

pppd_ztemf5202.subdir = pppd/zte_mf5202
pppd_ztemf5202.depends =

wm.subdir =wm 
wm.depends =

audioControlCenter.subdir =audioControlCenter
audioControlCenter.depends=wm

appmanager.subdir =appmanager
appmanager.depends =wm

caraction.subdir  =caraction
caraction.depends =

diskManagerCenter.subdir =diskManagerCenter
diskManagerCenter.depends =wm

diskManager.subdir =diskManager
diskManager.depends =diskManagerCenter

camera.subdir =camera
camera.depends =
capapp.subdir = capapp
capapp.depends = 

ic_card.subdir = ic_card
ic_card.depends = 

schoolcustom.subdir = schoolcustom
schoolcustom.depends = 

voicev1.subdir = voice.v1
voicev1.depends =audioControlCenter

audioControl.subdir = audioControl
audioControl.depends = wm

audioControlLib.subdir = audioControl/audioControlLib
audioControlLib.depends = audioControl wm

voicev2.subdir = voice.v2
voicev2.depends = wm audioControlLib

voicev2Lib.subdir = voice.v2/voiceLib
voicev2Lib.depends = wm audioControlLib voicev2

bsManager.subdir = bsManager
bsManager.depends = wm

bsManagerProcess.subdir =bsManager/bsManagerProcess
bsManagerProcess.depends =wm bsManager

systemSetting.subdir =systemSetting
systemSetting.depends =

dbusManager.subdir = dbusManager
dbusManager.depends =wm

inputmethod.subdir =inputmethod
ytinputmethod.depends =

mediaControlv1.subdir =multimedia/mediaControl.v1
mediaControlv1.depends =wm audioControlCenter

mediaControlv2.subdir =multimedia/mediaControl.v2
mediaControlv2.depends =wm audioControlCenter

recoveryDefaultData.subdir =recoveryDefaultData
recoveryDefaultData.depends =

dataManager.subdir = dataManager
dataManager.depends =

dataBasePlugin.subdir = dataManager/dataBasePlugin
dataBasePlugin.depends =

utility.subdir = utility
utility.depends =

tables.subdir = tables
tables.depends = utility

jt808.subdir = jt808
jt808.depends = utility tables gb19056

printer.subdir = printer
printer.depends = 

recordVoice.subdir = recordVoice
recordVoice.depends = 

devicemanager.subdir = deviceManager
devicemanager.depends = utility tables gb19056

deviceManager_sleep_mode.subdir = deviceManager_sleep_mode
deviceManager_sleep_mode.depends = utility tables gb19056

gpsinterface.subdir = gpsinterface
gpsinterface.depends =  utility tables

gb19056.subdir = gb19056
gb19056.depends = utility tables

remotecontrol.subdir = remotecontrol
remotecontrol.depends = tables

readsimiccid.subdir = readsimiccid
readsimiccid.depends = tables

