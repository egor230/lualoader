
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
 BGA = {128,4,20}, BGB = {128,4,20}, BOUNCA = {128,4,20}, BURGER = {128,4,20}, CGONA = {128,4,20}, CGONB = {128,4,20}, CGONC = {128,4,20},
 CHEF = {128,4,20}, CMRAMAN = {128,4,20}, COURIER = {128,4,20}, CREWA = {128,4,20}, CREWB = {128,4,20}, CSJUGGZ = {128,5,20}, 
 DGOONA = {128,4,20}, DGOONB = {128,4,20}, DGOONC = {128,4,20}, FLOOZYA = {128,5,20}, FLOOZYB = {128,5,20}, FLOOZYC = {128,5,20}, 
 FSFA = {128,4,20}, IGALSCB = {128,4,20}, IGBUDDY = {128,4,20}, IGBUDY2 = {128,4,20}, IGBUDY3 = {128,4,20}, IGCANDY = {128,5,20}, 
 IGCOLON = {128,4,20}, IGDIAZ = {128,4,20}, IGDICK = {128,4,20}, IGGONZ = {128,4,20}, IGHLARY = {128,4,20}, IGHLRY2 = {128,4,20}, 
 IGJEZZ = {128,4,20}, IGKEN = {128,4,20}, IGMERC = {128,5,20}, IGMIKE = {128,4,20}, IGMIKE2 = {128,4,20}, IGPERCY = {128,4,20}, 
 IGPHIL = {128,4,20}, IGPHIL2 = {128,4,20}, IGPHIL3 = {128,4,20}, IGSONNY = {128,4,20}, IGMERC2 = {128,5,20}, MBA = {128,4,20}, 
 MBB = {128,4,20}, MPORNA = {128,4,20}, MGOONA = {128,4,20}, MSERVER = {128,4,20}, MBA = {128,4,20}, MBB = {128,4,20}, MPORNA = {128,4,20},
 MGOONA = {128,4,20}, MSERVER = {128,4,20}, PLAY10 = {128,4,20}, PLAY11 = {128,4,20}, PLAY12 = {128,4,20}, PLAYER2 = {128,4,20}, 
 PLAYER3 = {128,4,20}, PLAYER4 = {128,4,20},  PLAYER5 = {128,4,20},  PLAYER6 = {128,4,20},   PLAYER7 = {128,4,20}, 
 PLAYER8 = {128,4,20}, PLAYER9 = {128,4,20}, PRINTRA = {128,4,20},  PRINTRB = {128,4,20}, PRINTRC = {128,4,20}, PSYCHO = {128,4,20},
 S_KEEP = {128,4,20}, SAM = {128,4,20}, SGC = {128,4,20}, SGOONA = {128,4,20}, SGOONB = {128,4,20}, SHOOTRA = {128,5,20}, 
 SHOOTRB = {128,4,20}, SPANDXA = {128,5,20},SPANDXB = {128,5,20}, STRIPA = {128,5,20}
}
			 
WEAPONS_MODELS_AND_TYPES = {unarmed = {0,0}, brassknuckle = {259,1}, screwdriver = {260,2}, golfclub = {261,3},
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
AMINATIONS_LIST ={
-- 0 - идет
-- 1 - бег
-- 2 - спринт
-- 3 - стоит
-- 4 - шаг
-- 5 - тормоз лево
-- 6 - тормоз право
-- 7 - руки назад
-- 8 - руки назад
-- 9 - устал
-- 10 - держит оружие
-- 11 - разговор
-- 12 - взмах рукой, ловит такси
-- 13 - падение на колени вперед
-- 14 - падение на колени вперед
-- 15 - падение на колени вперед
-- 16 - падение на колени вперед
-- 17 - держится за лицо, падение вперед
-- 18 - держится за живот, падение назад
-- 19 - держится за левое плечо, падение вперед
-- 20 - держится за правое плечо, падение вперед
-- 21 - держится за ногу, падение назад
-- 22 - держится за ногу, падение назад
-- 23 - переворот влево на земле на живот
-- 24 - переворот право на земле на живот
-- 25 - падение назад
-- 26 - переворот вправо на земле на спину
-- 27 - падение вперед
-- 28 - переворот влево на земле на спину
-- 29 - получает по голове спереди
-- 30 - получает по голове слева
-- 31 - получает по голове спереди
-- 32 - получает по голове справа
-- 33 - толкнули
-- 34 - толкнули слева
-- 35 - толкнули сзади
-- 36 - толкнули справа
-- 37 - пинают лежа
-- 38 - ударили в живот
-- 39 - ударили спереди
-- 40 - ударили спереди
-- 41 - ударили сзади
-- 42 - врезался в стену (rs перелазание)
-- 43 - пинают лежа на животе
-- 44 - шуганулся сбоку
-- 45 - в стойке
-- 46 - как-то дергается
-- 47 - идет вперед в стойке
-- 48 - удар вперед правой
-- 49 - удар головой
-- 50 - удар ногой влево
-- 51 - удар коленом
-- 52 - удар левой
-- 53 - удар правой в голову
-- 54 - удар вертушкой
-- 55 - удар ногой
-- 56 - удар правой рукой
-- 57 - удар левой рукой
-- 58 - удар логтем влево
-- 59 - удар логтем вправо
-- 60 - удар левой ногой назад
-- 61 - удар правой ногой назад
-- 62 - нажал взрыватель
-- 63 - удар правой рукой
-- 64 - удар правой рукой
-- 65 - пинок лежачего
-- 66 - бросает гранату
-- 67 - подходит в стойке
-- 68 - посадка в машину
-- 69
-- 70
-- 71
-- 72
-- 73
-- 74
-- 75
-- 76
-- 77
-- 78
-- 79
-- 80
-- 81
-- 82
-- 83
-- 84
-- 85
-- 86 - запрыгивает в кабриолет
-- 87
-- 88
-- 89
-- 90
-- 91
-- 92
-- 93
-- 94
-- 95
-- 96
-- 97
-- 98
-- 99
-- 100
-- 101
-- 102 - сидит в машине водителем
-- 103 - сидит в низкой машине водителем
-- 104 - сидит пассажиром
-- 105
-- 106
-- 107
-- 108
-- 109
-- 110
-- 111
-- 112
-- 113
-- 114 - сидит в машине глядя назад
-- 115 - стоит в лодке
-- 116 - поворачивает влево в лодке
-- 117 - поворачивает право в лодке
-- 118 - оглядывается назад в лодке
-- 119 - встает с земли справа
-- 120 - встает с земли слева
-- 121 - поднимает байк справа
-- 122 - поднимает байк слева
-- 123 - скидывает с байка справа
-- 124 - скидывает с байка слева
-- 125 - улетает с байка вперед
-- 126 - улетает с байка вперед
-- 127
-- 128
-- 129
-- 130
-- 131
-- 132
-- 133 - выползает из машины
-- 134 - выползает из машины
-- 135
-- 136
-- 137 - встает с земли
-- 138 - встает с земли
-- 139 - встает с земли
-- 140 - упал отжался
-- 141 - подпрыгивает
-- 142 - прыжок
-- 143 - скачет
-- 144 - свободное падение
-- 145 - падает стоя
-- 146 - скачет
-- 147 - кувырок
-- 148 - падает на спине	(заменено на быстрое плавание)
-- 149 - падает вперед (заменено на плавание)
-- 150 - дергается
-- 151 - падает от машины
-- 152 - чешет бошку
-- 153 - смотрит по сторонам
-- 154 - разворот вокруг себя
-- 155 - арестовывает
-- 156 - тонет
-- 157 - на корточках прячет голову
-- 158 - на корточках прячет голову
-- 159 - сидит на колене
-- 160 - стреляет с колена
-- 161 - поднимает руки вверх
-- 162 - шарахается
-- 163 - поднимает руку типа ты чо
-- 164 - телефон начало (rs бухло)
-- 165 - телефон конец (rs бухло)
-- 166 - телефон разговор (rs бухло)
-- 167 - садится на скамейку
-- 168 - встает со скамейки
-- 169 - сидит на скамейке
-- 170 - садится на скамейку
-- 171 - банкомат
-- 172 - висит на тросе
}

--for k, v in pairs(KEYS) do _G[k] = v end	
--for k, v in pairs(MODEL_CARS) do _G[k] = v end
--for k, v in pairs(MODEL_WEAPONS) do _G[k] = v end
--for k, v in pairs(PED_MODELS_AND_TYPES) do _G[k] = v end
--for k, v in pairs(WEAPONS_MODELS_AND_TYPES) do _G[k] = v end
for k, v in pairs(DOORS_CAR) do _G[k] = v end	
--for k, v in pairs(SPEC_PED_MODELS_AND_TYPES) do _G[k] = v end	


function delay()
for i=1,1000000 do u= i end
end

-- function Openalldoorscar(car)--открыть все двери авто.
 -- opendoorcar(car, 0) opendoorcar(car, 1) opendoorcar(car, 2)
 -- opendoorcar(car, 3) opendoorcar(car, 4) opendoorcar(car, 5)
-- end
function Ped_in_point_in_radius(ped, x,y,z, rx,ry,rz)
 wait(50)
 local res = ped_in_point_in_radius(ped, x,y,z, rx,ry,rz)
 return res
end
function Car_in_point_in_radius(car, x,y,z, r)
wait(50)
local res = car_in_point_in_radius(car, x,y,z, r)
return res
end

function getworldcoordped(ped, x,y)
local x2,y2,z =getcoordes(ped)
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
   then local x1, y1= worldcoord(ped, x,y)
    return x1,y1,z
    end
end
function getweaponslot(ped, nameweapon )-- получить номер слота оружие.
local m, t = model_and_type(nameweapon, WEAPONS_MODELS_AND_TYPES)	
 t = tonumber(t)
local g = Getweaponslot(ped,t)
  g = tonumber(g)
return g  
end  
--[[
 1 - за деньги, регенерируется почти сразу 
 2 - бесплатно, регенерация за 30 минут игрового времени, надо отойти недалеко 
 3 - бесплатно, одноразовый 9,11 - бомба 
 15 - эти регенерируется за 6 часов игрового времени или за 6 минут по-нормальному
]]-- 
-- function Create_pickup(model, typepickup, x,y,z) -- создать пикап.
-- loadmodel(model)
-- load_requested_models() 
-- while not availablemodel(model) do wait(1) loadmodel(model) end
-- local pickup = create_pickup(model,typepickup, x,y,z)
-- releasemodel(model)  
-- return pickup
-- end

function remove_current_weapon_ped(ped)	-- удалить текущее оружие.
  local m = get_model_current_weapon_ped(ped) -- получить текущее оружие.
 if m ~= 0
 then Remove_weapon_model(ped, m)
  end
end

function throw_current_weapon(ped)-- выбросить текущее оружие.	
 local m = get_model_current_weapon_ped(ped) -- получить текущее оружие.	
 local ammo = get_ammo_weapon_ped(ped) -- получить кол-во патронов текущего оружие.	
 local x,y,z = getcoordinates_on_y(ped, 5)
  Remove_weapon_model(ped, m)
if m ~= 0
 then 
local p = Create_weapon_pickup(m,3, ammo, x,y,z)-- создать пикап оружие.
return p
end
 end
 
-- function Keypress(key)
 -- if keypress(key)-- клавиша.
 -- then wait(200)
  -- while keypress(key) do wait(20) end
 -- return true 
 -- else return false
 -- end
-- end


function races()-- отсчет времени перед стартом.
  ped_frozen(0)
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
  ped_frozen(1)
  timer_donw(0,"R_TIME",0)-- Таймер гонки.
  return "R_TIME"
 end 
 

function car_in_radius_mod(car, x1, y1, z1, rx, ry, rz)
local x,y,z=getcarcoordes(car)
if rx == 0 and ry == 0 and x == x1 and y == y1
then return true
else r2 = rx * rx + ry * ry + rz * rz;
x = x - x1 y = y - y1 z = z - z1
end
if x * x + y * y + z * z <= r2
then return true
else return false
end
end 

function ped_in_radius_mod(player, x1, y1, z1, rx, ry, rz) 
wait(50)
local x,y,z=getpedcoordes(player)
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
local key = tostring(key)
 if keypress(key)-- клавиша.
 then
 wait(300)
 if not keypress(key) then 
 return true 
 end
 else return false
 end
end

function setcolorcar(car, first, second)-- установить первый и второй цвет авто.
 setcarfirstcolor(car, first) -- уст первый цвет авто.
 setcarseconscolor(car, second) -- уст второй цвет авто.
end

function end_mission(text)

  set_traffic(1)
  set_ped_traffic(1) 
 draw_corona(false, 4.5, 6, 0, 255, 0, 0, 0,0, 0)
 setflagmission(false) -- установить флаг миссии
 wait(200)
 showtext(text, 2500,0)-- вывод статуса миссии.
 destroy()-- удалить все объекты, которые были созданы скриптом.	 
end

function miss(money)  text="mission passed $"..tostring(money)
 play_sound(1) end_mission(text) wait(900)-- вывод статуса миссии. 
 givemoney(money) 
end

function checkmission(player)
while true == getflagmission() do wait()
 yield()
if not player_defined(player) or arrested()
then end_mission("mission failed!")
break
 end 
end
 end
 
function check_defined_and_arest()
 wait(300)
 player = findplayer()-- получить игрока
  if not player_defined(player) or arrested()
   then end_mission("mission failed!")
  end
end

function star_mission(player, cheat_word)
 statuscar, car = incar(player)
 if cheat(cheat_word) and false == statuscar and false == getflagmission()  -- получить статус миссии.
 and player_defined(player) and not Arrested()
  then setflagmission(true) -- установить флаг миссии
 --newthread(checkmission, player) -- в новом потоке, постоянная жив ли игрок?
return true
 end
end

function Star_mission_marker(t,x,y,z)
 player = findplayer()-- получить игрока
   if star_mission_marker(t,x,y,z) -- чит-код 
    then ped_frozen(0)
    local mycar = ped_car(player)
    while not 0 == getcarspeed(mycar) and is_car_stopped(mycar) do wait(100) end
      setflagmission(true) -- установить флаг миссии
      fade(0,1100) 
      wait(1000) 
      exitcar(player) 
      while true do wait(500)
       if not is_ped_in_car(player)
        then break    end 
          end
          ped_frozen(1)
          wait(1000)
          setcarcoordes(mycar,0.0,0.0,0.0)
          remove_car(mycar)
   return true
 end 
   if not star_mission_marker(t,x,y,z) -- чит-код 
   then return false
 end
end

function Getflagmission()
 player = findplayer()-- получить игрока
 if not player_defined(player) or arrested()
  then end_mission("mission failed!")
  end 
 wait(200)
 local flag = getflagmission()
 return flag
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
  if car_in_point_in_radius(car, x, y, z, r)
    then  break
	 end   end
 end
 end
 return true
 end )
 
-- follow_route_for_corona_for_playercar  = coroutine.wrap(-- ехать по маршруту.
-- function(mycar, road)
-- local iter = 1
-- for i, v in pairs(road) do
    -- if i == iter
    -- then x = road[i] y= road[i+1] z = road[i+2]  iter= iter+3   
   	-- blip = createmarker(1,2, x,y,z) -- создать маркер на карте. Принимает тип, размер, координаты, id маркера.
	-- setsizemarker(blip,2)--уст размер маркера. Принимает маркер, значение его размера
-- local t = {true, 4.5, 6, 0, 255, 0, 0, x,y,z}
  -- draw_corona(t) -- вкл корону
-- while lualoader == nil  do  wait(10)
 -- player = findplayer()-- получить игрока
 -- local b, mycar= incar(player) 
 -- coroutine.yield(false) 
 -- if b == true
 -- then  if car_in_point_in_radius(mycar, x, y, z, 4.0  )
  -- then	sound_coordinate(7, 0.0,0.0,0.0)
-- local t = {false, 4.5, 6, 0, 255, 0, 0, x,y,z}
  -- draw_corona(t) -- выкл корону
  -- remove_blip(blip)
	-- break end
	 -- end 
	 -- end 
 -- end
 -- end
 -- return true
-- end 
 -- )
 
function finish_road(car, road)
local x= road[#road-2] 
local y=road[#road-1] 
local z=road[#road] -- 

  if car_in_point_in_radius(car, x,y, z, 3.0)
  then  return true
  else return false
 end
 end
 
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

function sethealth(e, n)
if isped(e)
then setpedhealth(e, n)
end
if isvehicle(e)
then setpedhealth(e, n)
end

end

function create_marker(e)
if isped(e)
then local m= create_marker_actor(e)
return m
end
if isvehicle(e)
then local m = create_marker_car(e)
return m
end
if isobject(e)
then local m = create_marker_obj(e)
return m
end
end

function Remove(e)
if isped(e)
then remove_ped(e)
end
if isvehicle(e)
then remove_car(e)
end
if isobject(e)
then remove_obj(e)
end
end

function setcord(player,x,y,z)
if isped(player)
then setpedcoordes(player,x,y,z)
end
if isvehicle(player)
then setcarcoordes(player,x,y,z)
end
if isobject(player)
then setobjcoordes(player,x,y,z)
end
end

function setangle(player,angle)
if isped(player)
then setpedangle(player,angle)
end
if isvehicle(player)
then setcarangle(player,angle)
end
if isobject(player)
then setobjangle(player,angle)
end
end

function getcoordinates_on_y(ref,distance)
if isped(ref)
then local x,y,z = getpedcoordinates_on_y(ref,distance)
return x,y,z
end
if isvehicle(ref)
then local x,y,z = getcarcoordinates_on_y(ref,distance)
return x,y,z
end
if isobject(ref)
then local x,y,z = getobjcoordinates_on_y(ref,distance)
return x,y,z
end
end

function getcoordinates_on_x(ref,distance)
if isped(ref)
then local x,y,z = getpedcoordinates_on_x(ref,distance)
return x,y,z
end
if isvehicle(ref)
then local x,y,z = getcarcoordinates_on_x(ref,distance)
return x,y,z
end
if isobject(ref)
then local x,y,z = getobjcoordinates_on_x(ref,distance)
return x,y,z
end
end

function foel(model, player,weapon,x,y,z)
 local ped = Createped(model, x,y,z)
 Giveweaponped(ped,600, weapon)
 local m  = create_marker(ped)
 kill_char_any_means(ped, player)
 return ped, m
end

function foel1(model, player,weapon,x,y,z)
 local ped = Createped(model, x,y,z)
 Giveweaponped(ped,600, weapon)
 kill_ped_on_foot(ped, player)
 return ped
end

function Create_ped_and_give_weapon(model,weapon,x,y,z)
 local ped = Createped(model, x,y,z)
 Giveweaponped(ped,600, weapon)
 local m = create_marker(ped)
 return ped, m
end

function getcameracoordes()
 local x,y,z = Getcameracoordes() 
 local x1=tostring(x) local y1=tostring(y) local z1=tostring(z)

 return x1,y1,z1
end

function create_marker_and_sphere(x,y,z, size, radius) -- создать метку на карте с размером и сферу с радиусом.

 local blip = createmarker(1,2,x,y,z) -- создать маркер на карте. Принимает тип, размер, координаты, id маркера.
 setsizemarker(blip, size)--уст размер маркера. Принимает маркер, значение его размера
 local sp = create_sphere(x,y,z, radius)-- создать сферу.
return blip, sp
end 

function set_angle_camera(x,y,z,r, angle)
local z = z +1.0
local r = r *1.5
angle1 = angle + 180
if angle1 > 360
then angle1 = angle1 - 360
end  -- printmessage("angle = ".. tostring( angle1), 1300, 1)
 x1=  tonumber(tonumber(x)+r*math.cos(angle))
 y1 =  tonumber(tonumber(y)+r*math.sin(angle))
 camera_at_point(x, y, z, 1)
 set_camera_position(x1,y1,z, 0.0, 0.0, 0.0)
end

function mod_autoaim(weapon, switch)
 if weapon =="M4"
   then if switch =="on"
        then write_memory(0x5349DB, 2, 0x9090)
		end
		if switch =="off"
        then write_memory(0x5349DB, 2, -125627532)
		end
	end
	 if weapon =="ruger"
     then if switch =="on"
        then write_memory(0x5349E0, 2, 0x9090)
		end
		if switch =="off"
        then write_memory(0x5349E0, 2, -125628812)
		end
	end
	 if weapon =="M90"
     then if switch =="on"
        then write_memory(0x5349D1, 2, 0x9090)
		end
		if switch =="off"
        then write_memory(0x5349D1, 2,  -108852876)
		end
	end
end
-- res = read_memory(adres,2)
-- write_memory(adres, 2, 0x9090)
-- 05DF: write_memory 0x5349DB size 2 value 0x9090 virtual_protect 1 //M4 -125627532
-- 05DF: write_memory 0x5349E0 size 2 value 0x9090 virtual_protect 1 //ruger -125628812
-- 05DF: write_memory 0x5349E5 size 2 value 0x9090 virtual_protect 1 //M60 -108852876
-- 05DF: write_memory 0x5349D1 size 2 value 0x9090 virtual_protect 1 //sniper rifle (29)--125626252
-- 05DF: write_memory 0x5349D6 size 2 value 0x9090 virtual_protect 1 //sniper rifle (28)-125627532
-- 05DF: write_memory 0x5349C8 size 2 value 0x9090 virtual_protect 1 //rocket launcher-125627532 	
--[[
метки
0 Место назначения Маленький розовый квадрат, неиспользованный
1 Увеличенная версия значка позиции игрока, сбой игры,
если смотреть в карте паузы, не используется
2Положение игрока
Крошечная версия розовой стрелки позиции игрока, не
используется
3 Север, сбой игры, если смотреть в карте паузы, не
используется
4 RADAR_ SPRITE_ AVERY radar_avery LG_02 Эйвери Каррингтон

5 RADAR_ SPRITE_ BIKER radar_biker LG_03 Контакт для байкеров Большой Митч Бейкер, неиспользуемый
6 RADAR_ SPRITE_  CORTEZ radar_cortez LG_04 Полковник Кортес
7 RADAR_ SPRITE_ DIAZ radar_diaz LG_05 Рикардо Диас
8 RADAR_ SPRITE_ KENT radar_kent LG_06 Кент Пол
9 RADAR_ SPRITE_ ЮРИСТ radar_lawyer LG_07 Адвокат Кен Розенберг
10 RADAR_ SPRITE_ PHIL radar_phil LG_08 Фил Кэссиди
11 RADAR_ SPRITE_ BIKERS байкеры LG_03 Контакт для байкеров Большой Митч Бейкер
12 RADAR_ SPRITE_ BOATYARD катеростроительный завод LG_09 катеростроительный завод
13 RADAR_ SPRITE_ MALIBUCLUB клуб LG_10 Клуб Малибу
14 RADAR_ SPRITE_ CUBANS кубинцы LG_11 кубинцы Умберто Робин
15 RADAR_ SPRITE_ FILM киностудия LG_12 Киностудия Стив Скотт
16 RADAR_ SPRITE_ GUN пистолет LG_13 Амма­Nation
17 RADAR_ SPRITE_ HAITIANS гаитян LG_14 гаитян Тетя Poulet
18 RADAR_ SPRITE_HARDWARE аппаратные средства LG_15 Магазин оборудования
19 RADAR_ SPRITE_SAVEHOUSE radar_save LG_16 Безопасный дом

20 RADAR_ SPRITE_ STRIPCLUB radar_strip LG_37 Стрип­клуб Поул­позиция
21 RADAR_ SPRITE_ ICE мороженое LG_17 мороженое Черри Попперс
22 RADAR_ SPRITE_ KCABS kcabs LG_18 Кауфманские кабины Такси фирма
23 RADAR_ SPRITE_ LOVEFIST кулак любви LG_19 Кулак любви
24 RADAR_ SPRITE_ PRINTWORKS ситценабивная фабрика LG_20 Печатные работы 
25 RADAR_ SPRITE_  СОБСТВЕННОСТЬ­ ­ ­ ­ Без спрайта
26 RADAR_ SPRITE_ SUNYARD Sunyard LG_36 Sun Yard Sunshine Autos
27 RADAR_ SPRITE_ SPRAY спрей LG_22 Pay 'n' Spray
28
RADAR_ SPRITE_ 
TSHIRT
Футболка LG_23 Магазин одежды
29
RADAR_ SPRITE_ 
TOMMY
Томми LG_24 Особняк Томми
30
ТЕЛЕФОН RADAR_ 
SPRITE_
Телефон LG_25 телефон убийство
31
RADAR_ SPRITE_ 
RADIO_WILDSTYLE
RWildstyle LG_26
Радиостанция
Wildstyle
неиспользуемый
32
RADAR_ SPRITE_ 
RADIO_FLASH
RFlash LG_27
Flash FM­
радиостанция
неиспользуемый
33
RADAR_ SPRITE_ 
RADIO_KCHAT
RKchat LG_28
Радиостанция
KChat
неиспользуемый
34
RADAR_ SPRITE_ 
RADIO_FEVER
RFever LG_29
Лихорадка 105
Радиостанция
неиспользуемый
35
RADAR_ SPRITE_ 
RADIO_VROCK
RVRock LG_30
Радиостанция
VROK Radio Station
Sic, unused
36
RADAR_ SPRITE_ 
RADIO_VCPR
RVCPR LG_31
Радиостанция
VCPR
неиспользуемый
37
RADAR_ SPRITE_ 
RADIO_ESPANTOSO
REspantoso LG_32
Радиостанция
Эспаньозо
неиспользуемый 21.07.2017 Blip ­ GTAModding
38
RADAR_ SPRITE_ 
RADIO_EMOTION
REmotion LG_33
Эмоция 98.3
Радиостанция
неиспользуемый
39
RADAR_ SPRITE_ 
RADIO_WAVE
RWave LG_34
Радиостанция Wave
103
неиспользуемый 

]]--

-- function set_ped_in_car(car, ped, place)  -- уст водителя авто.
 -- local place = place or nil
 -- local m, t = Get_model_and_type_ped(ped)
 -- loadmodel(m)
 -- load_requested_models() 
-- while not availablemodel(m) do wait(1) loadmodel(m) end
 -- if place == nil 
  -- then local driver = setcardrive(car,m,t)
  -- else local driver = setcarpassenger(car,m,t,place)
-- end 
-- releasemodel(m) 
-- return driver
-- end
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

-- function Create_spec_ped(m, slot, x,y,z)--создать спец пед.
-- idmodel, t = model_and_type(m, SPEC_PED_MODELS_AND_TYPES)-- модель и тип.
-- tipe = idmodel -108- slot
-- create_spec_ped(m,idmodel,tipe, t ,slot, x,y,z) 
 -- return ped
-- end


-- function model_and_type(model, list)
-- local m,t
 -- for k,v in pairs(list) do
     -- if model == k
      -- then m = k -- модель
           -- t = v -- тип модели.
      -- end end
	-- m = tonumber(m)
-- t = tonumber(t)
-- givemoney(m)
   -- return m, t-- тип.
-- end

--макросы.

-- function Createped(m,x,y,z)--создать педа.
   -- local m, t = model_and_type(m, PED_MODELS_AND_TYPES)
   -- load_requested_models() 
   -- while not availablemodel(m) do wait(1) loadmodel(m) end
   -- local ped = createped(m,t, x,y,z)
   -- releasemodel(m) 
-- return ped
-- end

-- function Giveweaponped(ped, ammo, ...)-- дать педу оружие и патроны.
 -- for m1, v in pairs({...}) do	    
  -- local m, t = model_and_type(v, WEAPONS_MODELS_AND_TYPES)	

   -- loadmodel(m)
   -- load_requested_models()
   -- while not availablemodel(m) do wait(1) loadmodel(m) end
   -- giveweaponped(ped, m, t, ammo)
   -- releasemodel(m) 
  -- end
-- end
-- function Createcar(m,x,y,z)-- создать машину.
	-- loadmodel(m)
	-- load_requested_models()
	-- while not availablemodel(m) do  wait(1) loadmodel(m) end
	-- local car = createcar(m, x,y,z)
	-- releasemodel(m) 
	-- return car
-- end

-- function Get_type_weapon_ped(ped) -- получить текущее оружие.	
-- for k,v in pairs(WEAPONS_MODELS_AND_TYPES) do
-- local model =tonumber(v[2])
-- if is_current_weapon_ped(ped, model)
-- then local m,t = v[1], v[2]
   -- break
-- end end
-- m = tonumber(m)
-- t = tonumber(t)
-- givemoney(t)
-- return m, t
-- end

-- function Create_weapon_pickup(m, typepickup, ammo, x,y,z)  -- создать пикап оружие.
	-- local model, t = model_and_type(m, WEAPONS_MODELS_AND_TYPES)	
	-- loadmodel(model)
	-- load_requested_models() 
	-- while not availablemodel(model) do wait(1) loadmodel(model) end
	-- local pickup = create_weapon_pickup(model,typepickup, ammo,x,y,z)
	-- releasemodel(model) 
	-- return pickup
-- end
 
--local mymod = {}
--function f()
-- printmessage("yes", 3000, 3)
--end

-- function ty()
-- local file ="test.txt"-- путь к файлу
 -- f = io.open(file, "w")-- дозапись в файл.
 -- f:write(package.path )
 -- f:close()
-- end
-- ty()

-- function isModuleAvailable(name)
  -- if package.loaded[name] then
    -- return true
  -- else
    -- for _, searcher in ipairs(package.searchers or package.loaders) do
      -- local loader = searcher(name)
      -- if type(loader) == 'function' then
        -- package.preload[name] = loader
        -- return true
      -- end
    -- end
    -- return false
  -- end
-- end
-- if isModuleAvailable("lualoader/mod")
-- then local file ="t.txt"-- путь к файлу
 -- f = io.open(file, "w")-- запись в файл.
 -- f:write(package.path)
 -- f:close()
-- end

