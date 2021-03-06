GTERM for windows control file

;Initbaud - initial baud rate
;
INITBAUD 2400

;Initport - initial com port
INITPORT 1

;Various modem control strings follow:
l
INITSTR ATZ^M
PREFIX ATDT
SUFFIX ^M
HANGUP ~~~+++~~~ATH^M
BUSY BUSY^M
NOCARRIER NO CARRIER^M
NODIALTONE NO DIALTONE^M
NOANSWER NO ANSWER^M
VOICE VOICE^M
AUTOBAUD NOSET
C300 CONNECT^M
C1200 CONNECT 1200^M
C2400 CONNECT 2400^M
C9600 CONNECT 9600^M
C19200 CONNECT 19200^M
C38400 CONNECT 38400^M
C57600 CONNECT 57600^M
CLOCK CONNECT
R300 300
R1200 1200
R2400 2400
R9600 9600
R19200 19200
R38400 38400
R57600 57600

;Timeout - timeout delay while wiating for carrier (dialing directory)
;
TIMEOUT 45

;Delay - delay between dial passes
;
DELAY 2

;Mod player stuff
;
AUDIOMOD OFF
MODPLAYERNAME MOD4WIN.EXE

;Map window border type
;
BORDER STANDARD

;Automap control flags
;
MAPPERON YES
TRACKON YES
AUTOMAPON YES

;Dialing directory entries
;
DIAL1 LOD Support #1,1-520-544-4655,38400
DIAL2 LOD Support #2,1-520-797-8573,38400
