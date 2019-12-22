--local mymod = {}
-- function pri()
-- printmessage("yes", 3000, 3)
-- end
local KEYS = {VK_LBUTTON = 0x01,VK_RBUTTON = 0x02,VK_CANCEL = 0x03,VK_MBUTTON = 0x04,VK_XBUTTON1 = 0x05,
VK_XBUTTON2 = 0x06,VK_BACK = 0x08,VK_TAB = 0x09,VK_CLEAR = 0x0C,VK_RETURN = 0x0D,VK_SHIFT = 0x10,
VK_CONTROL = 0x11,VK_MENU = 0x12,VK_PAUSE = 0x13,VK_CAPITAL = 0x14,VK_KANA = 0x15,VK_JUNJA = 0x17,
VK_FINAL = 0x18,VK_KANJI = 0x19,VK_ESCAPE = 0x1B,VK_CONVERT = 0x1C,VK_NONCONVERT = 0x1D,
VK_ACCEPT = 0x1E,VK_MODECHANGE = 0x1F,VK_SPACE = 0x20,VK_PRIOR = 0x21,VK_NEXT = 0x22,VK_END = 0x23,
VK_HOME = 0x24,VK_LEFT = 0x25,VK_UP = 0x26,VK_RIGHT = 0x27,VK_DOWN = 0x28,VK_SELECT = 0x29,
VK_PRINT = 0x2A,VK_EXECUTE = 0x2B,VK_SNAPSHOT = 0x2C,VK_INSERT = 0x2D, VK_DELETE = 0x2E,
VK_HELP = 0x2F,VK_key0 = 0x30,VK_key1 = 0x31,VK_key2 = 0x32, VK_key3 = 0x33,VK_key4 = 0x34,
VK_key5 = 0x35,VK_key6 = 0x36,VK_key7 = 0x37,VK_key8 = 0x38,VK_key9 = 0x39,VK_A = 0x41,
VK_B = 0x42,VK_C = 0x43,VK_D = 0x44,VK_E = 0x45,VK_F = 0x46,VK_G = 0x47,VK_H = 0x48,VK_I = 0x49,
VK_J = 0x4A,VK_K = 0x4B,VK_L = 0x4C,VK_M = 0x4D,VK_N = 0x4E,VK_O = 0x4F,VK_P = 0x50,VK_Q = 0x51,
VK_R = 0x52,VK_S = 0x53,VK_T = 0x54,VK_U = 0x55,VK_V = 0x56,VK_W = 0x57,VK_X = 0x58,VK_Y = 0x59,
VK_Z = 0x5A,VK_LWIN = 0x5B,VK_RWIN = 0x5C,VK_APPS = 0x5D,VK_SLEEP = 0x5F,VK_NUMPAD0 = 0x60,
VK_NUMPAD1 = 0x61,VK_NUMPAD2 = 0x62,VK_NUMPAD3 = 0x63,VK_NUMPAD4 = 0x64,VK_NUMPAD5 = 0x65,
VK_NUMPAD6 = 0x66,VK_NUMPAD7 = 0x67,VK_NUMPAD8 = 0x68,VK_NUMPAD9 = 0x69,VK_MULTIPLY = 0x6A,
VK_ADD = 0x6B, VK_SEPARATOR = 0x6C,VK_SUBTRACT = 0x6D,VK_DECIMAL = 0x6E,VK_DIVIDE = 0x6F,
VK_F1 = 0x70,VK_F2 = 0x71,VK_F3 = 0x72,VK_F4 = 0x73,VK_F5 = 0x74,VK_F6 = 0x75,VK_F7 = 0x76,
VK_F8 = 0x77,VK_F9 = 0x78,VK_F10 = 0x79,VK_F11 = 0x7A,VK_F12 = 0x7B,VK_F13 = 0x7C,VK_F14 = 0x7D,
VK_F15 = 0x7E,VK_F16 = 0x7F,VK_F17 = 0x80,VK_F18 = 0x81,VK_F19 = 0x82,VK_F20 = 0x83,
VK_F21 = 0x84,VK_F22 = 0x85,VK_F23 = 0x86,
VK_F24 = 0x87,VK_NUMLOCK = 0x90,VK_SCROLL = 0x91,VK_OEM_FJ_JISHO = 0x92,VK_OEM_FJ_MASSHOU = 0x93,
VK_OEM_FJ_TOUROKU = 0x94,VK_OEM_FJ_LOYA = 0x95,VK_OEM_FJ_ROYA = 0x96,VK_LSHIFT = 0xA0,VK_RSHIFT = 0xA1,
VK_LCONTROL = 0xA2,VK_RCONTROL = 0xA3,VK_LMENU = 0xA4,VK_RMENU = 0xA5,VK_BROWSER_BACK = 0xA6,
VK_BROWSER_FORWARD = 0xA7,VK_BROWSER_REFRESH = 0xA8,VK_BROWSER_STOP = 0xA9,VK_BROWSER_SEARCH = 0xAA,
VK_BROWSER_FAVORITES = 0xAB,VK_BROWSER_HOME = 0xAC,VK_VOLUME_MUTE = 0xAD,VK_VOLUME_DOWN = 0xAE,
VK_VOLUME_UP = 0xAF,VK_MEDIA_NEXT_TRACK = 0xB0,VK_MEDIA_PREV_TRACK = 0xB1,VK_MEDIA_STOP = 0xB2,
VK_MEDIA_PLAY_PAUSE = 0xB3,VK_LAUNCH_MAIL = 0xB4,VK_LAUNCH_MEDIA_SELECT = 0xB5,VK_LAUNCH_APP1 = 0xB6,
VK_LAUNCH_APP2 = 0xB7,VK_OEM_1 = 0xBA,VK_OEM_PLUS = 0xBB,VK_OEM_COMMA = 0xBC,VK_OEM_MINUS = 0xBD,
VK_OEM_PERIOD = 0xBE, VK_OEM_2 = 0xBF,VK_OEM_3 = 0xC0,VK_ABNT_C1 = 0xC1,VK_ABNT_C2 = 0xC2,
VK_OEM_4 = 0xDB,VK_OEM_5 = 0xDC,VK_OEM_6 = 0xDD,VK_OEM_7 = 0xDE,VK_OEM_8 = 0xDF,VK_OEM_AX = 0xE1,
VK_OEM_102 = 0xE2,VK_ICO_HELP = 0xE3,VK_PROCESSKEY = 0xE5,VK_ICO_CLEAR = 0xE6,VK_PACKET = 0xE7,
VK_OEM_RESET = 0xE9,VK_OEM_JUMP = 0xEA,VK_OEM_PA1 = 0xEB,VK_OEM_PA2 = 0xEC,VK_OEM_PA3 = 0xED,
VK_OEM_WSCTRL = 0xEE,VK_OEM_CUSEL = 0xEF,VK_OEM_ATTN = 0xF0,VK_OEM_FINISH = 0xF1,VK_OEM_COPY = 0xF2,
VK_OEM_AUTO = 0xF3,VK_OEM_ENLW = 0xF4,VK_OEM_BACKTAB = 0xF5,VK_ATTN = 0xF6,VK_CRSEL = 0xF7,
VK_EXSEL = 0xF8, VK_EREOF = 0xF9,VK_PLAY = 0xFA,VK_ZOOM = 0xFB,VK_PA1 = 0xFD, VK_OEM_CLEAR = 0xFE} 

MODEL_CARS = { MODEL_LANDSTAL = 130, MODEL_IDAHO = 131, MODEL_STINGER = 132,MODEL_LINERUN = 133, 
MODEL_PEREN = 134, MODEL_SENTINEL = 135, MODEL_RIO = 136, MODEL_FIRETRUK = 137, MODEL_TRASH = 138,
MODEL_STRETCH = 139, MODEL_MANANA = 140, MODEL_INFERNUS = 141, MODEL_VOODOO = 142, MODEL_PONY = 143,
MODEL_MULE = 144, MODEL_CHEETAH = 145, MODEL_AMBULAN = 146, MODEL_FBICAR = 147,MODEL_MOONBEAM = 148,
MODEL_ESPERANT = 149, MODEL_TAXI = 150, MODEL_WASHING = 151, MODEL_BOBCAT = 152, MODEL_MRWHOOP = 153,
MODEL_BFINJECT = 154, MODEL_HUNTER = 155, MODEL_POLICE = 156, MODEL_ENFORCER = 157,
MODEL_SECURICA = 158, MODEL_BANSHEE = 159, MODEL_PREDATOR = 160, MODEL_BUS = 161,MODEL_RHINO = 162,
MODEL_BARRACKS = 163,MODEL_CUBAN = 164,MODEL_CHOPPER = 165,MODEL_ANGEL = 166,MODEL_COACH = 167,
MODEL_CABBIE = 168,MODEL_STALLION = 169,MODEL_RUMPO = 170,MODEL_RCBANDIT = 171,MODEL_ROMERO = 172,
MODEL_PACKER = 173,MODEL_SENTXS = 174,MODEL_ADMIRAL = 175,MODEL_SQUALO = 176,MODEL_SEASPAR = 177,
MODEL_PIZZABOY = 178,MODEL_GANGBUR = 179,MODEL_AIRTRAIN = 180,MODEL_DEADDODO = 181,
MODEL_SPEEDER = 182, MODEL_REEFER = 183,MODEL_TROPIC = 184,MODEL_FLATBED = 185,MODEL_YANKEE = 186,
MODEL_CADDY = 187,MODEL_ZEBRA = 188,MODEL_TOPFUN = 189,MODEL_SKIMMER = 190,MODEL_PCJ600 = 191,
MODEL_FAGGIO = 192,MODEL_FREEWAY = 193,MODEL_RCBARON = 194,MODEL_RCRAIDER = 195,
MODEL_GLENDALE = 196, MODEL_OCEANIC = 197,MODEL_SANCHEZ = 198,MODEL_SPARROW = 199,
MODEL_PATRIOT = 200,MODEL_LOVEFIST = 201,MODEL_COASTG = 202,MODEL_DINGHY = 203,MODEL_HERMES = 204,
MODEL_SABRE = 205,MODEL_SABRETUR = 206,MODEL_PHEONIX = 207,MODEL_WALTON = 208,
MODEL_REGINA = 209,MODEL_COMET = 210,MODEL_DELUXO = 211,MODEL_BURRITO = 212,
MODEL_SPAND = 213,MODEL_MARQUIS = 214,MODEL_BAGGAGE = 215,MODEL_KAUFMAN = 216,
MODEL_MAVERICK = 217,MODEL_VCNMAV = 218,MODEL_RANCHER = 219,MODEL_FBIRANCH = 220,
MODEL_VIRGO = 221,MODEL_GREENWOO = 222,MODEL_JETMAX = 223,MODEL_HOTRING = 224,
MODEL_SANDKING = 225,MODEL_BLISTAC = 226,MODEL_POLMAV = 227,MODEL_BOXVILLE = 228,
MODEL_BENSON = 229,MODEL_MESA = 230,MODEL_RCGOBLIN = 231,MODEL_HOTRINA = 232,
MODEL_HOTRINB = 233, MODEL_BLOODRA = 234, MODEL_BLOODRB = 235,MODEL_VICECHEE = 236 }

MODEL_WEAPONS = {cellphone = 258, brassknuckle = 259, screwdriver = 260, golfclub = 261, nitestick = 262, knifecur = 263, 
bat = 264, hammer = 265, cleaver = 266, machete = 267, katana = 268, chnsaw = 269, grenade = 270,
teargas = 271, molotov = 272, missile = 273, colt45 = 274, python = 275, ruger = 276,
chromegun = 277, shotgspa = 278, buddyshot = 279, m4=280, tec9 = 281, uzi = 282, ingramsl = 283,
mp5lng = 284, sniper = 285,laser = 286, rocketla = 287, flame = 288, M60 = 289, minigun = 290,
bomb = 291, camera = 292, fingers= 293, minigun2 =294}
MODEL_PEDS ={ NULL = 0, COP = 1, SWAT = 2, FBI = 3, ARMY = 4,MEDIC = 5, FIREMAN = 6, HFYST = 9, 
HFOST = 10, HMYST = 11, HMOST = 12, HFYRI = 13, HFORI = 14, HMYRI = 15, HMORI = 16, HFYBE = 17, 
HFOBE = 18, HMYBE = 19, HMOBE = 20, HFYBU = 21, HFYMD = 22, HFYCG = 23, HFYPR = 24, HFOTR = 25, 
HMOTR = 26, HMYAP = 27, HMOCA = 28, BMODK = 29, BMYCR = 30, BFYST = 31, BFOST = 32, BMYST = 33, 
BMOST = 34, BFYRI = 35,BFORI = 36, BMYRI = 37,BFYBE = 38, BMYBE = 39,BFOBE = 40, BMOBE = 41,
BMYBU = 42, BFYPR = 43,BFOTR = 44, BMOTR = 45,BMYPI = 46, BMYBB = 47,WMYCR = 48, WFYST = 49, 
WFOST = 50, WMYST = 51,WMOST = 52, WFYRI = 53,WFORI = 54, WMYRI = 55,WMORI = 56, WFYBE = 57, 
WMYBE = 58, WFOBE = 59,WMOBE = 60, WMYCW = 61,WMYGO = 62, WFOGO = 63,WMOGO = 64, WFYLG = 65, 
WMYLG = 66, WFYBU = 67,WMYBU = 68,WMOBU = 69, WFYPR = 70,WFOTR = 71,WMOTR = 72, WMYPI = 73, 
WMOCA = 74, WFYJG = 75, WMYJG = 76, WFYSK = 77, WMYSK = 78, WFYSH = 79, WFOSH = 80, JFOTO = 81,
JMOTO = 82,CBA = 83, CBB = 84, HNA = 85, HNB = 86,SGA = 87, SGB = 88, CLA = 89, CLB = 90, GDA = 91, 
GDB = 92, BKA = 93, BKB = 94, PGA = 95, PGB = 96, VICE1 = 97, VICE2 = 98, VICE3 = 99, VICE4 = 100, 
VICE5 = 101, VICE6 = 102, VICE7 = 103, VICE8 = 104, WFYG1 = 105, WFYG2 = 106} 
PED_MODELS_AND_TYPES = { COP = {1,6}, SWAT = {2,6}, FBI = {3,6}, ARMY = {4,6}, MEDIC = {5,16}, FIREMAN = {6,17},
             HFYST = {9,5}, HFOST = {10,5},HMYST = {11,4},HMOST = {12,4}, HFYRI = {13,5}, HFORI = {14,5},
             HMYRI = {15,4},HMORI = {16,4},HFYBE = {17,5},HFOBE = {18,5},HMYBE = {19,4},HMOBE = {20,4},
             HFYBU = {21,5},HFYMD = {22,5},HFYCG = {23,5},HFYPR = {24,5},HFOTR = {25,5},HMOTR = {26,4},
             HMYAP = {27,4},HMOCA = {28,4},BMODK = {29,4},BMYCR = {30,4},BFYST = {31,5},BFOST = {32,5},
             BMYST = {33,4},BMOST = {34,4},BFYRI = {35,5},BFORI = {36,5},BMYRI = {37,4},BFYBE = {38,5},
             BMYBE = {39,4},BFOBE = {40,5},BMOBE = {41,4},BMYBU = {42,4},BFYPR = {43,5},BFOTR = {44,5},
             BMOTR = {45,4},BMYPI = {46,4},BMYBB = {47,4},WMYCR = {48,4},WFYST = {49,5},WFOST = {50,5},
             WMYST = {51,4},WMOST = {52,4},WFYRI = {53, 5},WFORI = {54, 5}, WMYRI = {55,4}, WMORI = {56,4 },
             WFYBE = {57,5}, WMYBE = {58,4}, WFOBE = {59,5}, WMOBE = {60,4}, WMYCW = {61,4}, WMYGO = {62,4},
             WFOGO = {63,5}, WMOGO = {64,4}, WFYLG = {65, 5}, WMYLG = {66, 4}, WFYBU = {67,5}, WMYBU = {68, 4},
             WMOBU = {69,4}, WFYPR = {70,5}, WFOTR = {71,5}, WMOTR = {72,4}, WMYPI = {73,4}, WMOCA = {74, 4},
             WFYJG = {75, 5}, WMYJG = {76,4}, WFYSK = {77,5}, WMYSK = {78,4}, WFYSH = {79,5}, WFOSH = {80,5 },
             JFOTO = {81, 5}, JMOTO = {82, 4}, CBA = {83,7}, CBB = {84,7}, HNA = {85,8}, HNB = {86,8},
             SGA = {87,9}, SGB = {88,9}, CLA = {89,10}, CLB ={90,10}, GDA = {91,11}, GDB = {92,11},
             BKA = {93, 12}, BKB = {94,12}, PGA = {95,13}, PGB = {96,13}, VICE1 = {97,18}, VICE2 = {98,18},
             VICE3 = {99,18}, VICE4 = {100,18}, VICE5 = {101,18}, VICE6 = {102,18}, VICE7 = {103,18},
             VICE8 = {104,18},WFYG1 = {105,5}, WFYG2 = {106,6}}
SPEC_PED_MODELS_AND_TYPES = {
 BGA = {129,4,20}, BGB = {129,4,20}, BOUNCA = {129,4,20}, BURGER = {129,4,20}, CGONA = {129,4,20}, CGONB = {129,4,20}, CGONC = {129,4,20},
 CHEF = {129,4,20}, CMRAMAN = {129,4,20}, COURIER = {129,4,20}, CREWA = {129,4,20}, CREWB = {129,4,20}, CSJUGGZ = {129,5,20}, 
 DGOONA = {129,4,20}, DGOONB = {129,4,20}, DGOONC = {129,4,20}, FLOOZYA = {129,5,20}, FLOOZYB = {129,5,20}, FLOOZYC = {129,5,20}, 
 FSFA = {129,4,20}, IGALSCB = {129,4,20}, IGBUDDY = {129,4,20}, IGBUDY2 = {129,4,20}, IGBUDY3 = {129,4,20}, IGCANDY = {129,5,20}, 
 IGCOLON = {129,4,20}, IGDIAZ = {129,4,20}, IGDICK = {129,4,20}, IGGONZ = {129,4,20}, IGHLARY = {129,4,20}, IGHLRY2 = {129,4,20}, 
 IGJEZZ = {129,4,20}, IGKEN = {129,4,20}, IGMERC = {129,5,20}, IGMIKE = {129,4,20}, IGMIKE2 = {129,4,20}, IGPERCY = {129,4,20}, 
 IGPHIL = {129,4,20}, IGPHIL2 = {129,4,20}, IGPHIL3 = {129,4,20}, IGSONNY = {129,4,20}, IGMERC2 = {129,5,20}, MBA = {129,4,20}, 
 MBB = {129,4,20}, MPORNA = {129,4,20}, MGOONA = {129,4,20}, MSERVER = {129,4,20}, MBA = {129,4,20}, MBB = {129,4,20}, MPORNA = {129,4,20},
 MGOONA = {129,4,20}, MSERVER = {129,4,20}, PLAY10 = {129,4,20}, PLAY11 = {129,4,20}, PLAY12 = {129,4,20}, PLAYER2 = {129,4,20}, 
 PLAYER3 = {129,4,20}, PLAYER4 = {129,4,20},  PLAYER5 = {129,4,20},  PLAYER6 = {129,4,20},   PLAYER7 = {129,4,20}, 
 PLAYER8 = {129,4,20}, PLAYER9 = {129,4,20}, PRINTRA = {129,4,20},  PRINTRB = {129,4,20}, PRINTRC = {129,4,20}, PSYCHO = {129,4,20},
 S_KEEP = {129,4,20}, SAM = {129,4,20}, SGC = {129,4,20}, SGOONA = {129,4,20}, SGOONB = {129,4,20}, SHOOTRA = {129,5,20}, 
 SHOOTRB = {129,4,20}, SPANDXA = {129,5,20},SPANDXB = {129,5,20}, STRIPA = {129,5,20}
}
			 
WEAPONS_MODELS_AND_TYPES = {brassknuckle = {259,1}, screwdriver = {260,2}, golfclub = {261,3},
nitestick = {262,4},knifecur = {263,5}, bat = {264,6}, hammer = {265,7}, cleaver = {266,8}, 
machete = {267,9}, katana = {268,10}, chnsaw = {269,11}, grenade = {270,12}, teargas = {271,14},
molotov = {272,15}, missile = {273,16}, colt45 = {274,17}, python = {275,18}, ruger = {276,27}, 
chromegun = {277,19}, shotgspa = {278,20}, buddyshot = {279,21}, m4 = {280,26}, tec9 = {281,22}, 
uzi = {282,23}, ingramsl = {283,24}, mp5lng = {284,25}, sniper = {285,32}, laser = {286,29}, 
rocketla = {287,30}, flame = {288,31},M60 = {289,32}, minigun = {290,33},bomb = {291,13}, 
camera = {292,36},fingers = {293,0}, minigun2 ={294, 0}} 

DOORS_CAR = { hood = 0, trunk =1, DFL=2, DFR=3, DRL=4, DRR=5 }--[[
0 капот, 1 багажник, 2 левая передняя дверь, 3 правая передняя дверь, 
4 левая задняя дверь, 5 правая задняя дверь.]]

for k, v in pairs(KEYS) do 	_G[k] = v end	
for k, v in pairs(MODEL_CARS) do _G[k] = v end
for k, v in pairs(MODEL_WEAPONS) do _G[k] = v end
for k, v in pairs(PED_MODELS_AND_TYPES) do _G[k] = v end
for k, v in pairs(WEAPONS_MODELS_AND_TYPES) do _G[k] = v end
for k, v in pairs(DOORS_CAR) do _G[k] = v end	
for k, v in pairs(SPEC_PED_MODELS_AND_TYPES) do _G[k] = v end	


function delay()
for i=1,100 do u= i end
end

function Openalldoorscar(car)--открыть все двери авто.
opendoorcar(car, 0) opendoorcar(car, 1) opendoorcar(car, 2)
opendoorcar(car, 3) opendoorcar(car, 4) opendoorcar(car, 5)
end
function Ped_in_point_in_radius(ped, x,y,z, rx,ry,rz)
wait(50)
local res = ped_in_point_in_radius(ped, x,y,z, rx,ry,rz)
return res
end
function Car_in_point_in_radius(car, x,y,z, rx,ry,rz)
wait(50)
local res = car_in_point_in_radius(car, x,y,z, rx,ry,rz)
return res
end

function getworldcoordped(ped, x,y)
x2,y2,z =getcoordes(ped)
    if x == nil and y == nil
    then x = 1.0 y = 1.0  
local x1, y1= worldcoord(ped, x,y)
 return x1,y1,z
    end
 if x ~= nil and y == nil
   then y=1.0 
local x1, y1= worldcoord(ped, x,y)
    return x1,y1,z
  end
  if x ~= nil and y ~= nil
   then   local x1, y1= worldcoord(ped, x,y)
    return x1,y1,z
    end
end
function model_and_type(model, list)
 for k,v in pairs(list) do
        if model == k
        then m = v[1] -- модель.
            t =v[2] -- тип модель
        end end
   return m, t-- тип.
end
--макросы
function Createped(m,x,y,z)--создать педа.
m, t = model_and_type(m, PED_MODELS_AND_TYPES)
load_requested_models() 
while not availablemodel(m) do wait(1) loadmodel(m) end
ped= createped(m,t, x,y,z)
releasemodel(m) 
return ped
end
function Create_spec_ped(m, slot, x,y,z)--создать спец пед.
idmodel, t = model_and_type(m, SPEC_PED_MODELS_AND_TYPES)-- модель и тип.
tipe = idmodel -108- slot
create_spec_ped(m,idmodel,tipe, t ,slot, x,y,z) 
 return ped
end
function Giveweaponped(ped, ammo, ...)-- дать педу оружие и патроны.
    for m1, v in pairs({...}) do	    
        m, t = model_and_type(v, WEAPONS_MODELS_AND_TYPES)	
        loadmodel(m)
        load_requested_models()
        while not availablemodel(m) do wait(1) loadmodel(m) end
        giveweaponped(ped, m, t, ammo)
        releasemodel(m) 
    end
end
function Createcar(m,x,y,z)-- создать машину.
loadmodel(m)
load_requested_models()
while not availablemodel(m) do  wait(1) loadmodel(m) end
local car = createcar(m, x,y,z)
releasemodel(m) 
return car
end

function Get_type_weapon_ped(ped)	
for k,v in pairs(WEAPONS_MODELS_AND_TYPES) do
if is_current_weapon_ped(ped,v[2])
then t1 = v[2]
    m1 = v[1]
   t = t1-- тип.
   m = m1
   break
end end
return m, t
end

function Create_weapon_pickup(m, typepickup, ammo, x,y,z) 
model, t = model_and_type(m, WEAPONS_MODELS_AND_TYPES)	
loadmodel(model)
load_requested_models() 
while not availablemodel(model) do wait(1) loadmodel(model) end
pickup = create_weapon_pickup(model,typepickup, ammo,x,y,z)
releasemodel(model) 
return pickup
end

function Create_pickup(model, typepickup, x,y,z) 
 loadmodel(model)
load_requested_models() 
while not availablemodel(model) do wait(1) loadmodel(model) end
pickup = create_pickup(model,typepickup, x,y,z)
releasemodel(model)  
return pickup
end

-- function Keypress(key)
 -- if keypress(key)-- клавиша .
 -- then wait(200)
  -- while keypress(key) do wait(20) end
 -- return true 
 -- else return false
 -- end
-- end


function Kill_ped_on_foot(ped, ped2)
 kill_ped_on_foot(ped)
 kill_ped_on_foot(ped2)
end
function Kill_char_any_means(ped, ped2)
 kill_char_any_means(ped)
 kill_char_any_means(ped2)
end
function Draw_corona(...)
local t = {...} 
draw_corona(t) 
t= nil
end


Co_Draw_corona= coroutine.wrap(
 function(radius, t, glow_flare, red, green, blue,x,y,z)
 while true do  Draw_corona(radius, t, glow_flare, red, green, blue,x,y,z)
 coroutine.yield()   end
 end
 )
 function races()
  sound_coordinate(7, 0.0,0.0,0.0)
  show_text_styled("RACE2", 1500, 4)-- 3 для гонки
  wait(1700)
  sound_coordinate(7, 0.0,0.0,0.0)
  show_text_styled("RACE3", 1500, 4)-- 2 для гонки
  wait(1700)
  sound_coordinate(7, 0.0,0.0,0.0)
  show_text_styled("RACE4", 1500, 4)-- 1 для гонки
  wait(1700)
  sound_coordinate(7, 0.0,0.0,0.0)
  show_text_styled("RACE5", 1500, 4)-- go для гонки
  wait(1700)
 end 
 
function set_ped_in_car(car, ped, place)  -- уст водителя авто.
local place = place or nil
local m, t = model_and_type(ped, PED_MODELS_AND_TYPES)
load_requested_models() 
while not availablemodel(m) do wait(1) loadmodel(m) end
if place == nil 
then driver = setcardrive(car,m,t)
else driver = setcarpassenger(car,m,t,place)
end 
releasemodel(m) 
return driver
end

function car_in_radius(car, x1, y1, z1, rx, ry, rz)
local x,y,z=getcarcoordes(car)
if rx == 0 and ry == 0 and x == x1 and y == y1
then return true
else r2 = rx * rx + ry * ry + rz * rz;
x = x - x1 y = y - y1 z = z - z1
end
if x * x + y * y + z * z <= r2
then
return true
else
return false
end
end 

 Arrested1233 = coroutine.wrap(-- Проверка на арест.
 function ()
 coroutine.yield(true) 
 while true == arrested() do wait() end
 return false
 end
 )
 function Arrested()
  if  arrested()
  then  return Arrested1233()
  else return false
 end
 end
 
function Keypress(key)
 if keypress(key)-- клавиша .
 then
 wait(300)
 if not keypress(key) then 
 return true 
 end
 else return false
 end
end

 function Create_obj(model, x,y,z) 
 loadmodel(model)
load_requested_models() 
while not availablemodel(model) do wait(1) loadmodel(model) end
obj =  create_obj(model, x,y,z) 
releasemodel(model)  
return obj
end
 function star_mission(player, cheat_word)
 statuscar, car = incar(player)
 if cheat(cheat_word) and false == statuscar and false == getflagmission()  -- получить статус миссии.
and  player_defined(player) and not Arrested()
 then setflagmission(1) -- установить флаг миссии
return true
 end
end

 function miss(money)  text="mission passed $"..tostring(money)
 setflagmission(0) -- установить флаг миссии
 play_sound(1)  showtext(text, 2500,0) wait(900)-- вывод статуса миссии. 
 givemoney(money) 
 end
function setcolorcar(car, first, second)
 setcarfirstcolor(car, first) -- уст первый цвет авто.
 setcarseconscolor(car, second) -- уст второй цвет авто.
end
function end_mission(text)
 setflagmission(0) -- установить флаг миссии
 showtext(text, 2500,0)-- вывод статуса миссии.
 wait(900)
end
function checkmission(player)
while true == getflagmission() do wait()
 yield()
if not player_defined(player) or  Arrested()
then end_mission("mission failed!")
break
 end 
end
 end
rotate_obj = coroutine.wrap(-- ехать по маршруту.
function(obj)
local i=1
for i=1,360 do  move_rotate(obj, i,360,1)
  i=i+1
  coroutine.yield() 
  if i > 360
  then i=1
  end
  end
 end
 )
follow_route = coroutine.wrap(-- ехать по маршруту.
function(car, road)
local iter = 1
for i, v in pairs(road) do
    if i == iter
    then  x = road[i] y= road[i+1] z = road[i+2]
       iter= iter+3 cardrive(car, x, y, z)
while lualoader == nil  do  coroutine.yield(false) 
local rx = 3.0  ry = 4.0 rz = 2.0
  if car_in_point_in_radius(car, x, y, z, rx, ry, rz)
    then  break
	 end   end
 end
 end
 return true
 end )
follow_route_for_corona_for_playercar  = coroutine.wrap(-- ехать по маршруту.
function(mycar, road)
local iter = 1
for i, v in pairs(road) do
    if i == iter
    then x = road[i] y= road[i+1] z = road[i+2]  iter= iter+3   
   	blip = createmarker(1,2, x,y,z) -- создать маркер на карте. Принимает тип, размер, координаты, id маркера.
	setsizemarker(blip,2)--уст размер маркера. Принимает маркер, значение его размера
while lualoader == nil  do
 coroutine.yield(false) 
local t = {4.5, 6, 0, 255, 0, 0, x,y,z}
  draw_corona(t) 
 local rx = 3.0  ry = 3.0 rz = 3.0
  if car_in_point_in_radius(mycar, x, y, z, rx, ry, rz)
  then	sound_coordinate(7, 0.0,0.0,0.0)
  remove_blip(blip)
	break
	 end 
	 end 
 end
 end
 return true
end 
 )
 
 function turn_default_timer(turn, str_timer)
if turn == true
then local t ={turn,str_timer,1267,270, 1,1, 2.0, 2.0, 101, 193, 244}--
print_front(t)
local timer = star_timer(str_timer)
return timer
else stop_timer(str_timer)
local t ={turn,str_timer,1267,270, 1,1, 2.0, 2.0, 101, 193, 244}--
print_front(t)
end
end

clocktimer = coroutine.wrap(
  function()
      while true do 
     datetime = os.date("!*t",os.time())
      minu =tostring(datetime.min)
	  sec = tostring(datetime.sec) 
	  
      countminu =tonumber(datetime.min)
	  countsec = tonumber(datetime.sec) 
	 if countminu< 10
	 then sec = "0"..tostring(datetime.sec)  
	 end
	 
	 if countsec< 10
	 then minu = "0"..tostring(min.sec)  
	 end
	 
	  newtime = "time : ".. minu .." : "..sec    
	  wait()    coroutine.yield()
         if (newtime ~= old)
         then 
		 local t ={false,newtime,1267,270, 1,1, 2.0, 2.0, 101, 193, 244}--
             print_front(t) 
			 local t ={true,newtime,1267,270, 1,1, 2.0, 2.0, 101, 193, 244}--
             print_front(t)
		    old = newtime 
           coroutine.yield()
		   --end
          end
        end
     end
)
countdown = coroutine.wrap(
  function(time_count)
	 clock_time = time_count * -1 -- агрумент от сколько секундах отсчет время в - например 60 в -60.
  	 sec_start = tonumber(os.time())-- начальная точка в секундах. 
   while true do  wait()  coroutine.yield(true)
   	sec = tostring(os.time()) -- текущая временная точка в секундах, например, 12.
	  
	countsec = tonumber(sec)-- текущие секунды в числах.
    difference = sec_start - countsec -- разница между между начальной и конечнной точки 10 - 12 =-2
	 if clock_time < 0 and clock_time <=(difference)-- обновить время на экран.
	 then number =  time_count + difference
	  newtime = "time : "--[[.. minu .." : "]].. tostring(number) 
	  local  t ={false,newtime,1267,270, 1,1, 2.0, 2.0, 101, 193, 244}--
      print_front(t) 
	  local  t ={true,newtime,1517,270, 1,1, 2.0, 2.0, 101, 193, 244}--
      print_front(t) 
	  coroutine.yield(true)
	 else 
		local t ={false,newtime,1267,270, 1,1, 2.0, 2.0, 101, 193, 244}--
        print_front(t) 
		break
	    end
     end
	 return false		
     end
)
function Hold_cellphone(ped, status)
	loadmodel(258)
	load_requested_models()
	while not availablemodel(258) do wait(1) loadmodel(258) end
    hold_cellphone(player, status)
    releasemodel(258) 
end
-- function in_point_actor_in_radius(ped, x1, y1, z1, rx, ry, rz)
  -- x,y,z=getcoordes(ped)
-- if rx == 0 and ry == 0 and x == x1 and y == y1
-- then return true
-- else r2 = rx * rx + ry * ry + rz * rz;
-- x = x - x1 y = y - y1 z = z - z1
-- end
-- if x * x + y * y + z * z <= r2
-- then
-- return true
-- else
-- return false;
-- end
-- end
--return mymod


--[
-- 331 - Стиральная машинка

-- 332, 541 - Колеса

-- 333 - Газ плита

-- 334 - Автомат с содой

-- 335 - Кейс

-- 337 - Пачка денег

-- 345, 346 - Коробки

-- 349, 405 - Деревянные ящики

-- 356 - Раскрытая коробка

-- 361 - Корзина

-- 368 - Бронежилет

-- 370 - Банко мат

-- 376 - Надпись gtaIII

-- 380, 582 - Бомбы

-- 385, 573, 574, 575 - Кушетки с матрасом

-- 386, 414, 418 - Скамейки

-- 398, 399 - Пакеты с мусором

-- 410 - Статуэтка (Спрятанный пакет, поклажа)

-- 413 - Новостной ящик (Сам не знаю что такое)

-- 415 - Камера хранения

-- 416 - Деревянный столик с зонтиком

-- 420, 421, 422, 423, 424 - Ящики с газетами

-- 431, 502, 503, 504, 505, 507, - Наркотики (Пакет обмотанный скотчем)

-- 452, 455 - Комнатные цветы

-- 462, 473, 476 - Горшки с землей

-- 470 - Зонт

-- 471, 522, 634 - Стол и стулья

-- 500 - Стол Love Fist

-- 508 - Карточка

-- 509 - Плакат Love Fist

-- 510 - Коробка с пиццей

-- 511, 512, 513, 514, - Мишени (Чуваки, из GTA III)

-- 516, 517, 518, 519, 520, - Части чучела (Из амуниции в Down Town)

-- 526 - Мяч с острова Стар Фиш

-- 544, 550 - Камеры видео наблюдения

-- 552 - Железный ящик

-- 553, 554, 556, 557, 558 - Похоже на холодильник

-- 555 - Настенное радио

-- 564 - Боеголовка

-- 568, 569, 570, 571 - Полотенца

-- 572, 592, 596, 597, 598 - Бутылки

-- 578 - Канистра

-- 584 - Железка

-- 587, 589 - Люстры

-- 591, 599 - Кресла

-- 593, 598 - Пепельница

-- 594, 595, 598 - Стаканы и рюмки

-- 600 - Газовая граната

-- 604 - Бензоколонка

-- 633 - Гроб

-- Предметы улиц города:

-- 336 - Пожарный кран

-- 341, 387 - Телефон-автоматы

-- Бочки
-- 342 - ржавая
-- 343 -- желтая
-- 347 -- бледна красная
-- 350 - красная

-- 344 - Доски

-- 345, 346 - Коробки

-- 348, 351, 357, 358, 417, 425, 426, 429, 430 - Фонарные столбы

-- 349, 405 - Деревянные ящики

-- 353, 362, 501 - Мусорные контейнеры

-- 354, 363, 364, 369, 419, 576, 590, 694, - Ограждения

-- 355 - Столб автобусной остановки

-- 356 - Раскрытая коробка

-- 359 - Знак парковки

-- 360 - Знак телефона

-- 371, 588 - Рампы

-- 373, 374 - Скалы

-- 376 - Надпись gtaIII

-- 379 - Шлагбаум

-- 380, 582 - Бомбы

-- 384 - Деревянный электро столб

-- 386, 414, 418 - Скамейки

-- 389 - Автобусная остановка

-- 390, 412 - Почтовые ящики

-- 391, 392, 393, 400, 401, 402 - Стенды для рекламы

-- 394, 427, 428 - Светофорные столбы

-- 396, 397 - Светофоры

-- 398, 399 - Пакеты с мусором

-- 403, 404 - Паковочные счетчики

-- 413 - Новостной ящик (Сам не знаю что такое)

-- 416 - Деревянный столик с зонтиком

-- 420, 421, 422, 423, 424 - Ящики с газетами

-- 431, 502, 503, 504, 505, 507, - Наркотики (Пакет обмотанный скотчем)

-- 440, 441, 442, 443 - Деревья

-- 444, 445, 446, 447, 448, 449, 469, 474, 478 - Пальмы

-- 454 - Маленькое дерево

-- 459 - Дерево как пальма

-- 460, 461, 463, 466 - Куски травы

-- 464, 465, 468 - Мини клумбы

-- 467, 477, - Кусты

-- 470 - Зонт

-- 471, 522, 634 - Стол и стулья

-- 500 - Стол LoveFist

-- 509 - Плакат LoveFist

-- 515 - Рамка

-- 521 - Шипы

-- 523, 524 - Тарелки

-- 525 - Клумбы

-- 527, 528, 529, 530, 531, 532, 533, 534, 536, 537 - Водная живность

-- 538, 539 - Камни

-- 540 - Подводная лодка

-- 542, 543, 547, 549, - Знаки

-- 544, 550 - Камеры видео наблюдения

-- 545, 546, 548, 551, 563 - Кондиционеры

-- 552 - Железный ящик

-- 559, 560, 561, 562, 583 - Водные Рампы

-- 567 - Волейбольная сетка

-- 577- стекло 
-- 578 - канистра
-- 579 - темное стекло

-- 580, 581 - Заборы
-- 582 -- динамит
-- 583 - водной трамплин
-- 584 - водной трамплин
-- 585 - потолочный вентилятор
-- 586 - потолочный светильник
-- 587 - потолочный светильник
-- 588 - трамплин.
-- 601 - Столб

-- 602, 605, 608, 619, 622, 623, 624, 625, 626, 635- Лоды

-- 603, 606, 607, 609, 611, 614, 615, 617, 620, 636, 637, 638 - Модели 
-- домов, техники и ландшафта (Некоторых)

-- 604 – Бензоколонка

-- Пикапы:

-- 365 - Пикап информации

-- 366 - Пикап здоровья

-- 367 - Пикап адреналина

-- 368 - Бронежилет

-- 375 - Пикап звезды от копов

-- 382 - Пикап видеокамеры

-- 383 - Пикап буйства

-- 406 - Пикап покупки дома (Запрещенный - синий)

-- 407 - Пикап покупки дома (Разрешенный - зеленый)

-- 408 - Пикап сбора денег (Доллар $)

-- 409 - Пикап одежды

-- 411 - Пикап сохранения (Кассета)

-- 432 - Пикап музыки (Наушники)

--]